// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Utility functions for string processing.

#include "hardware/utils/string.h"

#include <stdio.h>

namespace hardware_utils {

size_t strnlen(const char* string, size_t maxlen) {
  const char* end = (char*)memchr(string, '\0', maxlen);
  return end ? (size_t) (end - string) : maxlen;  
}

void AlignRight(char* source, uint8_t width) {
  uint8_t len = strnlen(source, width);
  if (len == width) {
    return;
  }
  char* destination = source + width - 1;
  for (uint8_t i = 0; i < width; ++i) {
    if (i < len) {
      *destination-- = source[len - i - 1];
    } else {
      *destination-- = ' ';
    }
  }
}

void AlignLeft(char* source, uint8_t width) {
  uint8_t len = strnlen(source, width);
  while (len < width) {
    source[len++] = ' ';
  }
}

}  // namespace hardware_utils
