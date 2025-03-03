#include <iostream>

#include "io_tlv.h"
#include "SequenceEncoder.h"

//============================================================================

SequenceEncoder::SequenceEncoder(Parameters* params) : SequenceCodec(params)
{
  // determine the naming (ordering) of ply properties
  _plyAttrNames.position =
    axisOrderToPropertyNames(params->encoder.sps.geometry_axis_order);

  // NB: this is the raw origin before the encoder tweaks it
  _angularOrigin = params->encoder.gps.gpsAngularOrigin;
}

//----------------------------------------------------------------------------

int
SequenceEncoder::compress(Stopwatch* clock)
{
  bytestreamFile.open(params->compressedStreamPath, std::ios::binary);
  if (!bytestreamFile.is_open()) {
    return -1;
  }

  this->encoder.setMotionVectorFileName(params->motionVectorPath);
  const int lastFrameNum = params->firstFrameNum + params->frameCount;

  if (!params->encoder.gps.biPredictionEnabledFlag) {
    for (frameNum = params->firstFrameNum; frameNum < lastFrameNum;
         frameNum++) {
      this->encoder.setInterForCurrPic(
        params->encoder.gps.interPredictionEnabledFlag
        && ((frameNum - params->firstFrameNum) % params->encoder.randomAccessPeriod));

      if (compressOneFrame(clock))
        return -1;
    }
  } else {
    preIPFrame = -1;
    currentFrame = -1;
    codedGOF = false;
    for (frameNum = params->firstFrameNum; frameNum < lastFrameNum;
         frameNum += params->encoder.randomAccessPeriod) {
      gofSizePlusOne =
        ((frameNum + params->encoder.randomAccessPeriod) >= lastFrameNum)
        ? (lastFrameNum - frameNum)
        : (params->encoder.randomAccessPeriod + 1);

      if (compressOneGOF(clock)) {
        return -1;
      }
    }
  }

  std::cout << "Total bitstream size " << bytestreamFile.tellp() << " B\n";
  bytestreamFile.close();

  return 0;
}

//----------------------------------------------------------------------------

int
SequenceEncoder::compressOneFrame(Stopwatch* clock)
{
  const auto frameFileNum = params->encoder.gps.biPredictionEnabledFlag
    ? (encoder.getCurrFrameIndex() + params->firstFrameNum)
    : frameNum;
  std::string srcName{expandNum(params->uncompressedDataPath, frameFileNum)};
  PCCPointSet3 pointCloud;
  if (
    !ply::read(srcName, _plyAttrNames, params->inputScale, pointCloud)
    || pointCloud.getPointCount() == 0) {
    std::cout << "Error: can't open input file!" << std::endl;
    return -1;
  }
  // Some evaluations wish to scan the points in azimuth order to simulate
  // real-time acquisition (since the input has lost its original order).
  // NB: because this is trying to emulate the input order, binning is disabled
  if (params->sortInputByAzimuth)
    sortByAzimuth(
      pointCloud, 0, pointCloud.getPointCount(), 0., _angularOrigin,
      params->encoder.gps.geom_angular_azimuth_scale_log2_minus11 + 12,
      params->encoder.gps.geom_angular_azimuth_speed_minus1 + 1,
      params->encoder.gps.angularTheta, params->encoder.gps.angularZ);

  // Sanitise the input point cloud
  // todo(df): remove the following with generic handling of properties
  bool codeColour = params->encoder.attributeIdxMap.count("color");
  if (!codeColour)
    pointCloud.removeColors();
  assert(codeColour == pointCloud.hasColors());

  bool codeReflectance = params->encoder.attributeIdxMap.count("reflectance");
  if (!codeReflectance)
    pointCloud.removeReflectances();
  assert(codeReflectance == pointCloud.hasReflectances());

  clock->start();

  if (params->convertColourspace)
    convertFromGbr(params->encoder.sps.attributeSets, pointCloud);

  scaleAttributesForInput(params->encoder.sps.attributeSets, pointCloud);

  // The reconstructed point cloud
  CloudFrame recon;
  auto* reconPtr = params->reconstructedDataPath.empty()
      && !params->encoder.sps.inter_frame_prediction_enabled_flag
    ? nullptr
    : &recon;

  auto bytestreamLenFrameStart = bytestreamFile.tellp();

  int ret = (params->encoder.gps.biPredictionEnabledFlag == 2)
    ? encoder.compressHGOF(pointCloud, &params->encoder, this, reconPtr)
    : encoder.compress(pointCloud, &params->encoder, this, reconPtr);
  if (ret) {
    std::cout << "Error: can't compress point cloud!" << std::endl;
    return -1;
  }

  auto bytestreamLenFrameEnd = bytestreamFile.tellp();
  int frameLen = bytestreamLenFrameEnd - bytestreamLenFrameStart;
  std::cout << "Total frame size " << frameLen << " B" << std::endl;

  clock->stop();

  if (!params->reconstructedDataPath.empty())
    writeOutputFrame(params->reconstructedDataPath, {}, recon, recon.cloud);

  return 0;
}

int
SequenceEncoder::compressOneGOF(Stopwatch* clock)
{
  //Generate the indexes for I-frame and P-frame in one GOF
  std::vector<int> IPFrameIndexes;
  for (int i = 0; i < gofSizePlusOne;
       i += params->encoder.gps.biPredictionPeriod)
    IPFrameIndexes.push_back(i);
  if (IPFrameIndexes.back() != gofSizePlusOne - 1)
    IPFrameIndexes.push_back(gofSizePlusOne - 1);

  auto const firstFrameNum = params->firstFrameNum;
  auto const randomAccessPeriod = params->encoder.randomAccessPeriod;
  //First process the I-frame and P-frame
  //If the Bi-prediction is disabled, the other frames are set to P-frame
  //Otherwise, the other frames are set to B-frame
  for (int i = codedGOF ? 1 : 0; i < IPFrameIndexes.size(); i++) {
    int currentIPFrame = IPFrameIndexes[i] + frameNum;

    // First frame is always coded as I-frame
    if (preIPFrame == -1) {
      encoder.setInterForCurrPic(false);
      encoder.setBiPredEncodeParams(false, 0, -1, -1, 0);
      currentFrame = frameNum;
      if (compressOneFrame(clock))
        return -1;
    } else {
      const auto prevIPFrameDelta = preIPFrame - firstFrameNum;
      if (encoder.biPredictionEligibility(
            currentIPFrame - firstFrameNum, prevIPFrameDelta,
            &params->encoder)) {
        // Code the next I-frame/P-frame first
        currentFrame = currentIPFrame;
        encoder.setInterForCurrPic(
          (currentFrame - firstFrameNum) % randomAccessPeriod);
        encoder.setBiPredEncodeParams(
          false, currentFrame - firstFrameNum, prevIPFrameDelta, -1, 1);
        if (compressOneFrame(clock))
          return -1;

        // Code the other frames as B-frames
        // If the hierarchical GOF sturcture is applied
        if (params->encoder.gps.biPredictionEnabledFlag == 2) {
          if (
            currentIPFrame - preIPFrame - 1 != encoder.getCodeOrderListSize())
            encoder.initBiPredEncodeParamsGOF(currentIPFrame - preIPFrame);

          encoder.setRefTimesList();
          for (int i = 0; i < encoder.getCodeOrderListSize(); i++) {
            currentFrame = preIPFrame + encoder.getCodeOrder(i);
            encoder.setInterForCurrPic(true);
            encoder.setBiPredEncodeParams(
              true, currentFrame - firstFrameNum,
              encoder.getRefFrame(i * 2) + prevIPFrameDelta,
              encoder.getRefFrame(i * 2 + 1) + prevIPFrameDelta,
              encoder.getQPshift(i));
            encoder.setCurrFrameIndexInGOF(encoder.getCodeOrder(i));
            if (compressOneFrame(clock))
              return -1;
          }
        } else {
          // IF the IBBP GOF structure is applied
          for (currentFrame = preIPFrame + 1; currentFrame < currentIPFrame;
               currentFrame++) {
            auto const frameDelta = currentFrame - firstFrameNum;
            encoder.setInterForCurrPic(true);
            encoder.setBiPredEncodeParams(
              true, frameDelta, frameDelta - 1, currentIPFrame - firstFrameNum,
              2);
            if (compressOneFrame(clock))
              return -1;
          }
        }
      } else {
        for (currentFrame = preIPFrame + 1; currentFrame < currentIPFrame + 1;
             currentFrame++) {
          auto const frameDelta = currentFrame - firstFrameNum;
          encoder.setInterForCurrPic(frameDelta % randomAccessPeriod);
          encoder.setBiPredEncodeParams(
            false, frameDelta, frameDelta - 1, -1, 1);

          if (compressOneFrame(clock))
            return -1;
        }
      }
    }
    preIPFrame = currentIPFrame;
  }
  codedGOF = true;
  return 0;
}

//----------------------------------------------------------------------------

void
SequenceEncoder::onOutputBuffer(const PayloadBuffer& buf)
{
  writeTlv(buf, bytestreamFile);
}

//----------------------------------------------------------------------------

void
SequenceEncoder::onPostRecolour(const PCCPointSet3& cloud)
{
  if (params->postRecolorPath.empty()) {
    return;
  }

  // todo(df): don't allocate if conversion is not required
  PCCPointSet3 tmpCloud(cloud);
  CloudFrame frame;
  frame.setParametersFrom(params->encoder.sps, params->encoder.outputFpBits);
  frame.cloud = cloud;
  frame.frameNum = frameNum - params->firstFrameNum;

  writeOutputFrame(params->postRecolorPath, {}, frame, tmpCloud);
}

//============================================================================