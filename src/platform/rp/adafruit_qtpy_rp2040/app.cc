#include "platform/rp/AppLauncher.h"

constexpr platform::rp::NeoPixelGpio<true, true> neoPixelGpio = {
  .power = 11,
  .tx = 12,
};

constexpr platform::rp::SystemGpio systemGpio = {
  .power = 27,
  .reset = 26
};

constexpr platform::rp::KeyboardGpio keyboardGpio = {
  .start = 20,
  .select = 25,
  .option = 24,
  .k0 = 5,
  .k5 = 6,
  .kr1 = 4,
  .kr2 = 3
};

int main() {
  platform::rp::AppLauncher<platform::rp::AppTraits<neoPixelGpio, systemGpio, keyboardGpio>> appLauncher;
}
