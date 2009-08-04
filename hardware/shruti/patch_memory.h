// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Class responsible for patch persistence on EEPROM.

#ifndef HARDWARE_SHRUTI_PATCH_MEMORY_H_
#define HARDWARE_SHRUTI_PATCH_MEMORY_H_

#include "hardware/base/base.h"

namespace hardware_shruti {

struct PatchMemory {
  static void Read(uint8_t slot, uint16_t size, uint8_t* serialized_patch);
  static void Write(uint8_t slot, const uint8_t* serialized_patch,
                    uint16_t size);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_PATCH_MEMORY_H_
