#ifndef hal_OutputSignal_h
#define hal_OutputSignal_h

namespace hal {

struct OutputSignal {
  typedef void (* const Action)();

  constexpr OutputSignal(Action a, Action d)
    : activate(a)
    , deactivate(d) {
  }

  const Action activate;
  const Action deactivate;
};

} // namespace hal

#endif // ifndef hal_OutputSignal_h
