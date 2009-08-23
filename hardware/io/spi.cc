// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Fast SPI communication (using the hardware implementation).

#include "hardware/io/spi.h"

#ifndef __TEST__
#include <avr/interrupt.h>

namespace hardware_io {

/* extern */
void (*end_of_transmission_cback)();

/* extern */
int16_t (*buffer_read_cback)();

/* extern */
volatile uint8_t transmission_in_progress;

}  // namespace hardware_io;

using namespace hardware_io;

ISR(SPI_STC_vect) {
  // Attempt to get the next byte from the transmission buffer.
  int16_t value = buffer_read_cback();
  if (value < 0) {
    // If there is nothing more to transmit, disable the slave.
    end_of_transmission_cback();
  } else {
    // Otherwise, write the next byte.
    *SPDRRegister::ptr() = uint8_t(value);
  }
}

#endif // !__TEST__
