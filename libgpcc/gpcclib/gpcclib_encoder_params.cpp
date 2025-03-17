#include "gpcclib/encoder/params.hpp"
#include "gpcclib/internal/encoder/params.hpp"
#include "gpcclib/internal/common.hpp"
#include "gpcclib/internal/params.hpp"
#include "gpcclib/internal/expiration.hpp"
#include <cstddef>
#include <map>
#include <utility>

namespace gpcclib {
namespace encoder {

//============================================================================

void
CopyParams(pcc::AttributeBrickHeader& dst, const AttributeBrickHeader& src)
{
  CopyVector(dst.attr_layer_qp_delta_luma, src.getAttrLayerQpDeltaLuma());
  CopyVector(dst.attr_layer_qp_delta_chroma, src.getAttrLayerQpDeltaChroma());
}

//---------------------------------------------------------------------------

AttributeBrickHeaderImpl::AttributeBrickHeaderImpl(
  const AttributeBrickHeader& rhs)
  : attr_layer_qp_delta_luma(rhs.getAttrLayerQpDeltaLuma())
  , attr_layer_qp_delta_chroma(rhs.getAttrLayerQpDeltaChroma())
{}

const Int32VectorAdapterImpl&
AttributeBrickHeaderImpl::getAttrLayerQpDeltaLuma() const
{
  return attr_layer_qp_delta_luma;
}
Int32VectorAdapterImpl&
AttributeBrickHeaderImpl::getAttrLayerQpDeltaLuma()
{
  return attr_layer_qp_delta_luma;
}

const Int32VectorAdapterImpl&
AttributeBrickHeaderImpl::getAttrLayerQpDeltaChroma() const
{
  return attr_layer_qp_delta_chroma;
}
Int32VectorAdapterImpl&
AttributeBrickHeaderImpl::getAttrLayerQpDeltaChroma()
{
  return attr_layer_qp_delta_chroma;
}

//============================================================================

void
CopyParams(pcc::EncoderAttributeParams& dst, const EncoderAttributeParams& src)
{
  CopyParams(dst.abh, src.getAbh());
  dst.dist2PercentileEstimate = src.getDist2PercentileEstimate();
}

//---------------------------------------------------------------------------

EncoderAttributeParamsImpl::EncoderAttributeParamsImpl(
  const EncoderAttributeParams& rhs)
  : abh(rhs.getAbh())
  , dist2PercentileEstimate(rhs.getDist2PercentileEstimate())
{}

void
EncoderAttributeParamsImpl::release()
{
  delete this;
}

// NB: this only makes sense for setting configurable parameters
const AttributeBrickHeaderImpl&
EncoderAttributeParamsImpl::getAbh() const
{
  return abh;
}
AttributeBrickHeaderImpl&
EncoderAttributeParamsImpl::getAbh()
{
  return abh;
}

// Threshold for choosing dist2 out of the population of nearest neighbour
// distances.
float
EncoderAttributeParamsImpl::getDist2PercentileEstimate() const
{
  return dist2PercentileEstimate;
}
void
EncoderAttributeParamsImpl::setDist2PercentileEstimate(float distance)
{
  dist2PercentileEstimate = distance;
}

//---------------------------------------------------------------------------

EncoderAttributeParams*
EncoderAttributeParams::create()
{
  try {
    return new EncoderAttributeParamsImpl();
  }
  catch (std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

void
CopyParams(pcc::EncoderParams& dst, const EncoderParams& src)
{
  CopyParams(dst.sps, src.getSps());
  CopyParams(dst.gps, src.getGps());
  CopyParams(dst.gbh, src.getGbh());

  CopyObjectVector(dst.aps, src.getAps());
  CopyObjectVector(dst.attr, src.getAttr());

  //dst.attributeIdxMap = src.attributeIdxMap();
  for (size_t i = 0; i < src.getNumOfAttributes(); ++i) {
    const char* name = src.getAttributeName(i);
    dst.attributeIdxMap.insert(
      std::make_pair(std::string(name), src.getAttributeIndex(name)));
  }

  dst.autoSeqBbox = src.isAutoSeqBbox();

  dst.srcUnitLength = src.getSrcUnitLength();

  dst.codedGeomScale = src.getCodedGeomScale();

  dst.seqGeomScale = src.getSeqGeomScale();

  dst.extGeomScale = src.getExtGeomScale();

  dst.outputFpBits = src.getOutputFpBits();

  dst.geom = src.getGeom();

  dst.predGeom = src.getPredGeom();

  dst.partition = src.getPartition();

  dst.recolour = src.getRecolour();

  // unused param
  //dst.lidarHeadPosition = pcc::Vec3<int>(lidarHeadPosition);

  dst.numLasers = src.getNumLasers();

  CopyVector(dst.lasersTheta, src.getLasersTheta());
  CopyVector(dst.lasersZ, src.getLasersZ());
  CopyVector(dst.trisoupNodeSizesLog2, src.getTrisoupNodeSizesLog2());

  dst.enforceLevelLimits = src.isEnforceLevelLimits();

  dst.idcmQp = src.getIdcmQp();
}

//---------------------------------------------------------------------------

void
EncoderParamsImpl::release()
{
  delete this;
}

unsigned int
EncoderParamsImpl::getNumOfFrameThreads() const
{
  return numOfFrameThreads;
}
void
EncoderParamsImpl::setNumOfFrameThreads(unsigned int numOfThreads)
{
  numOfFrameThreads = numOfThreads;
}

bool
EncoderParamsImpl::doSliceParallelProc() const
{
  return doSliceParallel;
}
void
EncoderParamsImpl::setDoSliceParallelProc(bool parallel)
{
  doSliceParallel = parallel;
}

bool
EncoderParamsImpl::shouldSortInputByAzimuth() const
{
  return sortInputByAzimuth;
}

void
EncoderParamsImpl::setSortInputByAzimuth(bool enabled)
{
  sortInputByAzimuth = enabled;
}

bool
EncoderParamsImpl::shouldConvertColourspace() const
{
  return convertColourspace;
}
void
EncoderParamsImpl::setConvertColourspace(bool enabled)
{
  convertColourspace = enabled;
}

const SequenceParameterSetImpl&
EncoderParamsImpl::getSps() const
{
  return sps;
}
SequenceParameterSetImpl&
EncoderParamsImpl::getSps()
{
  return sps;
}

const GeometryParameterSetImpl&
EncoderParamsImpl::getGps() const
{
  return gps;
}
GeometryParameterSetImpl&
EncoderParamsImpl::getGps()
{
  return gps;
}

const GeometryBrickHeaderImpl&
EncoderParamsImpl::getGbh() const
{
  return gbh;
}
GeometryBrickHeaderImpl&
EncoderParamsImpl::getGbh()
{
  return gbh;
}

// NB: information about attributes is split between the SPS and the APS.
//  => The SPS enumerates the attributes, the APS controls coding params.
const AttributeParameterSetImplVector&
EncoderParamsImpl::getAps() const
{
  return aps;
}
AttributeParameterSetImplVector&
EncoderParamsImpl::getAps()
{
  return aps;
}

size_t
EncoderParamsImpl::getNumOfAttributes() const
{
  return attributeIdxMap.size();
}

const char*
EncoderParamsImpl::getAttributeName(size_t pos) const
{
  std::map<std::string, int>::const_iterator it = attributeIdxMap.cbegin();
  for (size_t i = 0; i < pos; ++i) {
    ++it;
  }
  return it->first.c_str();
}

bool
EncoderParamsImpl::hasAttributeIndex(const char* name) const
{
  return attributeIdxMap.count(name) != 0;
}

size_t
EncoderParamsImpl::getAttributeIndex(const char* name) const
{
  return attributeIdxMap.at(name);
}

void
EncoderParamsImpl::setAttributeIndex(const char* name, size_t index)
{
  attributeIdxMap.insert({name, index});
}

void
EncoderParamsImpl::clearAttributeIndex()
{
  attributeIdxMap.clear();
}

// Encoder specific parameters for attributes
const EncoderAttributeParamsImplVector&
EncoderParamsImpl::getAttr() const
{
  return attr;
}
EncoderAttributeParamsImplVector&
EncoderParamsImpl::getAttr()
{
  return attr;
}

// Determine the sequence bonuding box using the first input frame
bool
EncoderParamsImpl::isAutoSeqBbox() const
{
  return autoSeqBbox;
}
void
EncoderParamsImpl::setAutoSeqBbox(bool isAuto)
{
  autoSeqBbox = isAuto;
}

// Length of the source point cloud unit vectors.
double
EncoderParamsImpl::getSrcUnitLength() const
{
  return srcUnitLength;
}
void
EncoderParamsImpl::setSrcUnitLength(double length)
{
  srcUnitLength = length;
}

// Scale factor used to define the coordinate system used for coding.
// This is the coordinate system where slicing is performed.
//  P_cod = P_src * codedGeomScale
double
EncoderParamsImpl::getCodedGeomScale() const
{
  return codedGeomScale;
}
void
EncoderParamsImpl::setCodedGeomScale(double scale)
{
  codedGeomScale = scale;
}

// Scale factor used to define the sequence coordinate system.
//  P_seq = P_src * seqGeomScale
double
EncoderParamsImpl::getSeqGeomScale() const
{
  return seqGeomScale;
}
void
EncoderParamsImpl::setSeqGeomScale(double scale)
{
  seqGeomScale = scale;
}

// Scale factor used to define the external coordinate system.
//  P_ext = P_src * extGeomScale
double
EncoderParamsImpl::getExtGeomScale() const
{
  return extGeomScale;
}
void
EncoderParamsImpl::setExtGeomScale(double scale)
{
  extGeomScale = scale;
}

// Number of fractional bits used in output position representation.
int
EncoderParamsImpl::getOutputFpBits() const
{
  return outputFpBits;
}
void
EncoderParamsImpl::setOutputFpBits(int fractionbits)
{
  outputFpBits = fractionbits;
}

// Encoder specific parameters for geometry
const OctreeEncOpts&
EncoderParamsImpl::getGeom() const
{
  return geom;
}
OctreeEncOpts&
EncoderParamsImpl::getGeom()
{
  return geom;
}

// Options for the predictive geometry coder
const PredGeomEncOpts&
EncoderParamsImpl::getPredGeom() const
{
  return predGeom;
}
PredGeomEncOpts&
EncoderParamsImpl::getPredGeom()
{
  return predGeom;
}

// Parameters that control partitioning
const PartitionParams&
EncoderParamsImpl::getPartition() const
{
  return partition;
}
PartitionParams&
EncoderParamsImpl::getPartition()
{
  return partition;
}

// attribute recolouring parameters
const RecolourParams&
EncoderParamsImpl::getRecolour() const
{
  return recolour;
}
RecolourParams&
EncoderParamsImpl::getRecolour()
{
  return recolour;
}

// number of expected lasers
int
EncoderParamsImpl::getNumLasers() const
{
  return numLasers;
}
void
EncoderParamsImpl::setNumLasers(int num)
{
  numLasers = num;
}

// floating Lasers' theta (have to be converted to fixed point in gps)
const DoubleVectorAdapterImpl&
EncoderParamsImpl::getLasersTheta() const
{
  return lasersTheta;
}
DoubleVectorAdapterImpl&
EncoderParamsImpl::getLasersTheta()
{
  return lasersTheta;
}

// floating Lasers' H (have to be converted to fixed point in gps)
const DoubleVectorAdapterImpl&
EncoderParamsImpl::getLasersZ() const
{
  return lasersZ;
}
DoubleVectorAdapterImpl&
EncoderParamsImpl::getLasersZ()
{
  return lasersZ;
}

// per-slice trisoup node sizes
const Int32VectorAdapterImpl&
EncoderParamsImpl::getTrisoupNodeSizesLog2() const
{
  return trisoupNodeSizesLog2;
}
Int32VectorAdapterImpl&
EncoderParamsImpl::getTrisoupNodeSizesLog2()
{
  return trisoupNodeSizesLog2;
}

// Enable enforcement of level limits (encoder will abort if exceeded)
bool
EncoderParamsImpl::isEnforceLevelLimits() const
{
  return enforceLevelLimits;
}
void
EncoderParamsImpl::setEnforceLevelLimits(bool enable)
{
  enforceLevelLimits = enable;
}

// Qp used for IDCM quantisation (used to derive HLS values)
int
EncoderParamsImpl::getIdcmQp() const
{
  return idcmQp;
}
void
EncoderParamsImpl::setIdcmQp(int qp)
{
  idcmQp = qp;
}

//---------------------------------------------------------------------------

EncoderParams*
EncoderParams::create()
{
#if GPCCLIB_TRIAL_BUILD
  if (IsGpcclibExpired()) {
    return nullptr;
  }
#endif

  try {
    return new EncoderParamsImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

//============================================================================

} // namespace encoder
} // namespace gpcclib
