#ifndef hal_InputSignal_h
#define hal_InputSignal_h

namespace hal {

struct InputSignal {
  typedef bool (* const Action)();

  constexpr InputSignal(Action ia)
    : isActive(ia) {
  }

  const Action isActive;
};

} // namespace hal

#endif // ifndef hal_InputSignal_h
