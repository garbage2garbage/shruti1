// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Display.

#ifndef HARDWARE_SHRUTI_DISPLAY_H_
#define HARDWARE_SHRUTI_DISPLAY_H_

#include "hardware/base/base.h"
#include "hardware/io/devices/sparkfun_ser_lcd.h"
#include "hardware/io/pin.h"
#include "hardware/shruti/shruti.h"

using hardware_io::Display;
using hardware_io::Pin;

namespace hardware_shruti {

typedef Display<Pin<kPinLcdTx>,
    kMainTimerRate,
    kDisplayBaudRate,
    kLcdWidth,
    kLcdHeight> SoftwareSerialDisplay;

extern SoftwareSerialDisplay display;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_DISPLAY_H_
