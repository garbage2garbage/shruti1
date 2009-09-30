// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Real time clock.

#ifndef HARDWARE_BASE_TIME_H_
#define HARDWARE_BASE_TIME_H_

#include "hardware/base/base.h"

namespace hardware_base {

uint32_t milliseconds();

uint32_t Delay(uint32_t delay);

void SetupClock();

}  // namespace hardware_base

#endif  // HARDWARE_BASE_TIME_H_
