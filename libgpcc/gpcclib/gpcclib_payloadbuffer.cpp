#include "gpcclib/payloadbuffer.hpp"
#include "gpcclib/internal/payloadbuffer.hpp"
#include "gpcclib/internal/common.hpp"
#include "PayloadBuffer.h"

namespace gpcclib {

void
PayloadBufferImpl::release()
{
  delete this;
}

PayloadType
PayloadBufferImpl::getType() const
{
  return buffer_.type;
}

void
PayloadBufferImpl::setType(PayloadType type)
{
  buffer_.type = type;
}

size_t
PayloadBufferImpl::getSize() const
{
  return buffer_.size();
}

const char*
PayloadBufferImpl::getBuffer() const
{
  return &buffer_[0];
}

char*
PayloadBufferImpl::getBuffer()
{
  return &buffer_[0];
}

void
PayloadBufferImpl::resize(size_t size)
{
  buffer_.resize(size);
}

void
PayloadBufferImpl::reserve(size_t size)
{
  buffer_.reserve(size);
}

void
PayloadBufferImpl::clear()
{
  buffer_.clear();
}

const pcc::PayloadBuffer&
PayloadBufferImpl::getPayloadBuffer() const
{
  return buffer_;
}

pcc::PayloadBuffer&
PayloadBufferImpl::getPayloadBuffer()
{
  return buffer_;
}

// static function
PayloadBuffer*
PayloadBuffer::create()
{
  return new PayloadBufferImpl();
}

//============================================================================

ConstPayloadBufferWrapper::ConstPayloadBufferWrapper(
  const pcc::PayloadBuffer& buf)
  : buffer_(buf)
{}

void
ConstPayloadBufferWrapper::release()
{
  delete this;
}

PayloadType
ConstPayloadBufferWrapper::getType() const
{
  return buffer_.type;
}

void
ConstPayloadBufferWrapper::setType(PayloadType type)
{
  throw UnsupportedException();
  // buffer_.type = type;
}

size_t
ConstPayloadBufferWrapper::getSize() const
{
  return buffer_.size();
}

const char*
ConstPayloadBufferWrapper::getBuffer() const
{
  return &buffer_[0];
}

char*
ConstPayloadBufferWrapper::getBuffer()
{
  throw UnsupportedException();
  // return &buffer_[0];
}

void
ConstPayloadBufferWrapper::resize(size_t size)
{
  throw UnsupportedException();
  // buffer_.resize(size);
}

void
ConstPayloadBufferWrapper::reserve(size_t size)
{
  throw UnsupportedException();
  // buffer_.reserve(size);
}

void
ConstPayloadBufferWrapper::clear()
{
  throw UnsupportedException();
  // buffer_.clear();
}

const pcc::PayloadBuffer&
ConstPayloadBufferWrapper::getPayloadBuffer() const
{
  return buffer_;
}

pcc::PayloadBuffer&
ConstPayloadBufferWrapper::getPayloadBuffer()
{
  throw UnsupportedException();
  // return buffer_;
}

}  // namespace gpcclib
