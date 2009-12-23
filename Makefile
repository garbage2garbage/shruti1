# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# makefile for the Shruti-1 project.

ARDUINO_DIR  = /Applications/arduino-0016

TARGET       = shruti

PACKAGES     = hardware/base hardware/hal hardware/hal/devices hardware/midi hardware/utils hardware/shruti

EEPROM_DATA = hardware/shruti/data/patch_library.hex

ARDUINO_LIBS = 

MCU          = atmega328p
DMCU         = m328p
F_CPU        = 16000000
# ARDUINO_PORT = /dev/cu.usbserial-A6008iA6
ARDUINO_PORT = /dev/cu.usbserial-A6008hLO

NO_CORE      = 1

include arduino_project.mk

RES_PATH     = hardware/shruti/resources

resources:	$(wildcard $(RES_PATH)/*.py)
		python hardware/resources/arc.py hardware/shruti/resources/resources.py

build/$(TARGET).top_symbols:	build/$(TARGET).sym
		python hardware/tools/largest_symbols.py $< | c++filt -n > $@

fsize:	size
		cat firmware_size | awk '{ print $$1+$$2 }' | tail -n1 | figlet | cowsay -n -f moose

size_report:	build/$(TARGET).lss build/$(TARGET).top_symbols
