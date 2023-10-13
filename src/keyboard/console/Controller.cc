#include "Controller.h"

#include "sio/Pipe.h"

keyboard::console::Controller::Controller(
    ::hal::OutputSignal start,
    ::hal::OutputSignal select,
    ::hal::OutputSignal option,
    ::hal::OutputSignal reset,
    ::hal::OutputSignal power,
    ::hal::PowerOnSequence powerOnSequence)
  : m_start(start)
  , m_select(select)
  , m_option(option)
  , m_reset(reset)
  , m_power(power)
  , m_powerOnSequence(powerOnSequence)
  , m_powerActive(false) {
}

void keyboard::console::Controller::receiveInputReport(const KeyBitset keyBitset) {
  processKeyBit<KeyBit::Start>(keyBitset, m_start);
  processKeyBit<KeyBit::Select>(keyBitset, m_select);
  processKeyBit<KeyBit::Option>(keyBitset, m_option);
  processKeyBit<KeyBit::Reset>(keyBitset, m_reset);

  processKeyReleaseObserver<KeyBit::Power>(keyBitset, &m_powerKeyObserver, [this]() {
    togglePower();
  });
  processKeyReleaseObserver<KeyBit::Eject>(keyBitset, &m_ejectKeyObserver, []() {
    sio::Pipe::instance().tryPush(sio::Pipe::Message::EjectAll);
  });
  processKeyReleaseObserver<KeyBit::D1RotateDisk>(keyBitset, &m_d1RotateDiskKeyObserver, []() {
    sio::Pipe::instance().tryPush(sio::Pipe::Message::D1_RotateDisk);
  });
}

template<keyboard::console::KeyBit keyBit>
void keyboard::console::Controller::processKeyBit(const KeyBitset keyBitset, const ::hal::OutputSignal& signal) {
  if ((keyBitset & keyBit) != 0) {
    signal.activate();
  } else {
    signal.deactivate();
  }
}

template<keyboard::console::KeyBit keyBit>
void keyboard::console::Controller::processKeyReleaseObserver(const KeyBitset keyBitset, KeyReleaseObserver* observer, std::function<void()> action) {
  if ((keyBitset & keyBit) != 0) {
    observer->keyDown();
  } else {
    observer->keyUp();
  }

  if (observer->wasReleased()) {
    action();
    observer->reset();
  }
}

void keyboard::console::Controller::togglePower() {
  m_powerActive = !m_powerActive;
  if (m_powerActive) {
    m_powerOnSequence(m_power, m_reset);
  } else {
    m_power.deactivate();
  }
}
