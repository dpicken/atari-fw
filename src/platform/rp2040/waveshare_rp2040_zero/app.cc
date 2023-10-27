#include "platform/rp2040/AppLauncher.h"

constexpr platform::rp2040::NeoPixelGpio<false, false> neoPixelGpio = {
  .tx = 16,
};

constexpr platform::rp2040::SystemGpio systemGpio = {
  .power = 2,
  .reset = 3,
};

constexpr platform::rp2040::KeyboardGpio keyboardGpio = {
  .start = 6,
  .select = 5,
  .option = 4,

  .k0 = 26,
  .k5 = 27,
  .kr1 = 28,
  .kr2 = 29
};

constexpr platform::rp2040::SioGpio<true> sioGpio = {
  .command = 10,
  .uartInstance = 1,
  .uartRx = 9,
  .uartTx = 8,
  .motor = 7
};

constexpr platform::rp2040::SDGpio<true> sdGpio = {
  .detect = 11,
  .power = 1,
  .spiInstance = 1,
  .spiCs = 13,
  .spiRx = 12,
  .spiTx = 15,
  .spiClock = 14
};

int main() {
  platform::rp2040::AppLauncher<platform::rp2040::AppTraits<neoPixelGpio, systemGpio, keyboardGpio, sioGpio, sdGpio>> appLauncher;
}
