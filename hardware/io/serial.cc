// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Fast serial (for the onboard UART), using compile time optimizations.

#include "hardware/io/serial.h"
#include "hardware/io/pin.h"

#ifndef __TEST__
#include <avr/interrupt.h>

using namespace hardware_io;

ISR(USART_RX_vect) {
  SerialInput<SerialPort0>::Received();
}

#endif // !__TEST__