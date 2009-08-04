// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Class responsible for patch persistence on EEPROM.

#include "hardware/shruti/patch_memory.h"

#ifdef __TEST__
#include <stdio.h>
#else
#include <avr/eeprom.h>
#endif

namespace hardware_shruti {

/* static */
void PatchMemory::Read(uint8_t slot, uint16_t size, uint8_t* serialized_patch) {
#ifdef __TEST__
  FILE* fp;
  char file_name[32];
  sprintf(file_name, "patch_%02d.dat", slot);
  fp = fopen(file_name, "rb");
  if (!fp) {
    for (int i = 0; i < size; ++i) {
      *serialized_patch++ = 63;
    }
  } else {
    fread(serialized_patch, 1, size, fp);
  }
  fclose(fp);
#else
  int16_t offset = slot * size;
  for (int16_t i = 0; i < size; ++i) {
    *serialized_patch++ = eeprom_read_byte((uint8_t*)(i + offset));
  }
#endif  // __TEST__
}

/* static */
void PatchMemory::Write(uint8_t slot, const uint8_t* serialized_patch,
                        uint16_t size) {
#ifdef __TEST__
  FILE* fp;
  char file_name[32];
  sprintf(file_name, "patch_%02d.dat", slot);
  fp = fopen(file_name, "wb");
  CHECK(fp);
  fwrite(serialized_patch, 1, size, fp);
  fclose(fp);
#else
  int16_t offset = slot * size;
  for (int16_t i = 0; i < size; ++i) {
    eeprom_write_byte((uint8_t*)(i + offset), serialized_patch[i]);
  }
#endif  // __TEST__
}

}  // namespace hardware_shruti
