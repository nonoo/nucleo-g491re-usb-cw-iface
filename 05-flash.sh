#!/bin/bash
openocd -f interface/stlink.cfg -f target/stm32g4x.cfg -c 'program build/nucleo-g491re-usb-cw-iface.elf verify reset exit'
