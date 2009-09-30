// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Fast 16-bit pseudo random number generator.

#ifndef HARDWARE_UTILS_RANDOM_H_
#define HARDWARE_UTILS_RANDOM_H_

#include "hardware/base/base.h"

namespace hardware_utils {

class Random {
 public:
  static void Update() {
    // Galois LFSR with feedback polynomial = x^16 + x^14 + x^13 + x^11.
    // Period: 65535.
    rng_state_ = (rng_state_ >> 1) ^ (-(rng_state_ & 1) & 0xB400);    
  }

  static inline uint16_t state() { return rng_state_; }

  static inline uint8_t state_msb() {
    return static_cast<uint8_t>(rng_state_ >> 8);
  }

  static inline uint8_t GetByte() {
    Update();
    return state_msb();
  }

 private:
  static uint16_t rng_state_;
  
  DISALLOW_COPY_AND_ASSIGN(Random);
};

}  // namespace hardware_utils

#endif  // HARDWARE_UTILS_RANDOM_H_
