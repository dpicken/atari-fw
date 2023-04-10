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

Note: Do not flash the QT PY whilst it's installed in an Atari.

Connect the QT PY to the build host, then hold and release both its buttons to boot it into its UF2 bootloader.  Copy `./build_qtpy/atari-fw_qtpy.uf2` to the QT PY drive, or have the build system do it for you:

On macOS:

```
make qtpy_install
```

On other platforms:

```
QTPY_MOUNT=<qtpy-mount-path> make qtpy_install
```
