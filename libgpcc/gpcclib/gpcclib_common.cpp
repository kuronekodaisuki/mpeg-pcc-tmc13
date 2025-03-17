#include "gpcclib/common.hpp"
#include "TMC3Config.h"

#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <limits>

namespace gpcclib {

//============================================================================

const LibraryInfo::Version&
LibraryInfo::getVersion()
{
  static const LibraryInfo::Version LIBRARY_VERSION = {
    PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH};
  return LIBRARY_VERSION;
}

//============================================================================

// template<typename T>
// Vec3<T>::Vec3(const Vec3<T>& rhs)
// {
//   data[0] = rhs.data[0];
//   data[1] = rhs.data[1];
//   data[2] = rhs.data[2];
// }

// template<typename T>
// Vec3<T>::Vec3(const T a)
// {
//   data[0] = data[1] = data[2] = a;
// }

template<typename T>
Vec3<T>::Vec3(const T x, const T y, const T z)
{
  data[0] = x;
  data[1] = y;
  data[2] = z;
}

template<typename T>
Vec3<T>&
Vec3<T>::operator=(const Vec3<T>& rhs)
{
  data[0] = rhs.data[0];
  data[1] = rhs.data[1];
  data[2] = rhs.data[2];
  return *this;
}

template<typename T>
Vec3<T>&
Vec3<T>::operator+=(const T a)
{
  data[0] += a;
  data[1] += a;
  data[2] += a;
  return *this;
}

template<typename T>
Vec3<T>&
Vec3<T>::operator+=(const Vec3<T>& rhs)
{
  data[0] += rhs[0];
  data[1] += rhs[1];
  data[2] += rhs[2];
  return *this;
}

template<typename T>
Vec3<T>&
Vec3<T>::operator-=(const T a)
{
  data[0] += a;
  data[1] += a;
  data[2] += a;
  return *this;
}

template<typename T>
Vec3<T>&
Vec3<T>::operator-=(const Vec3<T>& rhs)
{
  data[0] += rhs[0];
  data[1] += rhs[1];
  data[2] += rhs[2];
  return *this;
}

// template<typename T>
// Vec3<T>
// operator+(const Vec3<T>& lhs, const T a)
// {
//   return Vec3<T>(lhs) += a;
// }

// template<typename T>
// Vec3<T>
// operator+(const Vec3<T>& lhs, const Vec3<T>& rhs)
// {
//   return Vec3<T>(lhs) += rhs;
// }

// template<typename T>
// Vec3<T>
// operator-(const Vec3<T>& lhs, const T a)
// {
//   return Vec3<T>(lhs) += a;
// }

// template<typename T>
// Vec3<T>
// operator-(const Vec3<T>& lhs, const Vec3<T>& rhs)
// {
//   return Vec3<T>(lhs) += rhs;
// }

template<typename T>
T*
Vec3<T>::begin()
{
  return &data[0];
}
template<typename T>
const T*
Vec3<T>::begin() const
{
  return &data[0];
}

template<typename T>
T*
Vec3<T>::end()
{
  return &data[3];
}
template<typename T>
const T*
Vec3<T>::end() const
{
  return &data[3];
}

template<typename T>
T&
Vec3<T>::operator[](size_t i)
{
  assert(i < 3);
  return data[i];
}
template<typename T>
const T&
Vec3<T>::operator[](size_t i) const
{
  assert(i < 3);
  return data[i];
}
template<typename T>
size_t
Vec3<T>::getElementCount() const
{
  return 3;
}

template<typename T>
T&
Vec3<T>::x()
{
  return data[0];
}
template<typename T>
T&
Vec3<T>::y()
{
  return data[1];
}
template<typename T>
T&
Vec3<T>::z()
{
  return data[2];
}
template<typename T>
const T&
Vec3<T>::x() const
{
  return data[0];
}
template<typename T>
const T&
Vec3<T>::y() const
{
  return data[1];
}
template<typename T>
const T&
Vec3<T>::z() const
{
  return data[2];
}

template<typename T>
T&
Vec3<T>::s()
{
  return data[0];
}
template<typename T>
T&
Vec3<T>::t()
{
  return data[1];
}
template<typename T>
T&
Vec3<T>::v()
{
  return data[2];
}
template<typename T>
const T&
Vec3<T>::s() const
{
  return data[0];
}
template<typename T>
const T&
Vec3<T>::t() const
{
  return data[1];
}
template<typename T>
const T&
Vec3<T>::v() const
{
  return data[2];
}

// The minimum element
template<typename T>
T
Vec3<T>::min() const
{
  return std::min({data[0], data[1], data[2]});
}

// The maximum element
template<typename T>
T
Vec3<T>::max() const
{
  return std::max({data[0], data[1], data[2]});
}

//---------------------------------------------------------------------------

template class GPCCLIB_INTERFACE_SPEC Vec3<int8_t>;
template class GPCCLIB_INTERFACE_SPEC Vec3<uint8_t>;  // not for library
template class GPCCLIB_INTERFACE_SPEC Vec3<int16_t>;  // not for library
template class GPCCLIB_INTERFACE_SPEC Vec3<uint16_t>; // attr_t
template class GPCCLIB_INTERFACE_SPEC Vec3<int32_t>;
template class GPCCLIB_INTERFACE_SPEC Vec3<uint32_t>; // not for library
template class GPCCLIB_INTERFACE_SPEC Vec3<int64_t>;
template class GPCCLIB_INTERFACE_SPEC Vec3<uint64_t>; // not for library
template class GPCCLIB_INTERFACE_SPEC Vec3<float>;    // not for library
template class GPCCLIB_INTERFACE_SPEC Vec3<double>;

//---------------------------------------------------------------------------

// template<typename T>
// Box3<T>::Box3(T min, T max)
//   : min(min)
//   , max(max)
// {}

template<typename T>
Box3<T>::Box3(const Vec3<T>& min, const Vec3<T>& max)
  : min(min)
  , max(max)
{}

template<typename T>
Box3<T>&
Box3<T>::operator=(const Box3<T>& rhs)
{
  if (this == &rhs) {
    return *this;
  }

  min = rhs.min;
  max = rhs.max;
  return *this;
}

template<typename T>
const typename Box3<T>::PointType&
Box3<T>::getMin() const
{
  return min;
}

template<typename T>
typename Box3<T>::PointType&
Box3<T>::getMin()
{
  return min;
}

template<typename T>
const typename Box3<T>::PointType&
Box3<T>::getMax() const
{
  return max;
}

template<typename T>
typename Box3<T>::PointType&
Box3<T>::getMax()
{
  return max;
}

template<typename T>
bool
Box3<T>::contains(const PointType& point) const
{
  return !(
    point.x() < min.x() || point.x() > max.x() || point.y() < min.y()
    || point.y() > max.y() || point.z() < min.z() || point.z() > max.z());
}

template<typename T>
Box3<T>
Box3<T>::merge(const Box3<T>& box)
{
  min.x() = std::min(min.x(), box.min.x());
  min.y() = std::min(min.y(), box.min.y());
  min.z() = std::min(min.z(), box.min.z());
  max.x() = std::max(max.x(), box.max.x());
  max.y() = std::max(max.y(), box.max.y());
  max.z() = std::max(max.z(), box.max.z());
  return box;
}

template<typename T>
bool
Box3<T>::intersects(const Box3<T>& box) const
{
  return max.x() >= box.min.x() && min.x() <= box.max.x()
    && max.y() >= box.min.y() && min.y() <= box.max.y()
    && max.z() >= box.min.z() && min.z() <= box.max.z();
}

template<typename T>
void
Box3<T>::insert(const PointType& point)
{
  min.x() = std::min(min.x(), point.x());
  min.y() = std::min(min.y(), point.y());
  min.z() = std::min(min.z(), point.z());
  max.x() = std::max(max.x(), point.x());
  max.y() = std::max(max.y(), point.y());
  max.z() = std::max(max.z(), point.z());
}

//---------------------------------------------------------------------------

template class GPCCLIB_INTERFACE_SPEC Box3<int32_t>;

//---------------------------------------------------------------------------

namespace {

//============================================================================
// Convert a floating point value to a rational representation

template<typename T>
static Rational
fromReal(T val, int maxQ)
{
  if (val == T())
    return Rational(0, 1);

  // Find a best rational approximation in the interval ndL <= val <= ndH.
  T ndL[2] = {std::nextafter(val, -std::numeric_limits<T>::infinity()), 1};
  T ndH[2] = {std::nextafter(val, +std::numeric_limits<T>::infinity()), 1};

  // pq is the state used in the calculation of the convergent P/Q using the
  // recurrence formula applied to successive terms of a simple continued
  // fraction.
  int pq[2][2] = {{1, 0}, {0, 1}};

  // Calculate the terms of the continued fractions of ndL and ndH.
  // Calculation proceeds until the terms (aiL, aiH) diverge.
  // The convergent P/Q is updated with each new term until the denominator
  // hits a limit.
  for (int i = 0; i < 10; i++) {
    int aiL = int(ndL[0] / ndL[1]);
    int aiH = int(ndH[0] / ndH[1]);

    int ai = aiL == aiH ? aiL : std::min(aiL, aiH) + 1;

    int p = ai * pq[0][0] + pq[1][0];
    int q = ai * pq[0][1] + pq[1][1];

    if (q > maxQ)
      break;

    pq[1][0] = pq[0][0];
    pq[1][1] = pq[0][1];
    pq[0][0] = p;
    pq[0][1] = q;

    if (aiL != aiH)
      break;

    auto remL = std::fmod(ndL[0], ndL[1]);
    auto remH = std::fmod(ndH[0], ndH[1]);

    ndL[0] = ndL[1];
    ndL[1] = remL;
    ndH[0] = ndH[1];
    ndH[1] = remH;
  }

  return Rational(pq[0][0], pq[0][1]);
}

} // namespace

Rational::Rational()
  : Rational(0, 1)
{}

Rational::Rational(int numerator)
  : Rational(numerator, 1)
{}

Rational::Rational(int numerator, int denominator)
  : numerator(numerator)
  , denominator(denominator)
{}

//----------------------------------------------------------------------------

Rational::Rational(float val)
{
  *this = fromReal(val, 1 << 16);
}

Rational::Rational(double val)
{
  *this = fromReal(val, 1 << 16);
}

//---------------------------------------------------------------------------

Rational::
operator double() const
{
  return double(numerator) / double(denominator);
}

Rational::
operator float() const
{
  return float(numerator) / float(denominator);
}

//---------------------------------------------------------------------------

} // namespace gpcclib
