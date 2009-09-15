// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)

#ifndef HARDWARE_SHRUTI_ENVELOPE_H_
#define HARDWARE_SHRUTI_ENVELOPE_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/parameters.h"
#include "hardware/shruti/resources.h"
#include "hardware/utils/signal.h"

using hardware_utils::Signal;

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
    if ((stage_ == ATTACK) ^ (value_ < target_)) {
      value_ = target_;
      ++stage_;
      Trigger(stage_);
    }
  }

 private:
  uint8_t release_;
  uint8_t stage_;
  int16_t increment_;
  int16_t target_;
  int16_t value_;
  int16_t stage_increment_[UNKNOWN + 1];
  int16_t stage_target_[UNKNOWN + 1];
   
  static uint16_t ScaleEnvelopeIncrement(uint8_t time, uint8_t scale);
  
  DISALLOW_COPY_AND_ASSIGN(Envelope);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_OSCILLATOR_H_
