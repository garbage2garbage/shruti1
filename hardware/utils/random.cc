// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Random number generator.

#include "hardware/utils/random.h"

namespace hardware_utils {

/* static */
uint16_t Random::rng_state_ = 0x21;

}  // namespace hardware_utils
