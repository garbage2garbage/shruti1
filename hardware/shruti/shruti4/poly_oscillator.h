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

#ifndef HARDWARE_SHRUTI_SHRUTI4_POLY_OSCILLATOR_H_
#define HARDWARE_SHRUTI_SHRUTI4_POLY_OSCILLATOR_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/patch.h"
#include "hardware/shruti/shruti4/resources.h"
#include "hardware/utils/op.h"

using namespace hardware_utils_op;

namespace hardware_shruti {

#define WAV_RES_SINE WAV_RES_BANDLIMITED_SQUARE_6
#define HALF_SAMPLE_RATE if (engine.oscillator_decimation() & 1) return;

static const uint16_t kWavetableSize = 129;
static const uint16_t kNumWavetables = 6;

static const prog_char wavetable_index[WAVEFORM_VOICE_AAH + 1] PROGMEM = {
  0,
  0,
  WAV_RES_SINE,
  WAV_RES_BANDLIMITED_SQUARE_1,
  WAV_RES_BANDLIMITED_SAW_1,
  WAV_RES_BANDLIMITED_REED_1_1,
  WAV_RES_BANDLIMITED_REED_2_1,
  WAV_RES_BANDLIMITED_VOICE_1_1,
  WAV_RES_BANDLIMITED_VOICE_2_1,
};

static const prog_char wavetable_size[WAVEFORM_VOICE_AAH + 1] PROGMEM = {
  0,
  0,
  0,
  5,
  5,
  5,
  5,
  5,
  5,
};

struct AlgorithmFn {
  void (*update)();
  void (*render)();
};

struct OscillatorVoice {
  uint16_t phase;
  uint16_t phase_increment;
  uint16_t phase_increment_2;
  uint8_t note;
  const prog_uint8_t* wave[2];
  uint8_t balance;
};

class PolyOscillator {
 public:
  static uint8_t Render() {
    (*fn_.render)();
    return held_sample_;
  }

  static inline void SetupAlgorithm(uint8_t shape) {
    fn_ = fn_table_[shape];
    wavetable_index_ = ResourcesManager::Lookup<uint8_t, uint8_t>(
        wavetable_index,
        shape);
    wavetable_size_ = ResourcesManager::Lookup<uint8_t, uint8_t>(
        wavetable_size,
        shape);
  }
  
  static inline void UpdateVoicePitch(
      uint8_t index,
      uint8_t note,
      uint16_t increment) {
    voice_[index].phase_increment = increment;
    voice_[index].phase_increment_2 = increment << 1;
    voice_[index].note = note;
  }
  
  static inline void UpdateParameter(
      uint8_t parameter) {
    parameter_ = parameter;
    if (fn_.update) {
      (*fn_.update)();
    }
  }
  
 private:
  // Current phase of the oscillator.
  static OscillatorVoice voice_[kPolyphony];
  static uint8_t parameter_;
  static uint8_t wavetable_index_;
  static uint8_t wavetable_size_;
  
  
  // Sample generated in the previous full call.
  static uint8_t held_sample_;
  static uint8_t drawbars_wavetable_[kWavetableSize * kNumWavetables];
  
  static AlgorithmFn fn_;
  static AlgorithmFn fn_table_[];
  
  static inline uint8_t ReadSample(const prog_uint8_t* table, uint16_t phase) {
    return ResourcesManager::Lookup<uint8_t, uint8_t>(table, phase >> 8);
  }
#ifdef USE_OPTIMIZED_OP
  static inline uint8_t InterpolateSample(
      const prog_uint8_t* table,
      uint16_t phase) {
    uint8_t result;
    asm(
      "movw r30, %A1"           "\n\t"  // copy base address to r30:r31
      "add r30, %B2"            "\n\t"  // increment table address by phaseH
      "adc r31, r1"             "\n\t"  // just carry
      "lpm %0, z+"              "\n\t"  // load sample[n]
      "lpm r1, z+"              "\n\t"  // load sample[n+1]
      "mul %A2, r1"             "\n\t"  // multiply second sample by phaseL
      "movw r30, r0"            "\n\t"  // result to accumulator
      "com %A2"                 "\n\t"  // 255 - phaseL -> phaseL
      "mul %A2, %0"             "\n\t"  // multiply first sample by phaseL
      "com %A2"                 "\n\t"  // 255 - phaseL -> phaseL
      "add r30, r0"             "\n\t"  // accumulate L
      "adc r31, r1"             "\n\t"  // accumulate H
      "eor r1, r1"              "\n\t"  // reset r1 after multiplication
      "mov %0, r31"             "\n\t"  // use sum H as output
      : "=r" (result)
      : "a" (table), "a" (phase)
      : "r30", "r31"
    );
    return result;
  }
#else
  static inline uint8_t InterpolateSample(const prog_uint8_t* table,
                                          uint16_t phase) {
    return Mix(
        ResourcesManager::Lookup<uint8_t, uint8_t>(table, phase >> 8),
        ResourcesManager::Lookup<uint8_t, uint8_t>(table, 1 + (phase >> 8)),
        phase & 0xff);
  }
#endif  // USE_OPTIMIZED_OP

  static inline uint8_t InterpolateTwoTables(
      const prog_uint8_t* table_a, const prog_uint8_t* table_b,
      uint16_t phase, uint8_t balance) {
    return Mix(
        InterpolateSample(table_a, phase),
        InterpolateSample(table_b, phase),
        balance);
  }
  
  static void RenderSilence() {
    held_sample_ = 128;
  }
  
  static void UpdateDrawbars() {
    ++drawbars_wavetable_[0];
    held_sample_ = drawbars_wavetable_[0];
  }
  
  static void UpdateWavetable() {
    for (uint8_t i = 0; i < kPolyphony; ++i) {
      if (voice_[i].note != 0) {
        uint8_t note = Swap4(voice_[i].note - 12);
        uint8_t wave_index = AddClip(note & 0xf, 0, wavetable_size_);
        voice_[i].wave[0] = waveform_table[wavetable_index_ + wave_index];
        wave_index = AddClip(wave_index, 1, wavetable_size_);
        voice_[i].wave[1] = waveform_table[wavetable_index_ + wave_index];
        voice_[i].balance = note & 0xf0;
      }
    }
  }
  
  static void RenderWavetable() {
    HALF_SAMPLE_RATE;
    held_sample_ = 0;
    for (uint8_t i = 0; i < kPolyphony; ++i) {
      if (voice_[i].note != 0) {
        voice_[i].phase += voice_[i].phase_increment_2;
        held_sample_ += InterpolateTwoTables(
          voice_[i].wave[0], voice_[i].wave[1],
          voice_[i].phase, voice_[i].balance) >> 2;
        held_sample_ += parameter_ >> 2;
      } else {
        held_sample_ += 32;
      }
    }
  }
  
  static void RenderDrawbars() {
    held_sample_ = 128;
  }
  
  static void RenderDigitalSaw() {
    held_sample_ = 0;
    for (uint8_t i = 0; i < kPolyphony; ++i) {
      if (voice_[i].note != 0) {
        held_sample_ += (voice_[i].phase >> 10) + (parameter_ >> 3);
        voice_[i].phase += voice_[i].phase_increment;
      } else {
        held_sample_ += 32;
      }
    }
  }

  static void RenderDigitalSquare() {
    held_sample_ = 0;
    for (uint8_t i = 0; i < kPolyphony; ++i) {
      if (voice_[i].note != 0) {
        held_sample_ += (voice_[i].phase >> 8) < (128 + parameter_) ? 0 : 63;
        voice_[i].phase += voice_[i].phase_increment;
      } else {
        held_sample_ += 32;
      }
    }
  }
  
  DISALLOW_COPY_AND_ASSIGN(PolyOscillator);
};

AlgorithmFn PolyOscillator::fn_;
OscillatorVoice PolyOscillator::voice_[kPolyphony];
uint8_t PolyOscillator::parameter_;
uint8_t PolyOscillator::held_sample_;
uint8_t PolyOscillator::wavetable_index_;
uint8_t PolyOscillator::wavetable_size_;
uint8_t PolyOscillator::drawbars_wavetable_[kWavetableSize * kNumWavetables];
AlgorithmFn PolyOscillator::fn_table_[] = {
  { NULL, &PolyOscillator::RenderSilence },
  { &PolyOscillator::UpdateDrawbars, &PolyOscillator::RenderDrawbars },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { &PolyOscillator::UpdateWavetable, &PolyOscillator::RenderWavetable },
  { NULL, &PolyOscillator::RenderDigitalSaw },
  { NULL, &PolyOscillator::RenderDigitalSquare },
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_SHRUTI4_POLY_OSCILLATOR_H_
