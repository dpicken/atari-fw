# atari-fw

Firmware for Atari 8-bit hardware projects, see [atari-hw](https://github.com/dpicken/atari-hw).

## Build firmware

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
mkdir ./third-party
git -C ./third-party clone https://github.com/raspberrypi/pico-sdk.git
git -C ./third-party clone https://github.com/raspberrypi/pico-examples.git
git -C ./third-party/pico-sdk checkout 2.0.0
git -C ./third-party/pico-sdk submodule update --init
git -C ./third-party/pico-examples checkout sdk-2.0.0
git clone https://github.com/dpicken/fab.git
git clone https://github.com/dpicken/atari-fw.git
cd atari-fw
make
```

## Install firmware

Note: Do not flash a board whilst it's installed (in a powered-on) Atari.

Connect the board to the build host, then boot it into it's bootloader (hold the boot button then press and release the reset button).  Next, copy the appropriate image to the `RPI-RP2` drive/mount (adjust the `RP_MOUNT` option as necessary):

  - [adafruit qtpy rp2040](https://www.adafruit.com/product/4900):
  ```
  RP_MOUNT=/Volumes/RPI-RP2 RP_BOARD=adafruit_qtpy_rp2040 make rp-install
  ```
  - [waveshare rp2040 zero](https://www.waveshare.com/rp2040-zero.htm):
  ```
  RP_MOUNT=/Volumes/RPI-RP2 RP_BOARD=waveshare_rp2040_zero make rp-install
  ```
  - [pimoroni tiny2350](https://shop.pimoroni.com/products/tiny-2350?variant=42092638699603):
  ```
  RP_MOUNT=/Volumes/RP2350 RP_BOARD=pimoroni_tiny2350 make rp-install
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
