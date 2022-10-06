#ifndef keyboard_usb_KeyCode_h
#define keyboard_usb_KeyCode_h

#include <cstdint>

namespace keyboard { namespace usb {

enum class KeyCode : std::uint8_t {
  NoEventIndicated,
  ErrorRollOver,
  ErrorPOSTFail,
  ErrorUndefined,
  A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
  One_Pling,
  Two_At,
  Three_Hash,
  Four_Dollar,
  Five_Percent,
  Six_Hat,
  Seven_Ampersand,
  Eight_Star,
  Nine_OpenParenthesis,
  Zero_CloseParenthesis,
  Return,
  Escape,
  Delete,
  Tab,
  Spacebar,
  Minus_Underscore,
  Equal_Plus,
  OpenSquareBracket_OpenBrace,
  CloseSquareBracket_CloseBrace,
  Backslash_Pipe,
  Hash_Tilde,
  SemiColon_Colon,
  SingleQuota_DoubleQuote,
  GraveAcent_Tilde,
  Comma_LessThan,
  Period_GreaterThan,
  Slash_QuestionMark,
  CapsLock,
  F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
  PrintScreen,
  ScrollLock,
  Pause,
  Insert,
  Home,
  PageUp,
  DeleteForward,
  End,
  PageDown,
  RightArrow,
  LeftArrow,
  DownArrow,
  UpArrow
};

} } // namespace keyboard::usb

#endif // ifndef keyboard_usb_KeyCode_h
