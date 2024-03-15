# atari-fw

USB keyboard controller firmware for 8-bit Atari systems.

## Hardware

See [atari-hw](https://github.com/dpicken/atari-hw).

## Building firmware

Install the build tools, e.g. on macOS via brew:

```
brew install cmake arm-none-eabi-gcc
```

To include a set of ATR images in the build (for the [SFF SD card adapter](https://github.com/dpicken/atari-hw/blob/main/doc/sbc-sd.md)):

```
mkdir ./atari-atr
cp ~/Downloads/*.atr ./atari-atr/
```

Clone and build:

```
git clone https://github.com/raspberrypi/pico-sdk.git
git clone https://github.com/raspberrypi/pico-examples.git
git -C pico-sdk submodule update --init
git -C pico-sdk checkout 1.5.1
git -C pico-sdk submodule update
git -C pico-examples git checkout sdk-1.5.1
git clone https://github.com/dpicken/fab.git
git clone https://github.com/dpicken/atari-fw.git
cd atari-fw
make
```

## Install firmware

Note: Do not flash an RP2040 board whilst it's installed (in a powered-on) Atari.

Connect the board to the build host, then boot it into it's bootloader (hold the boot button then press and release the reset button).  Next, copy the appropriate image to the `RPI-RP2` drive/mount (adjust the `RP2040_MOUNT` option as necessary):

  - [adafruit qtpy](https://www.adafruit.com/product/4900):
  ```
  RP2040_MOUNT=/Volumes/RPI-RP2 RP2040_BOARD=adafruit_qtpy_rp2040 make rp2040-install
  ```
  - [waveshare rp2040 zero](https://www.waveshare.com/rp2040-zero.htm):
  ```
  RP2040_MOUNT=/Volumes/RPI-RP2 RP2040_BOARD=waveshare_rp2040_zero make rp2040-install
  ```

## Default keymap

From [src/keyboard/Controller.cc](/src/keyboard/Controller.cc):

| USB Key   | Atari Key     |
|-----------|---------------|
| F1        | Help          |
| F2        | Start         |
| F3        | Select        |
| F4        | Option        |
| F10       | D1RotateDisk  |
| F11       | Eject         |
| F12       | Reset         |
| Ctrl-F12  | Power         |
