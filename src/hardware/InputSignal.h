#ifndef hardware_InputSignal_h
#define hardware_InputSignal_h

namespace hardware {

struct InputSignal {
  typedef bool (* const Action)();

  InputSignal(Action ia);

  const Action isActive;
};

} // namespace hardware

#endif // ifndef hardware_InputSignal_h
