#include "platform/rp2040/AppLauncher.h"

constexpr platform::rp2040::NeoPixelGpio<true, true> neoPixelGpio = {
  .power = 11,
  .tx = 12,
};

constexpr platform::rp2040::SystemGpio systemGpio = {
  .power = 27,
  .reset = 26
};

constexpr platform::rp2040::KeyboardGpio keyboardGpio = {
  .start = 20,
  .select = 25,
  .option = 24,
  .k0 = 5,
  .k5 = 6,
  .kr1 = 4,
  .kr2 = 3
};

int main() {
  platform::rp2040::AppLauncher<platform::rp2040::AppTraits<neoPixelGpio, systemGpio, keyboardGpio>> appLauncher;
}
