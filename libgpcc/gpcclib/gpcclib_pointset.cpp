#include "PCCPointSet.h"
#include "gpcclib/internal/pointset.hpp"
#include "gpcclib/internal/common.hpp"
#include "gpcclib/pointset.hpp"

namespace gpcclib {

void
IntermediatePointSet3::addRemoveAttributes(
  bool withColors, bool withReflectances)
{
  if (withColors)
    addColors();
  else
    removeColors();

  if (withReflectances)
    addReflectances();
  else
    removeReflectances();
}

//============================================================================

PointSet3Impl::PointSet3Impl(pcc::PCCPointSet3&& rhs) noexcept
  : cloud(rhs)
{}

void
PointSet3Impl::release()
{
  delete this;
}

size_t
PointSet3Impl::getPointCount() const
{
  return cloud.getPointCount();
}

PointSet3::PointType
PointSet3Impl::getPosition(size_t index) const
{
  return PointType(cloud[index]);
}

PointSet3::PointType&
PointSet3Impl::getPosition(size_t index)
{
  return cloud[index];
}

void
PointSet3Impl::setPosition(size_t index, const PointType& position)
{
  cloud[index] = pcc::PCCPointSet3::PointType(position);
}

PointSet3::AttrVec3
PointSet3Impl::getColor(size_t index) const
{
  return AttrVec3(cloud.getColor(index));
}

PointSet3::AttrVec3&
PointSet3Impl::getColor(size_t index)
{
  return cloud.getColor(index);
}

void
PointSet3Impl::setColor(size_t index, const AttrVec3& color)
{
  cloud.setColor(index, pcc::Vec3<attr_t>(color));
}

bool
PointSet3Impl::hasColors() const
{
  return cloud.hasColors();
}

void
PointSet3Impl::addColors()
{
  cloud.addColors();
}

void
PointSet3Impl::removeColors()
{
  cloud.removeColors();
}

attr_t
PointSet3Impl::getReflectance(size_t index) const
{
  return cloud.getReflectance(index);
}

attr_t&
PointSet3Impl::getReflectance(size_t index)
{
  return cloud.getReflectance(index);
}

void
PointSet3Impl::setReflectance(size_t index, const attr_t& reflectance)
{
  cloud.setReflectance(index, reflectance);
}

bool
PointSet3Impl::hasReflectances() const
{
  return cloud.hasReflectances();
}

void
PointSet3Impl::addReflectances()
{
  cloud.addReflectances();
}

void
PointSet3Impl::removeReflectances()
{
  cloud.removeReflectances();
}

uint8_t
PointSet3Impl::getFrameIndex(size_t index) const
{
  return cloud.getFrameIndex(index);
}

uint8_t&
PointSet3Impl::getFrameIndex(size_t index)
{
  return cloud.getFrameIndex(index);
}

void
PointSet3Impl::setFrameIndex(size_t index, uint8_t frameIndex)
{
  cloud.setFrameIndex(index, frameIndex);
}

bool
PointSet3Impl::hasFrameIndex() const
{
  return cloud.hasFrameIndex();
}

void
PointSet3Impl::addFrameIndex()
{
  cloud.addFrameIndex();
}

void
PointSet3Impl::removeFrameIndex()
{
  cloud.removeFrameIndex();
}

void
PointSet3Impl::swap(PointSet3& other)
{
  PointSet3Impl* impl = dynamic_cast<PointSet3Impl*>(&other);

  // TODO: error
  if (nullptr != impl) {
    cloud.swap(impl->cloud);
  }
}

void
PointSet3Impl::resize(size_t size)
{
  cloud.resize(size);
}

void
PointSet3Impl::reserve(size_t size)
{
  cloud.reserve(size);
}

void
PointSet3Impl::clear()
{
  cloud.clear();
}

void
PointSet3Impl::append(const PointSet3& src)
{
  const PointSet3Impl* impl = dynamic_cast<const PointSet3Impl*>(&src);

  // TODO: error
  if (nullptr != impl) {
    cloud.append(impl->cloud);
  }
}

void
PointSet3Impl::swapPoints(const size_t index1, const size_t index2)
{
  cloud.swapPoints(index1, index2);
}

PointSet3::BoundingBoxType
PointSet3Impl::computeBoundingBox() const
{
  auto bbox = cloud.computeBoundingBox();
  return BoundingBoxType(bbox.min, bbox.max);
}

const pcc::PCCPointSet3&
PointSet3Impl::getCloud() const
{
  return cloud;
}

pcc::PCCPointSet3&
PointSet3Impl::getCloud()
{
  return cloud;
}

// static function
PointSet3*
PointSet3::create()
{
  return new PointSet3Impl();
}

//============================================================================

ConstPointSet3Wrapper::ConstPointSet3Wrapper(const pcc::PCCPointSet3& refCloud)
  : IntermediatePointSet3()
  , cloud(refCloud)
{}

void
ConstPointSet3Wrapper::release()
{
  delete this;
}

size_t
ConstPointSet3Wrapper::getPointCount() const
{
  return cloud.getPointCount();
}

PointSet3::PointType
ConstPointSet3Wrapper::getPosition(size_t index) const
{
  return PointType(cloud[index]);
}

PointSet3::PointType&
ConstPointSet3Wrapper::getPosition(size_t index)
{
  throw UnsupportedException();
  // return cloud[index];
}

void
ConstPointSet3Wrapper::setPosition(size_t index, const PointType& position)
{
  cloud[index] = pcc::PCCPointSet3::PointType(position);
}

PointSet3::AttrVec3
ConstPointSet3Wrapper::getColor(size_t index) const
{
  return AttrVec3(cloud.getColor(index));
}

PointSet3::AttrVec3&
ConstPointSet3Wrapper::getColor(size_t index)
{
  throw UnsupportedException();
  // return cloud.getColor(index);
}

void
ConstPointSet3Wrapper::setColor(size_t index, const AttrVec3& color)
{
  throw UnsupportedException();
  // cloud.setColor(index, color);
}

bool
ConstPointSet3Wrapper::hasColors() const
{
  return cloud.hasColors();
}

void
ConstPointSet3Wrapper::addColors()
{
  throw UnsupportedException();
  // cloud.addColors();
}

void
ConstPointSet3Wrapper::removeColors()
{
  throw UnsupportedException();
  // cloud.removeColors();
}

attr_t
ConstPointSet3Wrapper::getReflectance(size_t index) const
{
  return cloud.getReflectance(index);
}

attr_t&
ConstPointSet3Wrapper::getReflectance(size_t index)
{
  throw UnsupportedException();
  // return cloud.getReflectance(index);
}

void
ConstPointSet3Wrapper::setReflectance(size_t index, const attr_t& reflectance)
{
  throw UnsupportedException();
  // cloud.setReflectance(index, reflectance);
}

bool
ConstPointSet3Wrapper::hasReflectances() const
{
  return cloud.hasReflectances();
}

void
ConstPointSet3Wrapper::addReflectances()
{
  throw UnsupportedException();
  // cloud.addReflectances();
}

void
ConstPointSet3Wrapper::removeReflectances()
{
  throw UnsupportedException();
  // cloud.removeReflectances();
}

uint8_t
ConstPointSet3Wrapper::getFrameIndex(size_t index) const
{
  return cloud.getFrameIndex(index);
}

uint8_t&
ConstPointSet3Wrapper::getFrameIndex(size_t index)
{
  throw UnsupportedException();
  // return cloud.getFrameIndex(index);
}

void
ConstPointSet3Wrapper::setFrameIndex(size_t index, uint8_t frameIndex)
{
  throw UnsupportedException();
  // cloud.setFrameIndex(index, frameIndex);
}

bool
ConstPointSet3Wrapper::hasFrameIndex() const
{
  return cloud.hasFrameIndex();
}

void
ConstPointSet3Wrapper::addFrameIndex()
{
  throw UnsupportedException();
  // cloud.addFrameIndex();
}

void
ConstPointSet3Wrapper::removeFrameIndex()
{
  throw UnsupportedException();
  // cloud.removeFrameIndex();
}

void
ConstPointSet3Wrapper::swap(PointSet3& other)
{
  throw UnsupportedException();
  // ConstPointSet3Wrapper* impl = dynamic_cast<ConstPointSet3Wrapper*>(&other);

  // // TODO: error
  // if (nullptr != impl) {
  //   cloud.swap(impl->cloud);
  // }
}

void
ConstPointSet3Wrapper::resize(size_t size)
{
  throw UnsupportedException();
  // cloud.resize(size);
}

void
ConstPointSet3Wrapper::reserve(size_t size)
{
  throw UnsupportedException();
  // cloud.reserve(size);
}

void
ConstPointSet3Wrapper::clear()
{
  throw UnsupportedException();
  // cloud.clear();
}

void
ConstPointSet3Wrapper::append(const PointSet3& src)
{
  throw UnsupportedException();
  // const ConstPointSet3Wrapper* impl = dynamic_cast<const ConstPointSet3Wrapper*>(&src);

  // // TODO: error
  // if (nullptr != impl) {
  //   cloud.append(impl->cloud);
  // }
}

void
ConstPointSet3Wrapper::swapPoints(const size_t index1, const size_t index2)
{
  throw UnsupportedException();
  // cloud.swapPoints(index1, index2);
}

PointSet3::BoundingBoxType
ConstPointSet3Wrapper::computeBoundingBox() const
{
  auto bbox = cloud.computeBoundingBox();
  return BoundingBoxType(bbox.min, bbox.max);
}

const pcc::PCCPointSet3&
ConstPointSet3Wrapper::getCloud() const
{
  return cloud;
}

pcc::PCCPointSet3&
ConstPointSet3Wrapper::getCloud()
{
  throw UnsupportedException();
  // return cloud;
}

} // namespace gpcclib
