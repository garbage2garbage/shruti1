// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Patch.

#include "hardware/shruti/parameters.h"
#include "hardware/shruti/patch.h"

namespace hardware_shruti {

void Patch::Pack(uint8_t* patch_buffer) const {
  /*patch_buffer[0] = (osc_algorithm[0] << 4) | osc_algorithm[1];
  patch_buffer[1] = (osc_option[1] << 1) | osc_option[0];
  patch_buffer[2] = (lfo_wave[0] << 4) | lfo_wave[1];
  patch_buffer[3] = ((mix_sub_osc_algorithm - WAVEFORM_TRIANGLE) << 7) | \
                    mix_sub_osc;
  
  patch_buffer[4] = osc_parameter[0];
  patch_buffer[5] = osc_parameter[1];
  patch_buffer[6] = osc_range[0] - 80;
  patch_buffer[7] = osc_range[1] - 80;
  patch_buffer[8] = mix_balance;
  patch_buffer[9] = mix_noise;
  patch_buffer[10] = filter_cutoff;
  patch_buffer[11] = filter_resonance;
  
  patch_buffer[12] = filter_env;
  patch_buffer[13] = filter_lfo;
  patch_buffer[14] = env_attack;
  patch_buffer[15] = env_decay;  
  patch_buffer[16] = env_sustain;
  patch_buffer[17] = env_release;
  patch_buffer[18] = lfo_rate[0];
  patch_buffer[19] = lfo_rate[1];
  
  for (int i = 0; i < 8; ++i) {
    patch_buffer[20 + i] = modulation_matrix.raw_modulation_data[3 * i + 2];
  }
  for (int i = 0; i < 8; ++i) {
    patch_buffer[28 + i] = modulation_matrix.raw_modulation_data[3 * i] << 4;
    patch_buffer[28 + i] |= modulation_matrix.raw_modulation_data[3 * i + 1];
  }
  for (int i = 0; i < 8; ++i) {
    patch_buffer[36 + i] = sequence[i];
  }
  
  ShiftAndInsertInLsb<8>(patch_buffer + 4, name[0]);
  ShiftAndInsertInLsb<8>(patch_buffer + 12, name[1]);
  ShiftAndInsertInLsb<8>(patch_buffer + 20, name[2]);
  patch_buffer[44] = name[3];
  patch_buffer[45] = name[4];
  patch_buffer[46] = name[5];
  patch_buffer[47] = name[6];
  patch_buffer[48] = name[7];*/
  for (uint8_t i = 0; i < 48; ++i) {
    patch_buffer[i] = osc_algorithm[i];
  }
  for (uint8_t i = 0; i < 8; ++i) {
    patch_buffer[i + 48] = osc_algorithm[i + 52];
  }
  for (uint8_t i = 0; i < 8; ++i) {
    patch_buffer[i + 56] = osc_algorithm[i + 64];
  }
}

/* static */
uint8_t Patch::Check(const uint8_t* patch_buffer) {
  for (uint8_t i = 6; i < 48; ++i) {
    if (patch_buffer[i] > 128) {
      return 0;
    }
  }
  for (uint8_t i = 56; i < 64; ++i) {
    if (patch_buffer[i] > 128) {
      return 0;
    }
  }
  return 1;
}

void Patch::Unpack(const uint8_t* patch_buffer) {
  /*osc_algorithm[0] = (patch_buffer[0] >> 4);
  osc_algorithm[1] = (patch_buffer[0] & 0xf);
  osc_option[0] = patch_buffer[1] & 1;
  osc_option[1] = patch_buffer[1] >> 1;
  lfo_wave[0] = patch_buffer[2] >> 4;
  lfo_wave[1] = patch_buffer[2] & 0xf;  
  mix_sub_osc_algorithm = (patch_buffer[3] >> 7) + WAVEFORM_TRIANGLE;
  mix_sub_osc = patch_buffer[3] & 0x7f;
  
  osc_parameter[0] = patch_buffer[4] >> 1;
  osc_parameter[1] = patch_buffer[5] >> 1;
  osc_range[0] = (patch_buffer[6] >> 1) + 80;
  osc_range[1] = (patch_buffer[7] >> 1) + 80;
  mix_balance = patch_buffer[8] >> 1;
  mix_noise = patch_buffer[9] >> 1;
  filter_cutoff = patch_buffer[10] >> 1;
  filter_resonance = patch_buffer[11] >> 1;
  name[0] = ReadLsb<8>(patch_buffer + 4);
  
  filter_env = patch_buffer[12] >> 1;
  filter_lfo = patch_buffer[13] >> 1;
  env_attack = patch_buffer[14] >> 1;
  env_decay = patch_buffer[15] >> 1;
  env_sustain = patch_buffer[16] >> 1;
  env_release = patch_buffer[17] >> 1;
  lfo_rate[0] = patch_buffer[18] >> 1;
  lfo_rate[1] = patch_buffer[19] >> 1;
  name[1] = ReadLsb<8>(patch_buffer + 12);
  
  for (int i = 0; i < 8; ++i) {
    modulation_matrix.raw_modulation_data[3 * i + 2] = (
        patch_buffer[20 + i] >> 1);
  }
  for (int i = 0; i < 8; ++i) {
    modulation_matrix.raw_modulation_data[3 * i + 1] = (
        patch_buffer[28 + i] & 0xf);
    modulation_matrix.raw_modulation_data[3 * i] = patch_buffer[28 + i] >> 4;
  }
  name[2] = ReadLsb<8>(patch_buffer + 20);
  for (int i = 0; i < 8; ++i) {
    sequence[i] = patch_buffer[36 + i];
  }
  name[3] = patch_buffer[44];
  name[4] = patch_buffer[45];
  name[5] = patch_buffer[46];
  name[6] = patch_buffer[47];
  name[7] = patch_buffer[48];*/
  for (uint8_t i = 0; i < 48; ++i) {
    osc_algorithm[i] = patch_buffer[i];
  }
  for (uint8_t i = 0; i < 8; ++i) {
    osc_algorithm[i + 52] = patch_buffer[i + 48];
  }
  for (uint8_t i = 0; i < 8; ++i) {
    osc_algorithm[i + 64] = patch_buffer[i + 56];
  }
}

}  // hardware_shruti
