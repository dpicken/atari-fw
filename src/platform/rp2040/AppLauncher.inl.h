#include "App.h"

#include "hal/InputSignal.h"
#include "hal/OutputSignal.h"
#include "hal/Uart.h"
#include "sio/App.h"
#include "sio/Traits.h"

#include <pico/multicore.h>
#include <pico/stdlib.h>

#include <type_traits>

namespace {

const hal::InputSignal nullInputSignal([](){ return false; });
const hal::OutputSignal nullOutputSignal([](){}, [](){});

std::uint32_t getSioUartXferTimeoutDurationUs(std::size_t byteCount) {
  auto bitCount = byteCount * (sio::Traits::uart_start_bit_count + sio::Traits::uart_data_bit_count + sio::Traits::uart_stop_bit_count);
  auto timeoutDurationUs = (bitCount * 1000 * 1000) / sio::Traits::uart_baud_rate;
  return static_cast<std::uint32_t>(timeoutDurationUs * 2);
}

template<uint gpio, bool activeValue>
hal::InputSignal makeInputSignal() {
  gpio_init(gpio);
  gpio_set_dir(gpio, GPIO_IN);
  if (activeValue) {
    gpio_pull_down(gpio);
  } else {
    gpio_pull_up(gpio);
  }
  return hal::InputSignal(
      [](){ return gpio_get(gpio) == activeValue; });
}

template<uint gpio, bool activeValue>
hal::OutputSignal makeOutputSignal() {
  gpio_init(gpio);
  gpio_set_dir(gpio, GPIO_OUT);
  gpio_put(gpio, !activeValue);
  return hal::OutputSignal(
      [](){ gpio_put(gpio, activeValue); },
      [](){ gpio_put(gpio, !activeValue); });
}

template<uint gpio, bool activeValue>
hal::OutputSignal makePowerOutputSignal() {
  gpio_init(gpio);
  gpio_set_dir(gpio, GPIO_OUT);
  gpio_put(gpio, !activeValue);
  return hal::OutputSignal(
      [](){ gpio_put(gpio, activeValue); platform::rp2040::App::instance().onSwitchedPowerChanged(true); },
      [](){ gpio_put(gpio, !activeValue); platform::rp2040::App::instance().onSwitchedPowerChanged(false); });
}

void powerOnSequence(const hal::OutputSignal& power, const hal::OutputSignal& reset) {
  reset.activate();
  power.activate();
  sleep_ms(500);
  reset.deactivate();
}

bool busyWaitEq(const hal::InputSignal& signal, bool value, hal::duration_us timeoutDurationUs) {
  static_assert(std::is_same<hal::duration_us, std::uint32_t>::value);
  std::uint32_t beginTimePoint = time_us_32();
  while (signal.isActive() != value) {
    if ((time_us_32() - beginTimePoint) >= timeoutDurationUs) {
      return false;
    }
  }
  return true;
}

void busyWait(hal::duration_us timeoutDurationUs) {
  static_assert(std::is_same<hal::duration_us, std::uint32_t>::value);
  std::uint32_t beginTimePoint = time_us_32();
  while ((time_us_32() - beginTimePoint) < timeoutDurationUs) {
  }
}

uart_inst_t* toUart(unsigned int uartInstance) {
  return (uartInstance == 0) ? uart0 : uart1;
}

bool uartRx(unsigned int uartInstance, std::uint8_t* buf, std::size_t byteCount) {
  auto uart = toUart(uartInstance);
  std::uint32_t timeoutDurationUs = getSioUartXferTimeoutDurationUs(byteCount);
  std::uint32_t beginTimePoint = time_us_32();
  for (auto it = buf, end = buf + byteCount; it != end; ++it) {
    while (!uart_is_readable(uart)) {
      if ((time_us_32() - beginTimePoint) >= timeoutDurationUs) {
        platform::rp2040::App::instance().onSioXferTimeout();
        return false;
      }
    }
    platform::rp2040::App::instance().onSioXfer(true);
    uart_read_blocking(uart, it, 1);
  }
  platform::rp2040::App::instance().onSioXfer(false);
  return true;
}

bool uartTx(unsigned int uartInstance, const std::uint8_t* buf, std::size_t byteCount) {
  auto uart = toUart(uartInstance);
  std::uint32_t timeoutDurationUs = getSioUartXferTimeoutDurationUs(byteCount);
  std::uint32_t beginTimePoint = time_us_32();
  for (auto it = buf, end = buf + byteCount; it != end; ++it) {
    while (!uart_is_writable(uart)) {
      if ((time_us_32() - beginTimePoint) >= timeoutDurationUs) {
        platform::rp2040::App::instance().onSioXferTimeout();
        return false;
      }
    }
    platform::rp2040::App::instance().onSioXfer(true);
    uart_write_blocking(uart, it, 1);
  }
  platform::rp2040::App::instance().onSioXfer(false);
  return true;
}

template<auto gpioSet>
hal::Uart makeUart() {
  if constexpr(gpioSet.supported) {
    static_assert(gpioSet.uartInstance <= 1, "invalid uart instance");
    auto uart = toUart(gpioSet.uartInstance);
    uart_init(uart, sio::Traits::uart_baud_rate);
    gpio_set_function(gpioSet.uartRx, GPIO_FUNC_UART);
    gpio_set_function(gpioSet.uartTx, GPIO_FUNC_UART);
    uart_set_hw_flow(uart, false, false);
    uart_set_format(uart, sio::Traits::uart_data_bit_count, sio::Traits::uart_stop_bit_count, UART_PARITY_NONE);
    return hal::Uart(
        [](std::uint8_t* buf, std::size_t byteCount) { return uartRx(gpioSet.uartInstance, buf, byteCount); },
        [](const std::uint8_t* buf, std::size_t byteCount) { return uartTx(gpioSet.uartInstance, buf, byteCount); });
  } else {
    (void) uartRx;
    (void) uartTx;
    return hal::Uart(
        [](std::uint8_t*, std::size_t) { return false; },
        [](const std::uint8_t*, std::size_t) { return false; });
  }
}

} // namespace

#define DEFINE_INPUT_SIGNAL(gpioSet, name, activeValue)         auto name = makeInputSignal<gpioSet.name, activeValue>()
#define DEFINE_OUTPUT_SIGNAL(gpioSet, name, activeValue)        auto name = makeOutputSignal<gpioSet.name, activeValue>()
#define DEFINE_POWER_OUTPUT_SIGNAL(gpioSet, name, activeValue)  auto name = makePowerOutputSignal<gpioSet.name, activeValue>()

#define MAKE_OPTIONAL_SIGNAL(gpioSet, name, activeValue, makeSignalFn, nullSignal)\
    [](){\
      if constexpr(gpioSet.supported) {\
        return makeSignalFn<gpioSet.name, activeValue>();\
      } else {\
        return nullSignal;\
      }\
    }()
#define MAKE_OPTIONAL_INPUT_SIGNAL(gpioSet, name, activeValue)  MAKE_OPTIONAL_SIGNAL(gpioSet, name, activeValue, makeInputSignal, nullInputSignal)
#define MAKE_OPTIONAL_OUTPUT_SIGNAL(gpioSet, name, activeValue) MAKE_OPTIONAL_SIGNAL(gpioSet, name, activeValue, makeOutputSignal, nullOutputSignal)

template<typename AppTraits>
platform::rp2040::AppLauncher<AppTraits>::AppLauncher() {
  if constexpr (AppTraits::neoPixelGpio.switchedPower) {
    DEFINE_OUTPUT_SIGNAL(AppTraits::neoPixelGpio, power, true);
    power.activate();
  }

  App::instance().initNeoPixel(AppTraits::neoPixelGpio.tx, AppTraits::neoPixelGpio.serializeColor);

  if (AppTraits::sioGpio.supported) {
    multicore_launch_core1(runKeyboardApp);
    // Note: Run the sio app on core0 because the uart doesn't appear functional via core1.
    runSioApp();
  } else {
    runKeyboardApp();
  }
}

template<typename AppTraits>
void platform::rp2040::AppLauncher<AppTraits>::runKeyboardApp() {
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

  ::keyboard::App keyboardApp(start, select, option, reset, power, powerOnSequence, k0, k5, kr1, kr2, busyWaitEq);
  App::instance().run(keyboardApp);
}

template<typename AppTraits>
void platform::rp2040::AppLauncher<AppTraits>::runSioApp() {
  auto sioCommand = MAKE_OPTIONAL_INPUT_SIGNAL(AppTraits::sioGpio, command, false);
  auto sioUart = makeUart<AppTraits::sioGpio>();

  ::sio::App sioApp(sioCommand, sioUart, busyWait, busyWaitEq);
  App::instance().run(sioApp);
}
