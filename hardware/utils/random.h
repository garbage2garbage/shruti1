// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Random number generator.

#ifndef HARDWARE_UTILS_RANDOM_H_
#define HARDWARE_UTILS_RANDOM_H_

#include "hardware/base/base.h"

namespace hardware_utils {

class Random {
 public:
  static uint16_t Short();
  static inline uint8_t Byte() {
    return uint8_t(Short() >> 8);
  }
 private:
  static uint16_t rng_state_;
  
  DISALLOW_COPY_AND_ASSIGN(Random);
};

}  // namespace hardware_utils

#endif  // HARDWARE_UTILS_RANDOM_H_
