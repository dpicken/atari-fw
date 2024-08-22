#ifndef platform_rp_App_h
#define platform_rp_App_h

#include "NeoPixelSerializeColor.h"

#include "keyboard/App.h"
#include "sio/App.h"

#include <bsp/board.h>
#include <tusb.h>

#include <atomic>
#include <cstdint>

namespace platform { namespace rp {

class App {
public:
  static App& instance();

  void initNeoPixel(unsigned int txGpio, NeoPixelSerializeColor neoPixelSerializeColor);

  [[noreturn]] void run(::keyboard::App& keyboardApp);
  [[noreturn]] void run(::sio::App& app);

  void onSwitchedPowerChanged(bool active);
  void onKeyboardAttached();
  void onKeyboardDetached();
  void onKeyboardInput(const keyboard::usb::InputReport& report);

  void onSioXfer(bool active);
  void onSioXferTimeout();

  void onSdXfer(bool active);
  void onSdXferTimeout();

private:
  App();

  void reflectStateInNeoPixel();
  void setNeoPixelRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);

  NeoPixelSerializeColor m_neoPixelSerializeColor;
  std::uint32_t m_neoPixelRefreshTimePoint;

  bool m_switchedPowerActive;

  std::atomic_bool m_sioXferActive;
  std::atomic_bool m_sioXferTimeout;
  std::atomic_bool m_sdXferActive;
  std::atomic_bool m_sdXferTimeout;

  ::keyboard::App* m_keyboardApp;
  ::sio::App* m_sioApp;
};

} } // namespace platform::rp

#endif // ifndef platform_rp_App_h
