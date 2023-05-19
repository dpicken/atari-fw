#include "Controller.h"

keyboard::console::Controller::Controller(
    ::hardware::OutputSignal start,
    ::hardware::OutputSignal select,
    ::hardware::OutputSignal option,
    ::hardware::OutputSignal reset,
    ::hardware::OutputSignal power,
    ::hardware::PowerOnSequence powerOnSequence)
  : m_start(start)
  , m_select(select)
  , m_option(option)
  , m_reset(reset)
  , m_power(power)
  , m_powerOnSequence(powerOnSequence)
  , m_powerLatched(false)
  , m_powerActive(false) {
}

template<keyboard::console::KeyBit keyBit>
void keyboard::console::Controller::processKeyBit(const KeyBitset keyBitset, const ::hardware::OutputSignal& signal) {
  if ((keyBitset & keyBit) != 0) {
    signal.activate();
  } else {
    signal.deactivate();
  }
}

template<>
void keyboard::console::Controller::processKeyBit<keyboard::console::KeyBit::Power>(const KeyBitset keyBitset, const ::hardware::OutputSignal& power) {
  if ((keyBitset & KeyBit::Power) != 0) {
    if (m_powerLatched) {
      return;
    }
    m_powerLatched = true;
    m_powerActive = !m_powerActive;
  } else {
    if (!m_powerLatched) {
      return;
    }
    m_powerLatched = false;
    if (m_powerActive) {
      m_powerOnSequence(power, m_reset);
    } else {
      power.deactivate();
    }
  }
}

void keyboard::console::Controller::receiveInputReport(const KeyBitset keyBitset) {
  processKeyBit<KeyBit::Start>(keyBitset, m_start);
  processKeyBit<KeyBit::Select>(keyBitset, m_select);
  processKeyBit<KeyBit::Option>(keyBitset, m_option);
  processKeyBit<KeyBit::Reset>(keyBitset, m_reset);
  processKeyBit<KeyBit::Power>(keyBitset, m_power);
}
