// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Initialization function for arduino-based projects. Based on the original
// initialization code by David A. Mellis (wiring.c) in the Arduino core
// libraries.

#include "hardware/base/init_arduino.h"

#include "hardware/base/time.h"
#include "hardware/io/adc.h"
#include "hardware/io/io.h"
#include "hardware/io/timer.h"

using hardware_io::Adc;
using hardware_io::Timer;
using hardware_io::MutableTimer0;
using hardware_io::TIMER_FAST_PWM;
using hardware_io::TIMER_PWM_PHASE_CORRECT;

IORegister(UCSR0B);

namespace hardware_base {

void InitArduino() {
  sei();
  
  SetupClock();
  
  // Setup the timers to run at 1kHz.
  Timer<1>::set_prescaler(3);
  Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);

  Timer<2>::set_prescaler(3);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);

  // ADC setup.
  Adc::set_prescale_factor(7);  // 128 -> 125kHz sampling rate.
  Adc::Enable();
  
  // Neuter the UART.
  *UCSR0BRegister::ptr() = 0;
}

}  // namespace hardware_base;
