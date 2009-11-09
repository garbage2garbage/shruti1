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
// LFO (cheap oscillator).
//
// Contrary to oscillators which are templatized "static singletons", to
// generate the fastest, most specialized code, LFOs are less
// performance-sensitive and are thus implemented as a traditional class.

#ifndef HARDWARE_SHRUTI_LFO_H_
#define HARDWARE_SHRUTI_LFO_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/patch.h"
#include "hardware/utils/random.h"

using hardware_utils::Random;

namespace hardware_shruti {

class Lfo {
 public:
  Lfo() { }
  uint8_t Render() {
    switch (algorithm_) {
      case LFO_WAVEFORM_S_H:
        if (static_cast<uint16_t>(phase_ + phase_increment_) < phase_) {
          value_ = Random::GetByte();
        }
        return value_;
      case LFO_WAVEFORM_TRIANGLE:
        return (phase_ & 0X8000) ?
            phase_ >> 7 :
            ~static_cast<uint8_t>(phase_ >> 7);
      case LFO_WAVEFORM_SQUARE:
        return (phase_ & 0x8000) ? 255 : 0;
      default:
        return phase_ >> 8;
    }
  }
  void Reset() {
    ResetPhase();
    phase_ = 0;
  }
  void Increment() {
    phase_ += phase_increment_;
  }
  void Update(uint8_t algorithm, uint16_t increment) {
    algorithm_ = algorithm;
    phase_increment_ = increment;
  }
  void ResetPhase() { phase_ = 0; }

 private:
  // Phase increment.
  uint16_t phase_increment_;

  // Current phase of the lfo.
  uint16_t phase_;
  
  // Copy of the algorithm used by this oscillator.
  uint8_t algorithm_;

  // Current value of S&H.
  uint8_t value_;
  
  DISALLOW_COPY_AND_ASSIGN(Lfo);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_OSCILLATOR_H_
