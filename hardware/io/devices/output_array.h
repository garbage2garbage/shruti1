// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Controller for an array of digital outputs behind a shift register. This
// class maintains an internal array reflecting the status of each output. A
// call to Out() writes the content of the array to a shift register, if the
// content of the array has changed.
//
// The num_intensity_level template parameter allows basic PWM on the output
// of the array. More specifically, each call to Out() will write a 0 or 1
// depending on the "analog" level. This can be used for example to adjust
// the brightness of a LED. More precisely, this is achieved in a PWM-like way
// (we cycle N times through the array and at time i, we power only the outputs
// with level above i/N).

#ifndef HARDWARE_IO_DEVICES_OUTPUT_ARRAY_H_
#define HARDWARE_IO_DEVICES_OUTPUT_ARRAY_H_

#include "hardware/io/devices/74hc595.h"
#include "hardware/io/size_to_type.h"

namespace hardware_io {

// A generic, not very efficient implementation, especially for small bit
// depths.
template<typename Latch, typename Clock, typename Data,
         uint8_t size = 16, uint8_t bit_depth = 1,
         DataOrder order = LSB_FIRST, bool safe = true>
class OutputArray {
 public:
  OutputArray() { }
  typedef ShiftRegister<Latch, Clock, Data, size, order> Register;
  typedef typename DataTypeForSize<bit_depth>::Type Value;
  typedef typename DataTypeForSize<size>::Type Index;
  static inline void Init() {
    if (safe) {
      memset(values_, 0, sizeof(values_));
      cycle_ = 0;
    }
    Register::Init();
  }
  static inline void set_value(Index output_index, Value intensity) {
    if (safe) {
      ARGUMENT_CHECK_LT(output_index, size);
      ARGUMENT_CHECK_LT(intensity, 1 << bit_depth);
    }
    values_[output_index] = intensity;
  }
  static inline Value value(Index output_index) {
    if (safe) {
      ARGUMENT_CHECK_LT(output_index, size);
    }
    return values_[output_index];
  }
  static inline void Out() {
    Index c = 0;
    Index bit = 1;
    for (Index i = 0; i < size; ++i) {
      if (values_[i] > cycle_) {
        c |= bit;
      }
      bit <<= 1;
    }
    ++cycle_;
    if (cycle_ == ((1 << bit_depth) - 1)) {
      cycle_ = 0;
    }
    Register::Write(c);
  }
 private:
  static Value values_[size];
  static Value cycle_;
  
  DISALLOW_COPY_AND_ASSIGN(OutputArray);
};

template<typename Latch, typename Clock, typename Data,
         uint8_t size, uint8_t bit_depth, DataOrder order, bool safe>
typename OutputArray<Latch, Clock, Data, size, bit_depth, order, safe>::Value
OutputArray<Latch, Clock, Data, size, bit_depth, order, safe>::values_[size];

template<typename Latch, typename Clock, typename Data,
         uint8_t size, uint8_t bit_depth, DataOrder order, bool safe>
typename OutputArray<Latch, Clock, Data, size, bit_depth, order, safe>::Value
OutputArray<Latch, Clock, Data, size, bit_depth, order, safe>::cycle_;


// A specialization that packs the data by nibble - this is the configuration
// used for Shruti.
template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
class OutputArray<Latch, Clock, Data, size, 4, order, safe> {
 public:
  typedef ShiftRegister<Latch, Clock, Data, size, order> Register;
  typedef typename DataTypeForSize<4>::Type Value;
  typedef typename DataTypeForSize<size>::Type Index;
  OutputArray() { }
  static inline void Init() {
    if (safe) {
      memset(values_, 0, sizeof(values_));
      cycle_ = 0;
    }
    Register::Init();
  }
  static inline void set_value(Index output_index, Value intensity) {
    if (safe) {
      ARGUMENT_CHECK_LT(output_index, size);
      ARGUMENT_CHECK_LT(intensity, 16);
    }
    if (output_index & 1) {
      output_index >>= 1;
      values_[output_index] = (intensity << 4) | (values_[output_index] & 0xf);
    } else {
      output_index >>= 1;
      values_[output_index] = intensity | (values_[output_index] & 0xf0);
    }
  }
  static inline Value value(Index output_index) {
    if (safe) {
      ARGUMENT_CHECK_LT(output_index, size);
    }
    if (output_index & 1) {
      return values_[output_index >> 1] >> 4;
    } else {
      return values_[output_index >> 1] & 0xf;
    }
  }
  static inline void Out() {
    Index c = 0;
    Index bit = 1;
    for (Index i = 0; i < size; ++i) {
      Value x = values_[i >> 1];
      uint8_t value = (x & 0xf);
      if (value > cycle_) {
        c |= bit;
      }
      bit <<= 1;
      ++i;
      if (i == size) {
        break;
      }
      x >>= 4;
      if (x > cycle_) {
        c |= bit;
      }
      bit <<= 1;
    }
    ++cycle_;
    cycle_ = (cycle_ & 15);
    Register::Write(c);
  }
 private:
  static Value values_[(size - 1) / 2 + 1];
  static Value cycle_;
  
  DISALLOW_COPY_AND_ASSIGN(OutputArray);
};

template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
typename OutputArray<Latch, Clock, Data, size, 4, order, safe>::Value
OutputArray<Latch, Clock, Data, size, 4, order, safe>::values_[
    (size - 1) / 2 + 1];

template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
typename OutputArray<Latch, Clock, Data, size, 4, order, safe>::Value
OutputArray<Latch, Clock, Data, size, 4, order, safe>::cycle_;


// A specialization with only two states. In this case, we can write data to the
// shift register only when a bit has changed in the array - there's no pseudo
// PWM brightness modulation.
template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
class OutputArray<Latch, Clock, Data, size, 1, order, safe> {
  typedef ShiftRegister<Latch, Clock, Data, size, order> Register;
 public:
  typedef typename DataTypeForSize<size>::Type T;
  OutputArray() { }
  static inline void Init() {
    if (safe) {
      touched_ = 1;
      bits_ = 0;
    }
    Register::Init();
  }
  static inline void set_value(uint8_t output_index, uint8_t intensity) {
    if (safe) {
      ARGUMENT_CHECK_LT(output_index, size);
      ARGUMENT_CHECK_LT(intensity, 2);
    }
    T mask = T(1) << output_index;
    if (intensity) {
      bits_ |= mask;
    } else {
      bits_ &= ~mask;
    }
  }
  static inline uint8_t value(uint8_t output_index) {
    if (safe) {
      ARGUMENT_CHECK_LT(output_index, size);
    }
    T mask = T(1) << output_index;
    return T(bits_ & mask) ? 1 : 0;
  }
  static inline void Out() {
    if (bits_ == last_bits_) {
      return;
    }
    Register::Write(bits_);
    last_bits_ = bits_;
  }
 private:
  static T bits_;
  static T last_bits_;
  static uint8_t touched_;
  
  DISALLOW_COPY_AND_ASSIGN(OutputArray);
};

template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
typename OutputArray<Latch, Clock, Data, size, 1, order, safe>::T
OutputArray<Latch, Clock, Data, size, 1, order, safe>::bits_;

template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
typename OutputArray<Latch, Clock, Data, size, 1, order, safe>::T
OutputArray<Latch, Clock, Data, size, 1, order, safe>::last_bits_;

template<typename Latch, typename Clock, typename Data,
         uint8_t size, DataOrder order, bool safe>
uint8_t OutputArray<Latch, Clock, Data, size, 1, order, safe>::touched_;

}  // namespace hardware_io

#endif   // HARDWARE_IO_DEVICES_OUTPUT_ARRAY_H_
