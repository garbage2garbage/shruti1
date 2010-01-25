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
// Bootloader compatible with STK500 and MIDI SysEx.
//
// Caveat: assumes the firmware flashing is always done from first to last
// block, in increasing order. Random access flashing is not supported!

#include <avr/boot.h>

#include "hardware/hal/devices/shift_register.h"
#include "hardware/hal/gpio.h"
#include "hardware/hal/serial.h"
#include "hardware/shruti/display.h"

using namespace hardware_hal;
using namespace hardware_shruti;

ShiftRegister<
    Gpio<kPinInputLatch>,
    Gpio<kPinClk>,
    Gpio<kPinData>, 8, MSB_FIRST> input_mux;
    
ShiftRegister<
    Gpio<kPinOutputLatch>,
    Gpio<kPinClk>,
    Gpio<kPinData>, 12, MSB_FIRST> leds;

DigitalInput<kPinDigitalInput> switch_input;

Serial<SerialPort0, 9600, POLLED, POLLED> serial;


const char id_response[] = { 0x14, 'A', 'V', 'R', ' ', 'I', 'S', 'P', 0x10 };
const char u_response[] = { 0x14, 0x1e, 0x095, 0x0f, 0x10 };

const uint8_t kNeverUsedResponseByte = 0xfa;
const uint8_t kMaxErrorCount = 5;
const uint8_t kVersion[] = { 0x02, 0x01, 0x10 };

uint16_t page = 0;
uint16_t base_led_mask = _BV(10);
uint8_t eeprom;
uint8_t rx_buffer[256];
uint8_t i;
uint8_t error_count = 0;

union AddressUnion {
  uint16_t word;
  uint8_t  byte[2];
} address;

union LengthUnion {
  uint16_t word;
  uint8_t  byte[2];
} length;


#define FAIL if (++error_count == kMaxErrorCount) app_start();

int main (void) __attribute__ ((naked,section (".init9")));
void (*app_start)(void) = 0x0000;

void Init() {
  cli();
  
  // Enable pull-up resistor on RX pin.
  DigitalInput<0>::EnablePullUpResistor();
  
  // Setup pin direction for shift registers (Shruti-1 only), and setup
  // multiplexer to monitor the 4th button.
  input_mux.Init();
  leds.Init();
  
  input_mux.Write(4);
  switch_input.EnablePullUpResistor();
  switch_input.Init();
}

// Shruti-1 only stuff: uses the 12 LEDs of the main board to display
// status information during firmware upload.
void SetStatus(uint8_t num_pages_written, uint8_t error) {
  uint8_t mask = 1;
  uint16_t leds_mask = base_led_mask;
  for (uint8_t i = 0; i < num_pages_written; ++i) {
    leds_mask |= mask;
    mask <<= 1;
  }
  if (error) {
    leds_mask |= _BV(11);
  }
  leds.Write(leds_mask);
}

void PutChar(char ch) {
  serial.Write(ch);
}

char GetCharOrDie() {
  uint32_t count;
  while (!serial.readable()) {
    ++count;
    /*if (count > (F_CPU >> 4)) {
      app_start();
    }*/
  }
  return serial.ImmediateRead();
}

char GetChar() {
  return serial.Read();
}

void SkipChars(uint8_t count) {
  while (count--) {
    GetCharOrDie();
  }
}

void Respond(uint8_t value) {
  if (GetCharOrDie() == ' ') {
    PutChar(0x14);
    if (value != kNeverUsedResponseByte) {
      PutChar(value);
    }
    PutChar(0x10);
  } else {
    FAIL;
  }
}

void RespondBuffer(const char* buffer, uint8_t size) {
  while (size--) {
    PutChar(*buffer++);
  }
}

void WriteBufferToFlash() {
  return;  /* REMOVE ME */
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

void DecodeBufferSpecs() {
  length.byte[1] = GetCharOrDie();
  length.byte[0] = GetCharOrDie();
  eeprom = GetCharOrDie() == 'E' ? 1 : 0;
  if (eeprom) {
    SetStatus(1 + (address.word >> 7), 0);
  } else {
    SetStatus(1 + (address.word >> 12), 0);
  }
}

void MidiLoop() {
  serial.Init(31250);
  base_led_mask = _BV(10) | _BV(8);
  SetStatus(0, 0);
  while(1);
}

void StkLoop() {
  serial.Init(57600);
  base_led_mask = _BV(10) | _BV(9);
  SetStatus(0, 0);
  
  uint8_t ch, ch2;
  uint16_t w;

  for (;;) {
    ch = GetCharOrDie();

    if (ch == '0') {
      Respond(kNeverUsedResponseByte);
    } else if (ch == '1') {
      if (GetCharOrDie() == ' ') {
        RespondBuffer(id_response, sizeof(id_response));
      } else {
        FAIL
      }
    } else if (ch == '@') {
      ch2 = GetCharOrDie();
      if (ch2 > 0x85) {
        GetCharOrDie();
      }
      Respond(kNeverUsedResponseByte);
    } else if (ch == 'A') {
      ch2 = GetCharOrDie();
      if (ch2 == 0x80) Respond(kVersion[0]);
      else if (ch2 == 0x81) Respond(kVersion[1]);
      else if (ch2 == 0x82) Respond(kVersion[2]);
      else if (ch2 == 0x98) Respond(0x03);
      else Respond(0x00);
    } else if (ch == 'B') {
      SkipChars(20);
      Respond(kNeverUsedResponseByte);
    } else if (ch == 'E') {
      SkipChars(5);
      Respond(kNeverUsedResponseByte);
    } else if (ch == 'P' || ch == 'R') {
      Respond(kNeverUsedResponseByte);
    } else if (ch == 'Q') {
      Respond(kNeverUsedResponseByte);
    } else if (ch == 'U') {
      address.byte[0] = GetCharOrDie();
      address.byte[1] = GetCharOrDie();
      address.word <<= 1;
      Respond(kNeverUsedResponseByte);
    } else if (ch == 'V') {
      if (GetCharOrDie() == 0x30) {
        GetCharOrDie();
        ch = GetCharOrDie();
        GetCharOrDie();
        Respond(u_response[ch + 1]);
      } else {
        SkipChars(3);
        Respond(0x00);
      }
    } else if (ch == 'd') {
      DecodeBufferSpecs();
      for (w = 0; w < length.word; ++w) {
        rx_buffer[w] = GetCharOrDie();
      }
      if (GetCharOrDie() == ' ') {
        if (eeprom) {
          for (w = 0; w < length.word; ++w) {
            eeprom_write_byte((uint8_t*)address.word, rx_buffer[w] );
            /*while (EECR & (1 << EEPE));
            EEAR = (uint16_t)(void*)address.word;
            EEDR = rx_buffer[w];
            EECR |= (1 << EEMPE);
            EECR |= (1 << EEPE);*/
            ++address.word;
          }   
        } else {
          // Ignore address...
          WriteBufferToFlash();
          page += SPM_PAGESIZE;
        }
        PutChar(0x14);
        PutChar(0x10);
      } else {
        FAIL
      }
    } else if (ch == 't') {
      DecodeBufferSpecs();
      if (GetCharOrDie() == ' ') {
        PutChar(0x14);
        for (w = 0; w < length.word; ++w) {
          if (eeprom) {
            PutChar(eeprom_read_byte((uint8_t*)address.word));
            /*while (EECR & (1 << EEPE));
            EEAR = (uint16_t)(void *)address.word;
            EECR |= (1 << EERE);
            PutChar(EEDR);*/
          } else {
            PutChar(pgm_read_byte_near(address.word));
          }
          ++address.word;
        }
        PutChar(0x10);
      }
    } else if (ch == 'u') {
      if (GetCharOrDie() == ' ') {
        RespondBuffer(u_response, sizeof(u_response));
      } else {
        FAIL
      }
    } else if(ch=='v') {
      Respond(0x00);
    } else {
      FAIL
    }
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
  SetStatus(0, 0);
  
  if (/*!(watchdog_status & _BV(EXTRF)) ||*/ switch_input.Read() == LOW) {
    MidiLoop();
  } else {
    StkLoop();
  }
}
