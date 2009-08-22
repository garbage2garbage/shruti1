// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Real time clock.

#include "hardware/io/timer.h"
#include "hardware/base/time.h"

using hardware_io::MutableTimer0;
using hardware_io::TIMER_FAST_PWM;

namespace hardware_base {

const unsigned long microseconds_per_timer0_overflow =
    (64 * 256) / (F_CPU / 1000000L);
const unsigned long milliseconds_increment =
    microseconds_per_timer0_overflow / 1000;

const unsigned long fractional_increment = (
    microseconds_per_timer0_overflow % 1000) >> 3;

const unsigned long fractional_max = 1000 >> 3;

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_milliseconds = 0;
static unsigned char timer0_fractional = 0;

ISR(TIMER0_OVF_vect)
{
  // copy these to local variables so they can be stored in registers
  // (volatile variables must be read from memory on every access)
  unsigned long m = timer0_milliseconds;
  unsigned char f = timer0_fractional;

  m += milliseconds_increment;
  f += fractional_increment;
  if (f >= fractional_max) {
    f -= fractional_max;
    m += 1;
  }

  timer0_fractional = f;
  timer0_milliseconds = m;
  timer0_overflow_count++;
}

unsigned long milliseconds()
{
  unsigned long m;
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
