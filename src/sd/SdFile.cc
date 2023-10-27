#include "SdFile.h"

sd::SdFile::SdFile(Controller* controller, Controller::open_id_type openId, size_type offset, size_type size)
  : m_controller(controller)
  , m_openId(openId)
  , m_offset(offset)
  , m_size(size) {
}

sd::SdFile::size_type sd::SdFile::size() const {
  return m_size;
}

bool sd::SdFile::read(size_type offset, std::size_t byteCount, const sink_type& sink) const {
  return m_controller->read(m_openId, m_offset + offset, byteCount, sink);
}
