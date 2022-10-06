#include "OutputSignal.h"

hardware::OutputSignal::OutputSignal(Action a, Action d)
  : activate(a)
  , deactivate(d) {
}
