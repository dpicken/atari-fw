#include "Atr.h"

#include "util/PACKED.h"

#include <array>

namespace {

struct AtrHeaderBlock {
  std::uint16_t magic;
  std::uint16_t blockCount;
  std::uint16_t sectorSize;
  std::uint8_t reserved[10];

  std::size_t getSectorStreamSize() const {
    return blockCount * sizeof(AtrHeaderBlock);
  }

  std::size_t getSectorSize() const {
    return sectorSize;
  }

  const std::uint8_t* getSectorStreamBegin() const {
    return reinterpret_cast<const uint8_t*>(this + 1);
  }

  const std::uint8_t* getSectorStreamEnd() const {
    return getSectorStreamBegin() + getSectorStreamSize();
  }
} PACKED;

template<std::size_t sectorSize>
using Sector = std::array<std::uint8_t, sectorSize>;

template<std::size_t sectorSize>
constexpr Sector<sectorSize> makeZeroSector() {
  std::array<std::uint8_t, sectorSize> a;
  for (auto& e : a) {
    e = 0;
  }
  return a;
}

template<typename Traits>
struct AtrLayout {
  using sector_address_type = media::Disk::sector_address_type;

  static constexpr media::Disk::Density density = Traits::density;

  static constexpr auto sector_count = Traits::sector_count;
  static constexpr auto boot_sector_count = Traits::boot_sector_count;
  static constexpr auto nonboot_sector_count = Traits::nonboot_sector_count;

  static constexpr auto boot_sector_size = Traits::boot_sector_size;
  static constexpr auto nonboot_sector_size = Traits::nonboot_sector_size;

  static constexpr auto max_sector_stream_size = (boot_sector_size * boot_sector_count) + (nonboot_sector_size * nonboot_sector_count);

  static constexpr std::array<std::uint8_t, boot_sector_size> SparseBootSector = makeZeroSector<boot_sector_size>();
  static constexpr std::array<std::uint8_t, nonboot_sector_size> SparseNonBootSector = makeZeroSector<nonboot_sector_size>();

  const std::uint8_t* getSector(sector_address_type sectorAddress) const {
    if (sectorAddress == 0) {
      return nullptr;
    }

    sectorAddress = toPhysicalSectorAddress(sectorAddress);
    if (sectorAddress < boot_sector_count) {
      auto sector = m_bootSectors[sectorAddress].data();
      return (sector + boot_sector_size <= m_header.getSectorStreamEnd()) ? sector : SparseBootSector.data();
    } else if (sectorAddress < sector_count) {
      auto sector = m_nonbootSectors[sectorAddress - boot_sector_count].data();
      return (sector + nonboot_sector_size <= m_header.getSectorStreamEnd()) ? sector : SparseNonBootSector.data();
    } else {
      return nullptr;
    }
  }

  std::size_t getSectorSize(sector_address_type sectorAddress) const {
    if (sectorAddress == 0) {
      return 0;
    }

    sectorAddress = toPhysicalSectorAddress(sectorAddress);
    if (sectorAddress < boot_sector_count) {
      return boot_sector_size;
    } else if (sectorAddress < sector_count) {
      return nonboot_sector_size;
    } else {
      return 0;
    }
  }

private:
  static sector_address_type toPhysicalSectorAddress(sector_address_type sectorAddress) {
    return sectorAddress - 1;
  }

  AtrHeaderBlock m_header;
  Sector<boot_sector_size> m_bootSectors[boot_sector_count];
  Sector<nonboot_sector_size> m_nonbootSectors[nonboot_sector_count];
} PACKED;

const AtrHeaderBlock* toHeader(const std::uint8_t* data, std::size_t size) {
  if (size < sizeof(AtrHeaderBlock)) {
    return nullptr;
  }

  const AtrHeaderBlock* header = reinterpret_cast<const AtrHeaderBlock*>(data);
  if (header->magic != 662) {
    return nullptr;
  }

  if (sizeof(*header) + header->getSectorStreamSize() != size) {
    return nullptr;
  }

  return header;
}

template<typename Traits>
class Atr : public media::Disk {
public:
  using atr_layout = AtrLayout<Traits>;
  static constexpr auto max_sector_stream_size = atr_layout::max_sector_stream_size;

  Atr(const atr_layout* atrLayout)
    : m_atrLayout(atrLayout) {
  }

  Density getDensity() const override {
    return atr_layout::density;
  }

  sector_count_type getSectorCount() const override {
    return atr_layout::sector_count;
  }

  bool hasSector(sector_address_type sectorAddress) const override {
    return m_atrLayout->getSector(sectorAddress) != nullptr;
  }

  void readSector(sector_address_type sectorAddress, const Sink sink) override {
    sink(m_atrLayout->getSector(sectorAddress), m_atrLayout->getSectorSize(sectorAddress));
  }

protected:
  const atr_layout* const m_atrLayout;
};

template<
  media::Disk::Density densityV,
  media::Disk::sector_count_type sectorCount,
  media::Disk::sector_count_type bootSectorCount,
  std::size_t bootSectorSize,
  std::size_t nonbootSectorSize>
struct AtrTraits {
  static constexpr auto density = densityV;

  static constexpr auto sector_count = sectorCount;
  static constexpr auto boot_sector_count = bootSectorCount;
  static constexpr auto nonboot_sector_count = sector_count - boot_sector_count;

  static constexpr auto boot_sector_size = bootSectorSize;
  static constexpr auto nonboot_sector_size = nonbootSectorSize;
};

using SingleDensityAtr = Atr<AtrTraits<media::Disk::Density::Single, 720, 3, 128, 128>>;
using EnhancedDensityAtr = Atr<AtrTraits<media::Disk::Density::Enhanced, 1040, 3, 128, 128>>;

} // namespace

std::unique_ptr<media::Disk> media::makeAtr(const std::uint8_t* data, std::size_t size) {
  auto header = toHeader(data, size);
  if (header == nullptr) {
    return nullptr;
  }

  if (header->getSectorStreamEnd() != (data + size)) {
    return nullptr;
  }

  if (header->getSectorSize() == 128) {
    if (header->getSectorStreamSize() <= SingleDensityAtr::max_sector_stream_size) {
      return std::make_unique<SingleDensityAtr>(reinterpret_cast<const SingleDensityAtr::atr_layout*>(header));
    } else if (header->getSectorStreamSize() <= EnhancedDensityAtr::max_sector_stream_size) {
      return std::make_unique<EnhancedDensityAtr>(reinterpret_cast<const EnhancedDensityAtr::atr_layout*>(header));
    }
  }
  return nullptr;
}
