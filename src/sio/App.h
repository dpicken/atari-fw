#ifndef sio_App_h
#define sio_App_h

#include "AtariControl.h"
#include "Controller.h"
#include "DiskDrive.h"
#include "FileSystem.h"

#include "fs/DirectoryEnumerator.h"
#include "hal/InputSignal.h"
#include "hal/misc.h"
#include "hal/Spi.h"
#include "hal/Uart.h"
#include "sd/Controller.h"

namespace sio {

class App {
public:
  App(::hal::InputSignal command,
      ::hal::Uart uart,
      ::hal::InputSignal sdDetect,
      ::hal::OutputSignal sdPower,
      ::hal::OutputSignal sdCs,
      ::hal::Spi sdSpi,
      ::hal::BusyWait busyWait,
      ::hal::BusyWaitEq busyWaitEq);

  void schedule();

private:
  void pollPipe();

  ::fs::DirectoryEnumerator m_d1LibraryEnumerator;

  DiskDrive m_d1;
  AtariControl m_atariControl;
  FileSystem m_fileSystem;

  Controller m_controller;

  ::sd::Controller m_sdController;
};

} // namespace sio

#endif // ifndef sio_App_h
