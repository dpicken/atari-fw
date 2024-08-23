#ifndef platform_rp_AppLauncher_h
#define platform_rp_AppLauncher_h

#include "NeoPixelSerializeColor.h"

namespace platform { namespace rp {

enum class IndicatorType { NeoPixel, RGBLed };

template<bool switchedPower, bool isGRB>
struct NeoPixelGpio;

template<bool isGRB>
struct NeoPixelGpio<false, isGRB> {
  static constexpr auto type = IndicatorType::NeoPixel;
  static constexpr bool switchedPower = false;
  static constexpr NeoPixelSerializeColor serializeColor = isGRB ? neoPixelSerializeColorToGRB : neoPixelSerializeColorToRGB;
  const unsigned int tx;
};

template<bool isGRB>
struct NeoPixelGpio<true, isGRB> {
  static constexpr auto type = IndicatorType::NeoPixel;
  static constexpr bool switchedPower = true;
  static constexpr NeoPixelSerializeColor serializeColor = isGRB ? neoPixelSerializeColorToGRB : neoPixelSerializeColorToRGB;
  const unsigned int power;
  const unsigned int tx;
};

template<bool activeValue>
struct RGBLedGpio {
  static constexpr auto type = IndicatorType::RGBLed;
  static constexpr auto active_value = activeValue;
  const unsigned int red;
  const unsigned int green;
  const unsigned int blue;
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

template<bool supported>
struct SioGpio;

template<>
struct SioGpio<false> {
  static constexpr bool supported = false;
};

template<>
struct SioGpio<true> {
  static constexpr bool supported = true;
  const unsigned int command;
  const unsigned int uartInstance;
  const unsigned int uartRx;
  const unsigned int uartTx;
  //const unsigned int motor;
};

template<bool supported>
struct SDGpio;

template<>
struct SDGpio<false> {
  static constexpr bool supported = false;
};

template<>
struct SDGpio<true> {
  static constexpr bool supported = true;
  const unsigned int detect;
  const unsigned int power;
  const unsigned int spiInstance;
  const unsigned int spiCs;
  const unsigned int spiRx;
  const unsigned int spiTx;
  const unsigned int spiClock;
};

template<
    auto indicatorGpioValue,
    SystemGpio systemGpioValue,
    KeyboardGpio keyboardGpioValue,
    auto sioGpioValue = SioGpio<false>(),
    auto sdGpioValue = SDGpio<false>()>
struct AppTraits {
  static constexpr auto indicatorGpio = indicatorGpioValue;
  static constexpr SystemGpio systemGpio = systemGpioValue;
  static constexpr KeyboardGpio keyboardGpio = keyboardGpioValue;
  static constexpr auto sioGpio = sioGpioValue;
  static constexpr auto sdGpio = sdGpioValue;
};

template<typename AppTraits>
class AppLauncher {
public:
  [[noreturn]] AppLauncher();

private:
  [[noreturn]] static void runKeyboardApp();
  [[noreturn]] static void runSioApp();
};

} } // namespace platform::rp

#include "AppLauncher.inl.h"

#endif // ifndef platform_rp_AppLauncher_h
