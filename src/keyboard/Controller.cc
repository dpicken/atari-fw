#include "Controller.h"

#include "pokey/KeyCode.h"
#include "usb/KeyCode.h"

#define usbTiedShift      (::keyboard::usb::ModifierBit::LeftShift | ::keyboard::usb::ModifierBit::RightShift)
#define usbTiedCtrl       (::keyboard::usb::ModifierBit::LeftCtrl | ::keyboard::usb::ModifierBit::RightCtrl)
#define usbTiedShiftCtrl  (usbTiedShift | usbTiedCtrl)

#define pokeyShift        pokey::ModifierCode::Shift
#define pokeyControl      pokey::ModifierCode::Control

#define MAKE_SIMPLE_MAPPING(usbKeyCode, pokeyKeyCode)\
  {{                  usb::KeyCode::usbKeyCode},  {              pokey::KeyCode::pokeyKeyCode}},\
  {{usbTiedShift,     usb::KeyCode::usbKeyCode},  {pokeyShift,   pokey::KeyCode::pokeyKeyCode}},\
  {{usbTiedCtrl,      usb::KeyCode::usbKeyCode},  {pokeyControl, pokey::KeyCode::pokeyKeyCode}}

#define MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(usbKeyCode, pokeyKeyCode)\
  {{                  usb::KeyCode::usbKeyCode},  {              pokey::KeyCode::pokeyKeyCode}},\
  {{usbTiedCtrl,      usb::KeyCode::usbKeyCode},  {pokeyControl, pokey::KeyCode::pokeyKeyCode}}

#define MAKE_SHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(usbKeyCode, pokeyKeyCode)\
  {{usbTiedShift,     usb::KeyCode::usbKeyCode},  {              pokey::KeyCode::pokeyKeyCode}},\
  {{usbTiedShiftCtrl, usb::KeyCode::usbKeyCode},  {pokeyControl, pokey::KeyCode::pokeyKeyCode}}

#define MAKE_UNSHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(usbKeyCode, pokeyKeyCode)\
  {{                  usb::KeyCode::usbKeyCode},  {pokeyShift,   pokey::KeyCode::pokeyKeyCode}}

#define MAKE_SHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(usbKeyCode, pokeyKeyCode)\
  {{usbTiedShift,     usb::KeyCode::usbKeyCode},  {pokeyShift,   pokey::KeyCode::pokeyKeyCode}}

#define MAKE_MODIFIER_MAPPING(usbModifierBitset, pokeyModifierCode)\
  {{usbModifierBitset                         },  {pokeyModifierCode}}

#define MAKE_UNSHIFTED_USB_TO_POKEY_MODIFIER(usbKeyCode, pokeyModifierCode)\
  {{                  usb::KeyCode::usbKeyCode},  {pokey::ModifierCode::pokeyModifierCode}}

namespace {

keyboard::usb::ModifierBitset tieShiftAndCtrl(keyboard::usb::ModifierBitset modifierBitset) {
  if ((modifierBitset & usbTiedShift) != 0) {
    modifierBitset |= usbTiedShift;
  }

  if ((modifierBitset & usbTiedCtrl) != 0) {
    modifierBitset |= usbTiedCtrl;
  }

  return modifierBitset;
}

}

keyboard::Controller::Controller(console::Controller* consoleController, pokey::Controller* pokeyController)
  : m_consoleController(consoleController)
  , m_usbToConsoleKeyMap{
      {usb::KeyCode::F2, console::KeyBit::Start},
      {usb::KeyCode::F3, console::KeyBit::Select},
      {usb::KeyCode::F4, console::KeyBit::Option},
      {usb::KeyCode::F10, console::KeyBit::D1RotateDisk},
      {usb::KeyCode::F11, console::KeyBit::Eject},
      {usb::KeyCode::F12, console::KeyBit::Reset},
      {{usbTiedCtrl, usb::KeyCode::F12}, console::KeyBit::Power}
    }
  , m_pokeyController(pokeyController)
  , m_usbToPokeyKeyMap{
      {{usb::KeyCode::F1}, {pokey::KeyCode::Help}},
      // Row 1
      MAKE_SIMPLE_MAPPING(Escape, Escape),
      MAKE_SIMPLE_MAPPING(One_Pling, One_Pling),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Two_At, Two_DoubleQuote),
      MAKE_SHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(SingleQuota_DoubleQuote, Two_DoubleQuote),
      MAKE_SIMPLE_MAPPING(Three_Hash, Three_Hash),
      MAKE_SIMPLE_MAPPING(Four_Dollar, Four_Dollar),
      MAKE_SIMPLE_MAPPING(Five_Percent, Five_Percent),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Six_Hat, Six_Ampersand),
      MAKE_SHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(Seven_Ampersand, Six_Ampersand),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Seven_Ampersand, Seven_SingleQuote),
      MAKE_UNSHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(SingleQuota_DoubleQuote, Seven_SingleQuote),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Eight_Star, Eight_At),
      MAKE_SHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(Two_At, Eight_At),
      MAKE_SIMPLE_MAPPING(Nine_OpenParenthesis, Nine_OpenParenthesis),
      MAKE_SIMPLE_MAPPING(Zero_CloseParenthesis, Zero_CloseParenthesis),
      MAKE_SHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Comma_LessThan, LessThan_Clear),
      MAKE_UNSHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(F6, LessThan_Clear),
      MAKE_SHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Period_GreaterThan, GreaterThan_Insert),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Insert, GreaterThan_Insert),
      MAKE_SIMPLE_MAPPING(Delete, Backspace),
      MAKE_UNSHIFTED_USB_TO_POKEY_MODIFIER(Pause, Break),
      // Row 2
      MAKE_SIMPLE_MAPPING(Tab, Tab),
      MAKE_SIMPLE_MAPPING(Q, Q),
      MAKE_SIMPLE_MAPPING(W, W),
      MAKE_SIMPLE_MAPPING(E, E),
      MAKE_SIMPLE_MAPPING(R, R),
      MAKE_SIMPLE_MAPPING(T, T),
      MAKE_SIMPLE_MAPPING(Y, Y),
      MAKE_SIMPLE_MAPPING(U, U),
      MAKE_SIMPLE_MAPPING(I, I),
      MAKE_SIMPLE_MAPPING(O, O),
      MAKE_SIMPLE_MAPPING(P, P),
      MAKE_SIMPLE_MAPPING(Minus_Underscore, Minus_Underscore),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(UpArrow, Minus_Underscore),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Equal_Plus, Equals_Pipe),
      MAKE_SHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(Backslash_Pipe, Equals_Pipe),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(DownArrow, Equals_Pipe),
      MAKE_SIMPLE_MAPPING(Return, Return),
      // Row 3
      MAKE_MODIFIER_MAPPING(usbTiedCtrl, pokeyControl),
      MAKE_SIMPLE_MAPPING(A, A),
      MAKE_SIMPLE_MAPPING(S, S),
      MAKE_SIMPLE_MAPPING(D, D),
      MAKE_SIMPLE_MAPPING(F, F),
      MAKE_SIMPLE_MAPPING(G, G),
      MAKE_SIMPLE_MAPPING(H, H),
      MAKE_SIMPLE_MAPPING(J, J),
      MAKE_SIMPLE_MAPPING(K, K),
      MAKE_SIMPLE_MAPPING(L, L),
      MAKE_SIMPLE_MAPPING(SemiColon_Colon, SemiColon_Colon),
      MAKE_SHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Equal_Plus, Plus_Backslash),
      MAKE_UNSHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(Backslash_Pipe, Plus_Backslash),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(LeftArrow, Plus_Backslash),
      MAKE_SHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Eight_Star, Star_Hat),
      MAKE_SHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(Six_Hat, Star_Hat),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(RightArrow, Star_Hat),
      MAKE_SIMPLE_MAPPING(CapsLock, CapsLock),
      // Row 4
      MAKE_MODIFIER_MAPPING(usbTiedShift, pokeyShift),
      MAKE_SIMPLE_MAPPING(Z, Z),
      MAKE_SIMPLE_MAPPING(X, X),
      MAKE_SIMPLE_MAPPING(C, C),
      MAKE_SIMPLE_MAPPING(V, V),
      MAKE_SIMPLE_MAPPING(B, B),
      MAKE_SIMPLE_MAPPING(N, N),
      MAKE_SIMPLE_MAPPING(M, M),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Comma_LessThan, Comma_OpenSquareBracket),
      MAKE_UNSHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(OpenSquareBracket_OpenBrace, Comma_OpenSquareBracket),
      MAKE_UNSHIFTED_USB_TO_UNSHIFTED_POKEY_MAPPING(Period_GreaterThan, Period_CloseSquareBracket),
      MAKE_UNSHIFTED_USB_TO_SHIFTED_POKEY_MAPPING(CloseSquareBracket_CloseBrace, Period_CloseSquareBracket),
      MAKE_SIMPLE_MAPPING(Slash_QuestionMark, Slash_QuestionMark),
      MAKE_SIMPLE_MAPPING(F7, Invert),
      // Row 5
      MAKE_SIMPLE_MAPPING(Spacebar, Space)
    } {
}

void keyboard::Controller::receiveInputReport(const usb::InputReport& report) {
  dispatchInputReportToConsole(report);
  dispatchInputReportToPokey(report);
}

void keyboard::Controller::dispatchInputReportToConsole(const usb::InputReport& report) {
  console::KeyBitset consoleKeyBitset = 0;
  for (auto usbKeyCode : report.keyCodes) {
    usb::InputReport simpleInputReport(tieShiftAndCtrl(report.modifierBitset), usb::KeyCode(usbKeyCode));
    auto it = m_usbToConsoleKeyMap.find(simpleInputReport);
    if (it != m_usbToConsoleKeyMap.end()) {
      consoleKeyBitset |= it->second;
    }
  }
  m_consoleController->receiveInputReport(consoleKeyBitset);
}

void keyboard::Controller::dispatchInputReportToPokey(const usb::InputReport& report) {
  usb::InputReport simpleInputReport(tieShiftAndCtrl(report.modifierBitset), usb::KeyCode(report.keyCodes[0]));
  const auto it = m_usbToPokeyKeyMap.find(simpleInputReport);
  m_pokeyController->receiveInputReport(it != m_usbToPokeyKeyMap.end() ? it->second : pokey::InputReport());
}
