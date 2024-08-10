#include "AtariControl.h"

#include "keyboard/Pipe.h"

sio::AtariControl::AtariControl(::hal::Uart uart, ::hal::BusyWait busyWait)
  : Device(uart, busyWait) {
}

void sio::AtariControl::handle(const Command* command) {
  switch (command->commandId) {
    case 0x00:
      handlePowerOff();
      break;

    case 0x01:
      handlePowerCycle();
      break;

    case 0x02:
      handleReset();
      break;

    default:
      commandNack();
  };
}

void sio::AtariControl::handlePowerOff() {
  commandAck();

  if (!keyboard::Pipe::instance().tryPush(keyboard::Pipe::Message::PowerOff)) {
    commandError();
  }

  commandComplete();
}

void sio::AtariControl::handlePowerCycle() {
  commandAck();

  if (!keyboard::Pipe::instance().tryPush(keyboard::Pipe::Message::PowerCycle)) {
    commandError();
  }

  commandComplete();
}

void sio::AtariControl::handleReset() {
  commandAck();

  if (!keyboard::Pipe::instance().tryPush(keyboard::Pipe::Message::Reset)) {
    commandError();
  }

  commandComplete();
}
