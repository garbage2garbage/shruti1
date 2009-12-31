# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# makefile for the Shruti-1 project.

ARDUINO_DIR  = /Applications/arduino-0016

TARGET       = shruti4
PP_DEFINES   = -DSHRUTI4

PACKAGES     = hardware/base hardware/hal hardware/hal/devices hardware/midi hardware/utils hardware/shruti hardware/shruti/shruti4

EEPROM_DATA = hardware/shruti/data/shruti4_patch_library.hex

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
			python hardware/resources/resources_compiler.py hardware/shruti/resources/resources_shruti4.py

fsize:	size
		cat $(TARGET).firmware_size | awk '{ print $$1+$$2 }' | tail -n1 | figlet | cowsay -n -f moose

size_report:	build/$(TARGET)/$(TARGET).lss build/$(TARGET)/$(TARGET).top_symbols

