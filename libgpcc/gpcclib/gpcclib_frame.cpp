#include "PCCPointSet.h"
#include "frame.h"
#include "gpcclib/internal/frame.hpp"
#include "gpcclib/internal/params.hpp"
#include "gpcclib/internal/pointset.hpp"
#include "hls.h"

namespace gpcclib {

CloudFrameImpl::CloudFrameImpl(pcc::CloudFrame&& rhs) noexcept
  : frameNum(std::move(rhs.frameNum))
  , geometry_axis_order(std::move(rhs.geometry_axis_order))
  , outputUnitLength(std::move(rhs.outputUnitLength))
  , outputUnit(std::move(rhs.outputUnit))
  , outputOrigin(std::move(rhs.outputOrigin))
  , outputFpBits(std::move(rhs.outputFpBits))
  //, attrDesc(std::move(rhs.attrDesc))
  , cloud(std::move(rhs.cloud))
{
  // TODO: movable
  for (const auto& a : rhs.attrDesc) {
    attrDesc.emplace_back(AttributeDescriptionImpl(a));
  }
}

CloudFrameImpl::CloudFrameImpl(pcc::PCCPointSet3&& rhs) noexcept
  : cloud(std::move(rhs))
{}

CloudFrameImpl::CloudFrameImpl(const pcc::CloudFrame& rhs) noexcept
  : frameNum(rhs.frameNum)
  , geometry_axis_order(rhs.geometry_axis_order)
  , outputUnitLength(rhs.outputUnitLength)
  , outputUnit(rhs.outputUnit)
  , outputOrigin(rhs.outputOrigin)
  , outputFpBits(rhs.outputFpBits)
//, attrDesc(std::move(rhs.attrDesc))
//, cloud(rhs.cloud)
{
  for (const auto& a : rhs.attrDesc) {
    attrDesc.emplace_back(AttributeDescriptionImpl(a));
  }
}

//
void
CloudFrameImpl::release()
{
  delete this;
}

// int frameNum;
int
CloudFrameImpl::getFrameNum() const
{
  return frameNum;
}
void
CloudFrameImpl::setFrameNum(int num)
{
  frameNum = num;
}

// AxisOrder geometry_axis_order;
AxisOrder
CloudFrameImpl::getGeometryAxisOrder() const
{
  return geometry_axis_order;
}
void
CloudFrameImpl::setGeometryAxisOrder(AxisOrder order)
{
  geometry_axis_order = order;
}

// double outputUnitLength;
double
CloudFrameImpl::getOutputUnitLength() const
{
  return outputUnitLength;
}
void
CloudFrameImpl::setOutputUnitLength(double length)
{
  outputUnitLength = length;
}

// ScaleUnit outputUnit;
ScaleUnit
CloudFrameImpl::getOutputUnit() const
{
  return outputUnit;
}
void
CloudFrameImpl::setOutputUnit(ScaleUnit val)
{
  outputUnit = val;
}

// Vec3<int> outputOrigin;
const point_t&
CloudFrameImpl::getOutputOrigin() const
{
  return outputOrigin;
}
point_t&
CloudFrameImpl::getOutputOrigin()
{
  return outputOrigin;
}
void
CloudFrameImpl::setOutputOrigin(const point_t& val)
{
  outputOrigin = val;
}

int
CloudFrameImpl::getOutputFpBits() const
{
  return outputFpBits;
}
void
CloudFrameImpl::setOutputFpBits(int bits)
{
  outputFpBits = bits;
}

// std::vector<AttributeDescription> attrDesc;
// 指定した KnownAttributeLabel の要素を返す。存在しない場合、 nullptr を返す。
const AttributeDescriptionImpl*
CloudFrameImpl::getAttrDesc(KnownAttributeLabel label) const
{
  size_t index = static_cast<size_t>(label);
  if (attrDesc.size() > index) {
    return &attrDesc[index];
  }

  return nullptr;
}
AttributeDescriptionImpl*
CloudFrameImpl::getAttrDesc(KnownAttributeLabel label)
{
  size_t index = static_cast<size_t>(label);
  if (attrDesc.size() > index) {
    return &attrDesc[index];
  }

  return nullptr;
}

// PCCPointSet3 cloud;
const IntermediatePointSet3&
CloudFrameImpl::getCloud() const
{
  return cloud;
}
IntermediatePointSet3&
CloudFrameImpl::getCloud()
{
  return cloud;
}

// PCCPointSet3 cloud;
const PointSet3Impl&
CloudFrameImpl::getRealCloud() const
{
  return cloud;
}
PointSet3Impl&
CloudFrameImpl::getRealCloud()
{
  return cloud;
}

void
CloudFrameImpl::setParametersFrom(
  const pcc::SequenceParameterSet& sps, int fixedPointBits)
{
  // How many bits should be preserved during global scaling:
  //  -1: all
  //   n: n bits, limited to all
  if (fixedPointBits) {
    int gsFracBits =
      pcc::ilog2(uint32_t(Rational(sps.globalScale).denominator));
    if (fixedPointBits < 0)
      fixedPointBits = gsFracBits;
    else
      fixedPointBits = std::min(fixedPointBits, gsFracBits);
  }

  this->geometry_axis_order = sps.geometry_axis_order;
  this->outputFpBits = fixedPointBits;
  this->outputOrigin = sps.seqBoundingBoxOrigin;
  this->outputUnitLength = pcc::reciprocal(sps.seqGeomScale);
  this->outputUnit = sps.seq_geom_scale_unit_flag;
  //this->attrDesc = sps.attributeSets;
  // TODO: movable
  for (const auto& a : sps.attributeSets) {
    attrDesc.emplace_back(AttributeDescriptionImpl(a));
  }
}

//---------------------------------------------------------------------------

CloudFrame*
CloudFrame::create()
{
  return new CloudFrameImpl();
}

//============================================================================

ConstCloudFrameWrapper::ConstCloudFrameWrapper(const pcc::CloudFrame& refFrame)
  : IntermediateCloudFrame()
  , cloudFrame(refFrame)
  , constCloud(refFrame.cloud)
{}

ConstCloudFrameWrapper::ConstCloudFrameWrapper(
  const pcc::CloudFrame& refFrame, const pcc::PCCPointSet3& refCloud)
  : IntermediateCloudFrame()
  , cloudFrame(refFrame)
  , constCloud(refCloud)
{}

//
void
ConstCloudFrameWrapper::release()
{
  delete this;
}

// int frameNum;
int
ConstCloudFrameWrapper::getFrameNum() const
{
  return cloudFrame.getFrameNum();
}
void
ConstCloudFrameWrapper::setFrameNum(int num)
{
  throw UnsupportedException();
  // cloudFrame.frameNum = num;
}

// AxisOrder geometry_axis_order;
AxisOrder
ConstCloudFrameWrapper::getGeometryAxisOrder() const
{
  return cloudFrame.getGeometryAxisOrder();
}
void
ConstCloudFrameWrapper::setGeometryAxisOrder(AxisOrder order)
{
  throw UnsupportedException();
  // cloudFrame.geometry_axis_order = order;
}

// double outputUnitLength;
double
ConstCloudFrameWrapper::getOutputUnitLength() const
{
  return cloudFrame.getOutputUnitLength();
}
void
ConstCloudFrameWrapper::setOutputUnitLength(double length)
{
  throw UnsupportedException();
  // cloudFrame.outputUnitLength = length;
}

// ScaleUnit outputUnit;
ScaleUnit
ConstCloudFrameWrapper::getOutputUnit() const
{
  return cloudFrame.getOutputUnit();
}
void
ConstCloudFrameWrapper::setOutputUnit(ScaleUnit val)
{
  throw UnsupportedException();
  // cloudFrame.outputUnit = val;
}

// Vec3<int> outputOrigin;
const point_t&
ConstCloudFrameWrapper::getOutputOrigin() const
{
  return cloudFrame.getOutputOrigin();
}
point_t&
ConstCloudFrameWrapper::getOutputOrigin()
{
  throw UnsupportedException();
  // return cloudFrame.outputOrigin;
}
void
ConstCloudFrameWrapper::setOutputOrigin(const point_t& val)
{
  throw UnsupportedException();
  // cloudFrame.outputOrigin = val;
}

int
ConstCloudFrameWrapper::getOutputFpBits() const
{
  return cloudFrame.getOutputFpBits();
}
void
ConstCloudFrameWrapper::setOutputFpBits(int bits)
{
  throw UnsupportedException();
  // cloudFrame.outputOrigin = val;
}

// std::vector<AttributeDescription> attrDesc;
// 指定した KnownAttributeLabel の要素を返す。存在しない場合、 nullptr を返す。
const AttributeDescriptionImpl*
ConstCloudFrameWrapper::getAttrDesc(KnownAttributeLabel label) const
{
  return cloudFrame.getAttrDesc(label);
}
AttributeDescriptionImpl*
ConstCloudFrameWrapper::getAttrDesc(KnownAttributeLabel label)
{
  throw UnsupportedException();
  // size_t index = static_cast<size_t>(label);
  // if (cloudFrame.attrDesc.size() > index) {
  //   //return &cloudFrame.attrDesc[index];
  // }

  return nullptr;
}

// PCCPointSet3 cloud;
const IntermediatePointSet3&
ConstCloudFrameWrapper::getCloud() const
{
  return constCloud;
}
IntermediatePointSet3&
ConstCloudFrameWrapper::getCloud()
{
  throw UnsupportedException();
}

// PCCPointSet3 cloud;
const ConstPointSet3Wrapper&
ConstCloudFrameWrapper::getRealCloud() const
{
  return constCloud;
}
ConstPointSet3Wrapper&
ConstCloudFrameWrapper::getRealCloud()
{
  throw UnsupportedException();
}

} // namespace gpcclib
