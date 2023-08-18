#ifndef platform_rp2040_AppLauncher_h
#define platform_rp2040_AppLauncher_h

#include "NeoPixelSerializeColor.h"

namespace platform { namespace rp2040 {

template<bool switchedPower, bool isGRB>
struct NeoPixelGpio;

template<bool isGRB>
struct NeoPixelGpio<false, isGRB> {
  static constexpr bool switchedPower = false;
  static constexpr NeoPixelSerializeColor serializeColor = isGRB ? neoPixelSerializeColorToGRB : neoPixelSerializeColorToRGB;
  const unsigned int tx;
};

template<bool isGRB>
struct NeoPixelGpio<true, isGRB> {
  static constexpr bool switchedPower = true;
  static constexpr NeoPixelSerializeColor serializeColor = isGRB ? neoPixelSerializeColorToGRB : neoPixelSerializeColorToRGB;
  const unsigned int power;
  const unsigned int tx;
};

struct SystemGpio {
  const unsigned int power;
  const unsigned int reset;
};

struct KeyboardGpio {
  const unsigned int start;
  const unsigned int select;
  const unsigned int option;
  const unsigned int k0;
  const unsigned int k5;
  const unsigned int kr1;
  const unsigned int kr2;
};

template<
    auto neoPixelGpioValue,
    SystemGpio systemGpioValue,
    KeyboardGpio keyboardGpioValue>
struct AppTraits {
  static constexpr auto neoPixelGpio = neoPixelGpioValue;
  static constexpr SystemGpio systemGpio = systemGpioValue;
  static constexpr KeyboardGpio keyboardGpio = keyboardGpioValue;
};

template<typename AppTraits>
class AppLauncher {
public:
  AppLauncher();
};

} } // namespace platform::rp2040

#include "AppLauncher.inl.h"

#endif // ifndef platform_rp2040_AppLauncher_h
