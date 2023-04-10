#include "hardware/InputSignal.h"
#include "hardware/OutputSignal.h"
#include "keyboard/Controller.h"
#include "keyboard/pokey/Controller.h"

#ifdef CFG_TUSB_MCU
#include <bsp/board.h>
#endif
#include <class/hid/hid_host.h>
#include <pico/stdlib.h>
#include <tusb.h>

#define DEFINE_INPUT_SIGNAL(name, gpio, activeValue)\
  gpio_init(gpio);\
  gpio_set_dir(gpio, GPIO_IN);\
  hardware::InputSignal name([](){ return gpio_get(gpio) == activeValue; })

#define DEFINE_OUTPUT_SIGNAL(name, gpio, activeValue)\
  gpio_init(gpio);\
  gpio_set_dir(gpio, GPIO_OUT);\
  gpio_put(gpio, !activeValue);\
  hardware::OutputSignal name([](){ gpio_put(gpio, activeValue); }, [](){ gpio_put(gpio, !activeValue); })

namespace {

class UsbState {
public:
  static UsbState& get() {
    static UsbState s_state;
    return s_state;
  }

  void incrementMounted(int delta) {
    m_mountCount += delta;
  }

  bool isMounted() const {
    return m_mountCount != 0;
  }

  void setCurrentInputReport(const keyboard::usb::InputReport& report) {
    m_currentInputReport = report;
  }

  const keyboard::usb::InputReport& getCurrentInputReport() const {
    return m_currentInputReport;
  }

private:
  UsbState()
    : m_mountCount(0) {
  }

  int m_mountCount;
  keyboard::usb::InputReport m_currentInputReport;
};

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

} // namespace

int main() {
#ifdef CFG_TUSB_MCU
  board_init();
#endif
  tusb_init();

#if 0
  DEFINE_OUTPUT_SIGNAL(reserved1, 29, false); // Pin 1
  DEFINE_OUTPUT_SIGNAL(reserved2, 28, false); // Pin 2
#endif
  DEFINE_OUTPUT_SIGNAL(power, 27, true);      // Pin 3
  DEFINE_OUTPUT_SIGNAL(reset, 26, false);     // Pin 4
  DEFINE_OUTPUT_SIGNAL(option, 24, false);    // Pin 5
  DEFINE_OUTPUT_SIGNAL(select, 25, false);    // Pin 6
  DEFINE_OUTPUT_SIGNAL(start, 20, false);     // Pin 7
  keyboard::console::Controller consoleController(start, select, option, reset, power, powerOnSequence);

  DEFINE_INPUT_SIGNAL(k0, 5, false);          // Pin 8
  DEFINE_INPUT_SIGNAL(k5, 6, false);          // Pin 9
  DEFINE_OUTPUT_SIGNAL(kr1, 4, false);        // Pin 10
  DEFINE_OUTPUT_SIGNAL(kr2, 3, false);        // Pin 11
  keyboard::pokey::Controller pokeyController(k0, k5, kr1, kr2, busyWaitEq);

  keyboard::Controller keyboardController(&consoleController, &pokeyController);

  const UsbState& usbState = UsbState::get();
  for(;;) {
    tuh_task();
    if (usbState.isMounted()) {
      keyboardController.receiveInputReport(usbState.getCurrentInputReport());
    }
  }
}

extern "C" {

void tuh_hid_mount_cb(uint8_t devAddr, uint8_t instance, uint8_t const* descReport, uint16_t descLen) {
  (void) descReport; (void) descLen;

  if (tuh_hid_interface_protocol(devAddr, instance) != HID_ITF_PROTOCOL_KEYBOARD) {
    return;
  }

  if (!tuh_hid_receive_report(devAddr, instance)) {
    return;
  }

  UsbState::get().incrementMounted(1);
}

void tuh_hid_umount_cb(uint8_t devAddr, uint8_t instance) {
  (void) devAddr; (void) instance;

  UsbState::get().incrementMounted(-1);
}

void tuh_hid_report_received_cb(uint8_t devAddr, uint8_t instance, uint8_t const* report, uint16_t len) {
  if (len == sizeof(keyboard::usb::InputReport)) {
    UsbState::get().setCurrentInputReport(*reinterpret_cast<const keyboard::usb::InputReport*>(report));
  }

  tuh_hid_receive_report(devAddr, instance);
}

} // extern "C"
