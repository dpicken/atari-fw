#include "app/App.h"
#include "hardware/InputSignal.h"
#include "hardware/misc.h"
#include "hardware/OutputSignal.h"

#ifdef CFG_TUSB_MCU
#include <bsp/board.h>
#else
#define CFG_TUSB_MCU OPT_MCU_NONE
#define TUP_DCD_ENDPOINT_MAX 8
#define __force_inline
#endif
#include <class/hid/hid_host.h>
#include <hardware/pio.h>
#include <pico/stdlib.h>
#include <tusb.h>
#include <ws2812.pio.h>

static app::App* s_app;
static bool s_switchedPowerActive = false;

#define DEFINE_INPUT_SIGNAL(name, gpio, activeValue)\
  gpio_init(gpio);\
  gpio_set_dir(gpio, GPIO_IN);\
  hardware::InputSignal name([](){ return gpio_get(gpio) == activeValue; })

#define DEFINE_OUTPUT_SIGNAL(name, gpio, activeValue)\
  gpio_init(gpio);\
  gpio_set_dir(gpio, GPIO_OUT);\
  gpio_put(gpio, !activeValue);\
  hardware::OutputSignal name([](){ gpio_put(gpio, activeValue); }, [](){ gpio_put(gpio, !activeValue); })

#define DEFINE_POWER_OUTPUT_SIGNAL(name, gpio, activeValue)\
  gpio_init(gpio);\
  gpio_set_dir(gpio, GPIO_OUT);\
  gpio_put(gpio, !activeValue);\
  hardware::OutputSignal name(\
      [](){ gpio_put(gpio, activeValue); s_switchedPowerActive = true; onBoardStateChanged(); },\
      [](){ gpio_put(gpio, !activeValue); s_switchedPowerActive = false; onBoardStateChanged(); })

namespace {

void initNeoPixel(uint gpioPower, uint gpioTx) {
  gpio_init(gpioPower);
  gpio_set_dir(gpioPower, GPIO_OUT);
  gpio_put(gpioPower, true);

  uint offset = pio_add_program(pio0, &ws2812_program);
  ws2812_program_init(pio0, 0, offset, gpioTx, 800000, true);
}

void setNeoPixelRGB(uint8_t r, uint8_t g, uint32_t b) {
  uint32_t pixelValue = 0;
  pixelValue |= 0x0;
  pixelValue <<= 8;
  pixelValue |= g;
  pixelValue <<= 8;
  pixelValue |= r;
  pixelValue <<= 8;
  pixelValue |= b;
  pixelValue <<= 8;
  pio_sm_put_blocking(pio0, 0, pixelValue);
}

void powerOnSequence(const hardware::OutputSignal& power, const hardware::OutputSignal& reset) {
  reset.activate();
  power.activate();
  sleep_ms(500);
  reset.deactivate();
}

bool busyWaitEq(const hardware::InputSignal& signal, bool value, std::uint64_t timeoutDurationUs) {
  std::uint64_t timeoutTimePoint = time_us_64() + timeoutDurationUs;
  while (signal.isActive() != value) {
    if (time_us_64() >= timeoutTimePoint) {
      return false;
    }
  }
  return true;
}

void onBoardStateChanged() {
  if (!s_switchedPowerActive) {
    setNeoPixelRGB(0x04, 0x00, 0x00);
  } else if (!s_app->isKeyboardAttached()) {
    setNeoPixelRGB(0x00, 0x00, 0x04);
  } else {
    setNeoPixelRGB(0x00, 0x04, 0x00);
  }
}

} // namespace

int main() {
  initNeoPixel(11, 12);
  setNeoPixelRGB(0x04, 0x04, 0x04);

#if CFG_TUSB_MCU != OPT_MCU_NONE
  board_init();
#endif
  tusb_init();

#if 0
  DEFINE_OUTPUT_SIGNAL(reserved1, 29, false);   // Pin 1
  DEFINE_OUTPUT_SIGNAL(reserved2, 28, false);   // Pin 2
#endif
  DEFINE_POWER_OUTPUT_SIGNAL(power, 27, true);  // Pin 3
  DEFINE_OUTPUT_SIGNAL(reset, 26, false);       // Pin 4
  DEFINE_OUTPUT_SIGNAL(option, 24, false);      // Pin 5
  DEFINE_OUTPUT_SIGNAL(select, 25, false);      // Pin 6
  DEFINE_OUTPUT_SIGNAL(start, 20, false);       // Pin 7
  DEFINE_INPUT_SIGNAL(k0, 5, false);            // Pin 8
  DEFINE_INPUT_SIGNAL(k5, 6, false);            // Pin 9
  DEFINE_OUTPUT_SIGNAL(kr1, 4, false);          // Pin 10
  DEFINE_OUTPUT_SIGNAL(kr2, 3, false);          // Pin 11
  app::App app(start, select, option, reset, power, powerOnSequence, k0, k5, kr1, kr2, busyWaitEq);
  s_app = &app;

  onBoardStateChanged();

  for(;;) {
    tuh_task();
    s_app->schedule();
  }
}

extern "C" {

void tuh_hid_mount_cb(uint8_t devAddr, uint8_t instance, uint8_t const* descReport, uint16_t descLen) {
  (void) descReport; (void) descLen;

  if (tuh_hid_interface_protocol(devAddr, instance) != HID_ITF_PROTOCOL_KEYBOARD) {
    return;
  }

  if (!tuh_hid_receive_report(devAddr, instance)) {
    setNeoPixelRGB(0x04, 0x04, 0x04);
  }

  s_app->onKeyboardAttached();
  onBoardStateChanged();
}

void tuh_hid_umount_cb(uint8_t devAddr, uint8_t instance) {
  (void) devAddr; (void) instance;

  if (tuh_hid_interface_protocol(devAddr, instance) != HID_ITF_PROTOCOL_KEYBOARD) {
    return;
  }

  s_app->onKeyboardDetached();
  onBoardStateChanged();
}

void tuh_hid_report_received_cb(uint8_t devAddr, uint8_t instance, uint8_t const* report, uint16_t len) {
  if (len == sizeof(keyboard::usb::InputReport)) {
    s_app->onKeyboardInput(*reinterpret_cast<const keyboard::usb::InputReport*>(report));
  }

  tuh_hid_receive_report(devAddr, instance);
}

} // extern "C"
