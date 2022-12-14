#ifndef hardware_OutputSignal_h
#define hardware_OutputSignal_h

namespace hardware {

struct OutputSignal {
  typedef void (* const Action)();

  OutputSignal(Action a, Action d);

  const Action activate;
  const Action deactivate;
};

} // namespace hardware

#endif // ifndef hardware_OutputSignal_h
