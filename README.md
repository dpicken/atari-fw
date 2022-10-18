# atari-fw

USB keyboard controller firmware for 8-bit Atari systems.

## Hardware

See [atari-hw](https://github.com/dpicken/atari-hw).

## Building firmware

Install the build tools, e.g. on macOS via brew:

```
brew install cmake arm-none-eabi-gcc
```

Clone and build:

```
git clone https://github.com/raspberrypi/pico-sdk.git
git -C pico-sdk submodule update --init
git -C pico-sdk checkout 1.4.0
git clone https://github.com/dpicken/fab.git
git clone https://github.com/dpicken/atari-fw.git
cd atari-fw
make
```

## Install firmware: QT PY RP2040

Connect the QT PY to the build host and boot into UF2 bootloader:

- Disconnect the Atari's power supply (to ensure power is not backfed into the build host)
- Disconnect POKEY interposer's J2 5V supply (to ensure the build host does not power up the Atari)
- Connect the QT PY via its USB-C port to the build host
- Hold down BOOTSEL button and then reset the QT PY

Copy `./build_qtpy/atari-fw_qtpy.uf2` to the QT PY drive, or have the build system do it for you:

On macOS:

```
make qtpy_install
```

On other platforms:

```
QTPY_MOUNT=<qtpy-mount-path> make qtpy_install
```

Reconnect POKEY interposer's J2 5V supply.
