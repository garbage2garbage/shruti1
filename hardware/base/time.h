// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Real time clock.

#ifndef HARDWARE_BASE_TIME_H_
#define HARDWARE_BASE_TIME_H_

namespace hardware_base {

unsigned long milliseconds();

unsigned long Delay(unsigned long delay);

void SetupClock();

}  // namespace hardware_base;

#endif  // HARDWARE_BASE_TIME_H_
