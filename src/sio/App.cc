#include "App.h"

#include "Pipe.h"

#include "fs/builtin/FileSystem.h"
#include "fs/ResolvePath.h"
#include "keyboard/Pipe.h"
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
  : m_sbcBootBuiltin(::media::makeAtr(::fs::resolveWellKnownFile("!sbc-boot.atr")))
  , m_d1LibraryEnumerator(::fs::builtin::FileSystem::instance()->getRootDirectory())
  , m_d1(uart, busyWait)
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

    case Pipe::Message::SbcBoot:
      m_d1.insert(m_sbcBootBuiltin);
      break;

    case Pipe::Message::D1Eject:
      m_d1.eject();
      m_d1LibraryEnumerator.reset();
      break;

    case Pipe::Message::D1RotateDisk:
      m_d1.eject();
      if (!m_d1LibraryEnumerator.isValid()) {
        m_d1LibraryEnumerator.reset();
      }
      if (m_d1LibraryEnumerator.isValid()) {
        m_d1.insert(::media::makeAtr(m_d1LibraryEnumerator.openFile()));
        m_d1LibraryEnumerator.next();
      }
      break;
  }
}
