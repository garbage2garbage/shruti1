// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Driver for a MCP492x DAC (SPI single/dual 12-bits DAC).

#ifndef HARDWARE_IO_DEVICES_MCP492X_H_
#define HARDWARE_IO_DEVICES_MCP492X_H_

#include "hardware/io/spi.h"
#include "hardware/utils/op.h"

using hardware_utils::Op;

namespace hardware_io {

static const uint8_t kDacSpeed = 2;

enum DacVoltageReference {
  BUFFERED_REFERENCE,
  UNBUFFERED_REFERENCE
};

template<uint8_t slave_select_pin,
         DacVoltageReference voltage_reference = UNBUFFERED_REFERENCE,
         uint8_t gain = 1>
class Dac {
 public:
  enum {
    buffer_size = 0,
    data_size = 8,
  };
  Dac() { }
  
  static void Init() {
    DacInterface::Init();
  }
  
  static void Write(uint8_t value) {
    Write(value, 0);
  }

  static void Write(uint8_t value, uint8_t channel) {
    value = Swap4(value);
    uint8_t command;
    command = (value & 0x0f) | 0x10;
    if (channel) {
      command |= 0x80;
    }
    if (voltage_reference == BUFFERED) {
      command |= 0x40;
    }
    if (gain == 1) {
      command |= 0x20;
    }
    DacInterface::Write(command, value & 0xf0);
  }

 private:
  typedef Spi<slave_select_pin, MSB_FIRST, kDacSpeed> DacInterface;
};

}  // namespace hardware_io

#endif   // HARDWARE_IO_DEVICES_MCP492X_H_
