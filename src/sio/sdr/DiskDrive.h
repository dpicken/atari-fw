#ifndef sio_sdr_DiskDrive_h
#define sio_sdr_DiskDrive_h

#include "Types.h"

namespace sio { namespace sdr {

struct Status {
  std::uint8_t driveStatus = 0x00;
  std::uint8_t fdcStatus = 0xFF;
  std::uint8_t formatTimeout = 0xE0;
  std::uint8_t reserved = 0x00;
} PACKED;

} } // namespace sio::sdr

#endif // ifndef sio_sdr_DiskDrive_h
