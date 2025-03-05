#include "platform/rp/AppLauncher.h"

// Cart/ROM pins (5V tolerant):
//   [0,  28]: Address (16), Data (8), CS (1), spare (4)

constexpr platform::rp::NeoPixelGpio<false, false> neoPixelGpio = {
  .tx = 45,
};

constexpr platform::rp::SystemGpio systemGpio = {
  .power = 47,
  .reset = 46,
};

constexpr platform::rp::KeyboardGpio keyboardGpio = {
  .start = 29,
  .select = 30,
  .option = 31,

  .k0 = 32,
  .k5 = 33,
  .kr1 = 34,
  .kr2 = 35
};

constexpr platform::rp::SioGpio<true> sioGpio = {
  .command = 36,
  .uartInstance = 1,
  .uartRx = 37,
  .uartTx = 38
};

constexpr platform::rp::SDGpio<true> sdGpio = {
  .detect = 39,
  .power = 40,
  .spiInstance = 1,
  .spiCs = 41,
  .spiRx = 44,
  .spiTx = 43,
  .spiClock = 42
};

int main() {
  platform::rp::AppLauncher<platform::rp::AppTraits<neoPixelGpio, systemGpio, keyboardGpio, sioGpio, sdGpio>> appLauncher;
}
