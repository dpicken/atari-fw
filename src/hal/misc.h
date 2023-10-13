#ifndef hal_misc_h
#define hal_misc_h

#include "duration_us.h"
#include "InputSignal.h"
#include "OutputSignal.h"

#include <cstdint>

namespace hal {

  typedef void(*PowerOnSequence)(const OutputSignal& power, const OutputSignal& reset);

  typedef bool(*BusyWaitEq)(const InputSignal& signal, bool value, duration_us timeoutDurationUs);

  typedef void(*BusyWait)(duration_us timeoutDurationUS);

} // namespace hal

#endif // ifndef hal_misc_h
