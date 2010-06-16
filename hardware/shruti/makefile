# Copyright 2009 Olivier Gillet.
#
# Author: Olivier Gillet (ol.gillet@gmail.com)
# Parts of this file adapted from the Arduino command line tools Makefile
# M J Oldfield ; mellis, eighthave, oli.keller.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


AVR_TOOLS_PATH = /usr/local/CrossPack-AVR/bin
AVR_ETC_PATH   = /usr/local/CrossPack-AVR/etc

VERSION        = 0.58
TARGET         = shruti1
PACKAGES       = hardware/base hardware/hal hardware/hal/devices hardware/midi hardware/utils hardware/shruti
RESOURCES      = hardware/shruti/resources
BUILD_DIR      = build/$(TARGET)
EEPROM_DATA    = hardware/shruti/data/patch_library.hex
PATCH_LIBRARY  = hardware/shruti/data/patch_library.txt 

MCU            = atmega328p
DMCU           = m328p
F_CPU          = 16000000
# SERIAL_PORT  = /dev/cu.usbserial-A800crwF
SERIAL_PORT  = /dev/cu.usbserial-A6008iA6
# SERIAL_PORT    = /dev/cu.usbserial-A6008hLO

# ------------------------------------------------------------------------------

VPATH          = $(PACKAGES)
CC_FILES       = $(notdir $(wildcard $(patsubst %,%/*.cc,$(PACKAGES))))
AS_FILES       = $(notdir $(wildcard $(patsubst %,%/*.as,$(PACKAGES))))
OBJ_FILES      = $(CC_FILES:.cc=.o) $(AS_FILES:.S=.o)
OBJS           = $(patsubst %,$(BUILD_DIR)/%,$(OBJ_FILES))
DEPS           = $(OBJS:.o=.d)

TARGET_HEX     = $(BUILD_DIR)/$(TARGET).hex
TARGET_ELF     = $(BUILD_DIR)/$(TARGET).elf
TARGETS        = $(BUILD_DIR)/$(TARGET).*
DEP_FILE       = $(BUILD_DIR)/depends.mk

CC             = $(AVR_TOOLS_PATH)/avr-gcc
CXX            = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY        = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP        = $(AVR_TOOLS_PATH)/avr-objdump
AR             = $(AVR_TOOLS_PATH)/avr-ar
SIZE           = $(AVR_TOOLS_PATH)/avr-size
NM             = $(AVR_TOOLS_PATH)/avr-nm
AVRDUDE        = $(AVR_TOOLS_PATH)/avrdude
REMOVE         = rm -f
CAT            = cat

CPPFLAGS      = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I. \
			-g -Os -w -Wall \
			-ffunction-sections -fdata-sections
CXXFLAGS      = -fno-exceptions
ASFLAGS       = -mmcu=$(MCU) -I. -x assembler-with-cpp
LDFLAGS       = -mmcu=$(MCU) -lm -Wl,--gc-sections -Os

# ------------------------------------------------------------------------------
# Source compiling
# ------------------------------------------------------------------------------

$(BUILD_DIR)/%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(CC) -c $(CPPFLAGS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.d: %.cc
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(BUILD_DIR)/%.d: %.s
	$(CC) -MM $(CPPFLAGS) $(ASFLAGS) $< -MF $@ -MT $(@:.d=.o)


# ------------------------------------------------------------------------------
# Object file conversion
# ------------------------------------------------------------------------------

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BUILD_DIR)/%.eep: $(BUILD_DIR)/%.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
		--change-section-lma .eeprom=0 -O ihex $< $@

$(BUILD_DIR)/%.lss: $(BUILD_DIR)/%.elf
	$(OBJDUMP) -h -S $< > $@

$(BUILD_DIR)/%.sym: $(BUILD_DIR)/%.elf
	$(NM) -n $< > $@

# ------------------------------------------------------------------------------
# AVRDude
# ------------------------------------------------------------------------------

AVRDUDE_CONF     = $(AVR_ETC_PATH)/avrdude.conf
AVRDUDE_COM_OPTS = -V -p $(DMCU)
AVRDUDE_COM_OPTS += -C $(AVRDUDE_CONF)
AVRDUDE_SER_OPTS = -c stk500v1 -b 57600 -P $(SERIAL_PORT)
AVRDUDE_ISP_OPTS = -c avrispmkII -P usb

# ------------------------------------------------------------------------------
# Main targets
# ------------------------------------------------------------------------------

all: 		$(BUILD_DIR) $(TARGET_HEX)

$(BUILD_DIR):
		mkdir -p $(BUILD_DIR)

$(TARGET_ELF): 	$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $(OBJS) $(SYS_OBJS)

$(DEP_FILE):	$(BUILD_DIR) $(DEPS)
		cat $(DEPS) > $(DEP_FILE)

upload:		$(TARGET_HEX)
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(AVRDUDE_SER_OPTS) \
			-U flash:w:$(TARGET_HEX):i

ispload:		$(TARGET_HEX)
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(AVRDUDE_ISP_OPTS) \
			-U flash:w:$(TARGET_HEX):i

clean:
		$(REMOVE) $(OBJS) $(TARGETS) $(DEP_FILE) $(DEPS)

depends:	$(DEPS)
		cat $(DEPS) > $(DEP_FILE)

firmware_size:	$(TARGET_ELF)
		$(SIZE) $(TARGET_ELF) > firmware_size

$(BUILD_DIR)/$(TARGET).top_symbols:	$(TARGET_ELF)
		$(NM) $(TARGET_ELF) --size-sort -C -f bsd -r > $@

eeprom_backup:
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(AVRDUDE_SER_OPTS) \
			-U eeprom:r:$(EEPROM_DATA):i

eeprom_restore:
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(AVRDUDE_SER_OPTS) \
			-U eeprom:w:$(EEPROM_DATA):i

size:	firmware_size
		cat firmware_size | awk '{ print $$1+$$2 }' | tail -n1 | figlet | cowsay -n -f moose

size_report:	build/$(TARGET)/$(TARGET).lss build/$(TARGET)/$(TARGET).top_symbols

.PHONY:	all clean depends upload


# ------------------------------------------------------------------------------
# Resources
# ------------------------------------------------------------------------------

RESOURCE_COMPILER = hardware/tools/resources_compiler/resources_compiler.py

resources:	$(wildcard $(RESOURCES)/*.py) 
		python $(RESOURCE_COMPILER) $(RESOURCES)/resources.py


# ------------------------------------------------------------------------------
# Midi files for firmware update
# ------------------------------------------------------------------------------

HEX2SYSEX = hardware/tools/hex2sysex/hex2sysex.py

$(BUILD_DIR)/%.mid: $(BUILD_DIR)/%.hex
	$(HEX2SYSEX) -o $@ $<

midi: $(BUILD_DIR)/$(TARGET).mid


# ------------------------------------------------------------------------------
# Patch libraries
# ------------------------------------------------------------------------------

LIBRARIAN = hardware/tools/librarian/librarian.py

dump_patches: eeprom_backup
	$(LIBRARIAN) -d $(EEPROM_DATA) > $(PATCH_LIBRARY)

upload_patches:
	$(LIBRARIAN) -c $(PATCH_LIBRARY) > $(EEPROM_DATA)
	$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(AVRDUDE_SER_OPTS) \
		-U eeprom:w:$(EEPROM_DATA):i


# ------------------------------------------------------------------------------
# Publish a firmware version on the website
# ------------------------------------------------------------------------------

REMOTE_HOST = mutable-instruments.net
REMOTE_USER = shruti
REMOTE_PATH = public_html/static/firmware

publish: $(BUILD_DIR)/$(TARGET).mid $(BUILD_DIR)/$(TARGET).hex
	scp $(BUILD_DIR)/$(TARGET).mid $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_PATH)/$(TARGET)_$(VERSION).mid
		scp $(BUILD_DIR)/$(TARGET).hex $(REMOTE_USER)@$(REMOTE_HOST):$(REMOTE_PATH)//$(TARGET)_$(VERSION).hex


include $(DEP_FILE)
