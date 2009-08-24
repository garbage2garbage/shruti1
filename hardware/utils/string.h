// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Utility functions for string processing.

#ifndef HARDWARE_UTILS_STRING_UTILS_H_
#define HARDWARE_UTILS_STRING_UTILS_H_

#include "hardware/base/base.h"
#include <string.h>

namespace hardware_utils {

size_t strnlen(const char* string, size_t maxlen);

void AlignRight(char* source, uint8_t width);
void AlignLeft(char* source, uint8_t width);

template<typename T>
struct TypeInfo {
  enum {
    has_sign = 0,
    max_size = 5
  };
};

template<> struct TypeInfo<uint8_t> { enum { has_sign = 0, max_size = 3 }; };
template<> struct TypeInfo<int8_t> { enum { has_sign = 1, max_size = 4 }; };
template<> struct TypeInfo<uint16_t> { enum { has_sign = 0, max_size = 5 }; };
template<> struct TypeInfo<int16_t> { enum { has_sign = 1, max_size = 6 }; };
template<> struct TypeInfo<uint32_t> { enum { has_sign = 0, max_size = 10 }; };
template<> struct TypeInfo<int32_t> { enum { has_sign = 1, max_size = 11 }; };

template<typename T>
void Itoa(T i, uint8_t width, char* destination) {
  unsigned char digits[TypeInfo<T>::max_size + 1];
  if (width == 0) {
    return;
  }
  if (i == 0) {
    *destination++ = '0';
    width--;
  } else {
    if (TypeInfo<T>::has_sign && i < 0) {
      *destination++ = '-';
      width--;
      i = -i;
    }
    uint8_t digit = 0;
    while (i > 0) {
      digits[digit++] = i % 10;
      i /= 10;
    }
    while (digit) {
      *destination++ = 48 + digits[--digit];
      width--;
    }
  }
  if (width) {
    *destination++ = '\0';
  }
}

}  // namespace hardware_string

#endif  // HARDWARE_UTILS_STRING_UTILS_H_
