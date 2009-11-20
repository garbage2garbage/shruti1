# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# makefile for the Shruti-1 project.

ARDUINO_DIR  = /Applications/arduino-0016

TARGET       = shruti

PACKAGES     = hardware/base hardware/io hardware/io/devices hardware/midi hardware/utils hardware/shruti

ARDUINO_LIBS = 

MCU          = atmega328p
DMCU         = m328p
F_CPU        = 16000000
ARDUINO_PORT = /dev/cu.usbserial-A6008iA6

NO_CORE      = 1

include arduino_project.mk

RES_PATH     = hardware/shruti/resources
resources:	$(wildcard $(RES_PATH)/*.py)
		python hardware/resources/arc.py hardware/shruti/resources/resources.py

build/$(TARGET).top_symbols:	build/$(TARGET).sym
		python hardware/tools/largest_symbols.py $< | c++filt -n > $@

size_report:	build/$(TARGET).lss build/$(TARGET).top_symbols
