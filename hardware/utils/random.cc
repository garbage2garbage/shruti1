// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Random number generator.

#include "hardware/utils/random.h"

namespace hardware_utils {

/* static */
uint16_t Random::rng_state_ = 0x21;

/* static */
uint16_t Random::Short() {
  // Galois LFSR with feedback polynomial = x^16 + x^14 + x^13 + x^11.
  // Period: 65535.
  rng_state_ = (rng_state_ >> 1) ^ (-(rng_state_ & 1) & 0xB400);    
  return rng_state_;
}

}  // namespace hardware_utils
