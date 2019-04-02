// Copyright 2009 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
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

#include "hardware/hal/init_atmega.h"
#include "hardware/hal/gpio.h"
#include "hardware/hal/time.h"

using namespace hardware_hal;

Gpio<13> led;

int main(void) {
  InitAtmega(false);
  led.set_mode(DIGITAL_OUTPUT);
  while (1) {
    led.High();
    Delay(250);
    led.Low();
    Delay(250);
  }
}
