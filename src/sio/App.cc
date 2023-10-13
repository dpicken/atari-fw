#include "App.h"

#include "Pipe.h"

#include "media/DiskLibrary.h"

sio::App::App(
    ::hal::InputSignal command,
    ::hal::Uart uart,
    ::hal::BusyWait busyWait,
    ::hal::BusyWaitEq busyWaitEq)
  : m_d1(uart, busyWait)
  , m_controller(command, uart, busyWaitEq, &m_d1) {
}

void sio::App::schedule() {
  pollPipe();
  m_controller.poll();
}

void sio::App::pollPipe() {
  switch (Pipe::instance().tryPop()) {
    case Pipe::Message::Null:
      break;

    case Pipe::Message::EjectAll:
      ::media::DiskLibrary::instance().push(m_d1.eject());
      break;

    case Pipe::Message::D1_RotateDisk:
      ::media::DiskLibrary::instance().push(m_d1.eject());
      m_d1.insert(::media::DiskLibrary::instance().pop());
      break;
  }
}
