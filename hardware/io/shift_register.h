// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Shift register

#ifndef HARDWARE_IO_SHIFT_REGISTER_H_
#define HARDWARE_IO_SHIFT_REGISTER_H_

#include "hardware/io/pin.h"
#include "hardware/io/size_to_type.h"

namespace hardware_io {

template<typename Latch, typename Clock, typename Data>
struct BaseShiftRegister {
  static void Init() {
    Clock::set_mode(DIGITAL_OUTPUT);
    Latch::set_mode(DIGITAL_OUTPUT);
    Data::set_mode(DIGITAL_OUTPUT);
    Latch::High();
  }
};

template<typename Latch, typename Clock, typename Data,
         uint8_t size = 8, ShiftRegisterOrder order = LSB_FIRST>
struct ShiftRegister : public BaseShiftRegister<Latch, Clock, Data> {
};

template<typename Latch, typename Clock, typename Data,
         uint8_t size>
struct ShiftRegister<Latch, Clock, Data, size, LSB_FIRST>
  : public BaseShiftRegister<Latch, Clock, Data> {
  static void Write(typename DataTypeForSize<size>::Type data) {
    LOG(INFO) << "pin_" << Data::number() << "\tshift\t" << int(data);
    Latch::Low();
    Data::Low();
    for (uint8_t i = size; i > 0; --i) {
      Clock::Low();
      Data::set_value(data & 1);
      data >>= 1;
      Clock::High();
      Data::Low();
    }
    Clock::Low();
    Latch::High();
  }
};

template<typename Latch, typename Clock, typename Data, uint8_t size>
struct ShiftRegister<Latch, Clock, Data, size, MSB_FIRST>
  : public BaseShiftRegister<Latch, Clock, Data> {
  typedef typename DataTypeForSize<size>::Type T;
  static void Write(T data) {
    LOG(INFO) << "pin_" << Data::number() << "\tshift\t" << int(data);
    Latch::Low();
    Data::Low();
    T mask = (T(1) << (size - 1));
    for (uint8_t i = size; i > 0; --i) {
      Clock::Low();
      Data::set_value(data & mask ? HIGH : LOW);
      mask >>= 1;
      Clock::High();
      Data::Low();
    }
    Clock::Low();
    Latch::High();
  }
};

template<typename Latch, typename Clock, typename Data>
struct ShiftRegister<Latch, Clock, Data, 8, LSB_FIRST>
  : public BaseShiftRegister<Latch, Clock, Data> {
  static void Write(uint8_t data) {
    LOG(INFO) << "pin_" << Data::number() << "\tshift\t" << int(data);
    Latch::Low();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 1);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 2);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 4);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 8);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 16);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 32);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 64);
    Clock::High();
    Data::Low();

    Clock::Low();
    Data::set_value(data & 128);
    Clock::High();
    Data::Low();

    Clock::Low();
    Latch::High();
  }
};

}  // namespace hardware_io

#endif   // HARDWARE_IO_SHIFT_REGISTER_H_
