#pragma once
#include "frame.h"
#include "PCCTMC3Encoder.h"
#include "PCCTMC3Decoder.h"

enum class OutputSystem
{
  // Output after global scaling, don't convert to external system
  kConformance = 0,

  // Scale output to external coordinate system
  kExternal = 1,
};

std::array<const char*, 3> axisOrderToPropertyNames(pcc::AxisOrder order);
void convertFromGbr(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);
  //----------------------------------------------------------------------------

struct Parameters {
  bool isDecoder;

  // command line parsing should adjust dist2 values according to PQS
  bool positionQuantizationScaleAdjustsDist2;

  // Scale factor to apply when loading the ply before integer conversion.
  // Eg, If source point positions are in fractional metres converting to
  // millimetres will allow some fidelity to be preserved.
  double inputScale;

  // Length of the output point clouds unit vectors.
  double outputUnitLength;

  // output mode for ply writing (binary or ascii)
  bool outputBinaryPly;

  // Fractional fixed-point bits retained in conformance output
  int outputFpBits;

  // Output coordinate system to use
  OutputSystem outputSystem;

  // when true, configure the encoder as if no attributes are specified
  bool disableAttributeCoding;

  // Frame number of first file in input sequence.
  int firstFrameNum;

  // Number of frames to process.
  int frameCount;

  std::string uncompressedDataPath;
  std::string compressedStreamPath;
  std::string reconstructedDataPath;

  // Filename for saving recoloured point cloud (encoder).
  std::string postRecolorPath;

  // Filename for saving pre inverse scaled point cloud (decoder).
  std::string preInvScalePath;

  pcc::EncoderParams encoder;
  pcc::DecoderParams decoder;

  // perform attribute colourspace conversion on ply input/output.
  bool convertColourspace;

  // resort the input points by azimuth angle
  bool sortInputByAzimuth;

  std::string motionVectorPath;
};

class SequenceCodec {
public:
  // NB: params must outlive the lifetime of the decoder.
  SequenceCodec(Parameters* params) : params(params) {}

  // Perform conversions and write output point cloud
  //  \params cloud  a mutable copy of reconFrame.cloud
  void writeOutputFrame(
    const std::string& postInvScalePath,
    const std::string& preInvScalePath,
    const pcc::CloudFrame& reconFrame,
    pcc::PCCPointSet3& cloud);

  // determine the output ply scale factor
  double outputScale(const pcc::CloudFrame& cloud) const;

  // the output ply origin, scaled according to output coordinate system
  pcc::Vec3<double> outputOrigin(const pcc::CloudFrame& cloud) const;

  void scaleAttributesForInput(
    const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);

  void scaleAttributesForOutput(
    const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud);

protected:
  Parameters* params;
};

bool ParseParameters(int argc, char* argv[], Parameters& params);