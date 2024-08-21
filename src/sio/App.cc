#include "App.h"

#include "Pipe.h"

#include "media/DiskLibrary.h"
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

    case Pipe::Message::EjectAll:
      m_d1.eject();
      ::media::DiskLibrary::instance().reset();
      break;

    case Pipe::Message::D1_RotateDisk:
      ::media::DiskLibrary::instance().push(m_d1.eject());
      m_d1.insert(::media::DiskLibrary::instance().pop());
      break;
  }
}
