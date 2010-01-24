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
// Real time clock.

#ifndef HARDWARE_HAL_WATCHDOG_TIMER_
#define HARDWARE_HAL_WATCHDOG_TIMER_

#include "hardware/base/base.h"

namespace hardware_hal {

// TODO(pichenettes): implement more.

IORegister(WDTCSR);
BitInRegister<WDTCSRRegister, WDE> watchdog_system_reset;

// Note: this requires the bootloader to clear the Watchdog timer flags just
// after start-up.
inline void SystemReset() {
  // Assume that the watchdog timer is in its default state, ie all prescaler
  // bits to 0 (reset if not pat every 16ms), interrupts disabled.
  watchdog_system_reset.set();
  while(1);  // Will reset after 16ms
}

}  // namespace hardware_hal

#endif  // HARDWARE_HAL_WATCHDOG_TIMER_
