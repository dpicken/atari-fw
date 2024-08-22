#ifndef platform_rp_NeoPixelSerializeColor_h
#define platform_rp_NeoPixelSerializeColor_h

#include <cstdint>

namespace platform { namespace rp {

typedef std::uint32_t(*NeoPixelSerializeColor)(std::uint8_t r, std::uint8_t g, std::uint8_t b);

inline std::uint32_t neoPixelSerializeColorToRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
  std::uint32_t pixelValue = 0;
  pixelValue |= 0x0;
  pixelValue <<= 8;
  pixelValue |= r;
  pixelValue <<= 8;
  pixelValue |= g;
  pixelValue <<= 8;
  pixelValue |= b;
  pixelValue <<= 8;
  return pixelValue;
}

inline std::uint32_t neoPixelSerializeColorToGRB(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
  std::uint32_t pixelValue = 0;
  pixelValue |= 0x0;
  pixelValue <<= 8;
  pixelValue |= g;
  pixelValue <<= 8;
  pixelValue |= r;
  pixelValue <<= 8;
  pixelValue |= b;
  pixelValue <<= 8;
  return pixelValue;
}

} } // namespace platform::rp

#endif // ifndef platform_rp_NeoPixelSerializeColor_h
