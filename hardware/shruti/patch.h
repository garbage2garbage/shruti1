// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Patch definition. The order of the fields must match the numbering of the
// parameters in parameters.h. Contains methods for packing/unpacking to a
// buffer of bytes (DISABLED because of generated code size).
// Includes code for checking whether a buffer looks like a reasonable patch.

#ifndef HARDWARE_SHRUTI_PATCH_H_
#define HARDWARE_SHRUTI_PATCH_H_

#include "hardware/base/base.h"

namespace hardware_shruti {

const uint8_t kPatchNameSize = 8;
const uint8_t kSerializedPatchSize = 64;
const uint8_t kModulationMatrixSize = 10;

struct Modulation {
  uint8_t source;
  uint8_t destination;
  uint8_t amount;
};

union ModulationMatrix {
  Modulation modulation[kModulationMatrixSize];
  uint8_t raw_modulation_data[kModulationMatrixSize * 3];
};

struct Patch {
  uint8_t keep_me_at_the_top;

  // Offset: 0-8
  uint8_t osc_algorithm[2];
  uint8_t osc_parameter[2];
  uint8_t osc_range[2];
  uint8_t osc_option[2];
  
  // Offset: 8-12
  uint8_t mix_balance;
  uint8_t mix_sub_osc;
  uint8_t mix_noise;
  uint8_t mix_sub_osc_algorithm;

  // Offset: 12-16
  uint8_t filter_cutoff;
  uint8_t filter_resonance;
  uint8_t filter_env;
  uint8_t filter_lfo;
  
  // Offset: 16-24
  uint8_t env_attack[2];
  uint8_t env_decay[2];
  uint8_t env_sustain[2];
  uint8_t env_release[2];
  
  // Offset: 24-28
  uint8_t lfo_wave[2];
  uint8_t lfo_rate[2];
  
  // Offset: 28-58
  ModulationMatrix modulation_matrix;
  
  // Offset: 58-62, not saved
  uint8_t arp_tempo;
  uint8_t arp_octave;
  uint8_t arp_pattern;
  uint8_t arp_swing;
  
  // Offset: 62-70
  uint8_t sequence[8];
  
  // Offset: 70-74, not saved
  uint8_t kbd_octave;
  uint8_t kbd_raga;
  uint8_t kbd_portamento;
  uint8_t kbd_midi_channel;

  // Offset: 74-82
  uint8_t name[kPatchNameSize];
  
  // Get the value of a step in the sequence.
  uint8_t sequence_step(uint8_t step) const {
    return (step & 1) ? sequence[step >> 1] << 4 : sequence[step >> 1] & 0xf0;
  }
  // Set the value of a step in the sequence.
  void set_sequence_step(uint8_t step, uint8_t value) {
    if (step & 1) {
      sequence[step >> 1] = (sequence[step >> 1] & 0xf0) | (value >> 4);
    } else {
      sequence[step >> 1] = (sequence[step >> 1] & 0x0f) | (value & 0xf0);
    }
  }

  void Pack(uint8_t* patch_buffer) const;
  static uint8_t Check(const uint8_t* patch_buffer);
  void Unpack(const uint8_t* patch_buffer);
  /*
  template<uint8_t n>
  void ShiftAndInsertInLsb(uint8_t* patch_buffer,
                           uint8_t c) const {
    for (int i = 0; i < n; ++i) {
      *patch_buffer <<= 1;
      *patch_buffer |= (c & 1);
      c >>= 1;
      ++patch_buffer;
    }
  }
  template<uint8_t n>
  uint8_t ReadLsb(const uint8_t* patch_buffer) {
    uint8_t c = 0;
    uint8_t mask = 1;
    for (int i = 0; i < n; ++i) {
      if (*patch_buffer & 1) {
        c |= mask;
      }
      mask <<= 1;
      ++patch_buffer;
    }
    return c;
  }*/
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_PATCH_H_
