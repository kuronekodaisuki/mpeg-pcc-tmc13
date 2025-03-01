#pragma once

// #include "TMC3.h"

#include <memory>

#include "PCCTMC3Encoder.h"       // encoder
#include "PCCTMC3Decoder.h"       // decoder
#include "constants.h"            // encoder decoder
#include "ply.h"

// #include "pointset_processing.h"  // encoder
#include "PCCPointSet.h"

#include "program_options_lite.h"
#include "io_tlv.h"               // decoder
#include "version.h"

using namespace std;
using namespace pcc;

//---------------------------------------------------------------------------
#if 0
std::array<const char*, 3>
axisOrderToPropertyNames(AxisOrder order)
{
  static const std::array<const char*, 3> kAxisOrderToPropertyNames[] = {
    {"z", "y", "x"}, {"x", "y", "z"}, {"x", "z", "y"}, {"y", "z", "x"},
    {"z", "y", "x"}, {"z", "x", "y"}, {"y", "x", "z"}, {"x", "y", "z"},
  };

  return kAxisOrderToPropertyNames[int(order)];
}
#endif
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

namespace pcc {
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
}  // namespace pcc

static std::istream& operator>>(std::istream& in, OutputSystem& val)
{  return readUInt(in, val);}

namespace pcc {
static std::istream& operator>>(std::istream& in, ColourMatrix& val)
{  return readUInt(in, val);}

static std::istream& operator>>(std::istream& in, AxisOrder& val)
{  return readUInt(in, val);}

static std::istream& operator>>(std::istream& in, AttributeEncoding& val)
{  return readUInt(in, val);}

static std::istream& operator>>(std::istream& in, LodDecimationMethod& val)
{  return readUInt(in, val);}

static std::istream& operator>>(std::istream& in, PartitionMethod& val)
{  return readUInt(in, val);}

static std::istream& operator>>(std::istream& in, PredGeomEncOpts::SortMode& val)
{  return readUInt(in, val);}

static std::istream& operator>>(std::istream& in, OctreeEncOpts::QpMethod& val)
{  return readUInt(in, val);}
}  // namespace pcc

static std::ostream& operator<<(std::ostream& out, const OutputSystem& val)
{
  switch (val) {
  case OutputSystem::kConformance: out << "0 (Conformance)"; break;
  case OutputSystem::kExternal: out << "1 (External)"; break;
  }
  return out;
}

namespace pcc {
static std::ostream& operator<<(std::ostream& out, const ScaleUnit& val)
{
  switch (val) {
  case ScaleUnit::kDimensionless: out << "0 (Dimensionless)"; break;
  case ScaleUnit::kMetre: out << "1 (Metre)"; break;
  }
  return out;
}

static std::ostream& operator<<(std::ostream& out, const ColourMatrix& val)
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

static std::ostream& operator<<(std::ostream& out, const AxisOrder& val)
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

static std::ostream& operator<<(std::ostream& out, const AttributeEncoding& val)
{
  switch (val) {
  case AttributeEncoding::kRAHTransform: out << "0 (RAHT)"; break;
  case AttributeEncoding::kPredictingTransform: out << "1 (Pred)"; break;
  case AttributeEncoding::kLiftingTransform: out << "2 (Lift)"; break;
  case AttributeEncoding::kRaw: out << "3 (Raw)"; break;
  }
  return out;
}

static std::ostream& operator<<(std::ostream& out, const LodDecimationMethod& val)
{
  switch (val) {
  case LodDecimationMethod::kNone: out << "0 (None)"; break;
  case LodDecimationMethod::kPeriodic: out << "1 (Periodic)"; break;
  case LodDecimationMethod::kCentroid: out << "2 (Centroid)"; break;
  }
  return out;
}

static std::ostream& operator<<(std::ostream& out, const PartitionMethod& val)
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

static std::ostream& operator<<(std::ostream& out, const PredGeomEncOpts::SortMode& val)
{
  switch (val) {
    using SortMode = PredGeomEncOpts::SortMode;
  case SortMode::kNoSort: out << int(val) << " (None)"; break;
  case SortMode::kSortMorton: out << int(val) << " (Morton)"; break;
  case SortMode::kSortAzimuth: out << int(val) << " (Azimuth)"; break;
  case SortMode::kSortRadius: out << int(val) << " (Radius)"; break;
  default: out << int(val) << " (Unknown)"; break;
  }
  return out;
}

static std::ostream& operator<<(std::ostream& out, const OctreeEncOpts::QpMethod& val)
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
}  // namespace pcc

#if 1
namespace df {
namespace program_options_lite {
  template<typename T>
  struct option_detail<pcc::Vec3<T>> {
    static constexpr bool is_container = true;
    static constexpr bool is_fixed_size = true;
    typedef T* output_iterator;

    static void clear(pcc::Vec3<T>& container){};
    static output_iterator make_output_iterator(pcc::Vec3<T>& container)
    {
      return &container[0];
    }
  };
}  // namespace program_options_lite
}  // namespace df
#endif

//---------------------------------------------------------------------------
// :: Command line / config parsing

