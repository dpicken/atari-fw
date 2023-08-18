#include "App.h"

#include <class/hid/hid_host.h>
#include <hardware/pio.h>
#include <ws2812.pio.h>

platform::rp2040::App::App()
  : m_neoPixelSerializeColor(nullptr)
  , m_switchedPowerActive(false)
  , m_keyboardAttachFailed(false)
  , m_app(nullptr) {
}

platform::rp2040::App& platform::rp2040::App::instance() {
  static App s_app;
  return s_app;
}

void platform::rp2040::App::run(::app::App& app) {
  m_app = &app;
  for(;;) {
    tuh_task();
    m_app->schedule();
  }
}

void platform::rp2040::App::onSwitchedPowerChanged(bool active) {
  m_switchedPowerActive = active;
  reflectStateInNeoPixel();
}

void platform::rp2040::App::onKeyboardAttachFailed() {
  m_keyboardAttachFailed = true;
  reflectStateInNeoPixel();
}

void platform::rp2040::App::onKeyboardAttached() {
  m_keyboardAttachFailed = false;
  m_app->onKeyboardAttached();
  reflectStateInNeoPixel();
}

void platform::rp2040::App::onKeyboardDetached() {
  m_app->onKeyboardDetached();
  reflectStateInNeoPixel();
}

void platform::rp2040::App::onKeyboardInput(const keyboard::usb::InputReport& report) {
  m_app->onKeyboardInput(report);
}

void platform::rp2040::App::initNeoPixel(unsigned int txGpio, NeoPixelSerializeColor neoPixelSerializeColor) {
  uint offset = pio_add_program(pio0, &ws2812_program);
  ws2812_program_init(pio0, 0, offset, txGpio, 800000, true);
  m_neoPixelSerializeColor = neoPixelSerializeColor;
  reflectStateInNeoPixel();
}

void platform::rp2040::App::reflectStateInNeoPixel() {
  if (m_keyboardAttachFailed && !m_app->isKeyboardAttached()) {
    setNeoPixelRGB(0xFF, 0xFF, 0xFF);
  } else if (!m_switchedPowerActive) {
    setNeoPixelRGB(0x04, 0x00, 0x00);
  } else if (!m_app->isKeyboardAttached()) {
    setNeoPixelRGB(0x00, 0x00, 0x04);
  } else {
    setNeoPixelRGB(0x00, 0x04, 0x00);
  }
}

void platform::rp2040::App::setNeoPixelRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
  pio_sm_put_blocking(pio0, 0, m_neoPixelSerializeColor(r, g, b));
}

extern "C" {

void tuh_hid_mount_cb(uint8_t devAddr, uint8_t instance, uint8_t const* descReport, uint16_t descLen) {
  (void) descReport; (void) descLen;

  if (tuh_hid_interface_protocol(devAddr, instance) != HID_ITF_PROTOCOL_KEYBOARD) {
    return;
  }

  if (!tuh_hid_receive_report(devAddr, instance)) {
    platform::rp2040::App::instance().onKeyboardAttachFailed();
    return;
  }

  platform::rp2040::App::instance().onKeyboardAttached();
}

void tuh_hid_umount_cb(uint8_t devAddr, uint8_t instance) {
  (void) devAddr; (void) instance;

  if (tuh_hid_interface_protocol(devAddr, instance) != HID_ITF_PROTOCOL_KEYBOARD) {
    return;
  }

  platform::rp2040::App::instance().onKeyboardDetached();
}

void tuh_hid_report_received_cb(uint8_t devAddr, uint8_t instance, uint8_t const* report, uint16_t len) {
  if (len == sizeof(keyboard::usb::InputReport)) {
    platform::rp2040::App::instance().onKeyboardInput(*reinterpret_cast<const keyboard::usb::InputReport*>(report));
  }

  tuh_hid_receive_report(devAddr, instance);
}

} // extern "C"
