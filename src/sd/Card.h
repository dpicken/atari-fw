#ifndef sd_Card_h
#define sd_Card_h

#include "Controller.fwd.h"

#include "fs/File.h"

namespace sd {

class Card : public ::fs::File {
public:
  using ptr_type = ptr<Card>;

  Card(Controller* controller, size_type size);

  static ptr_type make(Controller* controller, size_type size);

  size_type size() override;

  std::string name() const;

protected:
  bool readImpl(size_type offset, buffer_size_type byteCount, const sink_type& sink) override;

private:
  Controller* const m_controller;
  const size_type m_size;
};

} // namespace sd

#endif // ifndef sd_Card_h
