// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Fast SPI communication (using the hardware implementation). This will take
// ownership of the pins 11 (data output),  12 (data input) and 13 (clock), +
// a user-definable pin for slave selection.
//
// This is a fairly basic implementation:
// - everything is buffered, no option for a "Write it now I can wait" mode.
// - the arduino is always configured as a master.
// - no support for input.
//
// This implementation supports basic concurrency. You may have two "instances"
// of this class with different slaves. In this case, writes to a slave will be
// buffered if the other slave is in use.

#ifndef HARDWARE_IO_SPI_H_
#define HARDWARE_IO_SPI_H_

#include "hardware/io/io.h"
#include "hardware/io/pin.h"

namespace hardware_io {

const uint8_t kSpiOutputBufferSize = 8;
const uint8_t kSpiDataOutPin = 11;
const uint8_t kSpiDataInPin = 12;
const uint8_t kSpiClockPin = 13;

IORegister(SPDR);
IORegister(SPCR);
IORegister(SPSR);
typedef BitInRegister<SPSRRegister, SPI2X> DoubleSpeed;
typedef BitInRegister<SPSRRegister, SPIF> TransferComplete;

// Those callbacks pointers are used in the interrupt handler.
extern void (*end_of_transmission_cback)();
extern int16_t (*buffer_read_cback)();
// Flag to check whether a transmission is already in progress.
extern volatile uint8_t transmission_in_progress;

template<uint8_t slave_select_pin = 10,
         DataOrder order = MSB_FIRST,
         uint8_t speed = 4>
class Spi {
 public:
  enum {
    buffer_size = kSpiOutputBufferSize,
    data_size = 8
  };
  typedef uint8_t Value;  
  typedef Spi<slave_select_pin, order, speed> Me;
  typedef Buffer<Me> OutputBuffer;
  
  static void Init() {
    Clock::set_mode(DIGITAL_OUTPUT);
    DataIn::set_mode(DIGITAL_INPUT);
    DataOut::set_mode(DIGITAL_OUTPUT);
    SlaveSelect::set_mode(DIGITAL_OUTPUT);
    
    EndTransmission();
    
    uint8_t configuration = _BV(SPIE) | _BV(SPE) | _BV(MSTR);
    if (order == LSB_FIRST) {
      configuration |= _BV(DORD);
    }
    switch (speed) {
      case 2:
        DoubleSpeed::set();
      case 4:
        break;
      case 8:
        DoubleSpeed::set();
      case 16:
        configuration |= _BV(SPR0);
        break;
      case 32:
        DoubleSpeed::set();
      case 64:
        configuration |= _BV(SPR1);
        break;
      case 128:
        configuration |= _BV(SPR0);
        configuration |= _BV(SPR1);
        break;
    }
    *SPCRRegister::ptr() = configuration;
  }
  
  static inline void Write(uint8_t v) { while (!writable()); Overwrite(v); }
  static inline void Overwrite(uint8_t v) { OutputBuffer::Overwrite(v); }
  static inline uint8_t writable() { return OutputBuffer::writable(); }
  static inline uint8_t NonBlockingWrite(uint8_t v) {
    if (!writable()) {
      return 0;
    }
    Overwrite(v);
    return 1;
  }
  static uint8_t NonBlockingTransmit() {
    if (transmission_in_progress || !OutputBuffer::readable()) {
      return 0;
    }
    ImmediateTransmit();
    return 1;
  }
  
  static uint8_t ImmediateTransmit() {
    transmission_in_progress = 1;
    end_of_transmission_cback = &EndTransmission;
    buffer_read_cback = &OutputBuffer::NonBlockingRead;
    uint8_t value = OutputBuffer::ImmediateRead();
    SlaveSelect::Low();
    *SPDRRegister::ptr() = value;
  }
  
 private:
  typedef Pin<slave_select_pin> SlaveSelect;
  typedef Pin<kSpiDataOutPin> DataOut;
  typedef Pin<kSpiDataInPin> DataIn;
  typedef Pin<kSpiClockPin> Clock;
  
  static void EndTransmission() {
    SlaveSelect::High();
    end_of_transmission_cback = NULL;
    buffer_read_cback = NULL;
    transmission_in_progress = 0;
  }
};

}  // namespace hardware_io

#endif HARDWARE_IO_SPI_H_
