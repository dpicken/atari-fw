#ifndef platform_rp2040_App_h
#define platform_rp2040_App_h

#include "NeoPixelSerializeColor.h"

#include "app/App.h"
#include "hardware/InputSignal.h"
#include "hardware/OutputSignal.h"

#ifdef CFG_TUSB_MCU
#include <bsp/board.h>
#else
#define CFG_TUSB_MCU OPT_MCU_NONE
#define TUP_DCD_ENDPOINT_MAX 8
#define __force_inline
#endif
#include <tusb.h>

#include <cstdint>

namespace platform { namespace rp2040 {

class App {
public:
  static App& instance();

  void initNeoPixel(unsigned int txGpio, NeoPixelSerializeColor neoPixelSerializeColor);

  [[noreturn]] void run(::app::App& app);

  void onSwitchedPowerChanged(bool active);
  void onKeyboardAttachFailed();
  void onKeyboardAttached();
  void onKeyboardDetached();
  void onKeyboardInput(const keyboard::usb::InputReport& report);

private:
  App();

  void reflectStateInNeoPixel();
  void setNeoPixelRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);

  NeoPixelSerializeColor m_neoPixelSerializeColor;
  bool m_switchedPowerActive;
  bool m_keyboardAttachFailed;
  ::app::App* m_app;
};

} } // namespace platform::rp2040

#endif // ifndef platform_rp2040_App_h
