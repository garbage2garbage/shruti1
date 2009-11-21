// Copyright 2009 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Instance of the display class, configured for the Shruti-1 project.

#ifndef HARDWARE_SHRUTI_DISPLAY_H_
#define HARDWARE_SHRUTI_DISPLAY_H_

#include "hardware/base/base.h"
#include "hardware/hal/devices/sparkfun_ser_lcd.h"
#include "hardware/hal/gpio.h"
#include "hardware/shruti/shruti.h"

using hardware_hal::Display;
using hardware_hal::Gpio;

namespace hardware_shruti {

typedef Display<
    Gpio<kPinLcdTx>,
    kMainTimerRate,
    kDisplayBaudRate,
    kLcdWidth,
    kLcdHeight> SoftwareSerialDisplay;

extern SoftwareSerialDisplay display;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_DISPLAY_H_
