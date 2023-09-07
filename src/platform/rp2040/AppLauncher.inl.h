#include "App.h"

#include "hal/InputSignal.h"
#include "hal/OutputSignal.h"

#include <pico/stdlib.h>

#define DEFINE_INPUT_SIGNAL(gpioSet, name, activeValue)\
  gpio_init(gpioSet.name);\
  gpio_set_dir(gpioSet.name, GPIO_IN);\
  hal::InputSignal name(\
      [](){ return gpio_get(gpioSet.name) == activeValue; })

#define DEFINE_OUTPUT_SIGNAL(gpioSet, name, activeValue)\
  gpio_init(gpioSet.name);\
  gpio_set_dir(gpioSet.name, GPIO_OUT);\
  gpio_put(gpioSet.name, !activeValue);\
  hal::OutputSignal name(\
      [](){ gpio_put(gpioSet.name, activeValue); },\
      [](){ gpio_put(gpioSet.name, !activeValue); })

#define DEFINE_POWER_OUTPUT_SIGNAL(gpioSet, name, activeValue)\
  gpio_init(gpioSet.name);\
  gpio_set_dir(gpioSet.name, GPIO_OUT);\
  gpio_put(gpioSet.name, !activeValue);\
  hal::OutputSignal name(\
      [](){ gpio_put(gpioSet.name, activeValue); App::instance().onSwitchedPowerChanged(true); },\
      [](){ gpio_put(gpioSet.name, !activeValue); App::instance().onSwitchedPowerChanged(false); })

namespace {

void powerOnSequence(const hal::OutputSignal& power, const hal::OutputSignal& reset) {
  reset.activate();
  power.activate();
  sleep_ms(500);
  reset.deactivate();
}

bool busyWaitEq(const hal::InputSignal& signal, bool value, std::uint64_t timeoutDurationUs) {
  std::uint64_t timeoutTimePoint = time_us_64() + timeoutDurationUs;
  while (signal.isActive() != value) {
    if (time_us_64() >= timeoutTimePoint) {
      return false;
    }
  }
  return true;
}

} // namespace

template<typename AppTraits>
platform::rp2040::AppLauncher<AppTraits>::AppLauncher() {
  if constexpr (AppTraits::neoPixelGpio.switchedPower) {
    DEFINE_OUTPUT_SIGNAL(AppTraits::neoPixelGpio, power, true);
    power.activate();
  }
  App::instance().initNeoPixel(AppTraits::neoPixelGpio.tx, AppTraits::neoPixelGpio.serializeColor);

#if CFG_TUSB_MCU != OPT_MCU_NONE
  board_init();
#endif
  tusb_init();

  DEFINE_POWER_OUTPUT_SIGNAL(AppTraits::systemGpio, power, true);
  DEFINE_OUTPUT_SIGNAL(AppTraits::systemGpio, reset, false);
  DEFINE_OUTPUT_SIGNAL(AppTraits::keyboardGpio, start, false);
  DEFINE_OUTPUT_SIGNAL(AppTraits::keyboardGpio, select, false);
  DEFINE_OUTPUT_SIGNAL(AppTraits::keyboardGpio, option, false);
  DEFINE_INPUT_SIGNAL(AppTraits::keyboardGpio, k0, false);
  DEFINE_INPUT_SIGNAL(AppTraits::keyboardGpio, k5, false);
  DEFINE_OUTPUT_SIGNAL(AppTraits::keyboardGpio, kr1, false);
  DEFINE_OUTPUT_SIGNAL(AppTraits::keyboardGpio, kr2, false);
  ::app::App app(start, select, option, reset, power, powerOnSequence, k0, k5, kr1, kr2, busyWaitEq);
  App::instance().run(app);
}
