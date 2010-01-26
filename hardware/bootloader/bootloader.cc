// Copyright 2009 Olivier Gillet.
//
// Adapted by Olivier Gillet (ol.gillet@gmail.com) from the original Arduino
// bootloader contributed by: D. Mellis, D. Cuartielles, Dr. Erik Lins,
// Karl Pitrich, Jason P. Kyle. Some of the makefile settings used here
// are from D. Brink's 1k bootloader.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Bootloader compatible with STK500 and MIDI SysEx, for ATMega328p.
//
// Caveat: assumes the firmware flashing is always done from first to last
// block, in increasing order. Random access flashing is not supported!

#include <avr/boot.h>
#include <avr/pgmspace.h>

#include "hardware/bootloader/shruti1_status_leds.h"
#include "hardware/hal/devices/shift_register.h"
#include "hardware/hal/gpio.h"
#include "hardware/hal/serial.h"
#include "hardware/shruti/shruti.h"

using namespace hardware_hal;
using namespace hardware_shruti;

ShiftRegister<
    Gpio<kPinInputLatch>,
    Gpio<kPinClk>,
    Gpio<kPinData>, 8, MSB_FIRST> input_mux;
DigitalInput<kPinDigitalInput> switch_input;

Shruti1StatusIndicator status_leds;

Serial<SerialPort0, 9600, POLLED, POLLED> serial;


const uint8_t kProgrammerId[] = { 0x14, 'A', 'V', 'R', ' ', 'I', 'S', 'P', 0x10 };
const uint8_t kSignature[] = { 0x14, 0x1e, 0x095, 0x0f, 0x10 };
const uint8_t kVersion[] = { 0x02, 0x01, 0x10 };

const uint8_t kNeverUsedResponseByte = 0xfa;
const uint8_t kMaxErrorCount = 5;


uint16_t page = 0;
uint8_t eeprom;
uint8_t rx_buffer[257];
uint8_t i;
uint8_t num_failures = 0;

Word address;
Word length;

#define FAIL if (++num_failures == kMaxErrorCount) { StartApp(); }

int main (void) __attribute__ ((naked,section (".init9")));
void (*main_entry_point)(void) = 0x0000;

void StartApp() {
  status_leds.Clear();
  main_entry_point();
}

void Init() {
  cli();

  // Enable pull-up resistor on RX pin.
  DigitalInput<0>::EnablePullUpResistor();

  // Setup pin direction for shift registers (Shruti-1 only), and setup
  // multiplexer to monitor the 4th button.
  input_mux.Init();
  input_mux.Write(4);
  switch_input.EnablePullUpResistor();
  switch_input.Init();

  status_leds.Init();
}

void Write(uint8_t value) {
  serial.Write(value);
}

uint8_t ReadOrTimeout() {
  uint32_t count = 0;
  while (!serial.readable()) {
    if (count > (F_CPU >> 4)) {
      StartApp();
    }
    count++;
  }
  return serial.ImmediateRead();
}

uint8_t Read() {
  return serial.Read();
}

void SkipInput(uint8_t count) {
  while (count--) {
    ReadOrTimeout();
  }
}

void StkWriteByte(uint8_t value) {
  if (ReadOrTimeout() == ' ') {
    Write(0x14);
    if (value != kNeverUsedResponseByte) {
      Write(value);
    }
    Write(0x10);
  } else {
    FAIL;
  }
}

void WriteBuffer(const uint8_t* buffer, uint8_t size) {
  while (size--) {
    Write(*buffer++);
  }
}

void WriteBufferToFlash() {
  uint16_t i;
  const uint8_t* p = rx_buffer;
	eeprom_busy_wait();

	boot_page_erase(page);
	boot_spm_busy_wait();

	for (i = 0; i < SPM_PAGESIZE; i += 2) {
		uint16_t w = *p++;
		w |= (*p++) << 8;
		boot_page_fill(page + i, w);
	}

	boot_page_write(page);
	boot_spm_busy_wait();
	boot_rww_enable();
}

void ReadRegionSpecs() {
  length.bytes[1] = ReadOrTimeout();
  length.bytes[0] = ReadOrTimeout();
  eeprom = ReadOrTimeout() == 'E' ? 1 : 0;
  status_leds.Flash();
  if (eeprom) {
    status_leds.SetProgress(1 + (address.value >> 7));
  } else {
    status_leds.SetProgress(1 + (address.value >> 12));
  }
}

static const uint8_t sysex_header[] = {
  0xf0,  // <SysEx>
  0x00, 0x20, 0x77,  // TODO(pichenettes): register manufacturer ID.
  0x00, 0x01,  // Product ID for Shruti-1.
};

enum SysExReceptionState {
  MATCHING_HEADER = 0,
  READING_COMMAND = 1,
  READING_DATA = 2,
};

void MidiLoop() {
  uint8_t byte;
  uint16_t bytes_read = 0;
  uint8_t rx_buffer_index;
  uint8_t state = MATCHING_HEADER;
  uint8_t checksum;
  uint8_t sysex_commands[2];

  serial.Init(31250);
  status_leds.set_reception_mode_mask(1);
  status_leds.WaitForData();
  page = 0;
  for (;;) {
    byte = Read();
    // In case we see a realtime message in the stream, safely ignore it.
    if (byte > 0xf0 && byte != 0xf7) {
      continue;
    }

    switch (state) {
      case MATCHING_HEADER:
        if (byte == sysex_header[bytes_read]) {
          ++bytes_read;
          if (bytes_read == sizeof(sysex_header)) {
            bytes_read = 0;
            state = READING_COMMAND;
          }
        } else {
          status_leds.ReportError();
          bytes_read = 0;
        }
        break;

      case READING_COMMAND:
        if (byte < 0x80) {
          status_leds.ClearError();
          sysex_commands[bytes_read++] = byte;
          if (bytes_read == 2) {
            bytes_read = 0;
            rx_buffer_index = 0;
            checksum = 0;
            state = READING_DATA;
          }
        } else {
          status_leds.ReportError();
          state = MATCHING_HEADER;
          bytes_read = 0;
        }
        break;

      case READING_DATA:
        if (byte < 0x80) {
          if (bytes_read & 1) {
            rx_buffer[rx_buffer_index] |= byte & 0xf;
            if (rx_buffer_index < SPM_PAGESIZE) {
              checksum += rx_buffer[rx_buffer_index];
            }
            ++rx_buffer_index;
          } else {
            rx_buffer[rx_buffer_index] = (byte << 4);
          }
          ++bytes_read;
        } else if (byte == 0xf7) {
          if (sysex_commands[0] == 0x7f &&
              sysex_commands[1] == 0x00 &&
              bytes_read == 0) {
            // Reset.
            StartApp();
          } else if (rx_buffer_index == SPM_PAGESIZE + 1 &&
                     sysex_commands[0] == 0x7e &&
                     sysex_commands[1] == 0x00 &&
                     rx_buffer[rx_buffer_index - 1] == checksum) {
            // Block write.
            WriteBufferToFlash();
            page += SPM_PAGESIZE;
            status_leds.SetProgress(1 + (page >> 12));
            status_leds.Flash();
            state = MATCHING_HEADER;
            bytes_read = 0;
          } else {
            status_leds.ReportError();
            state = MATCHING_HEADER;
            bytes_read = 0;
          }
        }
        break;
    }
  }
}

void StkLoop() {
  uint8_t byte;

  serial.Init(57600);
  status_leds.set_reception_mode_mask(2);
  page = 0;

  for (;;) {
    byte = ReadOrTimeout();
    switch (byte) {
      case '0':
        StkWriteByte(kNeverUsedResponseByte);
        break;

      case '1':
        if (ReadOrTimeout() == ' ') {
          WriteBuffer(kProgrammerId, sizeof(kProgrammerId));
        } else {
          FAIL;
        }
        break;

      case '@':
        byte = ReadOrTimeout();
        if (byte > 0x85) {
          ReadOrTimeout();
        }
        StkWriteByte(kNeverUsedResponseByte);
        break;

      case 'A':
        byte = ReadOrTimeout();
        if (byte == 0x80) {
          StkWriteByte(kVersion[0]);
        } else if (byte == 0x81) {
          StkWriteByte(kVersion[1]);
        } else if (byte == 0x82) {
          StkWriteByte(kVersion[2]);
        } else if (byte == 0x98) {
          StkWriteByte(0x03);
        } else {
          StkWriteByte(0x00);
        }
        break;

      case 'B':
        SkipInput(20);
        StkWriteByte(kNeverUsedResponseByte);
        break;

      case 'E':
        SkipInput(5);
        StkWriteByte(kNeverUsedResponseByte);
        break;

      case 'P':
      case 'Q':
      case 'R':
        StkWriteByte(kNeverUsedResponseByte);
        break;

      case 'U':
        address.bytes[0] = ReadOrTimeout();
        address.bytes[1] = ReadOrTimeout();
        address.value <<= 1;
        StkWriteByte(kNeverUsedResponseByte);
        break;

      case 'V':
        if (ReadOrTimeout() == 0x30) {
          ReadOrTimeout();
          byte = ReadOrTimeout();
          ReadOrTimeout();
          StkWriteByte(kSignature[byte + 1]);
        } else {
          SkipInput(3);
          StkWriteByte(0x00);
        }
        break;

      case 'd':
        ReadRegionSpecs();
        for (uint16_t i = 0; i < length.value; ++i) {
          rx_buffer[i] = ReadOrTimeout();
        }
        if (ReadOrTimeout() == ' ') {
          if (eeprom) {
            for (uint16_t i = 0; i < length.value; ++i) {
              eeprom_write_byte((uint8_t*)address.value, rx_buffer[i]);
              ++address.value;
            }
          } else {
            // Ignore address...
            WriteBufferToFlash();
            page += SPM_PAGESIZE;
          }
          Write(0x14);
          Write(0x10);
        } else {
          FAIL;
        }
        break;

      case 't':
        status_leds.Flash();
        ReadRegionSpecs();
        if (ReadOrTimeout() == ' ') {
          Write(0x14);
          for (uint16_t i = 0; i < length.value; ++i) {
            if (eeprom) {
              Write(eeprom_read_byte((uint8_t*)address.value));
            } else {
              Write(pgm_read_byte_near(address.value));
            }
            ++address.value;
          }
          Write(0x10);
        }
        break;

      case 'u':
        if (ReadOrTimeout() == ' ') {
          WriteBuffer(kSignature, sizeof(kSignature));
        } else {
          FAIL;
        }
        break;

      case 'v':
        StkWriteByte(0x00);
        break;

      default:
        FAIL;
    }
    status_leds.WaitForData();
  }
}

int main(void) {
  uint8_t watchdog_status;
  uint8_t enter_sysex_mode;

  // Initialize watchdog timer and check whether we should enter the SysEx
  // loop because of a firmware-initiated reset.
  watchdog_status = MCUSR;
  MCUSR = 0;
  WDTCSR |= _BV(WDCE) | _BV(WDE);
  WDTCSR = 0;

  Init();
  if ((watchdog_status & _BV(WDRF)) || switch_input.Read() == LOW) {
    MidiLoop();
  } else {
    StkLoop();
  }
}
