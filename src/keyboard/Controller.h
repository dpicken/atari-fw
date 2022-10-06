#ifndef keyboard_Controller_h
#define keyboard_Controller_h

#include "hardware/OutputSignal.h"
#include "pokey/Controller.h"
#include "pokey/InputReport.h"
#include "usb/InputReport.h"

#include <unordered_map>
#include <utility>
#include <vector>

namespace keyboard {

class Controller {
public:
  Controller(
      ::hardware::OutputSignal start,
      ::hardware::OutputSignal select,
      ::hardware::OutputSignal option,
      ::hardware::OutputSignal reset,
      const pokey::Controller* pokeyController);

  /** Receive a USB (boot interface) keyboard report. */
  void receiveInputReport(const usb::InputReport& report) const;

private:
  void dispatchInputReportToNonPokey(const usb::InputReport& report) const;
  void dispatchInputReportToPokey(const usb::InputReport& report) const;

  const std::vector<std::pair<const usb::InputReport, const ::hardware::OutputSignal>> m_usbToNonPokeyKeyMap;

  const std::unordered_map<usb::InputReport, const pokey::InputReport, usb::InputReport::Hash> m_usbToPokeyKeyMap;

  const pokey::Controller* const m_pokeyController;
};

} // namespace keyboard

#endif // ifndef keyboard_Controller_h
