// Copyright 2009 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
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
// -----------------------------------------------------------------------------
//
// Basic ATMega328p initialization.

#ifndef HARDWARE_HAL_INIT_ATMEGA_H_
#define HARDWARE_HAL_INIT_ATMEGA_H_

#include "hardware/hal/adc.h"
#include "hardware/hal/hal.h"
#include "hardware/hal/time.h"
#include "hardware/hal/timer.h"

IORegister(UCSR0B);

namespace hardware_hal {

inline void InitAtmega(bool init_timers) {
  sei();
  
  if (init_timers) {
    Timer<1>::set_prescaler(3);
    Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);

    Timer<2>::set_prescaler(3);
    Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  }
  
  InitClock();
  
  // ADC setup.
  Adc::set_prescaler(7);  // 128 -> 125kHz sampling rate.
  Adc::Enable();
  
  // Neuter the UART.
  UCSR0B = 0;
}

}  // hardware_hal

#endif  // HARDWARE_HAL_INIT_ATMEGA_H_
