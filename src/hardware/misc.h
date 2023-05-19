#ifndef hardware_misc_h
#define hardware_misc_h

#include "InputSignal.h"
#include "OutputSignal.h"

#include <cstdint>

namespace hardware {

  typedef void(*PowerOnSequence)(const ::hardware::OutputSignal& power, const ::hardware::OutputSignal& reset);

  typedef bool(*BusyWaitEq)(const ::hardware::InputSignal& signal, bool value, std::uint64_t timeoutDurationUs);

} // namespace hardware

#endif // ifndef hardware_misc_h
