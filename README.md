USB CW interface for Nucleo G491RE devboard
===========================================

Lets you connect a CW keyer to your computer via USB.

The devboard will show up as a USB keyboard and MIDI interface and it is compatible with the [Vail Adapter](https://github.com/Vail-CW/vail-adapter/).

If `USB_KEYBOARD_ENABLED` is set to 1 in `src/config.h`, the devboard will send the keyer input as keyboard events:

| Key          | Keyboard event |
|--------------|----------------|
| Straight key | Space          |
| Dit          | Left CTRL      |
| Dah          | Right CTRL     |

If `USB_MIDI_ENABLED` is set to 1, it will send the keyer input as MIDI events:

| Key          | MIDI note |
|--------------|-----------|
| Straight key | Note C    |
| Dit          | Note C#   |
| Dah          | Note D    |

# Building

Built and tested on Linux.

1. Install openocd: `sudo dnf install openocd`
2. Run `01-configure.sh`
3. Run `02-build.sh`
4. Run `03-flash.sh` to flash the firmware to the devboard

Configuration (ex. used pins) can be changed in `src/config.h`

# Pinouts

![Devboard image](board.svg)

Connect your CW keyer to the following pins on the Nucleo G491RE devboard:

| Name | ST morpho connector / pin number | Function     |
|------|----------------------------------|--------------|
| PA0  | CN7 / 28                         | Dit          |
| PA1  | CN7 / 30                         | Dah          |
| PA2  | CN7 / 32                         | Straight key |

Connect a USB cable to the following pins:

| Name  | ST morpho connector / pin number  | Function     | Wire color |
|-------|-----------------------------------|--------------|------------|
| PA12  | CN10 / 12                         | USB Data +   | Green      |
| PA11  | CN10 / 14                         | USB Data -   | White      |
| E5V   | CN7 / 6                           | USB VBUS     | Red        |
| GND   | CN7 / 8                           | USB GND      | Black      |

Change JP5 to E5V.

Optionally short JP1 to keep the ST-Link in reset state to reduce power consumption.
You can use the jumper from CN12 to do this.
