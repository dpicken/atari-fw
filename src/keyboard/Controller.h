#ifndef keyboard_Controller_h
#define keyboard_Controller_h

#include "console/Controller.h"
#include "console/KeyBit.h"
#include "pokey/Controller.h"
#include "pokey/InputReport.h"
#include "usb/InputReport.h"

#include <unordered_map>
#include <utility>
#include <vector>

namespace keyboard {

class Controller {
public:
  Controller(console::Controller* consoleController, pokey::Controller* pokeyController);

  /** Receive a USB (boot interface) keyboard report. */
  void receiveInputReport(const usb::InputReport& report);

private:
  void dispatchInputReportToConsole(const usb::InputReport& report);
  void dispatchInputReportToPokey(const usb::InputReport& report);

  console::Controller* const m_consoleController;
  const std::unordered_map<usb::InputReport, const console::KeyBit, usb::InputReport::Hash> m_usbToConsoleKeyMap;

  pokey::Controller* const m_pokeyController;
  const std::unordered_map<usb::InputReport, const pokey::InputReport, usb::InputReport::Hash> m_usbToPokeyKeyMap;
};

} // namespace keyboard

#endif // ifndef keyboard_Controller_h
