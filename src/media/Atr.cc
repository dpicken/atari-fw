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
} PACKED;

template<std::size_t sectorSize>
using Sector = std::array<std::uint8_t, sectorSize>;

template<std::size_t sectorSize>
constexpr Sector<sectorSize> makeZeroSector() {
  std::array<std::uint8_t, sectorSize> zeroSector{};
  return zeroSector;
}

template<typename Traits>
class Atr : public media::Disk {
public:
  using traits_type = Traits;

  static constexpr auto max_sector_stream_size =
      (traits_type::boot_sector_size * traits_type::boot_sector_count) + (traits_type::nonboot_sector_size * traits_type::nonboot_sector_count);

  Atr(fs::File::ptr_type atrFile)
    : m_atrFile(std::move(atrFile)) {
  }

  Density getDensity() const override {
    return traits_type::density;
  }

  sector_count_type getSectorCount() const override {
    return traits_type::sector_count;
  }

  bool hasSector(sector_address_type sectorAddress) const override {
    if (sectorAddress == 0) {
      return false;
    }

    sectorAddress = toPhysicalSectorAddress(sectorAddress);
    return sectorAddress < traits_type::sector_count;
  }

  bool readSector(sector_address_type sectorAddress, const sector_sink_type& sink) override {
    if (sectorAddress == 0) {
      return false;
    }

    sectorAddress = toPhysicalSectorAddress(sectorAddress);
    if (sectorAddress < traits_type::boot_sector_count) {
      auto sectorByteOffset = sectorAddress * traits_type::boot_sector_size;
      return readSector<traits_type::boot_sector_size>(sectorByteOffset, sink);
    } else if (sectorAddress < traits_type::sector_count) {
      sector_address_type nonbootSectorAddress = sectorAddress - traits_type::boot_sector_count;
      auto sectorByteOffset = (traits_type::boot_sector_count * traits_type::boot_sector_size) + (nonbootSectorAddress * traits_type::nonboot_sector_size);
      return readSector<traits_type::nonboot_sector_size>(sectorByteOffset, sink);
    } else {
      return false;
    }
  }

private:
  static sector_address_type toPhysicalSectorAddress(sector_address_type sectorAddress) {
    return sectorAddress - 1;
  }

  template<std::size_t sectorSize>
  bool readSector(std::uint64_t sectorByteOffset, const sector_sink_type& sink) {
    auto fileOffset = sizeof(AtrHeaderBlock) + sectorByteOffset;
    if (fileOffset + sectorSize <= m_atrFile->size()) {
      Sector<sectorSize> sectorBuffer;
      if (!bufferSector(sectorBuffer, fileOffset)) {
          return false;
      }
      sink(sectorBuffer.data(), sectorBuffer.size());
    } else {
      Sector<sectorSize> sparseSector = makeZeroSector<sectorSize>();
      sink(sparseSector.data(), sparseSector.size());
    }
    return true;
  }

  template<std::size_t sectorSize>
  bool bufferSector(Sector<sectorSize>& sectorBuffer, std::uint64_t fileOffset) {
    std::size_t sectorBufferOffset = 0;
    return m_atrFile->read(fileOffset, sectorSize, [&sectorBuffer, &sectorBufferOffset](const std::uint8_t* data, std::size_t size) {
      std::copy(data, data + size, sectorBuffer.data() + sectorBufferOffset);
      sectorBufferOffset += size;
    });
  }

  const fs::File::ptr_type m_atrFile;
};

template<
  media::Disk::Density densityValue,
  media::Disk::sector_count_type sectorCount,
  media::Disk::sector_count_type bootSectorCount,
  std::size_t bootSectorSize,
  std::size_t nonbootSectorSize>
struct AtrTraits {
  static constexpr auto density = densityValue;

  static constexpr auto sector_count = sectorCount;
  static constexpr auto boot_sector_count = bootSectorCount;
  static constexpr auto nonboot_sector_count = sector_count - boot_sector_count;

  static constexpr auto boot_sector_size = bootSectorSize;
  static constexpr auto nonboot_sector_size = nonbootSectorSize;
};

using SingleDensityAtr = Atr<AtrTraits<media::Disk::Density::Single, 720, 3, 128, 128>>;
using EnhancedDensityAtr = Atr<AtrTraits<media::Disk::Density::Enhanced, 1040, 3, 128, 128>>;
using DoubleDensityAtr = Atr<AtrTraits<media::Disk::Density::Double, 720, 3, 128, 256>>;
using DoubleDensityNonStandardBootSectorSizeAtr = Atr<AtrTraits<media::Disk::Density::Double, 720, 3, 256, 256>>;

using NonStandardSingleDensityAtr = Atr<AtrTraits<media::Disk::Density::Single, 8192, 3, 128, 128>>;
using NonStandardDoubleDensityAtr = Atr<AtrTraits<media::Disk::Density::Double, 8192, 3, 128, 256>>;

} // namespace

std::unique_ptr<media::Disk> media::makeAtr(::fs::File::ptr_type atrFile) {
  if (atrFile == nullptr) {
    return nullptr;
  }

  AtrHeaderBlock header;
  if (!atrFile->read(0, &header)) {
    return nullptr;
  }

  if (header.magic != 662) {
    return nullptr;
  }

  if (header.getSectorSize() == 128) {
    if (header.getSectorStreamSize() == SingleDensityAtr::max_sector_stream_size) {
      return std::make_unique<SingleDensityAtr>(std::move(atrFile));
    } else if (header.getSectorStreamSize() == EnhancedDensityAtr::max_sector_stream_size) {
      return std::make_unique<EnhancedDensityAtr>(std::move(atrFile));
    } else if (header.getSectorStreamSize() <= NonStandardSingleDensityAtr::max_sector_stream_size) {
      return std::make_unique<NonStandardSingleDensityAtr>(std::move(atrFile));
    }
  } else if (header.getSectorSize() == 256) {
    if (header.getSectorStreamSize() == DoubleDensityAtr::max_sector_stream_size) {
      return std::make_unique<DoubleDensityAtr>(std::move(atrFile));
    } else if (header.getSectorStreamSize() == DoubleDensityNonStandardBootSectorSizeAtr::max_sector_stream_size) {
      return std::make_unique<DoubleDensityNonStandardBootSectorSizeAtr>(std::move(atrFile));
    } else if (header.getSectorStreamSize() <= NonStandardDoubleDensityAtr::max_sector_stream_size) {
      return std::make_unique<NonStandardDoubleDensityAtr>(std::move(atrFile));
    }
  }

  return nullptr;
}
