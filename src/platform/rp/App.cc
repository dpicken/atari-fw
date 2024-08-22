#include "App.h"

#include <class/hid/hid_host.h>
#include <hardware/pio.h>
#include <ws2812.pio.h>

platform::rp::App::App()
  : m_neoPixelRefreshTimePoint(time_us_32())
  , m_switchedPowerActive(false)
  , m_sioXferActive(false)
  , m_sioXferTimeout(false)
  , m_sdXferActive(false)
  , m_sdXferTimeout(false)
  , m_keyboardApp(nullptr)
  , m_sioApp(nullptr) {
}

platform::rp::App& platform::rp::App::instance() {
  static App instance;
  return instance;
}

void platform::rp::App::initNeoPixel(unsigned int txGpio, NeoPixelSerializeColor neoPixelSerializeColor) {
  uint offset = pio_add_program(pio0, &ws2812_program);
  ws2812_program_init(pio0, 0, offset, txGpio, 800000, true);
  m_neoPixelSerializeColor = neoPixelSerializeColor;
}

void platform::rp::App::run(::keyboard::App& app) {
  m_keyboardApp = &app;
  for(;;) {
    tuh_task();
    reflectStateInNeoPixel();
    m_keyboardApp->schedule();
  }
}

void platform::rp::App::run(::sio::App& app) {
  m_sioApp = &app;
  for (;;) {
    m_sioApp->schedule();
  }
}

void platform::rp::App::onSwitchedPowerChanged(bool active) {
  m_switchedPowerActive = active;
}

void platform::rp::App::onKeyboardAttached() {
  m_keyboardApp->onKeyboardAttached();
}

void platform::rp::App::onKeyboardDetached() {
  m_keyboardApp->onKeyboardDetached();
}

void platform::rp::App::onKeyboardInput(const keyboard::usb::InputReport& report) {
  m_keyboardApp->onKeyboardInput(report);
}

void platform::rp::App::onSioXfer(bool active) {
  m_sioXferTimeout.store(false, std::memory_order_relaxed);
  m_sioXferActive.store(active, std::memory_order_relaxed);
}

void platform::rp::App::onSioXferTimeout() {
  m_sioXferActive.store(false, std::memory_order_relaxed);
  m_sioXferTimeout.store(true, std::memory_order_relaxed);
}

void platform::rp::App::onSdXfer(bool active) {
  m_sdXferTimeout.store(false, std::memory_order_relaxed);
  m_sdXferActive.store(active, std::memory_order_relaxed);
}

void platform::rp::App::onSdXferTimeout() {
  m_sdXferActive.store(false, std::memory_order_relaxed);
  m_sdXferTimeout.store(true, std::memory_order_relaxed);
}

void platform::rp::App::reflectStateInNeoPixel() {
  std::uint32_t currentTimePoint = time_us_32();
  if (currentTimePoint - m_neoPixelRefreshTimePoint < 250 * 1000) {
    return;
  }
  m_neoPixelRefreshTimePoint = currentTimePoint;

  if (!m_keyboardApp->isKeyboardAttached()) {
    setNeoPixelRGB(0x00, 0x00, 0xFF); // Blue (bright)
  } else if (!m_switchedPowerActive) {
    setNeoPixelRGB(0x04, 0x00, 0x00); // Red
  } else if (m_sdXferTimeout.load(std::memory_order_relaxed)) {
    setNeoPixelRGB(0xFF, 0x00, 0xFF); // Purple (bright)
  } else if (m_sioXferTimeout.load(std::memory_order_relaxed)) {
    setNeoPixelRGB(0x04, 0x00, 0x04); // Purple
  } else if (m_sdXferActive.load(std::memory_order_relaxed)) {
    setNeoPixelRGB(0xFF, 0xFF, 0xFF); // White (bright)
  } else if (m_sioXferActive.load(std::memory_order_relaxed)) {
    setNeoPixelRGB(0x04, 0x04, 0x04); // White
  } else {
    setNeoPixelRGB(0x00, 0x04, 0x00); // Green
  }
}

void platform::rp::App::setNeoPixelRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
  pio_sm_put_blocking(pio0, 0, m_neoPixelSerializeColor(r, g, b));
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

  platform::rp::App::instance().onKeyboardAttached();
}

void tuh_hid_umount_cb(uint8_t devAddr, uint8_t instance) {
  (void) devAddr; (void) instance;

  if (tuh_hid_interface_protocol(devAddr, instance) != HID_ITF_PROTOCOL_KEYBOARD) {
    return;
  }

  platform::rp::App::instance().onKeyboardDetached();
}

void tuh_hid_report_received_cb(uint8_t devAddr, uint8_t instance, uint8_t const* report, uint16_t len) {
  if (len == sizeof(keyboard::usb::InputReport)) {
    platform::rp::App::instance().onKeyboardInput(*reinterpret_cast<const keyboard::usb::InputReport*>(report));
  }

  tuh_hid_receive_report(devAddr, instance);
}

} // extern "C"
