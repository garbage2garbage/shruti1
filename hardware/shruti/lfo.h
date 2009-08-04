// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// LFO (cheap oscillator).

#ifndef HARDWARE_SHRUTI_LFO_H_
#define HARDWARE_SHRUTI_LFO_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/parameters.h"
#include "hardware/utils/random.h"

using hardware_utils::Random;

namespace hardware_shruti {

template<int id>
class Lfo {
 public:
  static inline uint8_t Render() {
    uint8_t value;
    switch (algorithm_) {
      case LFO_WAVEFORM_S_H:
        if (uint16_t(phase_ + phase_increment_) < phase_) {
          value_ = Random::Byte();
        }
        value = value_;
        break;
      case LFO_WAVEFORM_TRIANGLE:
        value = (phase_ & 0X8000) ? phase_ >> 7 : ~uint8_t(phase_ >> 7);
        break;
      case LFO_WAVEFORM_SQUARE:
        value = (phase_ & 0x8000) ? 255 : 0;
        break;
      case LFO_WAVEFORM_SAW:
        value = phase_ >> 8;
        break;
    }
    return value;
  }
  static inline void Reset() {
    ResetPhase();
    phase_ = 0;
  }
  static inline void Increment() {
    phase_ += phase_increment_;
  }
  static inline void Update(uint8_t algorithm, uint16_t increment) {
    algorithm_ = algorithm;
    phase_increment_ = increment;
  }
  static inline void ResetPhase() { phase_ = 0; }

 private:
  // Phase increment.
  static uint16_t phase_increment_;

  // Current phase of the lfo.
  static uint16_t phase_;
  
  // Copy of the algorithm used by this oscillator.
  static uint8_t algorithm_;

  // Current value of S&H.
  static uint8_t value_;
  
  DISALLOW_COPY_AND_ASSIGN(Lfo);
};

template<int id> uint16_t Lfo<id>::phase_;
template<int id> uint16_t Lfo<id>::phase_increment_;
template<int id> uint8_t Lfo<id>::algorithm_;
template<int id> uint8_t Lfo<id>::value_;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_OSCILLATOR_H_
