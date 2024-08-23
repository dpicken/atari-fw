#include "platform/rp/AppLauncher.h"

constexpr platform::rp::NeoPixelGpio<false, false> neoPixelGpio = {
  .tx = 16,
};

constexpr platform::rp::SystemGpio systemGpio = {
  .power = 2,
  .reset = 3,
};

constexpr platform::rp::KeyboardGpio keyboardGpio = {
  .start = 6,
  .select = 5,
  .option = 4,

  .k0 = 26,
  .k5 = 27,
  .kr1 = 28,
  .kr2 = 29
};

constexpr platform::rp::SioGpio<true> sioGpio = {
  .command = 10,
  .uartInstance = 1,
  .uartRx = 9,
  .uartTx = 8,
  //.motor = 7
};

constexpr platform::rp::SDGpio<true> sdGpio = {
  .detect = 11,
  .power = 1,
  .spiInstance = 1,
  .spiCs = 13,
  .spiRx = 12,
  .spiTx = 15,
  .spiClock = 14
};

int main() {
  platform::rp::AppLauncher<platform::rp::AppTraits<neoPixelGpio, systemGpio, keyboardGpio, sioGpio, sdGpio>> appLauncher;
}
