// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Interface to the onboard analog->digital converter, and analog multiplexer.

#include "hardware/io/adc.h"

namespace hardware_io {

/* static */
uint8_t Adc::reference_ = ADC_DEFAULT << 6;

}  // namespace hardware_io
