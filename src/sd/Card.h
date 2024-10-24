#ifndef sd_Card_h
#define sd_Card_h

#include "Controller.fwd.h"

#include "fs/File.h"

#include <string>
#include <string_view>

namespace sd {

class Card : public ::fs::File {
public:
  using ptr_type = ptr<Card>;

  Card(Controller* controller, size_type, std::string name);

  static ptr_type make(Controller* controller, size_type size);
  static ptr_type makeInvalid(Controller* controller, const std::string_view& reason);

  block_size_type blockSize() const override;

  size_type size() override;

  std::string name() const;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  Controller* const m_controller;
  const size_type m_size;
  const std::string m_name;
};

} // namespace sd

#endif // ifndef sd_Card_h
