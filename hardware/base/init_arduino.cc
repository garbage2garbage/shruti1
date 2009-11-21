// Copyright 2009 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
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
// Basic Arduino initialization. Put the board in the same state as the original
// initialization code of the arduino core library by David A. Mellis
// (wiring.c).

#include "hardware/base/init_arduino.h"

#include "hardware/base/time.h"
#include "hardware/hal/adc.h"
#include "hardware/hal/hal.h"
#include "hardware/hal/timer.h"

using hardware_hal::Adc;
using hardware_hal::Timer;
using hardware_hal::MutableTimer0;
using hardware_hal::TIMER_FAST_PWM;
using hardware_hal::TIMER_PWM_PHASE_CORRECT;

IORegister(UCSR0B);

namespace hardware_base {

void InitArduino() {
  sei();
  
  SetupClock();
  
  // Setup the PWM timers to run at 1kHz.
  Timer<1>::set_prescaler(3);
  Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);

  Timer<2>::set_prescaler(3);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);

  // ADC setup.
  Adc::set_prescaler(7);  // 128 -> 125kHz sampling rate.
  Adc::Enable();
  
  // Neuter the UART.
  *UCSR0BRegister::ptr() = 0;
}

}  // namespace hardware_base
