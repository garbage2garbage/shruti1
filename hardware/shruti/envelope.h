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
//
// -----------------------------------------------------------------------------
//
// Envelopes

#ifndef HARDWARE_SHRUTI_ENVELOPE_H_
#define HARDWARE_SHRUTI_ENVELOPE_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/patch.h"
#include "hardware/utils/op.h"

using namespace hardware_utils_op;

namespace hardware_shruti {

enum EnvelopeStage {
  ATTACK = 0,
  DECAY = 1,
  SUSTAIN = 2,
  RELEASE = 3,
  DEAD = 4,
  // Padding added so that the size of an envelope object is exactly 32 bytes.
  UNKNOWN = 5,
};


class Envelope {
 public:
  Envelope() { }
   
  void Init();
  
  uint8_t dead() { return stage_ == DEAD; }
  uint8_t stage() { return stage_; }
  int16_t value() { return value_; }

  void Trigger(uint8_t stage);

  void Update(uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release);

  void Render() {
    value_ += increment_;
    // This code makes the assumption that only the ATTACK stage has a positive
    // slope. This is true for the classical ADSR envelope. To support more
    // complex multistage envelopes, the correct code is:
    //
    // if ((increment_ > 0) ^ (value_ < target_)) {
    //
    // but the first test is more expensive on AVR...
    // TODO(pichenettes): to get a more exponential feel, divide the increment
    // by 2 mid-stage.
    if ((stage_ == ATTACK) ^ (value_ < target_)) {
      value_ = target_;
      ++stage_;
      Trigger(stage_);
    }
  }

 private:
  uint8_t release_;  // release time.
  uint8_t stage_;  // current envelope stage.
  int16_t increment_;  // envelope value increment.
  int16_t target_;  // target value (moves to next stage once reached).
  int16_t value_;  // envelope value, 0-16384.
  // Increment and target for each stage of the envelope.
  int16_t stage_increment_[UNKNOWN + 1];
  int16_t stage_target_[UNKNOWN + 1];
   
  static uint16_t ScaleEnvelopeIncrement(uint8_t time, uint8_t scale);
  
  DISALLOW_COPY_AND_ASSIGN(Envelope);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_OSCILLATOR_H_
