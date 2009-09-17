// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)

#ifndef HARDWARE_SHRUTI_ENVELOPE_H_
#define HARDWARE_SHRUTI_ENVELOPE_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/patch.h"
#include "hardware/shruti/resources.h"
#include "hardware/utils/op.h"

using hardware_utils::Op;

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
    uint16_t value = value_;
    value_ += increment_;
    // This code makes the assumption that only the ATTACK stage has a positive
    // slope. This is true for the classical ADSR envelope. To support more
    // complex multistage envelopes, the correct code is:
    //
    // if ((increment_ > 0) ^ (value_ < target_)) {
    //
    // but the first test is more expensive on AVR...
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
