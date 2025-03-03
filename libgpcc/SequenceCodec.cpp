///
/// Codec
/// 

#include <iostream>
#include "SequenceCodec.h"
#include "pointset_processing.h"
#include "ply.h"

//---------------------------------------------------------------------------
// :: Command line / config parsing helpers

template<typename T>
static std::istream&
readUInt(std::istream& in, T& val)
{
  unsigned int tmp;
  in >> tmp;
  val = T(tmp);
  return in;
}

static std::ostream&
operator<<(std::ostream& out, const OutputSystem& val)
{
  switch (val) {
  case OutputSystem::kConformance: out << "0 (Conformance)"; break;
  case OutputSystem::kExternal: out << "1 (External)"; break;
  }
  return out;
}


static std::istream&
operator>>(std::istream& in, OutputSystem& val)
{
  return readUInt(in, val);
}

const pcc::AttributeDescription*
findColourAttrDesc(const std::vector<pcc::AttributeDescription>& attrDescs)
{
  // todo(df): don't assume that there is only one colour attribute in the sps
  for (const auto& desc : attrDescs) {
    if (desc.attributeLabel == pcc::KnownAttributeLabel::kColour)
      return &desc;
  }
  return nullptr;
}

//----------------------------------------------------------------------------
std::array<const char*, 3>
axisOrderToPropertyNames(pcc::AxisOrder order)
{
  static const std::array<const char*, 3> kAxisOrderToPropertyNames[] = {
    {"z", "y", "x"}, {"x", "y", "z"}, {"x", "z", "y"}, {"y", "z", "x"},
    {"z", "y", "x"}, {"z", "x", "y"}, {"y", "x", "z"}, {"x", "y", "z"},
  };

  return kAxisOrderToPropertyNames[int(order)];
}

void
convertToGbr(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud)
{
  const pcc::AttributeDescription* attrDesc = findColourAttrDesc(attrDescs);
  if (!attrDesc)
    return;

  switch (attrDesc->params.cicp_matrix_coefficients_idx) {
  case pcc::ColourMatrix::kBt709 : convertYCbCrBt709ToGbr(cloud);
    break;

  case pcc::ColourMatrix::kYCgCo
      :
    // todo(df): select YCgCoR vs YCgCo
    // NB: bitdepth is the transformed bitdepth, not the source
    convertYCgCoRToGbr(attrDesc->bitdepth - 1, cloud);
    break;

  default: break;
  }
}

//----------------------------------------------------------------------------

void
convertFromGbr(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud)
{
  const pcc::AttributeDescription* attrDesc = findColourAttrDesc(attrDescs);
  if (!attrDesc)
    return;

  switch (attrDesc->params.cicp_matrix_coefficients_idx) {
  case pcc::ColourMatrix::kBt709 : convertGbrToYCbCrBt709(cloud);
    break;

  case pcc::ColourMatrix::kYCgCo
      :
    // todo(df): select YCgCoR vs YCgCo
    // NB: bitdepth is the transformed bitdepth, not the source
    convertGbrToYCgCoR(attrDesc->bitdepth - 1, cloud);
    break;

  default: break;
  }
}

//============================================================================

const pcc::AttributeDescription*
findReflAttrDesc(const std::vector<pcc::AttributeDescription>& attrDescs)
{
  // todo(df): don't assume that there is only one in the sps
  for (const auto& desc : attrDescs) {
    if (desc.attributeLabel == pcc::KnownAttributeLabel::kReflectance)
      return &desc;
  }
  return nullptr;
}

//----------------------------------------------------------------------------

struct AttrFwdScaler {
  template<typename T>
  T operator()(const pcc::AttributeParameters& params, T val) const
  {
    int scale = params.attr_scale_minus1 + 1;
    return ((val - params.attr_offset) << params.attr_frac_bits) / scale;
  }
};

//----------------------------------------------------------------------------

struct AttrInvScaler {
  template<typename T>
  T operator()(const pcc::AttributeParameters& params, T val) const
  {
    int scale = params.attr_scale_minus1 + 1;
    return ((val * scale) >> params.attr_frac_bits) + params.attr_offset;
  }
};

//----------------------------------------------------------------------------

template<typename Op>
void
scaleAttributes(
  const std::vector<pcc::AttributeDescription>& attrDescs,
  pcc::PCCPointSet3& cloud,
  Op scaler)
{
  // todo(df): extend this to other attributes
  const pcc::AttributeDescription* attrDesc = findReflAttrDesc(attrDescs);
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

namespace pcc
{
static std::istream&
operator>>(std::istream& in, ScaleUnit& val)
{
  try {
    readUInt(in, val);
  }
  catch (...) {
    in.clear();
    std::string str;
    in >> str;

    val = ScaleUnit::kDimensionless;
    if (str == "metre")
      val = ScaleUnit::kMetre;
    else if (!str.empty())
      throw std::runtime_error("Cannot parse unit");
  }
  return in;
}

static std::istream&
operator>>(std::istream& in, ColourMatrix& val)
{
  return readUInt(in, val);
}

static std::istream&
operator>>(std::istream& in, AxisOrder& val)
{
  return readUInt(in, val);
}

static std::istream&
operator>>(std::istream& in, AttributeEncoding& val)
{
  return readUInt(in, val);
}


static std::istream&
operator>>(std::istream& in, LodDecimationMethod& val)
{
  return readUInt(in, val);
}

static std::istream&
operator>>(std::istream& in, PartitionMethod& val)
{
  return readUInt(in, val);
}


static std::istream&
operator>>(std::istream& in, PredGeomEncOpts::SortMode& val)
{
  return readUInt(in, val);
}


static std::istream&
operator>>(std::istream& in, OctreeEncOpts::QpMethod& val)
{
  return readUInt(in, val);
}


static std::istream&
operator>>(std::istream& in, InterGeomEncOpts::LPUType& val)
{
  return readUInt(in, val);
}


static std::istream&
operator>>(std::istream& in, InterGeomEncOpts::MotionSource& val)
{
  return readUInt(in, val);
}


static std::ostream&
operator<<(std::ostream& out, const ScaleUnit& val)
{
  switch (val) {
  case ScaleUnit::kDimensionless: out << "0 (Dimensionless)"; break;
  case ScaleUnit::kMetre: out << "1 (Metre)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const ColourMatrix& val)
{
  switch (val) {
  case ColourMatrix::kIdentity: out << "0 (Identity)"; break;
  case ColourMatrix::kBt709: out << "1 (Bt709)"; break;
  case ColourMatrix::kUnspecified: out << "2 (Unspecified)"; break;
  case ColourMatrix::kReserved_3: out << "3 (Reserved)"; break;
  case ColourMatrix::kUsa47Cfr73dot682a20:
    out << "4 (Usa47Cfr73dot682a20)";
    break;
  case ColourMatrix::kBt601: out << "5 (Bt601)"; break;
  case ColourMatrix::kSmpte170M: out << "6 (Smpte170M)"; break;
  case ColourMatrix::kSmpte240M: out << "7 (Smpte240M)"; break;
  case ColourMatrix::kYCgCo: out << "8 (kYCgCo)"; break;
  case ColourMatrix::kBt2020Ncl: out << "9 (Bt2020Ncl)"; break;
  case ColourMatrix::kBt2020Cl: out << "10 (Bt2020Cl)"; break;
  case ColourMatrix::kSmpte2085: out << "11 (Smpte2085)"; break;
  default: out << "Unknown"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const AxisOrder& val)
{
  switch (val) {
  case AxisOrder::kZYX: out << "0 (zyx)"; break;
  case AxisOrder::kXYZ: out << "1 (xyz)"; break;
  case AxisOrder::kXZY: out << "2 (xzy)"; break;
  case AxisOrder::kYZX: out << "3 (yzx)"; break;
  case AxisOrder::kZYX_4: out << "4 (zyx)"; break;
  case AxisOrder::kZXY: out << "5 (zxy)"; break;
  case AxisOrder::kYXZ: out << "6 (yxz)"; break;
  case AxisOrder::kXYZ_7: out << "7 (xyz)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const AttributeEncoding& val)
{
  switch (val) {
  case AttributeEncoding::kRAHTransform: out << "0 (RAHT)"; break;
  case AttributeEncoding::kPredictingTransform: out << "1 (Pred)"; break;
  case AttributeEncoding::kLiftingTransform: out << "2 (Lift)"; break;
  case AttributeEncoding::kRaw: out << "3 (Raw)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const LodDecimationMethod& val)
{
  switch (val) {
  case LodDecimationMethod::kNone: out << "0 (None)"; break;
  case LodDecimationMethod::kPeriodic: out << "1 (Periodic)"; break;
  case LodDecimationMethod::kCentroid: out << "2 (Centroid)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const PartitionMethod& val)
{
  switch (val) {
  case PartitionMethod::kNone: out << "0 (None)"; break;
  case PartitionMethod::kUniformGeom: out << "2 (UniformGeom)"; break;
  case PartitionMethod::kOctreeUniform: out << "3 (UniformOctree)"; break;
  case PartitionMethod::kUniformSquare: out << "4 (UniformSquare)"; break;
  case PartitionMethod::kNpoints: out << "5 (NPointSpans)"; break;
  default: out << int(val) << " (Unknown)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const PredGeomEncOpts::SortMode& val)
{
  switch (val) {
    using SortMode = PredGeomEncOpts::SortMode;
  case SortMode::kNoSort: out << int(val) << " (None)"; break;
  case SortMode::kSortMorton: out << int(val) << " (Morton)"; break;
  case SortMode::kSortAzimuth: out << int(val) << " (Azimuth)"; break;
  case SortMode::kSortRadius: out << int(val) << " (Radius)"; break;
  case SortMode::kSortLaserAngle: out << int(val) << " (LaserAngle)"; break;
  default: out << int(val) << " (Unknown)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const OctreeEncOpts::QpMethod& val)
{
  switch (val) {
    using Method = OctreeEncOpts::QpMethod;
  case Method::kUniform: out << int(val) << " (Uniform)"; break;
  case Method::kRandom: out << int(val) << " (Random)"; break;
  case Method::kByDensity: out << int(val) << " (ByDensity)"; break;
  default: out << int(val) << " (Unknown)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const InterGeomEncOpts::LPUType& val)
{
  switch (val) {
    using Method = InterGeomEncOpts::LPUType;
  case Method::kRoadObjClassfication:
    out << int(val) << " (RoadObjClassfication)";
    break;
  case Method::kCuboidPartition:
    out << int(val) << " (CuboidPartition)";
    break;
  default: out << int(val) << " (Unknown)"; break;
  }
  return out;
}


static std::ostream&
operator<<(std::ostream& out, const InterGeomEncOpts::MotionSource& val)
{
  switch (val) {
    using Method = InterGeomEncOpts::MotionSource;
  case Method::kExternalGMSrc: out << int(val) << " (ExternalGMSrc)"; break;
  case Method::kInternalLMSGMSrc:
    out << int(val) << " (InternalLMSGMSrc)";
    break;
  case Method::kInternalICPGMSrc:
    out << int(val) << " (InternalICPGMSrc)";
    break;
  default: out << int(val) << " (Unknown)"; break;
  }
  return out;
}



}  // namespace pcc

//============================================================================

double
SequenceCodec::outputScale(const pcc::CloudFrame& frame) const
{
  switch (params->outputSystem) {
  case OutputSystem::kConformance: return 1.;

  case OutputSystem::kExternal:
    // The scaling converts from the frame's unit length to configured output.
    // In terms of specification this is the external coordinate system.
    return frame.outputUnitLength / params->outputUnitLength;
  }
}

//----------------------------------------------------------------------------

pcc::Vec3<double>
SequenceCodec::outputOrigin(const pcc::CloudFrame& frame) const
{
  switch (params->outputSystem) {
  case OutputSystem::kConformance: return 0.;

  case OutputSystem::kExternal: return frame.outputOrigin * outputScale(frame);
  }
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

  if (params->convertColourspace)
    convertToGbr(frame.attrDesc, cloud);

  // the order of the property names must be determined from the sps
  pcc::ply::PropertyNameMap attrNames;
  attrNames.position = axisOrderToPropertyNames(frame.geometry_axis_order);

  // offset frame number
  int frameNum = frame.frameNum + params->firstFrameNum;

  // Dump the decoded colour using the pre inverse scaled geometry
  if (!preInvScalePath.empty()) {
    std::string filename{pcc::expandNum(preInvScalePath, frameNum)};
    pcc::ply::write(cloud, attrNames, 1.0, 0.0, filename, !params->outputBinaryPly);
  }

  auto plyScale = outputScale(frame) / (1 << frame.outputFpBits);
  auto plyOrigin = outputOrigin(frame);
  std::string decName{pcc::expandNum(postInvScalePath, frameNum)};
  if (!pcc::ply::write(
        cloud, attrNames, plyScale, plyOrigin, decName,
        !params->outputBinaryPly)) {
    std::cout << "Error: can't open output file!" << std::endl;
  }
}

void
SequenceCodec::scaleAttributesForInput(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud)
{
  scaleAttributes(attrDescs, cloud, AttrFwdScaler());
}

//----------------------------------------------------------------------------

void
SequenceCodec::scaleAttributesForOutput(
  const std::vector<pcc::AttributeDescription>& attrDescs, pcc::PCCPointSet3& cloud)
{
  scaleAttributes(attrDescs, cloud, AttrInvScaler());
}