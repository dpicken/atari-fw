#include "platform/rp/AppLauncher.h"

constexpr platform::rp::RGBLedGpio<false> rgbLedGpio = {
  .red = 18,
  .green = 19,
  .blue = 20
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
  .command = 7,
  .uartInstance = 0,
  .uartRx = 13,
  .uartTx = 12
};

int main() {
  platform::rp::AppLauncher<platform::rp::AppTraits<rgbLedGpio, systemGpio, keyboardGpio, sioGpio>> appLauncher;
}
