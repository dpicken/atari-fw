#ifndef media_Disk_h
#define media_Disk_h

#include <cstddef>
#include <cstdint>

#include <functional>
#include <memory>

namespace media {

class Disk {
public:
  using sector_address_type = std::uint16_t;
  using sector_count_type = std::uint16_t;
  using sector_sink_type = std::function<void(const std::uint8_t* sector, std::size_t sectorSize)>;

  using ptr_type = std::shared_ptr<Disk>;

  virtual ~Disk();

  sector_address_type getBeginSectorAddress() const {
    return 1U;
  }

  sector_address_type getEndSectorAddress() const {
    return getSectorCount() + 1U;
  }

  enum class Density { Single, Enhanced, Double };
  virtual Density getDensity() const = 0;

  virtual sector_count_type getSectorCount() const = 0;

  virtual bool hasSector(sector_address_type sectorAddress) const = 0;

  virtual bool readSector(sector_address_type sectorAddress, const sector_sink_type& sink) = 0;
};

} // namespace media

#endif // ifndef media_Disk_h
