// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Patch.

#include "hardware/shruti/parameters.h"
#include "hardware/shruti/patch.h"
#include "hardware/utils/signal.h"

using hardware_utils::Signal;

namespace hardware_shruti {

void Patch::Pack(uint8_t* patch_buffer) const {
  for (uint8_t i = 0; i < 28; ++i) {
    patch_buffer[i] = osc_algorithm[i];
  }
  for (uint8_t i = 0; i < kModulationMatrixSize; ++i) {
    patch_buffer[2 * i + 28] = modulation_matrix.modulation[i].source |
        Signal::ShiftLeft4(modulation_matrix.modulation[i].destination);
    patch_buffer[2 * i + 29] = modulation_matrix.modulation[i].amount;
  }
  for (uint8_t i = 0; i < 8; ++i) {
    patch_buffer[48 + i] = sequence[i];
  }
  for (uint8_t i = 0; i < kPatchNameSize; ++i) {
    patch_buffer[56 + i] = name[i];
  }
}

/* static */
uint8_t Patch::Check(const uint8_t* patch_buffer) {
  for (uint8_t i = 6; i < 28; ++i) {
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
  for (uint8_t i = 0; i < 28; ++i) {
    osc_algorithm[i] = patch_buffer[i];
  }
  for (uint8_t i = 0; i < kModulationMatrixSize; ++i) {
    modulation_matrix.modulation[i].source = patch_buffer[2 * i + 28] & 0xf;
    modulation_matrix.modulation[i].destination = Signal::ShiftRight4(
        patch_buffer[2 * i + 28]);
    modulation_matrix.modulation[i].amount = patch_buffer[2 * i + 29];
  }
  for (uint8_t i = 0; i < 8; ++i) {
    sequence[i] = patch_buffer[i + 48];
  }
  for (uint8_t i = 0; i < 8; ++i) {
    name[i] = patch_buffer[i + 56];
  }
}

}  // hardware_shruti
