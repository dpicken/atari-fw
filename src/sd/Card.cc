#include "Card.h"

#include "Controller.h"

#include <sstream>

namespace {

std::string makeCardName(sd::Card::size_type size, const std::string_view& nameStem) {
  std::ostringstream os;
  os << "sd-" << size;
  if (!nameStem.empty()) {
    os << "-" << nameStem;
  }
  return os.str();
}

}

sd::Card::Card(Controller* controller, size_type size, const std::string_view& nameStem)
  : m_controller(controller)
  , m_size(size)
  , m_name(makeCardName(size, nameStem)) {
}

sd::Card::ptr_type sd::Card::make(Controller* controller, size_type size) {
  return std::make_shared<Card>(controller, size);
}

sd::Card::ptr_type sd::Card::makeInvalid(Controller* controller, const std::string_view& reason) {
  return std::make_shared<Card>(controller, 0, reason);
}

sd::Card::size_type sd::Card::size() {
  return m_size;
}

bool sd::Card::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  return m_controller->read(this, offset, byteCount, sink);
}

std::string sd::Card::name() const {
  return m_name;
}
