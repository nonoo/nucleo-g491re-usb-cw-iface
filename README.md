USB CW interface for Nucleo G491RE devboard
===========================================

Lets you connect a CW keyer to your computer via USB.

The devboard will show up as a USB keyboard and MIDI interface and it is compatible with [Vail Adapter](https://github.com/Vail-CW/vail-adapter/).

# Building

Built and tested on Linux.

1. Install openocd: `sudo dnf install openocd`
2. Run `01-configure.sh`
3. Run `02-build.sh`
4. Run `03-flash.sh` to flash the firmware to the devboard

Configuration (ex. used pins) can be changed in `src/config.h`

# Pinouts

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
