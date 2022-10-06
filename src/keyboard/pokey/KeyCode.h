#ifndef keyboard_pokey_KeyCode_h
#define keyboard_pokey_KeyCode_h

#include "Traits.h"

namespace keyboard { namespace pokey {

enum class KeyCode : Traits::scan_code_value_type {
  L,
  J,
  SemiColon_Colon,
  F1,
  F2,
  K,
  Plus_Backslash,
  Star_Hat,
  O,
  Reserved0,
  P,
  U,
  Return,
  I,
  Minus_Underscore,
  Equals_Pipe,
  V,
  Help,
  C,
  F3,
  F4,
  B,
  X,
  Z,
  Four_Dollar,
  Reserved1,
  Three_Hash,
  Six_Ampersand,
  Escape,
  Five_Percent,
  Two_DoubleQuote,
  One_Pling,
  Comma_OpenSquareBracket,
  Space,
  Period_CloseSquareBracket,
  N,
  Reserved2,
  M,
  Slash_QuestionMark,
  Invert,
  R,
  Reserved3,
  E,
  Y,
  Tab,
  T,
  W,
  Q,
  Nine_OpenParenthesis,
  Reserved4,
  Zero_CloseParenthesis,
  Seven_SingleQuote,
  Backspace,
  Eight_At,
  LessThan_Clear,
  GreaterThan_Insert,
  F,
  H,
  D,
  Reserved5,
  CapsLock,
  G,
  S,
  A,

  NoEventIndicated = Traits::ScanCodeEnd
};

} } // namespace keyboard::pokey

#endif // ifndef keyboard_pokey_KeyCode_h
