#ifndef hal_misc_h
#define hal_misc_h

#include "InputSignal.h"
#include "OutputSignal.h"

#include <cstdint>

namespace hal {

  typedef void(*PowerOnSequence)(const OutputSignal& power, const OutputSignal& reset);

  typedef bool(*BusyWaitEq)(const InputSignal& signal, bool value, std::uint64_t timeoutDurationUs);

} // namespace hal

#endif // ifndef hal_misc_h
