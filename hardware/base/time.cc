// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Real time clock. Based on the code in the arduino core library
// by David A. Mellis.

#include "hardware/io/timer.h"
#include "hardware/base/time.h"

using hardware_io::MutableTimer0;
using hardware_io::TIMER_FAST_PWM;

namespace hardware_base {

const uint32_t microseconds_per_timer0_overflow =
    (64 * 256) / (F_CPU / 1000000L);
const uint32_t milliseconds_increment =
    microseconds_per_timer0_overflow / 1000;

const uint32_t fractional_increment = (
    microseconds_per_timer0_overflow % 1000) >> 3;

const uint32_t fractional_max = 1000 >> 3;

volatile uint32_t timer0_milliseconds = 0;
static uint8_t timer0_fractional = 0;

TIMER_0_TICK {
  uint32_t m = timer0_milliseconds;
  uint8_t f = timer0_fractional;

  m += milliseconds_increment;
  f += fractional_increment;
  if (f >= fractional_max) {
    f -= fractional_max;
    m += 1;
  }

  timer0_fractional = f;
  timer0_milliseconds = m;
}

uint32_t Delay(uint32_t delay) {
  uint32_t t = milliseconds();
  while (milliseconds() - t < delay);
}

uint32_t milliseconds() {
  uint32_t m;
  uint8_t oldSREG = SREG;
  cli();
  m = timer0_milliseconds;
  SREG = oldSREG;
  return m;
}

void SetupClock() {
  MutableTimer0::set_prescaler(3);
  MutableTimer0::set_mode(TIMER_FAST_PWM);
  MutableTimer0::Start();
}

}  //namespace hardware_base
