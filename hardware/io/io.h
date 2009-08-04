// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Base interfaces for input/output.
//
// Important: All buffer sizes are expected to be less than 256! (fit in 8
// bits), and must be powers of 2.

#ifndef HARDWARE_IO_IO_H_
#define HARDWARE_IO_IO_H_

#ifndef __TEST__
#include <avr/io.h>
#endif  // !__TEST__

#include "hardware/base/base.h"
#include "hardware/io/size_to_type.h"

namespace hardware_io {

enum ShiftRegisterOrder {
  LSB_FIRST = 0,
  MSB_FIRST = 1
};

enum DigitalValue {
  LOW = 0,
  HIGH = 1
};

// <avr/io.h> is full of useful defines, but they cannot be used as template
// arguments because they are of the form: (*(volatile uint8_t *)(0x80))
// The following define wraps this reference into a class to make it easier to
// pass it as a template argument.
#ifndef __TEST__
#define IORegister(reg) struct reg##Register { \
  static volatile uint8_t* ptr() { return &reg; } \
};
#define SpecialFunctionRegister(reg) struct reg##Register { \
  static volatile uint8_t* ptr() { return &_SFR_BYTE(reg); } \
};
#endif !__TEST__

// Represents a bit in an i/o port register.
template<typename Register, uint8_t bit>
struct BitInRegister {
  static void clear() {
    *Register::ptr() &= ~_BV(bit);
  }
  static void set() {
    *Register::ptr() |= _BV(bit);
  }
  static uint8_t value() {
    return *Register::ptr() & _BV(bit) ? 1 : 0;
  }
};

// These classes implement/define the basic input/output interface. The default
// implementation is that of an infinite stream of incoming/outgoing 0s.
struct Input {
  enum {
    buffer_size = 0,  // Recommended buffer size, when using buffered input.
    data_size = 0,  // 0 for disabled port, 1 for digital, 8 for byte.
  };
  typedef uint8_t Value;
  
  // Blocking!
  static inline Value Read() { while (!readable()); return ImmediateRead(); }  
  
  // Number of bytes available for read.
  static inline uint8_t readable() { return 1; }
  
  // A byte, or -1 if reading failed.
  static inline int16_t NonBlockingRead() { return readable() ? Read() : -1; }
  
  // No check for ready state.
  static inline Value ImmediateRead() { return 0; }
  
  // Called in data reception interrupt.
  static inline void Received() { }
};

struct Output {
  enum {
    buffer_size = 0,  // Recommended buffer size, when using buffered output.
    data_size = 0  // 0 for disabled port, 1 for digital, 8 for byte.
  };
  typedef uint8_t Value;
  
  // Blocking!
  static inline void Write(Value v) { while (!writable()); Overwrite(v); }
  
  // Number of bytes that can be fed.
  static inline uint8_t writable() { return 1; }
  
  // 1 if success.
  static inline uint8_t NonBlockingWrite(Value v) {
    if (!writable()) {
      return 0;
    }
    Overwrite(v);
    return 1;
  } 
  
  // No check for ready state.
  static inline void Overwrite(Value) { return; }
  
  // Called in data emission interrupt.
  static inline Value Requested() { return 0; }  
};

// An object capable both of input and output, composed from an Input and an
// Output implementation.
template<typename I, typename O>
struct InputOutput {
  typedef I Input;
  typedef O Output;
  
  static inline void Write(typename O::Value v) { O::Write(v); }
  static inline uint8_t writable() { return O::wriable(); }
  static inline uint8_t NonBlockingWrite(typename O::Value v ) {
    return O::NonBlockingWrite(v);
  }
  static inline void Overwrite(typename O::Value v) { O::Overwrite(v); }
  static inline typename O::Value Requested() { return O::Requested(); }
  static inline typename I::Value Read() { return I::Read(); }
  static inline uint8_t readable() { return I::readable(); }
  static inline int16_t NonBlockingRead() { return I::NonBlockingRead(); }
  static inline typename I::Value ImmediateRead() { return I::ImmediateRead(); }
  static inline void Received() { I::Received(); }  
};

// Circular buffer, used for example for Serial input, Software serial output,
// Audio rendering... A buffer is created for each Owner - for example,
// Buffer<AudioClient> represents the audio buffer used by AudioClient.
template<typename Owner>
class Buffer : public Input, Output {
 public:
  typedef typename Owner::Value Value;
  enum {
    size = Owner::buffer_size,
    data_size = Owner::data_size
  };
  static inline void Write(Value v) {
    while (!writable());
    Overwrite(v);
  }
  static inline uint8_t writable() {
    return (read_ptr_ - write_ptr_ - 1) & (size - 1);
  }
  static inline uint8_t NonBlockingWrite(Value v) {
    if (writable()) {
      Overwrite(v);
      return 1;
    } else {
      return 0;
    }
  }
  static inline void Overwrite(Value v) {
    buffer_[write_ptr_] = v;
    write_ptr_ = (write_ptr_ + 1) & (size - 1);
  }
  static inline uint8_t Requested() { return 0; }
  static inline Value Read() {
    while (!readable());
    return ImmediateRead();
  }
  static inline uint8_t readable() {
    return (write_ptr_ - read_ptr_) & (size - 1);
  }
  static inline int16_t NonBlockingRead() {
    if (readable()) {
      return ImmediateRead();
    } else {
      return -1;
    }
  }
  static inline Value ImmediateRead() {
    Value result = buffer_[read_ptr_];
    read_ptr_ = (read_ptr_ + 1) & (size - 1);
    return result;
  }
  static inline void Flush() {
    write_ptr_ = read_ptr_;
  }
 private:
  static Value buffer_[size];
  static volatile uint8_t read_ptr_;
  static volatile uint8_t write_ptr_;
  
  DISALLOW_COPY_AND_ASSIGN(Buffer);
};

// Static variables created for each buffer.
template<typename T> volatile uint8_t Buffer<T>::read_ptr_ = 0;
template<typename T> volatile uint8_t Buffer<T>::write_ptr_ = 0;
template<typename T> typename T::Value Buffer<T>::buffer_[];

// Dummy class that can be passed whenever we expect Input/Output types, and
// which do not perform any IO.
typedef Input DisabledInput;
typedef Output DisabledOutput;
typedef InputOutput<DisabledInput, DisabledOutput> DisabledInputOutput;

enum PortMode {
  DISABLED = 0,
  POLLED = 1,
  BUFFERED = 2
};

}  // namespace hardware_io

#endif   // HARDWARE_IO_IO_H_
