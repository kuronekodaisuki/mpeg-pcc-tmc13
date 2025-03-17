#include "gpcclib/params.hpp"
#include "gpcclib/internal/common.hpp"
#include "gpcclib/internal/params.hpp"
#include <new>

namespace gpcclib {

//============================================================================

void
CopyParams(pcc::Oid& dst, const Oid& src)
{
  CopyVector(dst.contents, src.getConents());
}

//---------------------------------------------------------------------------

OidImpl::OidImpl(const uint8_t* contents, size_t contentsSize)
  : Oid()
  , contents_(contents, contents + contentsSize)
{}

OidImpl::OidImpl(const pcc::Oid& rhs)
  : Oid()
  , contents_(rhs.contents)
{}

OidImpl::OidImpl(const Oid& rhs)
  : Oid()
  , contents_(rhs.getConents())
{}

void
OidImpl::release()
{
  delete this;
}

const UInt8VectorAdapterImpl&
OidImpl::getConents() const
{
  return contents_;
}

UInt8VectorAdapterImpl&
OidImpl::getConents()
{
  return contents_;
}

//---------------------------------------------------------------------------

Oid*
Oid::create()
{
  try {
    return new OidImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

Oid*
Oid::create(const uint8_t* contents, size_t contentsSize)
{
  try {
    return new OidImpl(contents, contentsSize);
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::AttributeLabel& dst, const AttributeLabel& src)
{
  dst.known_attribute_label = src.getKnownAttributeLabel();
  CopyParams(dst.oid, src.getOid());
}

//---------------------------------------------------------------------------

AttributeLabelImpl::AttributeLabelImpl(const pcc::AttributeLabel& rhs)
  : AttributeLabel()
  , known_attribute_label(rhs.known_attribute_label)
  , oid(rhs.oid)
{}

AttributeLabelImpl::AttributeLabelImpl(const AttributeLabel& rhs)
  : AttributeLabel()
  , known_attribute_label(rhs.getKnownAttributeLabel())
  , oid(rhs.getOid())
{}

void
AttributeLabelImpl::release()
{
  delete this;
}

KnownAttributeLabel
AttributeLabelImpl::getKnownAttributeLabel() const
{
  return known_attribute_label;
}

void
AttributeLabelImpl::setKnownAttributeLabel(KnownAttributeLabel label)
{
  known_attribute_label = label;
}

const OidImpl&
AttributeLabelImpl::getOid() const
{
  return oid;
}

OidImpl&
AttributeLabelImpl::getOid()
{
  return oid;
}

//---------------------------------------------------------------------------

AttributeLabel*
AttributeLabel::create()
{
  try {
    return new AttributeLabelImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

AttributeLabel*
AttributeLabel::create(KnownAttributeLabel known_attribute_label)
{
  try {
    AttributeLabelImpl* attributeLabel = new AttributeLabelImpl();
    attributeLabel->setKnownAttributeLabel(known_attribute_label);
    return attributeLabel;
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(
  pcc::OpaqueAttributeParameter& dst, const OpaqueAttributeParameter& src)
{
  dst.attr_param_type = src.getAttrParamType();
  dst.attr_param_itu_t_t35_country_code = src.getAttrParamItuTT35CountryCode();
  dst.attr_param_itu_t_t35_country_code_extension =
    src.getAttrParamItuTT35CountryCodeExtension();
  CopyParams(dst.attr_param_oid, src.getAttrParamOid());
  CopyVector(dst.attr_param_byte, src.getAttrParamByte());
}

//---------------------------------------------------------------------------

OpaqueAttributeParameterImpl::OpaqueAttributeParameterImpl(
  const pcc::OpaqueAttributeParameter& rhs)
  : OpaqueAttributeParameter()
  , attr_param_type(rhs.attr_param_type)
  , attr_param_itu_t_t35_country_code(rhs.attr_param_itu_t_t35_country_code)
  , attr_param_itu_t_t35_country_code_extension(
      rhs.attr_param_itu_t_t35_country_code_extension)
  , attr_param_oid(rhs.attr_param_oid)
  , attr_param_byte(rhs.attr_param_byte)
{}

OpaqueAttributeParameterImpl::OpaqueAttributeParameterImpl(
  const OpaqueAttributeParameter& rhs)
  : OpaqueAttributeParameter()
  , attr_param_type(rhs.getAttrParamType())
  , attr_param_itu_t_t35_country_code(rhs.getAttrParamItuTT35CountryCode())
  , attr_param_itu_t_t35_country_code_extension(
      rhs.getAttrParamItuTT35CountryCodeExtension())
  , attr_param_oid(rhs.getAttrParamOid())
  , attr_param_byte(rhs.getAttrParamByte())
{}

void
OpaqueAttributeParameterImpl::release()
{
  delete this;
}

AttributeParameterType
OpaqueAttributeParameterImpl::getAttrParamType() const
{
  return attr_param_type;
}

void
OpaqueAttributeParameterImpl::setAttrParamType(AttributeParameterType type)
{
  attr_param_type = type;
}

int
OpaqueAttributeParameterImpl::getAttrParamItuTT35CountryCode() const
{
  return attr_param_itu_t_t35_country_code;
}

void
OpaqueAttributeParameterImpl::setAttrParamItuTT35CountryCode(int code)
{
  attr_param_itu_t_t35_country_code = code;
}

int
OpaqueAttributeParameterImpl::getAttrParamItuTT35CountryCodeExtension() const
{
  return attr_param_itu_t_t35_country_code_extension;
}

void
OpaqueAttributeParameterImpl::setAttrParamItuTT35CountryCodeExtension(int val)
{
  attr_param_itu_t_t35_country_code_extension = val;
}

const OidImpl&
OpaqueAttributeParameterImpl::getAttrParamOid() const
{
  return attr_param_oid;
}

OidImpl&
OpaqueAttributeParameterImpl::getAttrParamOid()
{
  return attr_param_oid;
}

const UInt8VectorAdapterImpl&
OpaqueAttributeParameterImpl::getAttrParamByte() const
{
  return attr_param_byte;
}

UInt8VectorAdapterImpl&
OpaqueAttributeParameterImpl::getAttrParamByte()
{
  return attr_param_byte;
}

//---------------------------------------------------------------------------

OpaqueAttributeParameter*
OpaqueAttributeParameter::create()
{
  try {
    return new OpaqueAttributeParameterImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::AttributeParameters& dst, const AttributeParameters& src)
{
  dst.cicpParametersPresent = src.getCicpParametersPresent();
  dst.cicp_colour_primaries_idx = src.getCicpColourPrimariesIdx();
  dst.cicp_transfer_characteristics_idx =
    src.getCicpTransferCharacteristicsIdx();
  dst.cicp_matrix_coefficients_idx = src.getCicpMatrixCoefficientsIdx();
  dst.cicp_video_full_range_flag = src.getCicpVideoFullRangeFlag();
  dst.scalingParametersPresent = src.getScalingParametersPresent();
  dst.attr_scale_minus1 = src.getAttrScaleMinus1();
  dst.attr_frac_bits = src.getAttrFracBits();
  dst.attr_offset = src.getAttrOffset();

  CopyVector(dst.attr_default_value, src.getAttrDefaultValue());

  CopyObjectVector(dst.opaqueParameters, src.getOpaqueParameters());
}

//---------------------------------------------------------------------------

AttributeParametersImpl::AttributeParametersImpl(
  const pcc::AttributeParameters& rhs)
  : AttributeParameters()
  , cicpParametersPresent(rhs.cicpParametersPresent)
  , cicp_colour_primaries_idx(rhs.cicp_colour_primaries_idx)
  , cicp_transfer_characteristics_idx(rhs.cicp_transfer_characteristics_idx)
  , cicp_matrix_coefficients_idx(rhs.cicp_matrix_coefficients_idx)
  , cicp_video_full_range_flag(rhs.cicp_video_full_range_flag)
  , scalingParametersPresent(rhs.scalingParametersPresent)
  , attr_scale_minus1(rhs.attr_scale_minus1)
  , attr_frac_bits(rhs.attr_frac_bits)
  , attr_offset(rhs.attr_offset)
  , attr_default_value(rhs.attr_default_value)
//, opaqueParameters(rhs.opaqueParameters)
{
  for (const auto& o : rhs.opaqueParameters) {
    opaqueParameters.emplaceBack(OpaqueAttributeParameterImpl(o));
  }
}

AttributeParametersImpl::AttributeParametersImpl(
  const AttributeParameters& rhs)
  : AttributeParameters()
  , cicpParametersPresent(rhs.getCicpParametersPresent())
  , cicp_colour_primaries_idx(rhs.getCicpColourPrimariesIdx())
  , cicp_transfer_characteristics_idx(rhs.getCicpTransferCharacteristicsIdx())
  , cicp_matrix_coefficients_idx(rhs.getCicpMatrixCoefficientsIdx())
  , cicp_video_full_range_flag(rhs.getCicpVideoFullRangeFlag())
  , scalingParametersPresent(rhs.getScalingParametersPresent())
  , attr_scale_minus1(rhs.getAttrScaleMinus1())
  , attr_frac_bits(rhs.getAttrFracBits())
  , attr_offset(rhs.getAttrOffset())
  , attr_default_value(rhs.getAttrDefaultValue())
//, opaqueParameters(rhs.opaqueParameters)
{
  for (size_t i = 0; i < rhs.getOpaqueParameters().getSize(); ++i) {
    opaqueParameters.emplaceBack(
      OpaqueAttributeParameterImpl(rhs.getOpaqueParameters().at(i)));
  }
}

void
AttributeParametersImpl::release()
{
  delete this;
}

// bool cicpParametersPresent;
bool
AttributeParametersImpl::getCicpParametersPresent() const
{
  return cicpParametersPresent;
}

void
AttributeParametersImpl::setCicpParametersPresent(bool val)
{
  cicpParametersPresent = val;
}

// int cicp_colour_primaries_idx;
int
AttributeParametersImpl::getCicpColourPrimariesIdx() const
{
  return cicp_colour_primaries_idx;
}

void
AttributeParametersImpl::setCicpColourPrimariesIdx(int idx)
{
  cicp_colour_primaries_idx = idx;
}

// int cicp_transfer_characteristics_idx;
int
AttributeParametersImpl::getCicpTransferCharacteristicsIdx() const
{
  return cicp_transfer_characteristics_idx;
}

void
AttributeParametersImpl::setCicpTransferCharacteristicsIdx(int idx)
{
  cicp_transfer_characteristics_idx = idx;
}

// ColourMatrix cicp_matrix_coefficients_idx;
ColourMatrix
AttributeParametersImpl::getCicpMatrixCoefficientsIdx() const
{
  return cicp_matrix_coefficients_idx;
}

void
AttributeParametersImpl::setCicpMatrixCoefficientsIdx(ColourMatrix idx)
{
  cicp_matrix_coefficients_idx = idx;
}

// bool cicp_video_full_range_flag;
bool
AttributeParametersImpl::getCicpVideoFullRangeFlag() const
{
  return cicp_video_full_range_flag;
}

void
AttributeParametersImpl::setCicpVideoFullRangeFlag(bool flag)
{
  cicp_video_full_range_flag = flag;
}

// bool scalingParametersPresent;
bool
AttributeParametersImpl::getScalingParametersPresent() const
{
  return scalingParametersPresent;
}

void
AttributeParametersImpl::setScalingParametersPresent(bool val)
{
  scalingParametersPresent = val;
}

// int attr_scale_minus1;
int
AttributeParametersImpl::getAttrScaleMinus1() const
{
  return attr_scale_minus1;
}

void
AttributeParametersImpl::setAttrScaleMinus1(int val)
{
  attr_scale_minus1 = val;
}

// int attr_frac_bits;
int
AttributeParametersImpl::getAttrFracBits() const
{
  return attr_frac_bits;
}

void
AttributeParametersImpl::setAttrFracBits(int bits)
{
  attr_frac_bits = bits;
}

// int attr_offset;
int
AttributeParametersImpl::getAttrOffset() const
{
  return attr_offset;
}

void
AttributeParametersImpl::setAttrOffset(int offset)
{
  attr_offset = offset;
}

// std::vector<int> attr_default_value;
const Int32VectorAdapterImpl&
AttributeParametersImpl::getAttrDefaultValue() const
{
  return attr_default_value;
}

Int32VectorAdapterImpl&
AttributeParametersImpl::getAttrDefaultValue()
{
  return attr_default_value;
}

// std::vector<OpaqueAttributeParameter> opaqueParameters;
const OpaqueAttributeParameterImplVector&
AttributeParametersImpl::getOpaqueParameters() const
{
  return opaqueParameters;
}

OpaqueAttributeParameterImplVector&
AttributeParametersImpl::getOpaqueParameters()
{
  return opaqueParameters;
}

void
AttributeParametersImpl::clear()
{
  cicpParametersPresent = false;
  scalingParametersPresent = false;
  attr_default_value.clear();
  opaqueParameters.clear();
}

//---------------------------------------------------------------------------

AttributeParameters*
AttributeParameters::create()
{
  try {
    return new AttributeParametersImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::AttributeDescription& dst, const AttributeDescription& src)
{
  dst.attr_num_dimensions_minus1 = src.getAttrNumDimensionsMinus1();
  dst.attr_instance_id = src.getAttrInstanceId();
  dst.bitdepth = src.getBitDepth();
  CopyParams(dst.attributeLabel, src.getAttributeLabel());
  CopyParams(dst.params, src.getParams());
}

//---------------------------------------------------------------------------

AttributeDescriptionImpl::AttributeDescriptionImpl(
  const pcc::AttributeDescription& rhs)
  : AttributeDescription()
  , attr_num_dimensions_minus1(rhs.attr_num_dimensions_minus1)
  , attr_instance_id(rhs.attr_instance_id)
  , bitdepth(rhs.bitdepth)
  , attributeLabel(rhs.attributeLabel)
  , params(rhs.params)
{}

AttributeDescriptionImpl::AttributeDescriptionImpl(
  const AttributeDescription& rhs)
  : AttributeDescription()
  , attr_num_dimensions_minus1(rhs.getAttrNumDimensionsMinus1())
  , attr_instance_id(rhs.getAttrInstanceId())
  , bitdepth(rhs.getBitDepth())
  , attributeLabel(rhs.getAttributeLabel())
  , params(rhs.getParams())
{}

void
AttributeDescriptionImpl::release()
{
  delete this;
}

// int attr_num_dimensions_minus1;
int
AttributeDescriptionImpl::getAttrNumDimensionsMinus1() const
{
  return attr_num_dimensions_minus1;
}

void
AttributeDescriptionImpl::setAttrNumDimensionsMinus1(int num)
{
  attr_num_dimensions_minus1 = num;
}

// int attr_instance_id;
int
AttributeDescriptionImpl::getAttrInstanceId() const
{
  return attr_instance_id;
}

void
AttributeDescriptionImpl::setAttrInstanceId(int id)
{
  attr_instance_id = id;
}

// int bitdepth;
int
AttributeDescriptionImpl::getBitDepth() const
{
  return bitdepth;
}

void
AttributeDescriptionImpl::setBitDepth(int depth)
{
  bitdepth = depth;
}

// AttributeLabel attributeLabel;
const AttributeLabelImpl&
AttributeDescriptionImpl::getAttributeLabel() const
{
  return attributeLabel;
}

AttributeLabelImpl&
AttributeDescriptionImpl::getAttributeLabel()
{
  return attributeLabel;
}

// AttributeParameters params;
const AttributeParametersImpl&
AttributeDescriptionImpl::getParams() const
{
  return params;
}

AttributeParametersImpl&
AttributeDescriptionImpl::getParams()
{
  return params;
}

//---------------------------------------------------------------------------

AttributeDescription*
AttributeDescription::create()
{
  try {
    return new AttributeDescriptionImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::ProfileCompatibility& dst, const ProfileCompatibility& src)
{
  dst.main_profile_compatibility_flag = src.getMainProfileCompatiblityFlag();
  dst.reserved_profile_compatibility_21bits =
    src.getReservedProfileCompatibility21Bits();
  dst.slice_reordering_constraint_flag =
    src.getSliceReorderingConstraintFlag();
  dst.unique_point_positions_constraint_flag =
    src.getUniquePointPositionsConstraintFlag();
}

//---------------------------------------------------------------------------

ProfileCompatibilityImpl::ProfileCompatibilityImpl(
  const pcc::ProfileCompatibility& rhs)
  : main_profile_compatibility_flag(rhs.main_profile_compatibility_flag)
  , reserved_profile_compatibility_21bits(
      rhs.reserved_profile_compatibility_21bits)
  , slice_reordering_constraint_flag(rhs.slice_reordering_constraint_flag)
  , unique_point_positions_constraint_flag(
      rhs.unique_point_positions_constraint_flag)
{}

ProfileCompatibilityImpl::ProfileCompatibilityImpl(
  const ProfileCompatibility& rhs)
  : main_profile_compatibility_flag(rhs.getMainProfileCompatiblityFlag())
  , reserved_profile_compatibility_21bits(
      rhs.getReservedProfileCompatibility21Bits())
  , slice_reordering_constraint_flag(rhs.getSliceReorderingConstraintFlag())
  , unique_point_positions_constraint_flag(
      rhs.getUniquePointPositionsConstraintFlag())
{}

void
ProfileCompatibilityImpl::release()
{
  delete this;
}

// bool main_profile_compatibility_flag;
bool
ProfileCompatibilityImpl::getMainProfileCompatiblityFlag() const
{
  return main_profile_compatibility_flag;
}
void
ProfileCompatibilityImpl::setMainProfileCompatiblityFlag(bool compatible)
{
  main_profile_compatibility_flag = compatible;
}

// int reserved_profile_compatibility_21bits;
int
ProfileCompatibilityImpl::getReservedProfileCompatibility21Bits() const
{
  return reserved_profile_compatibility_21bits;
}
void
ProfileCompatibilityImpl::setReservedProfileCompatibility21Bits(int bits)
{
  reserved_profile_compatibility_21bits = bits;
}

// bool slice_reordering_constraint_flag;
bool
ProfileCompatibilityImpl::getSliceReorderingConstraintFlag() const
{
  return slice_reordering_constraint_flag;
}
void
ProfileCompatibilityImpl::setSliceReorderingConstraintFlag(bool flag)
{
  slice_reordering_constraint_flag = flag;
}

// bool unique_point_positions_constraint_flag;
bool
ProfileCompatibilityImpl::getUniquePointPositionsConstraintFlag() const
{
  return unique_point_positions_constraint_flag;
}
void
ProfileCompatibilityImpl::setUniquePointPositionsConstraintFlag(bool flag)
{
  unique_point_positions_constraint_flag = flag;
}

//---------------------------------------------------------------------------

ProfileCompatibility*
ProfileCompatibility::create()
{
  try {
    return new ProfileCompatibilityImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(
  pcc::SequenceParameterSet::GlobalScale& dst,
  const SequenceParameterSet::GlobalScale& src)
{
  dst.numeratorMulLog2 = src.getNumeratorMulLog2();
  dst.numeratorModDenominator = src.getNumeratorModDenominator();
  dst.denominatorLog2 = src.getDenominatorLog2();
}

//---------------------------------------------------------------------------

GlobalScaleImpl::GlobalScaleImpl(
  const pcc::SequenceParameterSet::GlobalScale& rhs)
  : numeratorMulLog2(rhs.numeratorMulLog2)
  , numeratorModDenominator(rhs.numeratorModDenominator)
  , denominatorLog2(rhs.denominatorLog2)
{}

GlobalScaleImpl::GlobalScaleImpl(const SequenceParameterSet::GlobalScale& rhs)
  : numeratorMulLog2(rhs.getNumeratorMulLog2())
  , numeratorModDenominator(rhs.getNumeratorModDenominator())
  , denominatorLog2(rhs.getDenominatorLog2())
{}

//int numeratorMulLog2 = 0;
int
GlobalScaleImpl::getNumeratorMulLog2() const
{
  return numeratorMulLog2;
}
void
GlobalScaleImpl::setNumeratorMulLog2(int val)
{
  numeratorMulLog2 = val;
}

// int numeratorModDenominator = 0;
int
GlobalScaleImpl::getNumeratorModDenominator() const
{
  return numeratorModDenominator;
}
void
GlobalScaleImpl::setNumeratorModDenominator(int val)
{
  numeratorModDenominator = val;
}

// int denominatorLog2 = 0;
int
GlobalScaleImpl::getDenominatorLog2() const
{
  return denominatorLog2;
}
void
GlobalScaleImpl::setDenominatorLog2(int val)
{
  denominatorLog2 = val;
}

//---------------------------------------------------------------------------

void
CopyParams(pcc::SequenceParameterSet& dst, const SequenceParameterSet& src)
{
  dst.sps_seq_parameter_set_id = src.getSpsSeqParameterSetId();
  CopyParams(dst.profile, src.getProfile());
  dst.level = src.getLevel();

  dst.sps_bounding_box_offset_bits = src.getSpsBoundingBoxOffsetBits();
  dst.seqBoundingBoxOrigin = pcc::Vec3<int>(src.getSeqBoundingBoxOrigin());
  dst.sps_bounding_box_size_bits = src.getSpsBoundingBoxSizeBits();
  dst.seqBoundingBoxSize = pcc::Vec3<int>(src.getSeqBoundingBoxSize());

  dst.seqGeomScale = src.getSeqGeomScale();
  dst.seq_geom_scale_unit_flag = src.getSeqGeomScaleUnit();
  CopyParams(dst.globalScale, src.getGlobalScale());

  CopyObjectVector(dst.attributeSets, src.getAttributeSets());

  dst.frame_ctr_bits = src.getFrameCtrBits();
  dst.slice_tag_bits = src.getSliceTagBits();

  dst.geometry_axis_order = src.getGeometryAxisOrder();
  dst.cabac_bypass_stream_enabled_flag = src.isCabacBypassStreamEnabled();
  dst.entropy_continuation_enabled_flag = src.isEntropyContinuationEnabled();
}

//---------------------------------------------------------------------------
SequenceParameterSetImpl::SequenceParameterSetImpl(
  const pcc::SequenceParameterSet& rhs)
  : sps_seq_parameter_set_id(rhs.sps_seq_parameter_set_id)
  , profile(rhs.profile)
  , level(rhs.level)
  , sps_bounding_box_offset_bits(rhs.sps_bounding_box_offset_bits)
  , seqBoundingBoxOrigin(rhs.seqBoundingBoxOrigin)
  , sps_bounding_box_size_bits(rhs.sps_bounding_box_size_bits)
  , seqBoundingBoxSize(rhs.seqBoundingBoxSize)
  , seqGeomScale(rhs.seqGeomScale)
  , seq_geom_scale_unit_flag(rhs.seq_geom_scale_unit_flag)
  , globalScale(rhs.globalScale)
  // , attributeSets(rhs.attributeSets)
  , frame_ctr_bits(rhs.frame_ctr_bits)
  , slice_tag_bits(rhs.slice_tag_bits)
  , geometry_axis_order(rhs.geometry_axis_order)
  , cabac_bypass_stream_enabled_flag(rhs.cabac_bypass_stream_enabled_flag)
  , entropy_continuation_enabled_flag(rhs.entropy_continuation_enabled_flag)
{
  for (const auto& a : rhs.attributeSets) {
    attributeSets.emplaceBack(AttributeDescriptionImpl(a));
  }
}

SequenceParameterSetImpl::SequenceParameterSetImpl(
  const SequenceParameterSet& rhs)
  : sps_seq_parameter_set_id(rhs.getSpsSeqParameterSetId())
  , profile(rhs.getProfile())
  , level(rhs.getLevel())
  , sps_bounding_box_offset_bits(rhs.getSpsBoundingBoxOffsetBits())
  , seqBoundingBoxOrigin(rhs.getSeqBoundingBoxOrigin())
  , sps_bounding_box_size_bits(rhs.getSpsBoundingBoxSizeBits())
  , seqBoundingBoxSize(rhs.getSeqBoundingBoxSize())
  , seqGeomScale(rhs.getSeqGeomScale())
  , seq_geom_scale_unit_flag(rhs.getSeqGeomScaleUnit())
  , globalScale(rhs.getGlobalScale())
  // , attributeSets(rhs.attributeSets)
  , frame_ctr_bits(rhs.getFrameCtrBits())
  , slice_tag_bits(rhs.getSliceTagBits())
  , geometry_axis_order(rhs.getGeometryAxisOrder())
  , cabac_bypass_stream_enabled_flag(rhs.isCabacBypassStreamEnabled())
  , entropy_continuation_enabled_flag(rhs.isEntropyContinuationEnabled())
{
  for (size_t i = 0; i < rhs.getAttributeSets().getSize(); ++i) {
    attributeSets.emplaceBack(
      AttributeDescriptionImpl(rhs.getAttributeSets().at(i)));
  }
}

void
SequenceParameterSetImpl::release()
{
  delete this;
}

// int sps_seq_parameter_set_id;
int
SequenceParameterSetImpl::getSpsSeqParameterSetId() const
{
  return sps_seq_parameter_set_id;
}
void
SequenceParameterSetImpl::setSpsSeqParameterSetId(int id)
{
  sps_seq_parameter_set_id = id;
}

// ProfileCompatibility profile;
const ProfileCompatibility&
SequenceParameterSetImpl::getProfile() const
{
  return profile;
}
ProfileCompatibility&
SequenceParameterSetImpl::getProfile()
{
  return profile;
}

// int level;
int
SequenceParameterSetImpl::getLevel() const
{
  return level;
}
void
SequenceParameterSetImpl::setLevel(int val)
{
  level = val;
}

// int sps_bounding_box_offset_bits;
int
SequenceParameterSetImpl::getSpsBoundingBoxOffsetBits() const
{
  return sps_bounding_box_offset_bits;
}
void
SequenceParameterSetImpl::setSpsBoundingBoxOffsetBits(int bits)
{
  sps_bounding_box_offset_bits = bits;
}

// Vec3<int> seqBoundingBoxOrigin;
const point_t&
SequenceParameterSetImpl::getSeqBoundingBoxOrigin() const
{
  return seqBoundingBoxOrigin;
}
point_t&
SequenceParameterSetImpl::getSeqBoundingBoxOrigin()
{
  return seqBoundingBoxOrigin;
}
void
SequenceParameterSetImpl::setSeqBoundingBoxOrigin(const point_t& origin)
{
  seqBoundingBoxOrigin = origin;
}

// int sps_bounding_box_size_bits;
int
SequenceParameterSetImpl::getSpsBoundingBoxSizeBits() const
{
  return sps_bounding_box_size_bits;
}
void
SequenceParameterSetImpl::setSpsBoundingBoxSizeBits(int bits)
{
  sps_bounding_box_size_bits = bits;
}

// Vec3<int> seqBoundingBoxSize;
const point_t&
SequenceParameterSetImpl::getSeqBoundingBoxSize() const
{
  return seqBoundingBoxSize;
}
point_t&
SequenceParameterSetImpl::getSeqBoundingBoxSize()
{
  return seqBoundingBoxSize;
}
void
SequenceParameterSetImpl::setSeqBoundingBoxSize(const point_t& size)
{
  seqBoundingBoxSize = size;
}

// Rational seqGeomScale;
const Rational&
SequenceParameterSetImpl::getSeqGeomScale() const
{
  return seqGeomScale;
}
Rational&
SequenceParameterSetImpl::getSeqGeomScale()
{
  return seqGeomScale;
}

// ScaleUnit seq_geom_scale_unit_flag;
ScaleUnit
SequenceParameterSetImpl::getSeqGeomScaleUnit() const
{
  return seq_geom_scale_unit_flag;
}
void
SequenceParameterSetImpl::setSeqGeomScaleUnit(ScaleUnit scaleUnit)
{
  seq_geom_scale_unit_flag = scaleUnit;
}

// GlobalScale globalScale;
const GlobalScaleImpl&
SequenceParameterSetImpl::getGlobalScale() const
{
  return globalScale;
}
GlobalScaleImpl&
SequenceParameterSetImpl::getGlobalScale()
{
  return globalScale;
}

// std::vector<AttributeDescription> attributeSets;
const AttributeDescriptionImplVector&
SequenceParameterSetImpl::getAttributeSets() const
{
  return attributeSets;
}
AttributeDescriptionImplVector&
SequenceParameterSetImpl::getAttributeSets()
{
  return attributeSets;
}

// int frame_ctr_bits;
int
SequenceParameterSetImpl::getFrameCtrBits() const
{
  return frame_ctr_bits;
}
void
SequenceParameterSetImpl::setFrameCtrBits(int bits)
{
  frame_ctr_bits = bits;
}

// int slice_tag_bits;
int
SequenceParameterSetImpl::getSliceTagBits() const
{
  return slice_tag_bits;
}
void
SequenceParameterSetImpl::setSliceTagBits(int bits)
{
  slice_tag_bits = bits;
}

// AxisOrder geometry_axis_order;
AxisOrder
SequenceParameterSetImpl::getGeometryAxisOrder() const
{
  return geometry_axis_order;
}
void
SequenceParameterSetImpl::setGeometryAxisOrder(AxisOrder axisOrder)
{
  geometry_axis_order = axisOrder;
}

// bool cabac_bypass_stream_enabled_flag;
bool
SequenceParameterSetImpl::isCabacBypassStreamEnabled() const
{
  return cabac_bypass_stream_enabled_flag;
}
void
SequenceParameterSetImpl::setCabacBypassStreamEnabled(bool enabled)
{
  cabac_bypass_stream_enabled_flag = enabled;
}

// bool entropy_continuation_enabled_flag;
bool
SequenceParameterSetImpl::isEntropyContinuationEnabled() const
{
  return entropy_continuation_enabled_flag;
}
void
SequenceParameterSetImpl::setEntropyContinuationEnabled(bool enabled)
{
  entropy_continuation_enabled_flag = enabled;
}

//---------------------------------------------------------------------------

SequenceParameterSet*
SequenceParameterSet::create()
{
  try {
    return new SequenceParameterSetImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::GeometryParameterSet& dst, const GeometryParameterSet& src)
{
  dst.gps_geom_parameter_set_id = src.getGpsGeomParameterSetId();
  dst.gps_seq_parameter_set_id = src.getGpsSeqParameterSetId();

  dst.geom_box_log2_scale_present_flag = src.getGeomBoxLog2ScalePresentFlag();

  dst.gps_geom_box_log2_scale = src.getGpsGeomBoxLog2Scale();

  dst.predgeom_enabled_flag = src.isPredgeomEnabled();

  dst.geom_unique_points_flag = src.getGeomUniquePointsFlag();

  dst.neighbour_avail_boundary_log2_minus1 =
    src.getNeighbourAvailBoundaryLog2Minus1();

  dst.inferred_direct_coding_mode = src.getInferredDirectCodingMode();

  dst.joint_2pt_idcm_enabled_flag = src.isJoint2ptIdcmEnabled();

  dst.bitwise_occupancy_coding_flag = src.getBitwiseOccupancyCodingFlag();

  dst.adjacent_child_contextualization_enabled_flag =
    src.isAdjacentChildContextualizationEnabled();

  dst.intra_pred_max_node_size_log2 = src.getIntraPredMaxNodeSizeLog2();

  dst.trisoup_enabled_flag = src.isTrisoupEnabled();

  dst.trisoup_sampling_value = src.getTrisoupSamplingValue();

  dst.geom_scaling_enabled_flag = src.isGeomScalingEnabled();

  dst.geom_qp_multiplier_log2 = src.getGeomQpMultiplierLog2();

  dst.geom_base_qp = src.getGeomBaseQp();

  dst.geom_idcm_qp_offset = src.getGeomIdcmQpOffset();

  dst.qtbt_enabled_flag = src.isQtbtEnabled();

  dst.geom_planar_mode_enabled_flag = src.isGeomPlanarModeEnabled();
  dst.geom_planar_threshold0 = src.getGeomPlanarThreshold0();
  dst.geom_planar_threshold1 = src.getGeomPlanarThreshold1();
  dst.geom_planar_threshold2 = src.getGeomPlanarThreshold2();
  dst.geom_idcm_rate_minus1 = src.getGeomIdcmRateMinus1();

  dst.geom_angular_mode_enabled_flag = src.isGeomAngularModeEnabled();

  dst.geom_slice_angular_origin_present_flag =
    src.getGeomSliceAngularOriginPresentFlag();

  dst.gpsAngularOrigin = pcc::Vec3<int>(src.getGpsAngularOrigin());

  CopyVector(dst.angularTheta, src.getAngularTheta());
  CopyVector(dst.angularZ, src.getAngularZ());
  CopyVector(dst.angularNumPhiPerTurn, src.getAngularNumPhiPerTurn());

  dst.planar_buffer_disabled_flag = src.isPlanarBufferDisabled();

  dst.geom_qp_offset_intvl_log2 = src.getGeomQpOffsetIntvlLog2();

  dst.geom_angular_azimuth_scale_log2_minus11 =
    src.getGeomAngularAzimuthScaleLog2Minus11();
  dst.geom_angular_azimuth_speed_minus1 =
    src.getGeomAngularAzimuthSpeedMinus1();

  dst.geom_angular_radius_inv_scale_log2 =
    src.getGeomAngularRadiusInvScaleLog2();

  dst.octree_point_count_list_present_flag =
    src.getOctreePointCountListPresentFlag();
}

//---------------------------------------------------------------------------

GeometryParameterSetImpl::GeometryParameterSetImpl(
  const pcc::GeometryParameterSet& rhs)
  : gps_geom_parameter_set_id(rhs.gps_geom_parameter_set_id)
  , gps_seq_parameter_set_id(rhs.gps_seq_parameter_set_id)
  , geom_box_log2_scale_present_flag(rhs.geom_box_log2_scale_present_flag)
  , gps_geom_box_log2_scale(rhs.gps_geom_box_log2_scale)
  , predgeom_enabled_flag(rhs.predgeom_enabled_flag)
  , geom_unique_points_flag(rhs.geom_unique_points_flag)
  , neighbour_avail_boundary_log2_minus1(
      rhs.neighbour_avail_boundary_log2_minus1)
  , inferred_direct_coding_mode(rhs.inferred_direct_coding_mode)
  , joint_2pt_idcm_enabled_flag(rhs.joint_2pt_idcm_enabled_flag)
  , bitwise_occupancy_coding_flag(rhs.bitwise_occupancy_coding_flag)
  , adjacent_child_contextualization_enabled_flag(
      rhs.adjacent_child_contextualization_enabled_flag)
  , intra_pred_max_node_size_log2(rhs.intra_pred_max_node_size_log2)
  , trisoup_enabled_flag(rhs.trisoup_enabled_flag)
  , trisoup_sampling_value(rhs.trisoup_sampling_value)
  , geom_scaling_enabled_flag(rhs.geom_scaling_enabled_flag)
  , geom_qp_multiplier_log2(rhs.geom_qp_multiplier_log2)
  , geom_base_qp(rhs.geom_base_qp)
  , geom_idcm_qp_offset(rhs.geom_idcm_qp_offset)
  , qtbt_enabled_flag(rhs.qtbt_enabled_flag)
  , geom_planar_mode_enabled_flag(rhs.geom_planar_mode_enabled_flag)
  , geom_planar_threshold0(rhs.geom_planar_threshold0)
  , geom_planar_threshold1(rhs.geom_planar_threshold1)
  , geom_planar_threshold2(rhs.geom_planar_threshold2)
  , geom_idcm_rate_minus1(rhs.geom_idcm_rate_minus1)
  , geom_angular_mode_enabled_flag(rhs.geom_angular_mode_enabled_flag)
  , geom_slice_angular_origin_present_flag(
      rhs.geom_slice_angular_origin_present_flag)
  , gpsAngularOrigin(rhs.gpsAngularOrigin)
  , angularTheta(rhs.angularTheta)
  , angularZ(rhs.angularZ)
  , angularNumPhiPerTurn(rhs.angularNumPhiPerTurn)
  , planar_buffer_disabled_flag(rhs.planar_buffer_disabled_flag)
  , geom_qp_offset_intvl_log2(rhs.geom_qp_offset_intvl_log2)
  , geom_angular_azimuth_scale_log2_minus11(
      rhs.geom_angular_azimuth_scale_log2_minus11)
  , geom_angular_azimuth_speed_minus1(rhs.geom_angular_azimuth_speed_minus1)
  , geom_angular_radius_inv_scale_log2(rhs.geom_angular_radius_inv_scale_log2)
  , octree_point_count_list_present_flag(
      rhs.octree_point_count_list_present_flag)
{}

GeometryParameterSetImpl::GeometryParameterSetImpl(
  const GeometryParameterSet& rhs)
  : gps_geom_parameter_set_id(rhs.getGpsGeomParameterSetId())
  , gps_seq_parameter_set_id(rhs.getGpsSeqParameterSetId())
  , geom_box_log2_scale_present_flag(rhs.getGeomBoxLog2ScalePresentFlag())
  , gps_geom_box_log2_scale(rhs.getGpsGeomBoxLog2Scale())
  , predgeom_enabled_flag(rhs.isPredgeomEnabled())
  , geom_unique_points_flag(rhs.getGeomUniquePointsFlag())
  , neighbour_avail_boundary_log2_minus1(
      rhs.getNeighbourAvailBoundaryLog2Minus1())
  , inferred_direct_coding_mode(rhs.getInferredDirectCodingMode())
  , joint_2pt_idcm_enabled_flag(rhs.isJoint2ptIdcmEnabled())
  , bitwise_occupancy_coding_flag(rhs.getBitwiseOccupancyCodingFlag())
  , adjacent_child_contextualization_enabled_flag(
      rhs.isAdjacentChildContextualizationEnabled())
  , intra_pred_max_node_size_log2(rhs.getIntraPredMaxNodeSizeLog2())
  , trisoup_enabled_flag(rhs.isTrisoupEnabled())
  , trisoup_sampling_value(rhs.getTrisoupSamplingValue())
  , geom_scaling_enabled_flag(rhs.isGeomScalingEnabled())
  , geom_qp_multiplier_log2(rhs.getGeomQpMultiplierLog2())
  , geom_base_qp(rhs.getGeomBaseQp())
  , geom_idcm_qp_offset(rhs.getGeomIdcmQpOffset())
  , qtbt_enabled_flag(rhs.isQtbtEnabled())
  , geom_planar_mode_enabled_flag(rhs.isGeomPlanarModeEnabled())
  , geom_planar_threshold0(rhs.getGeomPlanarThreshold0())
  , geom_planar_threshold1(rhs.getGeomPlanarThreshold1())
  , geom_planar_threshold2(rhs.getGeomPlanarThreshold2())
  , geom_idcm_rate_minus1(rhs.getGeomIdcmRateMinus1())
  , geom_angular_mode_enabled_flag(rhs.isGeomAngularModeEnabled())
  , geom_slice_angular_origin_present_flag(
      rhs.getGeomSliceAngularOriginPresentFlag())
  , gpsAngularOrigin(rhs.getGpsAngularOrigin())
  , angularTheta(rhs.getAngularTheta())
  , angularZ(rhs.getAngularZ())
  , angularNumPhiPerTurn(rhs.getAngularNumPhiPerTurn())
  , planar_buffer_disabled_flag(rhs.isPlanarBufferDisabled())
  , geom_qp_offset_intvl_log2(rhs.getGeomQpOffsetIntvlLog2())
  , geom_angular_azimuth_scale_log2_minus11(
      rhs.getGeomAngularAzimuthScaleLog2Minus11())
  , geom_angular_azimuth_speed_minus1(rhs.getGeomAngularAzimuthSpeedMinus1())
  , geom_angular_radius_inv_scale_log2(rhs.getGeomAngularRadiusInvScaleLog2())
  , octree_point_count_list_present_flag(
      rhs.getOctreePointCountListPresentFlag())
{}

void
GeometryParameterSetImpl::release()
{
  delete this;
}

// int gps_geom_parameter_set_id;
int
GeometryParameterSetImpl::getGpsGeomParameterSetId() const
{
  return gps_geom_parameter_set_id;
}
void
GeometryParameterSetImpl::setGpsGeomParameterSetId(int id)
{
  gps_geom_parameter_set_id = id;
}

// int gps_seq_parameter_set_id;
int
GeometryParameterSetImpl::getGpsSeqParameterSetId() const
{
  return gps_seq_parameter_set_id;
}
void
GeometryParameterSetImpl::setGpsSeqParameterSetId(int id)
{
  gps_seq_parameter_set_id = id;
}

// bool geom_box_log2_scale_present_flag;
bool
GeometryParameterSetImpl::getGeomBoxLog2ScalePresentFlag() const
{
  return geom_box_log2_scale_present_flag;
}
void
GeometryParameterSetImpl::setGeomBoxLog2ScalePresentFlag(bool flag)
{
  geom_box_log2_scale_present_flag = flag;
}

// int gps_geom_box_log2_scale;
int
GeometryParameterSetImpl::getGpsGeomBoxLog2Scale() const
{
  return gps_geom_box_log2_scale;
}
void
GeometryParameterSetImpl::setGpsGeomBoxLog2Scale(int val)
{
  gps_geom_box_log2_scale = val;
}

// bool predgeom_enabled_flag;
bool
GeometryParameterSetImpl::isPredgeomEnabled() const
{
  return predgeom_enabled_flag;
}
void
GeometryParameterSetImpl::setPredgeomEnabled(bool enabled)
{
  predgeom_enabled_flag = enabled;
}

// bool geom_unique_points_flag;
bool
GeometryParameterSetImpl::getGeomUniquePointsFlag() const
{
  return geom_unique_points_flag;
}
void
GeometryParameterSetImpl::setGeomUniquePointsFlag(bool flag)
{
  geom_unique_points_flag = flag;
}

// int neighbour_avail_boundary_log2_minus1;
int
GeometryParameterSetImpl::getNeighbourAvailBoundaryLog2Minus1() const
{
  return neighbour_avail_boundary_log2_minus1;
}
void
GeometryParameterSetImpl::setNeighbourAvailBoundaryLog2Minus1(int val)
{
  neighbour_avail_boundary_log2_minus1 = val;
}

// int inferred_direct_coding_mode;
int
GeometryParameterSetImpl::getInferredDirectCodingMode() const
{
  return inferred_direct_coding_mode;
}
void
GeometryParameterSetImpl::setInferredDirectCodingMode(int val)
{
  inferred_direct_coding_mode = val;
}

// bool joint_2pt_idcm_enabled_flag;
bool
GeometryParameterSetImpl::isJoint2ptIdcmEnabled() const
{
  return joint_2pt_idcm_enabled_flag;
}
void
GeometryParameterSetImpl::setJoint2ptIdcmEnabled(bool enabled)
{
  joint_2pt_idcm_enabled_flag = enabled;
}

// bool bitwise_occupancy_coding_flag;
bool
GeometryParameterSetImpl::getBitwiseOccupancyCodingFlag() const
{
  return bitwise_occupancy_coding_flag;
}
void
GeometryParameterSetImpl::setBitwiseOccupancyCodingFlag(bool flag)
{
  bitwise_occupancy_coding_flag = flag;
}

// bool adjacent_child_contextualization_enabled_flag;
bool
GeometryParameterSetImpl::isAdjacentChildContextualizationEnabled() const
{
  return adjacent_child_contextualization_enabled_flag;
}
void
GeometryParameterSetImpl::setAdjacentChildContextualizationEnabled(
  bool enabled)
{
  adjacent_child_contextualization_enabled_flag = enabled;
}

// int intra_pred_max_node_size_log2;
int
GeometryParameterSetImpl::getIntraPredMaxNodeSizeLog2() const
{
  return intra_pred_max_node_size_log2;
}
void
GeometryParameterSetImpl::setIntraPredMaxNodeSizeLog2(int val)
{
  intra_pred_max_node_size_log2 = val;
}

// bool trisoup_enabled_flag;
bool
GeometryParameterSetImpl::isTrisoupEnabled() const
{
  return trisoup_enabled_flag;
}
void
GeometryParameterSetImpl::setTrisoupEnabled(bool enabled)
{
  trisoup_enabled_flag = enabled;
}

// int trisoup_sampling_value;
int
GeometryParameterSetImpl::getTrisoupSamplingValue() const
{
  return trisoup_sampling_value;
}
void
GeometryParameterSetImpl::setTrisoupSamplingValue(int val)
{
  trisoup_sampling_value = val;
}

// bool geom_scaling_enabled_flag;
bool
GeometryParameterSetImpl::isGeomScalingEnabled() const
{
  return geom_scaling_enabled_flag;
}
void
GeometryParameterSetImpl::setGeomScalingEnabled(bool enabled)
{
  geom_scaling_enabled_flag = enabled;
}

// int geom_qp_multiplier_log2;
int
GeometryParameterSetImpl::getGeomQpMultiplierLog2() const
{
  return geom_qp_multiplier_log2;
}
void
GeometryParameterSetImpl::setGeomQpMultiplierLog2(int val)
{
  geom_qp_multiplier_log2 = val;
}

// int geom_base_qp;
int
GeometryParameterSetImpl::getGeomBaseQp() const
{
  return geom_base_qp;
}
void
GeometryParameterSetImpl::setGeomBaseQp(int qp)
{
  geom_base_qp = qp;
}

// int geom_idcm_qp_offset;
int
GeometryParameterSetImpl::getGeomIdcmQpOffset() const
{
  return geom_idcm_qp_offset;
}
void
GeometryParameterSetImpl::setGeomIdcmQpOffset(int val)
{
  geom_idcm_qp_offset = val;
}

// bool qtbt_enabled_flag;
bool
GeometryParameterSetImpl::isQtbtEnabled() const
{
  return qtbt_enabled_flag;
}
void
GeometryParameterSetImpl::setQtbtEnabled(bool enabled)
{
  qtbt_enabled_flag = enabled;
}

// bool geom_planar_mode_enabled_flag;
// int geom_planar_threshold0;
// int geom_planar_threshold1;
// int geom_planar_threshold2;
// int geom_idcm_rate_minus1;
bool
GeometryParameterSetImpl::isGeomPlanarModeEnabled() const
{
  return geom_planar_mode_enabled_flag;
}
void
GeometryParameterSetImpl::setGeomPlanarModeEnabled(bool enabled)
{
  geom_planar_mode_enabled_flag = enabled;
}
int
GeometryParameterSetImpl::getGeomPlanarThreshold0() const
{
  return geom_planar_threshold0;
}
void
GeometryParameterSetImpl::setGeomPlanarThreshold0(int val)
{
  geom_planar_threshold0 = val;
}
int
GeometryParameterSetImpl::getGeomPlanarThreshold1() const
{
  return geom_planar_threshold1;
}
void
GeometryParameterSetImpl::setGeomPlanarThreshold1(int val)
{
  geom_planar_threshold1 = val;
}
int
GeometryParameterSetImpl::getGeomPlanarThreshold2() const
{
  return geom_planar_threshold2;
}
void
GeometryParameterSetImpl::setGeomPlanarThreshold2(int val)
{
  geom_planar_threshold2 = val;
}
int
GeometryParameterSetImpl::getGeomIdcmRateMinus1() const
{
  return geom_idcm_rate_minus1;
}
void
GeometryParameterSetImpl::setGeomIdcmRateMinus1(int val)
{
  geom_idcm_rate_minus1 = val;
}

// bool geom_angular_mode_enabled_flag;
bool
GeometryParameterSetImpl::isGeomAngularModeEnabled() const
{
  return geom_angular_mode_enabled_flag;
}
void
GeometryParameterSetImpl::setGeomAngularModeEnabled(bool enabled)
{
  geom_angular_mode_enabled_flag = enabled;
}

// bool geom_slice_angular_origin_present_flag;
bool
GeometryParameterSetImpl::getGeomSliceAngularOriginPresentFlag() const
{
  return geom_slice_angular_origin_present_flag;
}
void
GeometryParameterSetImpl::setGeomSliceAngularOriginPresentFlag(bool flag)
{
  geom_slice_angular_origin_present_flag = flag;
}

// (in stv axis order).
// Vec3<int> gpsAngularOrigin;
const point_t&
GeometryParameterSetImpl::getGpsAngularOrigin() const
{
  return gpsAngularOrigin;
}
point_t&
GeometryParameterSetImpl::getGpsAngularOrigin()
{
  return gpsAngularOrigin;
}
void
GeometryParameterSetImpl::setGpsAngularOrigin(const point_t& origin)
{
  gpsAngularOrigin = origin;
}

// std::vector<int> angularTheta;
// std::vector<int> angularZ;
// std::vector<int> angularNumPhiPerTurn;
const Int32VectorAdapterImpl&
GeometryParameterSetImpl::getAngularTheta() const
{
  return angularTheta;
}
Int32VectorAdapterImpl&
GeometryParameterSetImpl::getAngularTheta()
{
  return angularTheta;
}
const Int32VectorAdapterImpl&
GeometryParameterSetImpl::getAngularZ() const
{
  return angularZ;
}
Int32VectorAdapterImpl&
GeometryParameterSetImpl::getAngularZ()
{
  return angularZ;
}
const Int32VectorAdapterImpl&
GeometryParameterSetImpl::getAngularNumPhiPerTurn() const
{
  return angularNumPhiPerTurn;
}
Int32VectorAdapterImpl&
GeometryParameterSetImpl::getAngularNumPhiPerTurn()
{
  return angularNumPhiPerTurn;
}

int
GeometryParameterSetImpl::getGeomAngularThetaPred(int i) const
{
  if (!--i)
    return angularTheta.getData()[i];
  return 2 * angularTheta.getData()[i] - angularTheta.getData()[i - 1];
}

// bool planar_buffer_disabled_flag;
bool
GeometryParameterSetImpl::isPlanarBufferDisabled() const
{
  return planar_buffer_disabled_flag;
}
void
GeometryParameterSetImpl::setPlanarBufferDisabled(bool disabled)
{
  planar_buffer_disabled_flag = disabled;
}

// int geom_qp_offset_intvl_log2;
int
GeometryParameterSetImpl::getGeomQpOffsetIntvlLog2() const
{
  return geom_qp_offset_intvl_log2;
}
void
GeometryParameterSetImpl::setGeomQpOffsetIntvlLog2(int val)
{
  geom_qp_offset_intvl_log2 = val;
}

// int geom_angular_azimuth_scale_log2_minus11;
// int geom_angular_azimuth_speed_minus1;
int
GeometryParameterSetImpl::getGeomAngularAzimuthScaleLog2Minus11() const
{
  return geom_angular_azimuth_scale_log2_minus11;
}
void
GeometryParameterSetImpl::setGeomAngularAzimuthScaleLog2Minus11(int val)
{
  geom_angular_azimuth_scale_log2_minus11 = val;
}
int
GeometryParameterSetImpl::getGeomAngularAzimuthSpeedMinus1() const
{
  return geom_angular_azimuth_speed_minus1;
}
void
GeometryParameterSetImpl::setGeomAngularAzimuthSpeedMinus1(int val)
{
  geom_angular_azimuth_speed_minus1 = val;
}

// int geom_angular_radius_inv_scale_log2;
int
GeometryParameterSetImpl::getGeomAngularRadiusInvScaleLog2() const
{
  return geom_angular_radius_inv_scale_log2;
}
void
GeometryParameterSetImpl::setGeomAngularRadiusInvScaleLog2(int val)
{
  geom_angular_radius_inv_scale_log2 = val;
}

// in each octree level.
// bool octree_point_count_list_present_flag;
bool
GeometryParameterSetImpl::getOctreePointCountListPresentFlag() const
{
  return octree_point_count_list_present_flag;
}
void
GeometryParameterSetImpl::setOctreePointCountListPresentFlag(bool flag)
{
  octree_point_count_list_present_flag = flag;
}

//---------------------------------------------------------------------------
GeometryParameterSet*
GeometryParameterSet::create()
{
  try {
    return new GeometryParameterSetImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::GeometryBrickFooter& dst, const GeometryBrickFooter& src)
{
  dst.geom_num_points_minus1 = src.getGeomNumPointsMinus1();
  CopyVector(
    dst.octree_lvl_num_points_minus1, src.getOctreeLvlNumPointsMinus1());
}

//---------------------------------------------------------------------------

GeometryBrickFooterImpl::GeometryBrickFooterImpl(
  const pcc::GeometryBrickFooter& rhs)
  : geom_num_points_minus1(rhs.geom_num_points_minus1)
  , octree_lvl_num_points_minus1(rhs.octree_lvl_num_points_minus1)
{}

GeometryBrickFooterImpl::GeometryBrickFooterImpl(
  const GeometryBrickFooter& rhs)
  : geom_num_points_minus1(rhs.getGeomNumPointsMinus1())
  , octree_lvl_num_points_minus1(rhs.getOctreeLvlNumPointsMinus1())
{}

void
GeometryBrickFooterImpl::release()
{
  delete this;
}

// int geom_num_points_minus1;
int
GeometryBrickFooterImpl::getGeomNumPointsMinus1() const
{
  return geom_num_points_minus1;
}
void
GeometryBrickFooterImpl::setGeomNumPointsMinus1(int val)
{
  geom_num_points_minus1 = val;
}

// std::vector<int> octree_lvl_num_points_minus1;
const Int32VectorAdapterImpl&
GeometryBrickFooterImpl::getOctreeLvlNumPointsMinus1() const
{
  return octree_lvl_num_points_minus1;
}
Int32VectorAdapterImpl&
GeometryBrickFooterImpl::getOctreeLvlNumPointsMinus1()
{
  return octree_lvl_num_points_minus1;
}

//---------------------------------------------------------------------------

GeometryBrickFooter*
GeometryBrickFooter::create()
{
  try {
    return new GeometryBrickFooterImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::GeometryBrickHeader& dst, const GeometryBrickHeader& src)
{
  dst.geom_geom_parameter_set_id = src.getGeomGeomParameterSetId();
  dst.slice_tag = src.getSliceTag();
  dst.geom_slice_id = src.getGeomSliceId();

  dst.frame_ctr_lsb = src.getFrameCtrLsb();

  dst.geomBoxOrigin = pcc::Vec3<int>(src.getGeomBoxOrigin());
  dst.geom_box_log2_scale = src.getGeomBoxLog2Scale();

  dst.geom_box_origin_bits_minus1 = src.getGeomBoxOriginBitsMinus1();

  dst.gbhAngularOrigin = pcc::Vec3<int>(src.getGbhAngularOrigin());

  dst.rootNodeSizeLog2 = pcc::Vec3<int>(src.getRootNodeSizeLog2());

  dst.pgeom_resid_abs_log2_bits =
    pcc::Vec3<int>(src.getPgeomResidAbsLog2Bits());

  dst.maxRootNodeDimLog2 = src.getMaxRootNodeDimLog2();

  CopyVector(dst.tree_lvl_coded_axis_list, src.getTreeLvlCodedAxisList());

  dst.geom_slice_qp_offset = src.getGeomSliceQpOffset();

  dst.geom_qp_offset_intvl_log2_delta = src.getGeomQpOffsetIntvlLog2Delta();

  dst.geom_stream_cnt_minus1 = src.getGeomStreamCntMinus1();

  dst.trisoup_node_size_log2_minus2 = src.getTrisoupNodeSizeLog2Minus2();

  dst.trisoup_sampling_value_minus1 = src.getTrisoupSamplingValueMinus1();

  dst.num_unique_segments_minus1 = src.getNumUniqueSegmentsMinus1();

  dst.num_unique_segments_bits_minus1 = src.getNumUniqueSegmentsBitsMinus1();

  CopyParams(dst.footer, src.getFooter());

  dst.entropy_continuation_flag = src.getEntropyContinuationFlag();

  dst.prev_slice_id = src.getPrevSliceId();

  dst.pgeom_min_radius = src.getPgeomMinRadius();
}

//---------------------------------------------------------------------------

GeometryBrickHeaderImpl::GeometryBrickHeaderImpl(
  const pcc::GeometryBrickHeader& rhs)
  : geom_geom_parameter_set_id(rhs.geom_geom_parameter_set_id)
  , slice_tag(rhs.slice_tag)
  , geom_slice_id(rhs.geom_slice_id)
  , frame_ctr_lsb(rhs.frame_ctr_lsb)
  , geomBoxOrigin(rhs.geomBoxOrigin)
  , geom_box_log2_scale(rhs.geom_box_log2_scale)
  , geom_box_origin_bits_minus1(rhs.geom_box_origin_bits_minus1)
  , gbhAngularOrigin(rhs.gbhAngularOrigin)
  , rootNodeSizeLog2(rhs.rootNodeSizeLog2)
  , pgeom_resid_abs_log2_bits(rhs.pgeom_resid_abs_log2_bits)
  , maxRootNodeDimLog2(rhs.maxRootNodeDimLog2)
  , tree_lvl_coded_axis_list(rhs.tree_lvl_coded_axis_list)
  , geom_slice_qp_offset(rhs.geom_slice_qp_offset)
  , geom_qp_offset_intvl_log2_delta(rhs.geom_qp_offset_intvl_log2_delta)
  , geom_stream_cnt_minus1(rhs.geom_stream_cnt_minus1)
  , trisoup_node_size_log2_minus2(rhs.trisoup_node_size_log2_minus2)
  , trisoup_sampling_value_minus1(rhs.trisoup_sampling_value_minus1)
  , num_unique_segments_minus1(rhs.num_unique_segments_minus1)
  , num_unique_segments_bits_minus1(rhs.num_unique_segments_bits_minus1)
  , footer(rhs.footer)
  , entropy_continuation_flag(rhs.entropy_continuation_flag)
  , prev_slice_id(rhs.prev_slice_id)
  , pgeom_min_radius(rhs.pgeom_min_radius)
{}

GeometryBrickHeaderImpl::GeometryBrickHeaderImpl(
  const GeometryBrickHeader& rhs)
  : geom_geom_parameter_set_id(rhs.getGeomGeomParameterSetId())
  , slice_tag(rhs.getSliceTag())
  , geom_slice_id(rhs.getGeomSliceId())
  , frame_ctr_lsb(rhs.getFrameCtrLsb())
  , geomBoxOrigin(rhs.getGeomBoxOrigin())
  , geom_box_log2_scale(rhs.getGeomBoxLog2Scale())
  , geom_box_origin_bits_minus1(rhs.getGeomBoxOriginBitsMinus1())
  , gbhAngularOrigin(rhs.getGbhAngularOrigin())
  , rootNodeSizeLog2(rhs.getRootNodeSizeLog2())
  , pgeom_resid_abs_log2_bits(rhs.getPgeomResidAbsLog2Bits())
  , maxRootNodeDimLog2(rhs.getMaxRootNodeDimLog2())
  , tree_lvl_coded_axis_list(rhs.getTreeLvlCodedAxisList())
  , geom_slice_qp_offset(rhs.getGeomSliceQpOffset())
  , geom_qp_offset_intvl_log2_delta(rhs.getGeomQpOffsetIntvlLog2Delta())
  , geom_stream_cnt_minus1(rhs.getGeomStreamCntMinus1())
  , trisoup_node_size_log2_minus2(rhs.getTrisoupNodeSizeLog2Minus2())
  , trisoup_sampling_value_minus1(rhs.getTrisoupSamplingValueMinus1())
  , num_unique_segments_minus1(rhs.getNumUniqueSegmentsMinus1())
  , num_unique_segments_bits_minus1(rhs.getNumUniqueSegmentsBitsMinus1())
  , footer(rhs.getFooter())
  , entropy_continuation_flag(rhs.getEntropyContinuationFlag())
  , prev_slice_id(rhs.getPrevSliceId())
  , pgeom_min_radius(rhs.getPgeomMinRadius())
{}

void
GeometryBrickHeaderImpl::release()
{
  delete this;
}

// int geom_geom_parameter_set_id;
// int slice_tag;
// int geom_slice_id;
int
GeometryBrickHeaderImpl::getGeomGeomParameterSetId() const
{
  return geom_geom_parameter_set_id;
}
void
GeometryBrickHeaderImpl::setGeomGeomParameterSetId(int id)
{
  geom_geom_parameter_set_id = id;
}
int
GeometryBrickHeaderImpl::getSliceTag() const
{
  return slice_tag;
}
void
GeometryBrickHeaderImpl::setSliceTag(int tag)
{
  slice_tag = tag;
}
int
GeometryBrickHeaderImpl::getGeomSliceId() const
{
  return geom_slice_id;
}
void
GeometryBrickHeaderImpl::setGeomSliceId(int id)
{
  geom_slice_id = id;
}

// int frame_ctr_lsb;
int
GeometryBrickHeaderImpl::getFrameCtrLsb() const
{
  return frame_ctr_lsb;
}
void
GeometryBrickHeaderImpl::setFrameCtrLsb(int val)
{
  frame_ctr_lsb = val;
}

// Vec3<int> geomBoxOrigin;
// int geom_box_log2_scale;
const point_t&
GeometryBrickHeaderImpl::getGeomBoxOrigin() const
{
  return geomBoxOrigin;
}
point_t&
GeometryBrickHeaderImpl::getGeomBoxOrigin()
{
  return geomBoxOrigin;
}
void
GeometryBrickHeaderImpl::setGeomBoxOrigin(const point_t& val)
{
  geomBoxOrigin = val;
}
int
GeometryBrickHeaderImpl::getGeomBoxLog2Scale() const
{
  return geom_box_log2_scale;
}
void
GeometryBrickHeaderImpl::setGeomBoxLog2Scale(int val)
{
  geom_box_log2_scale = val;
}

// int geom_box_origin_bits_minus1;
int
GeometryBrickHeaderImpl::getGeomBoxOriginBitsMinus1() const
{
  return geom_box_origin_bits_minus1;
}
void
GeometryBrickHeaderImpl::setGeomBoxOriginBitsMinus1(int val)
{
  geom_box_origin_bits_minus1 = val;
}

// (in stv axis order).
// Vec3<int> gbhAngularOrigin;
const point_t&
GeometryBrickHeaderImpl::getGbhAngularOrigin() const
{
  return gbhAngularOrigin;
}
point_t&
GeometryBrickHeaderImpl::getGbhAngularOrigin()
{
  return gbhAngularOrigin;
}
void
GeometryBrickHeaderImpl::getGbhAngularOrigin(const point_t& val)
{
  gbhAngularOrigin = val;
}

// NB: this is only needed for the initial node size determination at
//     the encoder
// Vec3<int> rootNodeSizeLog2;
const point_t&
GeometryBrickHeaderImpl::getRootNodeSizeLog2() const
{
  return rootNodeSizeLog2;
}
point_t&
GeometryBrickHeaderImpl::getRootNodeSizeLog2()
{
  return rootNodeSizeLog2;
}
void
GeometryBrickHeaderImpl::setRootNodeSizeLog2(const point_t& val)
{
  rootNodeSizeLog2 = val;
}

// Vec3<int> pgeom_resid_abs_log2_bits;
const point_t&
GeometryBrickHeaderImpl::getPgeomResidAbsLog2Bits() const
{
  return pgeom_resid_abs_log2_bits;
}
point_t&
GeometryBrickHeaderImpl::getPgeomResidAbsLog2Bits()
{
  return pgeom_resid_abs_log2_bits;
}
void
GeometryBrickHeaderImpl::setPgeomResidAbsLog2Bits(const point_t& val)
{
  pgeom_resid_abs_log2_bits = val;
}

// mutable int maxRootNodeDimLog2;
// NOTE: mutable
int
GeometryBrickHeaderImpl::getMaxRootNodeDimLog2() const
{
  return maxRootNodeDimLog2;
}
void
GeometryBrickHeaderImpl::setMaxRootNodeDimLog2(int val)
{
  maxRootNodeDimLog2 = val;
}

// std::vector<int8_t> tree_lvl_coded_axis_list;
const Int8VectorAdapterImpl&
GeometryBrickHeaderImpl::getTreeLvlCodedAxisList() const
{
  return tree_lvl_coded_axis_list;
}
Int8VectorAdapterImpl&
GeometryBrickHeaderImpl::getTreeLvlCodedAxisList()
{
  return tree_lvl_coded_axis_list;
}

// int geom_slice_qp_offset;
int
GeometryBrickHeaderImpl::getGeomSliceQpOffset() const
{
  return geom_slice_qp_offset;
}
void
GeometryBrickHeaderImpl::setGeomSliceQpOffset(int val)
{
  geom_slice_qp_offset = val;
}

// int geom_qp_offset_intvl_log2_delta;
int
GeometryBrickHeaderImpl::getGeomQpOffsetIntvlLog2Delta() const
{
  return geom_qp_offset_intvl_log2_delta;
}
void
GeometryBrickHeaderImpl::setGeomQpOffsetIntvlLog2Delta(int val)
{
  geom_qp_offset_intvl_log2_delta = val;
}

// int geom_stream_cnt_minus1;
int
GeometryBrickHeaderImpl::getGeomStreamCntMinus1() const
{
  return geom_stream_cnt_minus1;
}
void
GeometryBrickHeaderImpl::setGeomStreamCntMinus1(int val)
{
  geom_stream_cnt_minus1 = val;
}

// int trisoup_node_size_log2_minus2;
int
GeometryBrickHeaderImpl::getTrisoupNodeSizeLog2Minus2() const
{
  return trisoup_node_size_log2_minus2;
}
void
GeometryBrickHeaderImpl::setTrisoupNodeSizeLog2Minus2(int val)
{
  trisoup_node_size_log2_minus2 = val;
}

// int trisoup_sampling_value_minus1;
int
GeometryBrickHeaderImpl::getTrisoupSamplingValueMinus1() const
{
  return trisoup_sampling_value_minus1;
}
void
GeometryBrickHeaderImpl::setTrisoupSamplingValueMinus1(int val)
{
  trisoup_sampling_value_minus1 = val;
}

// int num_unique_segments_minus1;
int
GeometryBrickHeaderImpl::getNumUniqueSegmentsMinus1() const
{
  return num_unique_segments_minus1;
}
void
GeometryBrickHeaderImpl::setNumUniqueSegmentsMinus1(int val)
{
  num_unique_segments_minus1 = val;
}

// int num_unique_segments_bits_minus1;
int
GeometryBrickHeaderImpl::getNumUniqueSegmentsBitsMinus1() const
{
  return num_unique_segments_bits_minus1;
}
void
GeometryBrickHeaderImpl::setNumUniqueSegmentsBitsMinus1(int val)
{
  num_unique_segments_bits_minus1 = val;
}

const GeometryBrickFooterImpl&
GeometryBrickHeaderImpl::getFooter() const
{
  return footer;
}
GeometryBrickFooterImpl&
GeometryBrickHeaderImpl::getFooter()
{
  return footer;
}

// bool entropy_continuation_flag;
bool
GeometryBrickHeaderImpl::getEntropyContinuationFlag() const
{
  return entropy_continuation_flag;
}
void
GeometryBrickHeaderImpl::setEntropyContinuationFlag(bool flag)
{
  entropy_continuation_flag = flag;
}

// int prev_slice_id;
int
GeometryBrickHeaderImpl::getPrevSliceId() const
{
  return prev_slice_id;
}
void
GeometryBrickHeaderImpl::setPrevSliceId(int id)
{
  prev_slice_id = id;
}

// int pgeom_min_radius;
int
GeometryBrickHeaderImpl::getPgeomMinRadius() const
{
  return pgeom_min_radius;
}
void
GeometryBrickHeaderImpl::setPgeomMinRadius(int val)
{
  pgeom_min_radius = val;
}

//---------------------------------------------------------------------------

GeometryBrickHeader*
GeometryBrickHeader::create()
{
  try {
    return new GeometryBrickHeaderImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::AttributeParameterSet& dst, const AttributeParameterSet& src)
{
  dst.aps_attr_parameter_set_id = src.getApsAttrParameterSetId();
  dst.aps_seq_parameter_set_id = src.getApsSeqParameterSetId();
  dst.attr_encoding = src.getAttrEncoding();

  dst.lod_decimation_type = src.getLodDecimationType();
  dst.canonical_point_order_flag = src.getCanonicalPointOrderFlag();
  dst.num_pred_nearest_neighbours_minus1 =
    src.getNumPredNearestNeighboursMinus1();
  dst.max_num_direct_predictors = src.getMaxNumDirectPredictors();
  dst.direct_avg_predictor_disabled_flag = src.isDirectAvgPredictorDisabled();
  dst.adaptive_prediction_threshold = src.getAdaptivePredictionThreshold();
  dst.intra_lod_search_range = src.getIntraLodSearchRange();
  dst.inter_lod_search_range = src.getInterLodSearchRange();

  dst.lodNeighBias = pcc::Vec3<int>(src.getLodNeighBias());

  dst.intra_lod_prediction_skip_layers = src.getIntraLodPredictionSkipLayers();

  dst.inter_component_prediction_enabled_flag =
    src.isInterComponentPredictionEnabled();
  dst.last_component_prediction_enabled_flag =
    src.isLastComponentPredictionEnabled();

  dst.pred_weight_blending_enabled_flag = src.isPredWeightBlendingEnabled();

  dst.num_detail_levels_minus1 = src.getNumDetailLevelsMinus1();

  CopyVector(dst.lodSamplingPeriod, src.getLodSamplingPeriod());

  dst.dist2 = src.getDist2();
  dst.aps_slice_dist2_deltas_present_flag =
    src.getApsSliceDist2DeltasPresentFlag();

  dst.init_qp_minus4 = src.getInitQpMinus4();
  dst.aps_chroma_qp_offset = src.getApsChromaQpOffset();
  dst.aps_slice_qp_deltas_present_flag = src.getApsSliceQpDeltasPresenFlag();

  dst.raht_prediction_enabled_flag = src.isRahtPredictionEnabled();
  dst.raht_prediction_threshold0 = src.getRahtPredictionThreshold0();
  dst.raht_prediction_threshold1 = src.getRahtPredictionThreshold1();

  dst.scalable_lifting_enabled_flag = src.isScalableLiftingEnabled();
  dst.max_neigh_range_minus1 = src.getMaxNeighRangeMinus1();

  dst.spherical_coord_flag = src.getSphericalCoordFlag();

  dst.attr_coord_scale = pcc::Vec3<int>(src.getAttrCoordScale());

  dst.raw_attr_variable_len_flag = src.getRawAttrVariableLenFlag();
}

//---------------------------------------------------------------------------

AttributeParameterSetImpl::AttributeParameterSetImpl(
  const pcc::AttributeParameterSet& rhs)
  : AttributeParameterSet()
  , aps_attr_parameter_set_id(rhs.aps_attr_parameter_set_id)
  , aps_seq_parameter_set_id(rhs.aps_seq_parameter_set_id)
  , attr_encoding(rhs.attr_encoding)
  , lod_decimation_type(rhs.lod_decimation_type)
  , canonical_point_order_flag(rhs.canonical_point_order_flag)
  , num_pred_nearest_neighbours_minus1(rhs.num_pred_nearest_neighbours_minus1)
  , max_num_direct_predictors(rhs.max_num_direct_predictors)
  , direct_avg_predictor_disabled_flag(rhs.direct_avg_predictor_disabled_flag)
  , adaptive_prediction_threshold(rhs.adaptive_prediction_threshold)
  , intra_lod_search_range(rhs.intra_lod_search_range)
  , inter_lod_search_range(rhs.inter_lod_search_range)
  , lodNeighBias(rhs.lodNeighBias)
  , intra_lod_prediction_skip_layers(rhs.intra_lod_prediction_skip_layers)
  , inter_component_prediction_enabled_flag(
      rhs.inter_component_prediction_enabled_flag)
  , last_component_prediction_enabled_flag(
      rhs.last_component_prediction_enabled_flag)
  , pred_weight_blending_enabled_flag(rhs.pred_weight_blending_enabled_flag)
  , num_detail_levels_minus1(rhs.num_detail_levels_minus1)
  , lodSamplingPeriod(rhs.lodSamplingPeriod)
  , dist2(rhs.dist2)
  , aps_slice_dist2_deltas_present_flag(
      rhs.aps_slice_dist2_deltas_present_flag)
  , init_qp_minus4(rhs.init_qp_minus4)
  , aps_chroma_qp_offset(rhs.aps_chroma_qp_offset)
  , aps_slice_qp_deltas_present_flag(rhs.aps_slice_qp_deltas_present_flag)
  , raht_prediction_enabled_flag(rhs.raht_prediction_enabled_flag)
  , raht_prediction_threshold0(rhs.raht_prediction_threshold0)
  , raht_prediction_threshold1(rhs.raht_prediction_threshold1)
  , scalable_lifting_enabled_flag(rhs.scalable_lifting_enabled_flag)
  , max_neigh_range_minus1(rhs.max_neigh_range_minus1)
  , spherical_coord_flag(rhs.spherical_coord_flag)
  , attr_coord_scale(rhs.attr_coord_scale)
  , raw_attr_variable_len_flag(rhs.raw_attr_variable_len_flag)
{}

AttributeParameterSetImpl::AttributeParameterSetImpl(
  const AttributeParameterSet& rhs)
  : AttributeParameterSet()
  , aps_attr_parameter_set_id(rhs.getApsAttrParameterSetId())
  , aps_seq_parameter_set_id(rhs.getApsSeqParameterSetId())
  , attr_encoding(rhs.getAttrEncoding())
  , lod_decimation_type(rhs.getLodDecimationType())
  , canonical_point_order_flag(rhs.getCanonicalPointOrderFlag())
  , num_pred_nearest_neighbours_minus1(rhs.getNumPredNearestNeighboursMinus1())
  , max_num_direct_predictors(rhs.getMaxNumDirectPredictors())
  , direct_avg_predictor_disabled_flag(rhs.isDirectAvgPredictorDisabled())
  , adaptive_prediction_threshold(rhs.getAdaptivePredictionThreshold())
  , intra_lod_search_range(rhs.getIntraLodSearchRange())
  , inter_lod_search_range(rhs.getInterLodSearchRange())
  , lodNeighBias(rhs.getLodNeighBias())
  , intra_lod_prediction_skip_layers(rhs.getIntraLodPredictionSkipLayers())
  , inter_component_prediction_enabled_flag(
      rhs.isInterComponentPredictionEnabled())
  , last_component_prediction_enabled_flag(
      rhs.isLastComponentPredictionEnabled())
  , pred_weight_blending_enabled_flag(rhs.isPredWeightBlendingEnabled())
  , num_detail_levels_minus1(rhs.getNumDetailLevelsMinus1())
  , lodSamplingPeriod(rhs.getLodSamplingPeriod())
  , dist2(rhs.getDist2())
  , aps_slice_dist2_deltas_present_flag(
      rhs.getApsSliceDist2DeltasPresentFlag())
  , init_qp_minus4(rhs.getInitQpMinus4())
  , aps_chroma_qp_offset(rhs.getApsChromaQpOffset())
  , aps_slice_qp_deltas_present_flag(rhs.getApsSliceQpDeltasPresenFlag())
  , raht_prediction_enabled_flag(rhs.isRahtPredictionEnabled())
  , raht_prediction_threshold0(rhs.getRahtPredictionThreshold0())
  , raht_prediction_threshold1(rhs.getRahtPredictionThreshold1())
  , scalable_lifting_enabled_flag(rhs.isScalableLiftingEnabled())
  , max_neigh_range_minus1(rhs.getMaxNeighRangeMinus1())
  , spherical_coord_flag(rhs.getSphericalCoordFlag())
  , attr_coord_scale(rhs.getAttrCoordScale())
  , raw_attr_variable_len_flag(rhs.getRawAttrVariableLenFlag())
{}

void
AttributeParameterSetImpl::release()
{
  delete this;
}

// int aps_attr_parameter_set_id;
// int aps_seq_parameter_set_id;
// AttributeEncoding attr_encoding;
int
AttributeParameterSetImpl::getApsAttrParameterSetId() const
{
  return aps_attr_parameter_set_id;
}
void
AttributeParameterSetImpl::setApsAttrParameterSetId(int id)
{
  aps_attr_parameter_set_id = id;
}
int
AttributeParameterSetImpl::getApsSeqParameterSetId() const
{
  return aps_seq_parameter_set_id;
}
void
AttributeParameterSetImpl::setApsSeqParameterSetId(int id)
{
  aps_seq_parameter_set_id = id;
}
AttributeEncoding
AttributeParameterSetImpl::getAttrEncoding() const
{
  return attr_encoding;
}
void
AttributeParameterSetImpl::setAttrEncoding(AttributeEncoding enc)
{
  attr_encoding = enc;
}

//--- lifting/predicting transform parameters

// LodDecimationMethod lod_decimation_type;
// bool canonical_point_order_flag;
// int num_pred_nearest_neighbours_minus1;
// int max_num_direct_predictors;
// bool direct_avg_predictor_disabled_flag;
// int adaptive_prediction_threshold;
// int intra_lod_search_range;
// int inter_lod_search_range;
LodDecimationMethod
AttributeParameterSetImpl::getLodDecimationType() const
{
  return lod_decimation_type;
}
void
AttributeParameterSetImpl::setLodDecimationType(LodDecimationMethod val)
{
  lod_decimation_type = val;
}
bool
AttributeParameterSetImpl::getCanonicalPointOrderFlag() const
{
  return canonical_point_order_flag;
}
void
AttributeParameterSetImpl::setCanonicalPointOrderFlag(bool flag)
{
  canonical_point_order_flag = flag;
}
int
AttributeParameterSetImpl::getNumPredNearestNeighboursMinus1() const
{
  return num_pred_nearest_neighbours_minus1;
}
void
AttributeParameterSetImpl::setNumPredNearestNeighboursMinus1(int val)
{
  num_pred_nearest_neighbours_minus1 = val;
}
int
AttributeParameterSetImpl::getMaxNumDirectPredictors() const
{
  return max_num_direct_predictors;
}
void
AttributeParameterSetImpl::setMaxNumDirectPredictors(int val)
{
  max_num_direct_predictors = val;
}
bool
AttributeParameterSetImpl::isDirectAvgPredictorDisabled() const
{
  return direct_avg_predictor_disabled_flag;
}
void
AttributeParameterSetImpl::setDirectAvgPredictorDisabled(bool disabled)
{
  direct_avg_predictor_disabled_flag = disabled;
}
int
AttributeParameterSetImpl::getAdaptivePredictionThreshold() const
{
  return adaptive_prediction_threshold;
}
void
AttributeParameterSetImpl::setAdaptivePredictionThreshold(int val)
{
  adaptive_prediction_threshold = val;
}
int
AttributeParameterSetImpl::getIntraLodSearchRange() const
{
  return intra_lod_search_range;
}
void
AttributeParameterSetImpl::setIntraLodSearchRange(int val)
{
  intra_lod_search_range = val;
}
int
AttributeParameterSetImpl::getInterLodSearchRange() const
{
  return inter_lod_search_range;
}
void
AttributeParameterSetImpl::setInterLodSearchRange(int val)
{
  inter_lod_search_range = val;
}

// Vec3<int32_t> lodNeighBias;
const point_t&
AttributeParameterSetImpl::getLodNeighBias() const
{
  return lodNeighBias;
}
point_t&
AttributeParameterSetImpl::getLodNeighBias()
{
  return lodNeighBias;
}
void
AttributeParameterSetImpl::setLodNeighBias(const point_t& val)
{
  lodNeighBias = val;
}

// prediction.
// int intra_lod_prediction_skip_layers;
int
AttributeParameterSetImpl::getIntraLodPredictionSkipLayers() const
{
  return intra_lod_prediction_skip_layers;
}
void
AttributeParameterSetImpl::setIntraLodPredictionSkipLayers(int val)
{
  intra_lod_prediction_skip_layers = val;
}

// bool inter_component_prediction_enabled_flag;
// bool last_component_prediction_enabled_flag;
bool
AttributeParameterSetImpl::isInterComponentPredictionEnabled() const
{
  return inter_component_prediction_enabled_flag;
}
void
AttributeParameterSetImpl::setInterComponentPredictionEnabled(bool enabled)
{
  inter_component_prediction_enabled_flag = enabled;
}
bool
AttributeParameterSetImpl::isLastComponentPredictionEnabled() const
{
  return last_component_prediction_enabled_flag;
}
void
AttributeParameterSetImpl::setLastComponentPredictionEnabled(bool enabled)
{
  last_component_prediction_enabled_flag = enabled;
}

// bool pred_weight_blending_enabled_flag;
bool
AttributeParameterSetImpl::isPredWeightBlendingEnabled() const
{
  return pred_weight_blending_enabled_flag;
}
void
AttributeParameterSetImpl::setPredWeightBlendingEnabled(bool enabled)
{
  pred_weight_blending_enabled_flag = enabled;
}

// int num_detail_levels_minus1;
int
AttributeParameterSetImpl::getNumDetailLevelsMinus1() const
{
  return num_detail_levels_minus1;
}
void
AttributeParameterSetImpl::setNumDetailLevelsMinus1(int val)
{
  num_detail_levels_minus1 = val;
}

// std::vector<int> lodSamplingPeriod;
const Int32VectorAdapterImpl&
AttributeParameterSetImpl::getLodSamplingPeriod() const
{
  return lodSamplingPeriod;
}
Int32VectorAdapterImpl&
AttributeParameterSetImpl::getLodSamplingPeriod()
{
  return lodSamplingPeriod;
}

// int dist2;
// bool aps_slice_dist2_deltas_present_flag;
int
AttributeParameterSetImpl::getDist2() const
{
  return dist2;
}
void
AttributeParameterSetImpl::setDist2(int val)
{
  dist2 = val;
}
bool
AttributeParameterSetImpl::getApsSliceDist2DeltasPresentFlag() const
{
  return aps_slice_dist2_deltas_present_flag;
}
void
AttributeParameterSetImpl::setApsSliceDist2DeltasPresentFlag(bool flag)
{
  aps_slice_dist2_deltas_present_flag = flag;
}

// int init_qp_minus4;
// int aps_chroma_qp_offset;
// bool aps_slice_qp_deltas_present_flag;
int
AttributeParameterSetImpl::getInitQpMinus4() const
{
  return init_qp_minus4;
}
void
AttributeParameterSetImpl::setInitQpMinus4(int val)
{
  init_qp_minus4 = val;
}
int
AttributeParameterSetImpl::getApsChromaQpOffset() const
{
  return aps_chroma_qp_offset;
}
void
AttributeParameterSetImpl::setApsChromaQpOffset(int val)
{
  aps_chroma_qp_offset = val;
}
bool
AttributeParameterSetImpl::getApsSliceQpDeltasPresenFlag() const
{
  return aps_slice_qp_deltas_present_flag;
}
void
AttributeParameterSetImpl::setApsSliceQpDeltasPresenFlag(bool flag)
{
  aps_slice_qp_deltas_present_flag = flag;
}

// bool raht_prediction_enabled_flag;
// int raht_prediction_threshold0;
// int raht_prediction_threshold1;
bool
AttributeParameterSetImpl::isRahtPredictionEnabled() const
{
  return raht_prediction_enabled_flag;
}
void
AttributeParameterSetImpl::setRahtPredictionEnabled(bool enabled)
{
  raht_prediction_enabled_flag = enabled;
}
int
AttributeParameterSetImpl::getRahtPredictionThreshold0() const
{
  return raht_prediction_threshold0;
}
void
AttributeParameterSetImpl::setRahtPredictionThreshold0(int val)
{
  raht_prediction_threshold0 = val;
}
int
AttributeParameterSetImpl::getRahtPredictionThreshold1() const
{
  return raht_prediction_threshold1;
}
void
AttributeParameterSetImpl::setRahtPredictionThreshold1(int val)
{
  raht_prediction_threshold1 = val;
}

// bool scalable_lifting_enabled_flag;
// int max_neigh_range_minus1;
bool
AttributeParameterSetImpl::isScalableLiftingEnabled() const
{
  return scalable_lifting_enabled_flag;
}
void
AttributeParameterSetImpl::setScalableLiftingEnabled(bool enabled)
{
  scalable_lifting_enabled_flag = enabled;
}
int
AttributeParameterSetImpl::getMaxNeighRangeMinus1() const
{
  return max_neigh_range_minus1;
}
void
AttributeParameterSetImpl::setMaxNeighRangeMinus1(int val)
{
  max_neigh_range_minus1 = val;
}

// pseudo-spherical domain
// bool spherical_coord_flag;
bool
AttributeParameterSetImpl::getSphericalCoordFlag() const
{
  return spherical_coord_flag;
}
void
AttributeParameterSetImpl::setSphericalCoordFlag(bool flag)
{
  spherical_coord_flag = flag;
}

// Vec3<int> attr_coord_scale;
const point_t&
AttributeParameterSetImpl::getAttrCoordScale() const
{
  return attr_coord_scale;
}
point_t&
AttributeParameterSetImpl::getAttrCoordScale()
{
  return attr_coord_scale;
}
void
AttributeParameterSetImpl::setAttrCoordScale(const point_t& val)
{
  attr_coord_scale = val;
}

//bool raw_attr_variable_len_flag;
bool
AttributeParameterSetImpl::getRawAttrVariableLenFlag() const
{
  return raw_attr_variable_len_flag;
}
void
AttributeParameterSetImpl::setRawAttrVariableLenFlag(bool flag)
{
  raw_attr_variable_len_flag = flag;
}

//---------------------------------------------------------------------------

AttributeParameterSet*
AttributeParameterSet::create()
{
  try {
    return new AttributeParameterSetImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

} // namespace gpcclib
