#include "TMC3.h"

#include <memory>
#include <unistd.h>
#if 1
#  include "partitioning.h"
#  include "geometry_params.h"
#  include "pointset_processing.h"  // encoder
#  include "frame.h"
#else
#  include "PCCTMC3Encoder.h"       // encoder
#  include "PCCTMC3Decoder.h"       // decoder
#endif

#include "CodecCommon.hpp"
#include "constants.h"            // encoder decoder
#include "ply.h"

#include "PCCPointSet.h"

#include "program_options_lite.h"
#include "io_tlv.h"               // decoder
#include "version.h"

#include "util.hpp"

using namespace std;
using namespace pcc;

//============================================================================
//---------------------------------------------------------------------------
std::array<const char*, 3>
SequenceCodec::axisOrderToPropertyNames(pcc::AxisOrder order)
{
  static const std::array<const char*, 3> kAxisOrderToPropertyNames[] = {
    {"z", "y", "x"}, {"x", "y", "z"}, {"x", "z", "y"}, {"y", "z", "x"},
    {"z", "y", "x"}, {"z", "x", "y"}, {"y", "x", "z"}, {"x", "y", "z"},
  };

  return kAxisOrderToPropertyNames[int(order)];
}

//---------------------------------------------------------------------------
// :: Command line / config parsing
//---------------------------------------------------------------------------
bool SequenceCodec::isDecoder(int argc, char* argv[])
{
  namespace po = df::program_options_lite;
  bool isDecoder=false;

  po::Options opts;
  opts.addOptions()
  ("config,c", po::parseConfigFile, "configuration file name")
  ("mode", isDecoder, false,
    "The encoding/decoding mode:\n"
    "  0: encode\n"
    "  1: decode");

  po::setDefaults(opts);
  po::ErrorReporter err;
  const list<const char*>& argv_unhandled =
    po::scanArgv(opts, argc, (const char**)argv, err);

    return(isDecoder);
}

df::program_options_lite::OptionSpecific&
 SequenceCodec::addCommonOption(df::program_options_lite::OptionSpecific& opt)
{
  namespace po = df::program_options_lite;
  params_.print_help = false;

  return (opt
          ("help", params_.print_help, false, "this help text")
          ("config,c", po::parseConfigFile, "configuration file name")
          (po::Section("General"))
          ("mode", params_.isDecoder, false,
            "The encoding/decoding mode:\n"
            "  0: encode\n"
            "  1: decode")
          ("loopCount", params_.file_repeat_num_, 0,
            "loop num")
          // i/o parameters
          ("firstFrameNum",
            params_.firstFrameNum, 0,
            "Frame number for use with interpolating %d format specifiers "
            "in input/output filenames")
          ("frameCount",
            params_.frameCount, 1,
            "Number of frames to encode")
          ("reconstructedDataPath",
            params_.reconstructedDataPath, {},
            "The ouput reconstructed pointcloud file path (decoder only)")
          ("uncompressedDataPath",
            params_.uncompressedDataPath, {},
            "The input pointcloud file path")
          ("compressedStreamPath",
            params_.compressedStreamPath, {},
            "The compressed bitstream path (encoder=output, decoder=input)")
          ("plydir",
            params_.plydir, {},
            "The ply dir path (for iPhone app)")
          ("sleepWaitNextPly",
            params_.sleepWaitNextPly, 100,
            "sleep time wait for next ply(ms) (for iPhone app)")
          ("continousEncoding",
            params_.continousEncoding, 0,
            "1 : continous /0 :only first ply only (for iPhone app)")
          ("MaxExecuteListSize",
            params_.MaxExecutedListSize, 1000,
            "Number to be Excuted ply number (for iPhone app)")
          ("postRecolorPath",
            params_.postRecolorPath, {},
            "Recolored pointcloud file path (encoder only)")
          ("preInvScalePath",
            params_.preInvScalePath, {},
            "Pre inverse scaled pointcloud file path (decoder only)")
          ("convertPlyColourspace",
            params_.convertColourspace, true,
            "Convert ply colourspace according to attribute colourMatrix")
          ("outputBinaryPly",
            params_.outputBinaryPly, false,
            "Output ply files using binary (or otherwise ascii) format")
          ("outputUnitLength",
            params_.outputUnitLength, 0.,
            "Length of reconstructed point cloud x,y,z unit vectors\n"
            " 0: use srcUnitLength")
          ("outputScaling",
            params_.outputSystem, OutputSystem::kExternal,
            "Output coordnate system scaling\n"
            " 0: Conformance\n"
            " 1: External")
          ("outputPrecisionBits",
            params_.outputFpBits, -1,
            "Fractional bits in conformance output (prior to external scaling)\n"
            " 0: integer,  -1: automatic (full)")
          (po::Section("Transport"))
          ("ip_address",
            params_.ip_address, {},
            "dst addr(encoder), recv I/F addr(decoder)")
          ("port",
            params_.ip_port, 20101,
            "dst port(encoder), recv port(decoder)")
          ("ipv6",
            params_.ipv6, false,
            "IPv6 dst addr(encoder), recv I/F addr(decoder)")
          ("mtu_size",
            params_.mtu_size, 1300,
            "MTU size")
          (po::Section("ROS setting"))
          ("ros_subscribe_topic",
            params_.ros_subscribe_topic, {},
            "ROS topic")
          ("ros_subscribe_max_queue_size",
            params_.ros_subscribe_max_queue_size, 1,
            "Max queue size for ROS subscribe topic")
          ("ros_publish_topic",
            params_.ros_publish_topic, {},
            "ROS topic")
          ("ros_publish_max_queue_size",
            params_.ros_publish_max_queue_size, 1,
            "Max queue size for ROS publisher")
          ("ros_frameid",
            params_.ros_frameid, {},
            "ROS FrameID")
          ("dumpUncompressedDataPath",
            params_.dumpUncompressedDataPath, {},
            "The dump pointcloud file path")
          ("dumpROSTopicPath",
            params_.dumpROSTopicPath, {},
            "The dump rostopic file path")
          ("debugLogPath",
            params_.debugLogPath, {},
            "The debug log file path")
#ifdef DO_PARALLEL_CODING
          ("numOfFrameThreads",
            params_.numOfFrameThreads, 1,
            "Number of threads that operate in parallel in a frame by encoder or decoder")
          ("doSliceParallelProc",
            params_.doSliceParallelProc, false,
            "Whether or not parallel operation in sliced units in encoder or decoder")
#endif // DO_PARALLEL_CODING
          // This section controls all general geometry scaling parameters
          (po::Section("Cmn Coordinate system scaling"))
          ("inputScale",
            params_.inputScale, 1.,
            "Scale input while reading src ply. "
            "Eg, 1000 converts metres to integer millimetres")
          // (po::Section("Encoder"))
          ("sortInputByAzimuth",
            params_.sortInputByAzimuth, false,
            "Sort input points by azimuth angle")
          ("positionQuantizationScaleAdjustsDist2",
            params_.positionQuantizationScaleAdjustsDist2, false,
            "Scale dist2 values by squared positionQuantizationScale")
          );
}

//============================================================================
double
SequenceCodec::outputScale(const pcc::CloudFrame& frame) const
{
  switch (params_.outputSystem) {
  case OutputSystem::kConformance: return 1.;

  case OutputSystem::kExternal:
    // The scaling converts from the frame's unit length to configured output.
    // In terms of specification this is the external coordinate system.
    return frame.outputUnitLength / params_.outputUnitLength;
  }
  
  return 1.;
}

//----------------------------------------------------------------------------
Vec3<double>
SequenceCodec::outputOrigin(const pcc::CloudFrame& frame) const
{
  switch (params_.outputSystem) {
  case OutputSystem::kConformance: return 0.;

  case OutputSystem::kExternal: return frame.outputOrigin * outputScale(frame);
  }

  return 0.;
}

//----------------------------------------------------------------------------
void
SequenceCodec::writeOutputFrame(
  const std::string& postInvScalePath,
  const std::string& preInvScalePath,
  const pcc::CloudFrame& frame,
  pcc::PCCPointSet3& cloud)
{
  if (postInvScalePath.empty() && preInvScalePath.empty())
    return;

  scaleAttributesForOutput(frame.attrDesc, cloud);

  if (params_.convertColourspace)
    convertToGbr(frame.attrDesc, cloud);

  // the order of the property names must be determined from the sps
  ply::PropertyNameMap attrNames;
  attrNames.position = axisOrderToPropertyNames(frame.geometry_axis_order);

  // offset frame number
  int frameNum = frame.frameNum + params_.firstFrameNum;

  // Dump the decoded colour using the pre inverse scaled geometry
  if (!preInvScalePath.empty()) {
    std::string filename{expandNum(preInvScalePath, frameNum)};
    ply::write(cloud, attrNames, 1.0, 0.0, filename, !params_.outputBinaryPly);
  }

  auto plyScale = outputScale(frame) / (1 << frame.outputFpBits);
  auto plyOrigin = outputOrigin(frame);
  std::string decName{expandNum(postInvScalePath, frameNum)};
  if (!ply::write(
        cloud, attrNames, plyScale, plyOrigin, decName,
        !params_.outputBinaryPly)) {
    cout << "Error: can't open output file!" << endl;
  }
}

//============================================================================
const AttributeDescription*
findColourAttrDesc(const std::vector<AttributeDescription>& attrDescs)
{
  // todo(df): don't assume that there is only one colour attribute in the sps
  for (const auto& desc : attrDescs) {
    if (desc.attributeLabel == KnownAttributeLabel::kColour)
      return &desc;
  }
  return nullptr;
}

//----------------------------------------------------------------------------
void SequenceCodec::convertToGbr(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud)
{
  const pcc::AttributeDescription* attrDesc = findColourAttrDesc(attrDescs);
  if (!attrDesc)
    return;

  switch (attrDesc->params.cicp_matrix_coefficients_idx) {
  case ColourMatrix::kBt709: convertYCbCrBt709ToGbr(cloud); break;

  case ColourMatrix::kYCgCo:
    // todo(df): select YCgCoR vs YCgCo
    // NB: bitdepth is the transformed bitdepth, not the source
    convertYCgCoRToGbr(attrDesc->bitdepth - 1, cloud);
    break;

  default: break;
  }
}

//----------------------------------------------------------------------------
void SequenceCodec::convertFromGbr(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud)
{
  const pcc::AttributeDescription* attrDesc = findColourAttrDesc(attrDescs);
  if (!attrDesc)
    return;

  switch (attrDesc->params.cicp_matrix_coefficients_idx) {
  case ColourMatrix::kBt709: convertGbrToYCbCrBt709(cloud); break;

  case ColourMatrix::kYCgCo:
    // todo(df): select YCgCoR vs YCgCo
    // NB: bitdepth is the transformed bitdepth, not the source
    convertGbrToYCgCoR(attrDesc->bitdepth - 1, cloud);
    break;

  default: break;
  }
}

//============================================================================
const AttributeDescription*
findReflAttrDesc(const std::vector<AttributeDescription>& attrDescs)
{
  // todo(df): don't assume that there is only one in the sps
  for (const auto& desc : attrDescs) {
    if (desc.attributeLabel == KnownAttributeLabel::kReflectance)
      return &desc;
  }
  return nullptr;
}

//----------------------------------------------------------------------------
struct AttrFwdScaler {
  template<typename T>
  T operator()(const AttributeParameters& params, T val) const
  {
    int scale = params.attr_scale_minus1 + 1;
    return ((val - params.attr_offset) << params.attr_frac_bits) / scale;
  }
};

//----------------------------------------------------------------------------
struct AttrInvScaler {
  template<typename T>
  T operator()(const AttributeParameters& params, T val) const
  {
    int scale = params.attr_scale_minus1 + 1;
    return ((val * scale) >> params.attr_frac_bits) + params.attr_offset;
  }
};

//----------------------------------------------------------------------------
template<typename Op>
void
scaleAttributes(
  const std::vector<AttributeDescription>& attrDescs,
  PCCPointSet3& cloud,
  Op scaler)
{
  // todo(df): extend this to other attributes
  const AttributeDescription* attrDesc = findReflAttrDesc(attrDescs);
  if (!attrDesc || !attrDesc->params.scalingParametersPresent)
    return;

  auto& params = attrDesc->params;

  // Parameters present, but nothing to do
  bool unityScale = !params.attr_scale_minus1 && !params.attr_frac_bits;
  if (unityScale && !params.attr_offset)
    return;

  const auto pointCount = cloud.getPointCount();
  for (size_t i = 0; i < pointCount; ++i) {
    auto& val = cloud.getReflectance(i);
    val = scaler(params, val);
  }
}

//----------------------------------------------------------------------------
void
SequenceCodec::scaleAttributesForInput(
  const std::vector<AttributeDescription>& attrDescs, PCCPointSet3& cloud)
{
  scaleAttributes(attrDescs, cloud, AttrFwdScaler());
}

//----------------------------------------------------------------------------
void
SequenceCodec::scaleAttributesForOutput(
  const std::vector<AttributeDescription>& attrDescs, PCCPointSet3& cloud)
{
  scaleAttributes(attrDescs, cloud, AttrInvScaler());
}

int SequenceCodec::closeSocket(int32_t& sock)
{
  int res = 0;
  if (0 <= sock) {
    res = shutdown(sock, SHUT_RDWR);
    close(sock);
    sock = -1;
  }

  return (res);
}

/**
int32_t SequenceCodec::findChannelIndex(const sensor_msgs::PointCloud2* cloud, const std::string& channel)
{
  for (size_t i = 0; i < cloud->fields.size(); ++i)
  {
    if (cloud->fields[i].name == channel)
    {
      return i;
    }
  }

  return -1;
}
void SequenceCodec::writeRosTopic(const std::string& fileName, const sensor_msgs::PointCloud2* cloud)
{
  std::ofstream fout(fileName, std::ofstream::out);
  if (!fout.is_open()) {
    return;
  }

  int32_t xi = findChannelIndex(cloud, "x");
  int32_t yi = findChannelIndex(cloud, "y");
  int32_t zi = findChannelIndex(cloud, "z");
  int32_t rgbi = findChannelIndex(cloud, "rgb");

  if ((xi < 0) || (yi < 0) || (zi < 0)) {
    // invalid data
    return;
  }

  const size_t pointCount = cloud->width * cloud->height;
  const uint32_t xoff = cloud->fields[xi].offset;
  const uint32_t yoff = cloud->fields[yi].offset;
  const uint32_t zoff = cloud->fields[zi].offset;
  const uint32_t rgboff = (rgbi<0) ? -1 : cloud->fields[rgbi].offset;
  const uint32_t point_step = cloud->point_step;

  fout << "ply" << std::endl;

  fout << "format ascii 1.0" << std::endl;

  fout << "frameid :" << cloud->header.frame_id << std::endl;
  fout << "seq     :" << cloud->header.seq   << std::endl;
  fout << "time    :" << cloud->header.stamp << std::endl;

  fout << "element vertex "  << pointCount << std::endl;
  fout << "property float x" << std::endl;
  fout << "property float y" << std::endl;
  fout << "property float z" << std::endl;

  if (0 <= rgbi) {
    fout << "property uchar green" << std::endl;
    fout << "property uchar blue"  << std::endl;
    fout << "property uchar red"   << std::endl;
  }
#if 0 
  if (cloud.hasReflectances()) {
    fout << "property uint16 refc" << std::endl;
  }
  if (cloud.hasFrameIndex()) {
    fout << "property uint8 frameindex" << std::endl;
  }
#endif

  fout << "property point_step " << point_step << std::endl;

  fout << "property ofs[x] " << xoff << std::endl;
  fout << "property ofs[y] " << yoff << std::endl;
  fout << "property ofs[z] " << zoff << std::endl;
  fout << "property ofs[rgb] " << rgboff << std::endl;

  fout << "element face 0" << std::endl;
  fout << "property list uint8 int32 vertex_index" << std::endl;
  fout << "end_header" << std::endl;

  {
    //      fout << std::setprecision(std::numeric_limits<double>::max_digits10);
    fout << std::fixed << std::setprecision(5);


    const uint8_t *ptr = &cloud->data.front(), *ptr_end = &cloud->data.back();
    for (; ptr < ptr_end; ptr += point_step)
    {
      fout << *reinterpret_cast<const float*>(ptr + xoff) << " "
           << *reinterpret_cast<const float*>(ptr + yoff) << " "
           << *reinterpret_cast<const float*>(ptr + zoff);

      if (0 <= rgboff) {
        fout << " " << static_cast<uint32_t>(ptr[rgboff + 1]) << " "
             << static_cast<uint32_t>(ptr[rgboff + 0]) << " "
             << static_cast<uint32_t>(ptr[rgboff + 2]) ;
             // << " : " << std::hex << *reinterpret_cast<const uint32_t*>(ptr + rgboff);
      }
#if 0
      if (cloud.hasReflectances()) {
        fout << " " << static_cast<int>(cloud.getReflectance(i));
      }
      if (cloud.hasFrameIndex()) {
        fout << " " << static_cast<int>(cloud.getFrameIndex(i));
      }
#endif
      fout << std::endl;
    }
  }

  fout.close();
}
*/

//============================================================================
