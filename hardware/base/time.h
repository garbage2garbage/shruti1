// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Initialization function for arduino-based projects.

#ifndef HARDWARE_BASE_TIME_H_
#define HARDWARE_BASE_TIME_H_

namespace hardware_base {

unsigned long milliseconds();

void SetupClock();

}  // namespace hardware_base;

#endif  // HARDWARE_BASE_TIME_H_
