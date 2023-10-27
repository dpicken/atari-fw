#ifndef sd_SdFile_h
#define sd_SdFile_h

#include "Controller.h"

#include "io/File.h"

namespace sd {

class SdFile : public io::File {
public:
  SdFile(Controller* controller, Controller::open_id_type openId, size_type offset, size_type size);

  size_type size() const override;

  bool read(size_type offset, std::size_t byteCount, const sink_type& sink) const override;

private:
  Controller* const m_controller;
  const Controller::open_id_type m_openId;
  const std::uint64_t m_offset;
  const size_type m_size;
};

} // namespace sd

#endif // ifndef sd_SdFile_h
