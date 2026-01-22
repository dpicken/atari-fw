#include "App.h"

#include "Pipe.h"

#include "media/Atr.h"
#include "sd/Controller.h"

sio::App::App(
    ::hal::InputSignal command,
    ::hal::Uart uart,
    ::hal::InputSignal sdDetect,
    ::hal::OutputSignal sdPower,
    ::hal::OutputSignal sdCs,
    ::hal::Spi sdSpi,
    ::hal::BusyWait busyWait,
    ::hal::BusyWaitEq busyWaitEq)
  : m_d1(uart, busyWait)
  , m_atariControl(uart, busyWait)
  , m_fileSystem(uart, busyWait, &m_d1)
  , m_controller(command, uart, busyWaitEq, &m_d1, &m_atariControl, &m_fileSystem)
  , m_sdController(sdDetect, sdPower, sdCs, sdSpi, busyWait) {
}

void sio::App::schedule() {
  pollPipe();
  m_sdController.poll();
  m_controller.poll();
}

void sio::App::pollPipe() {
  switch (Pipe::instance().tryPop()) {
    case Pipe::Message::Null:
      break;

    case Pipe::Message::LoadSbcFiler:
      m_fileSystem.loadSbcFiler();
      break;

    case Pipe::Message::EjectD1:
      m_d1.eject();
      break;
  }
}
