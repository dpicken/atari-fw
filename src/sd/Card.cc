#include "Card.h"

#include "BlockSize.h"
#include "Controller.h"

#include <sstream>

sd::Card::Card(Controller* controller, size_type size, std::string name)
  : m_controller(controller)
  , m_size(size)
  , m_name(std::move(name)) {
}

sd::Card::ptr_type sd::Card::make(Controller* controller, size_type size) {
  return std::make_shared<Card>(controller, size, "sd");
}

sd::Card::ptr_type sd::Card::makeInvalid(Controller* controller, const std::string_view& reason) {
  return std::make_shared<Card>(controller, 0, std::string("sd:") + std::string(reason));
}

fs::File::block_size_type sd::Card::blockSize() const {
  return BlockSize;
}

fs::File::size_type sd::Card::size() {
  return m_size;
}

bool sd::Card::readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) {
  return m_controller->read(this, offset, byteCount, sink);
}

std::string sd::Card::name() const {
  return m_name;
}
