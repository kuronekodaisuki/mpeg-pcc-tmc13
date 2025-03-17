#include "PCCPointSet.h"
#include "gpcclib/encoder/encoder.hpp"

#include "gpcclib/encoder/params.hpp"
#include "gpcclib/internal/encoder/params.hpp"
#include "gpcclib/internal/codec.hpp"
#include "gpcclib/internal/frame.hpp"
#include "gpcclib/internal/payloadbuffer.hpp"
#include "gpcclib/internal/pointset.hpp"
#include "gpcclib/internal/expiration.hpp"
#include "PCCTMC3Encoder.h"
//#include "ParallelEncoder.h"
#include "frame.h"
#include "pointset_processing.h"

#ifdef LICENSE_CHECK
#include "load_license.hpp"
#include <iostream>
#endif

#include <new>

namespace gpcclib {
namespace encoder {

//============================================================================

class GPCCLIB_INTERFACE_SPEC EncoderImpl : public Encoder {
public:
  EncoderImpl(Callbacks& callbacks);
  ~EncoderImpl();

  void release() override;

  bool init(const EncoderParams& params);

  int compress(const PointSet3* inputPointCloud) override;

private:
  class TMC3Callbacks : public pcc::PCCTMC3Encoder3::Callbacks {
  public:
    explicit TMC3Callbacks(EncoderImpl& encoder);

    void init();

    void onOutputBuffer(const pcc::PayloadBuffer&) override;
    void onPostRecolour(const pcc::PCCPointSet3&) override;

  private:
    TMC3Callbacks(const TMC3Callbacks&) = delete;
    TMC3Callbacks& operator=(const TMC3Callbacks&) = delete;

    EncoderImpl& encoder;
    int lastEncoderFrameNum;
    int cbFrameNum;
  };

  EncoderImpl(const EncoderImpl&) = delete;
  EncoderImpl& operator=(const EncoderImpl&) = delete;

  unsigned int numOfFrameThreads;
  bool doSliceParallelProc;
  bool sortInputByAzimuth;
  bool convertColourspace;

  // pcc::ply::PropertyNameMap _plyAttrNames;

  // The raw origin used for input sorting
  pcc::point_t angularOrigin;

  Callbacks& callbacks;
  TMC3Callbacks tmc3Callbacks;

  pcc::PCCTMC3Encoder3* encoder;
  //ParallelEncoder* parallelEncoder;

  pcc::EncoderParams encoderParams;

  int frameNum;
};

//----------------------------------------------------------------------------
EncoderImpl::TMC3Callbacks::TMC3Callbacks(EncoderImpl& aEncoder)
  : encoder(aEncoder)
  , lastEncoderFrameNum(-1)
  , cbFrameNum(-1)
{}

void
EncoderImpl::TMC3Callbacks::init()
{
  lastEncoderFrameNum = -1;
  cbFrameNum = -1;
}

void
EncoderImpl::TMC3Callbacks::onOutputBuffer(const pcc::PayloadBuffer& buf)
{
  encoder.callbacks.onOutputBuffer(ConstPayloadBufferWrapper(buf));
}

void
EncoderImpl::TMC3Callbacks::onPostRecolour(const pcc::PCCPointSet3& cloud)
{
  // Encoder の frameNum が同じときは同じフレーム
  if (lastEncoderFrameNum != encoder.frameNum) {
    lastEncoderFrameNum = encoder.frameNum;
    ++cbFrameNum;
  }

  // todo(df): don't allocate if conversion is not required
  pcc::CloudFrame frame;
  frame.setParametersFrom(
    encoder.encoderParams.sps, encoder.encoderParams.outputFpBits);
  frame.cloud = cloud;
  // frame.frameNum = frameNum - params_.firstFrameNum;
  frame.frameNum = cbFrameNum;

  CodecUtils::applyOutputProcess(frame, encoder.convertColourspace);

  encoder.callbacks.onPostRecolour(ConstCloudFrameWrapper(frame));
}

//----------------------------------------------------------------------------
EncoderImpl::EncoderImpl(Callbacks& aCallbacks)
  : Encoder()
  , callbacks(aCallbacks)
  , tmc3Callbacks(*this)
  , encoder(nullptr)
  //, parallelEncoder(nullptr)
  , frameNum(0)
{}

EncoderImpl::~EncoderImpl()
{
  if (nullptr != encoder) {
    delete encoder;
    encoder = nullptr;
  }

  //if (nullptr != parallelEncoder) {
  //  delete parallelEncoder;
  //  parallelEncoder = nullptr;
  //}
}

void
EncoderImpl::release()
{
  delete this;
}

bool
EncoderImpl::init(const EncoderParams& params)
{
  tmc3Callbacks.init();

  CopyParams(encoderParams, params);

  numOfFrameThreads = params.getNumOfFrameThreads();
  doSliceParallelProc = params.doSliceParallelProc();
  sortInputByAzimuth = params.shouldSortInputByAzimuth();
  convertColourspace = params.shouldConvertColourspace();

  // determine the naming (ordering) of ply properties
  // _plyAttrNames.position =
  //   axisOrderToPropertyNames(encoderParams.sps.geometry_axis_order);

  // NB: this is the raw origin before the encoder tweaks it
  angularOrigin = encoderParams.gps.gpsAngularOrigin;

  frameNum = 0;

  if (1 == numOfFrameThreads) {
    try {
      encoder = new pcc::PCCTMC3Encoder3();
    }
    catch (const std::bad_alloc&) {
      return false;
    }
  } else {
    try {
      parallelEncoder = new ParallelEncoder();
    }
    catch (const std::bad_alloc&) {
      return false;
    }

    if (!parallelEncoder->init(numOfFrameThreads, doSliceParallelProc)) {
      return false;
    }
  }

  return true;
}

int
EncoderImpl::compress(const PointSet3* inputPointCloud)
{
  if (nullptr != inputPointCloud) {
    const IntermediatePointSet3* tmp =
      dynamic_cast<const IntermediatePointSet3*>(inputPointCloud);
    if (nullptr == tmp) {
      return -1;
    }

    pcc::PCCPointSet3 pointCloud;
    pointCloud = tmp->getCloud();

    // Some evaluations wish to scan the points in azimuth order to simulate
    // real-time acquisition (since the input has lost its original order).
    // NB: because this is trying to emulate the input order, binning is disabled
    if (sortInputByAzimuth)
      sortByAzimuth(
        pointCloud, 0, pointCloud.getPointCount(), 0., angularOrigin);

    // Sanitise the input point cloud
    // todo(df): remove the following with generic handling of properties
    bool codeColour = encoderParams.attributeIdxMap.count("color");
    if (!codeColour)
      pointCloud.removeColors();
    assert(codeColour == pointCloud.hasColors());

    bool codeReflectance = encoderParams.attributeIdxMap.count("reflectance");
    if (!codeReflectance)
      pointCloud.removeReflectances();
    assert(codeReflectance == pointCloud.hasReflectances());

    // clock->start();

    if (convertColourspace)
      CodecUtils::convertFromGbr(encoderParams.sps.attributeSets, pointCloud);

    CodecUtils::scaleAttributesForInput(
      encoderParams.sps.attributeSets, pointCloud);

    if (nullptr != encoder) {
      // The reconstructed point cloud
      pcc::CloudFrame recon;
      pcc::CloudFrame* reconPtr =
        (!callbacks.needsReconstructedCloud()) ? nullptr : &recon;

      int ret = encoder->compress(
        pointCloud, &encoderParams, &tmc3Callbacks, reconPtr);

      if (ret) {
        // cout << "Error: can't compress point cloud!" << endl;
        return -1;
      }

      // auto bytestreamLenFrameEnd = bytestreamFile.tellp();
      // int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;
      // std::cout << "Total frame size " << frameLen << " B" << std::endl;

      // clock->stop();

      if (reconPtr) {
        CodecUtils::applyOutputProcess(recon, convertColourspace);
        callbacks.onReconstructedCloud(CloudFrameImpl(std::move(recon)));
        //writeOutputFrame(params_.reconstructedDataPath, {}, recon, recon.cloud);
      }
    } else {
      pcc::CloudFrame* recon = nullptr;
      pcc::CloudFrame** reconPtr =
        (!callbacks.needsReconstructedCloud()) ? nullptr : &recon;

      int ret = parallelEncoder->compress(
        std::move(pointCloud), &encoderParams, &tmc3Callbacks, reconPtr);
      if (ret) {
        // cout << "Error: can't compress point cloud!" << endl;
        return -1;
      }

      // auto bytestreamLenFrameEnd = bytestreamFile.tellp();
      // int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;

      // if (0 != frameLen) {
      //   std::cout << "Total frame size " << frameLen << " B" << std::endl;
      // }

      // clock->stop();

      if (nullptr != reconPtr && nullptr != recon) {
        CodecUtils::applyOutputProcess(*recon, convertColourspace);
        callbacks.onReconstructedCloud(CloudFrameImpl(std::move(*recon)));
        // writeOutputFrame(
        //   params_.reconstructedDataPath, {}, *recon, recon->cloud);
      }
    }

    ++frameNum;
  }
  // if inputPointCloud == nullptr
  else {
    if (nullptr != encoder) {

      // not parallel
      return 1;

    } else {
      // 終端処理
      // The reconstructed point cloud
      pcc::CloudFrame* recon = nullptr;
      pcc::CloudFrame** reconPtr =
        (!callbacks.needsReconstructedCloud()) ? nullptr : &recon;

      int ret =
        parallelEncoder->compress(nullptr, nullptr, &tmc3Callbacks, reconPtr);

      if (0 > ret) {
        // cout << "Error: can't compress point cloud!" << endl;
        return -1;
      }

      // auto bytestreamLenFrameEnd = bytestreamFile.tellp();
      // int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;

      // if (0 != frameLen) {
      //   std::cout << "Total frame size " << frameLen << " B" << std::endl;
      // }

      // clock->stop();

      if (nullptr != reconPtr && nullptr != recon) {
        CodecUtils::applyOutputProcess(*recon, convertColourspace);
        callbacks.onReconstructedCloud(CloudFrameImpl(std::move(*recon)));
        // writeOutputFrame(
        //   params_.reconstructedDataPath, {}, *recon, recon->cloud);
      }

      return ret;
    }
  }

  return 0;
}

//---------------------------------------------------------------------------

#ifdef LICENSE_CHECK
  kteclicense::LicenseParser license_parser;
  bool LicenseIsFirst = true;
#endif

Encoder*
Encoder::create(const EncoderParams& encParams, Callbacks& callbacks)
{
#if GPCCLIB_TRIAL_BUILD
  if (IsGpcclibExpired()) {
    return nullptr;
  }
#endif
#ifdef LICENSE_CHECK
  if (LicenseIsFirst) {
    LicenseIsFirst = false;
    int load_ret = license_parser.load("./license.json", "KddiPrimitiveSDK");
    if (load_ret!=0) {
      std::cout << "license error encoder" << std::endl;
      return nullptr;
    }
    std::string opt = license_parser.getDOptionCode();
    std::string::size_type pos = opt.find("ENC");
    if (pos == std::string::npos) {
      std::cout << "license not allowed encode" << std::endl;
      return nullptr;
    }
  }
#endif

  EncoderImpl* encoder;
  try {
    encoder = new EncoderImpl(callbacks);
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }

  if (!encoder->init(encParams)) {
    encoder->release();
    return nullptr;
  }

  return encoder;
}

} // namespace encoder
} // namespace gpcclib
