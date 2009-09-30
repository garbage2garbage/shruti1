// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Pretty printer. Wraps any module implementing the basic Output interface
// (in fact, just a Write method), and provide string and integer formatting
// using the << stream operator.

#ifndef HARDWARE_IO_PRETTY_PRINTER_H_
#define HARDWARE_IO_PRETTY_PRINTER_H_

#include "hardware/utils/string.h"

using hardware_utils::Itoa;
using hardware_utils::TypeInfo;

namespace hardware_io {

enum EndOfLine {
  endl = 0
};

template<typename Output>
struct PrettyPrinter {
  static void Print(const char* string) {
    while (*string) {
      Output::Write(*string++);
    }
  }
  static void Print(uint8_t byte) {
    Output::Write(byte);
  }
  static void Print(char byte) {
    Output::Write(byte);
  }
  static void Print(uint16_t value) {
    char buffer[TypeInfo<uint16_t>::max_size + 1];
    Itoa(value, TypeInfo<uint16_t>::max_size + 1, buffer);
    Print(buffer);
  }
  static void Print(int16_t value) {
    char buffer[TypeInfo<int16_t>::max_size + 1];
    Itoa(value, TypeInfo<int16_t>::max_size + 1, buffer);
    Print(buffer);
  }
  static void Print(uint32_t value) {
    char buffer[TypeInfo<uint32_t>::max_size + 1];
    Itoa(value, TypeInfo<uint32_t>::max_size + 1, buffer);
    Print(buffer);
  }
  static void Print(EndOfLine e) {
    Print('\r');
    Print('\n');
  }
  template<class T>
  inline PrettyPrinter<Output>& operator<<(const T value) {
    Print(value);
    return *this;
  }
};

}  // namespace hardware_io

#endif   // HARDWARE_IO_PRETTY_PRINTER_H_
