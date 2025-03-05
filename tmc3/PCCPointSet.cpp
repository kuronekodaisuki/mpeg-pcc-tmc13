#include "PCCPointSet.h"

namespace pcc 
{
PCCPointSet3::PCCPointSet3()
{
  withColors = false;
  withReflectances = false;
  withFrameIndex = false;
  withLaserAngles = false;
}

void
PCCPointSet3::swap(PCCPointSet3& other)
{
  using std::swap;
  swap(positions, other.positions);
  swap(colors, other.colors);
  swap(reflectances, other.reflectances);
  swap(frameidx, other.frameidx);
  swap(laserAngles, other.laserAngles);
  swap(withColors, other.withColors);
  swap(withReflectances, other.withReflectances);
  swap(withFrameIndex, other.withFrameIndex);
  swap(withLaserAngles, other.withLaserAngles);
}

void
PCCPointSet3::swapPoints(std::vector<PointType>& other)
{
  positions.swap(other);
}

Vec3<attr_t>
PCCPointSet3::getColor(const size_t index) const
{
  assert(index < colors.size() && withColors);
  return colors[index];
}

Vec3<attr_t>&
PCCPointSet3::getColor(const size_t index)
{
  assert(index < colors.size() && withColors);
  return colors[index];
}

void
PCCPointSet3::setColor(const size_t index, const Vec3<attr_t> color)
{
  assert(index < colors.size() && withColors);
  colors[index] = color;
}

attr_t
PCCPointSet3::getReflectance(const size_t index) const
{
  assert(index < reflectances.size() && withReflectances);
  return reflectances[index];
}

attr_t&
PCCPointSet3::getReflectance(const size_t index)
{
  assert(index < reflectances.size() && withReflectances);
  return reflectances[index];
}

void
PCCPointSet3::setReflectance(const size_t index, const attr_t reflectance)
{
  assert(index < reflectances.size() && withReflectances);
  reflectances[index] = reflectance;
}

bool
PCCPointSet3::hasReflectances() const
{
  return withReflectances;
}

void
PCCPointSet3::addReflectances()
{
  withReflectances = true;
  resize(getPointCount());
}

void
PCCPointSet3::removeReflectances()
{
  withReflectances = false;
  reflectances.resize(0);
}

uint8_t
PCCPointSet3::getFrameIndex(const size_t index) const
{
  assert(index < frameidx.size() && withFrameIndex);
  return frameidx[index];
}

uint8_t&
PCCPointSet3::getFrameIndex(const size_t index)
{
  assert(index < frameidx.size() && withFrameIndex);
  return frameidx[index];
}

void
PCCPointSet3::setFrameIndex(const size_t index, const uint8_t frameindex)
{
  assert(index < frameidx.size() && withFrameIndex);
  frameidx[index] = frameindex;
}

bool
PCCPointSet3::hasFrameIndex() const
{
  return withFrameIndex;
}

void
PCCPointSet3::addFrameIndex()
{
  withFrameIndex = true;
  resize(getPointCount());
}

void
PCCPointSet3::removeFrameIndex()
{
  withFrameIndex = false;
  frameidx.resize(0);
}

int
PCCPointSet3::getLaserAngle(const size_t index) const
{
  assert(index < laserAngles.size() && withLaserAngles);
  return laserAngles[index];
}

int&
PCCPointSet3::getLaserAngle(const size_t index)
{
  assert(index < laserAngles.size() && withLaserAngles);
  return laserAngles[index];
}

void
PCCPointSet3::setLaserAngle(const size_t index, const int angle)
{
  assert(index < laserAngles.size() && withLaserAngles);
  laserAngles[index] = angle;
}

bool
PCCPointSet3::hasLaserAngles() const
{
  return withLaserAngles;
}

void
PCCPointSet3::addLaserAngles()
{
  withLaserAngles = true;
  resize(getPointCount());
}

void
PCCPointSet3::removeLaserAngles()
{
  withLaserAngles = false;
  laserAngles.resize(0);
}

bool
PCCPointSet3::hasColors() const
{
  return withColors;
}

void
PCCPointSet3::addColors()
{
  withColors = true;
  resize(getPointCount());
}

void
PCCPointSet3::removeColors()
{
  withColors = false;
  colors.resize(0);
}

void
PCCPointSet3::addRemoveAttributes(bool withColors, bool withReflectances)
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

void
PCCPointSet3::addRemoveAttributes(const PCCPointSet3& ref)
{
  ref.hasColors() ? addColors() : removeColors();
  ref.hasReflectances() ? addReflectances() : removeReflectances();
  ref.hasLaserAngles() ? addLaserAngles() : removeLaserAngles();
}

size_t
PCCPointSet3::getPointCount() const
{
  return positions.size();
}

void PCCPointSet3::resize(const size_t size)
{
  positions.resize(size);
  if (hasColors()) {
    colors.resize(size);
  }
  if (hasReflectances()) {
    reflectances.resize(size);
  }
  if (hasFrameIndex()) {
    frameidx.resize(size);
  }
  if (hasLaserAngles()) {
    laserAngles.resize(size);
  }
}

void
PCCPointSet3::reserve(const size_t size)
{
  positions.reserve(size);
  if (hasColors()) {
    colors.reserve(size);
  }
  if (hasReflectances()) {
    reflectances.reserve(size);
  }
  if (hasFrameIndex()) {
    frameidx.reserve(size);
  }
  if (hasLaserAngles()) {
    laserAngles.reserve(size);
  }
}

void
PCCPointSet3::clear()
{
  positions.clear();
  colors.clear();
  reflectances.clear();
  frameidx.clear();
  laserAngles.clear();
}

#ifdef EXPORT
size_t
PCCPointSet3::removeDuplicatePointInQuantizedPoint(int minGeomNodeSizeLog2)
{
  for (int i = 0; i < positions.size(); i++) {
    PointType newPoint = positions[i];
    if (minGeomNodeSizeLog2 > 0) {
      uint32_t mask = ((uint32_t)-1) << minGeomNodeSizeLog2;
      positions[i]._x = ((int32_t)(positions[i].x()) & mask);
      positions[i]._y = ((int32_t)(positions[i].y()) & mask);
      positions[i]._z = ((int32_t)(positions[i].z()) & mask);
    }
  }
  positions.erase(
    std::unique(positions.begin(), positions.end()), positions.end());

  return positions.size();
}

/// <summary>
/// äOê⁄éZèo
/// </summary>
/// <returns></returns>
Box3<int32_t>
PCCPointSet3::computeBoundingBox() const
{
  Box3<int32_t> bbox(
    std::numeric_limits<int32_t>::max(),
    std::numeric_limits<int32_t>::lowest());
  const size_t pointCount = getPointCount();

  for (size_t i = 0; i < pointCount; ++i) {
    const auto pt = (*this)[i];
    if (bbox.max[0] < pt._x)
      bbox.max[0] = pt._x;
    else if (pt._x < bbox.min[0])
      bbox.min[0] = pt._x;

    if (bbox.max[1] < pt._y)
      bbox.max[1] = pt._y;
    else if (pt._y < bbox.min[1])
      bbox.min[1] = pt._y;

    if (bbox.max[2] < pt._z)
      bbox.max[2] = pt._z;
    else if (pt._z < bbox.min[2])
      bbox.min[2] = pt._z;
  }
  return bbox;
}

void
PCCPointSet3::shiftPointPositions(const pcc::point_t shiftVal)
{
  const size_t pointCount = getPointCount();
  for (size_t i = 0; i < pointCount; ++i)
    positions[i] += shiftVal;
}
#else
size_t
PCCPointSet3::removeDuplicatePointInQuantizedPoint(int minGeomNodeSizeLog2)
{
  for (int i = 0; i < positions.size(); i++) {
    PointType newPoint = positions[i];
    if (minGeomNodeSizeLog2 > 0) {
      uint32_t mask = ((uint32_t)-1) << minGeomNodeSizeLog2;
      positions[i].x() = ((int32_t)(positions[i].x()) & mask);
      positions[i].y() = ((int32_t)(positions[i].y()) & mask);
      positions[i].z() = ((int32_t)(positions[i].z()) & mask);
    }
  }
  positions.erase(
    std::unique(positions.begin(), positions.end()), positions.end());

  return positions.size();
}

Box3<int32_t>
PCCPointSet3::computeBoundingBox() const
{
  Box3<int32_t> bbox(
    std::numeric_limits<int32_t>::max(),
    std::numeric_limits<int32_t>::lowest());
  const size_t pointCount = getPointCount();
  for (size_t i = 0; i < pointCount; ++i) {
    const auto& pt = (*this)[i];
    for (int k = 0; k < 3; ++k) {
      if (pt[k] > bbox.max[k]) {
        bbox.max[k] = pt[k];
      }
      if (pt[k] < bbox.min[k]) {
        bbox.min[k] = pt[k];
      }
    }
  }
  return bbox;
}

void
PCCPointSet3::shiftPointPositions(const pcc::point_t shiftVal)
{
  const size_t pointCount = getPointCount();
  for (size_t i = 0; i < pointCount; ++i)
    positions[i] += shiftVal;
}
#endif

void
PCCPointSet3::append(const PCCPointSet3& src)
{
  if (!getPointCount())
    addRemoveAttributes(src);

  size_t dstEnd = positions.size();
  size_t srcSize = src.positions.size();
  resize(dstEnd + srcSize);

  std::copy(
    src.positions.begin(), src.positions.end(),
    std::next(positions.begin(), dstEnd));

  if (hasColors() && src.hasColors())
    std::copy(
      src.colors.begin(), src.colors.end(), std::next(colors.begin(), dstEnd));

  if (hasReflectances() && src.hasReflectances())
    std::copy(
      src.reflectances.begin(), src.reflectances.end(),
      std::next(reflectances.begin(), dstEnd));

  if (hasLaserAngles())
    std::copy(
      src.laserAngles.begin(), src.laserAngles.end(),
      std::next(laserAngles.begin(), dstEnd));
}

/// <summary>
/// Add a set of points to the end of the point cloud.
/// </summary>
/// <param name="positions">point cloud captured by iPhone LiDAR</param>
void
PCCPointSet3::Append(std::vector<PointType> positions)
{
  if (!getPointCount())
    resize(positions.size());

  size_t dstEnd = this->positions.size();
  size_t srcSize = positions.size();
  resize(dstEnd + srcSize);
  std::copy(
    positions.begin(), positions.end(),
    std::next(this->positions.begin(), dstEnd));
}

void
PCCPointSet3::swapPoints(const size_t index1, const size_t index2)
{
  assert(index1 < getPointCount());
  assert(index2 < getPointCount());
  std::swap((*this)[index1], (*this)[index2]);
  if (hasColors()) {
    std::swap(getColor(index1), getColor(index2));
  }
  if (hasReflectances()) {
    std::swap(getReflectance(index1), getReflectance(index2));
  }
  if (hasLaserAngles()) {
    std::swap(getLaserAngle(index1), getLaserAngle(index2));
  }
}

//--------------------------------------------------------------------------
// Determine the bounding box of the set of points given by the indicies
// given by iterating over [begin, end)

}  // namespace pcc