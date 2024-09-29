#include "Card.h"

#include "Controller.h"

sd::Card::Card(Controller* controller, size_type size)
  : m_controller(controller)
  , m_size(size) {
}

sd::Card::ptr_type sd::Card::make(Controller* controller, size_type size) {
  return std::make_shared<Card>(controller, size);
}

sd::Card::size_type sd::Card::size() {
  return m_size;
}

bool sd::Card::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  return m_controller->read(this, offset, byteCount, sink);
}

std::string sd::Card::name() const {
  return std::string("sd-") + std::to_string(m_size);
}
