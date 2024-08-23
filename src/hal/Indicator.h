#ifndef hal_Indicator_h
#define hal_Indicator_h

#include <cstdint>

namespace hal {

struct Indicator {
  typedef void (* const SetRGB)(std::uint8_t r, std::uint8_t g, std::uint8_t b);

  constexpr Indicator(SetRGB action)
    : setRGB(action) {
  }

  const SetRGB setRGB;
};

} // namespace hal

#endif // ifndef hal_Indicator_h
