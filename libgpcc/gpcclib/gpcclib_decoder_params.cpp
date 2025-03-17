#include "PCCTMC3Decoder.h"
#include "gpcclib/internal/decoder/params.hpp"
#include "gpcclib/internal/expiration.hpp"
#include <new>

namespace gpcclib {
namespace decoder {

//============================================================================

void
CopyParams(pcc::DecoderParams& dst, const DecoderParams& src)
{
  dst.minGeomNodeSizeLog2 = src.getMinGeomNodeSizeLog2();
  dst.decodeMaxPoints = src.getDecodeMaxPoints();
  dst.outputFpBits = src.getOutputFpBits();
}

void
DecoderParamsImpl::release()
{
  delete this;
}

unsigned int
DecoderParamsImpl::getNumOfFrameThreads() const
{
  return numOfFrameThreads;
}
void
DecoderParamsImpl::setNumOfFrameThreads(unsigned int numOfThreads)
{
  numOfFrameThreads = numOfThreads;
}

bool
DecoderParamsImpl::doSliceParallelProc() const
{
  return doSliceParallel;
}
void
DecoderParamsImpl::setDoSliceParallelProc(bool enabled)
{
  doSliceParallel = enabled;
}

bool
DecoderParamsImpl::shouldConvertColourspace() const
{
  return convertColourspace;
}
void
DecoderParamsImpl::setConvertColourspace(bool enabled)
{
  convertColourspace = enabled;
}

// For partial decoding (aka, scalable bitstreams), the number of octree
// layers to skip during the decode process (attribute coding must take
// this into account)
int
DecoderParamsImpl::getMinGeomNodeSizeLog2() const
{
  return minGeomNodeSizeLog2;
}
void
DecoderParamsImpl::setMinGeomNodeSizeLog2(int val)
{
  minGeomNodeSizeLog2 = val;
}

// A maximum number of points to partially decode.
int
DecoderParamsImpl::getDecodeMaxPoints() const
{
  return decodeMaxPoints;
}
void
DecoderParamsImpl::setDecodeMaxPoints(int val)
{
  decodeMaxPoints = val;
}

// Number of fractional bits used in output position representation.
int
DecoderParamsImpl::getOutputFpBits() const
{
  return outputFpBits;
}
void
DecoderParamsImpl::setOutputFpBits(int val)
{
  outputFpBits = val;
}

//============================================================================

DecoderParams*
DecoderParams::create()
{
#if GPCCLIB_TRIAL_BUILD
  if (IsGpcclibExpired()) {
    return nullptr;
  }
#endif

  try {
    return new DecoderParamsImpl();
  }
  catch (const std::bad_alloc&) {
    return nullptr;
  }
}

} // namespace decoder
} // namespace gpcclib
