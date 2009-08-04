// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// ADC readouts.

#include "hardware/io/adc.h"

namespace hardware_io {

/* static */
uint8_t Adc::reference_ = ADC_DEFAULT << 6;

}  // namespace hardware_io
