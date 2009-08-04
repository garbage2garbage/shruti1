// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// An alternative pin i/o library based on templates.
//
// Examples of use:
//
// Pin<3>::set_mode(DIGITAL_INPUT)
// Pin<4>::set_mode(DIGITAL_OUTPUT)
// Pin<3>::value()
// Pin<4>::High()
// Pin<4>::Low()
// Pin<4>::set_value(1)
// Pin<4>::set_value(0)
//
// Using the logging facilities in logging.h, this can also generates debugging
// code (for compilation on desktop computers) which logs to stderr the activity
// on the pin.

#ifndef HARDWARE_IO_PIN_H_
#define HARDWARE_IO_PIN_H_

#ifndef __TEST__
#include <avr/io.h>
#endif  // !__TEST__

#include "hardware/io/io.h"
#include "hardware/io/timer.h"
#include "hardware/utils/logging.h"

namespace hardware_io {

enum PinMode {
  DIGITAL_INPUT = 0,
  DIGITAL_OUTPUT = 1,
  ANALOG_OUTPUT = 2
};

#ifndef __TEST__

// All the registers used in the following definitions are wrapped here.
IORegister(DDRB);
IORegister(DDRC);
IORegister(DDRD);

IORegister(PORTB);
IORegister(PORTC);
IORegister(PORTD);

IORegister(PINB);
IORegister(PINC);
IORegister(PIND);

// Represents a i/o port, which has input, output and mode registers.
template<typename InputRegister, typename OutputRegister,
         typename ModeRegister>
struct Port {
  typedef InputRegister Input;
  typedef OutputRegister Output;
  typedef ModeRegister Mode;
};

// Definition of I/O ports.
typedef Port<PINBRegister, PORTBRegister, DDRBRegister> PortB;
typedef Port<PINCRegister, PORTCRegister, DDRCRegister> PortC;
typedef Port<PINDRegister, PORTDRegister, DDRDRegister> PortD;

// The actual implementation of a pin, not very convenient to use because it
// requires the actual parameters of the pin to be passed as template
// parameters.
template<typename Port, typename PwmChannel, uint8_t bit, bool safe>
struct PinImpl {
  typedef BitInRegister<typename Port::Mode, bit> ModeBit;
  typedef BitInRegister<typename Port::Output, bit> OutputBit;
  typedef BitInRegister<typename Port::Input, bit> InputBit;
  
  // Mode change.
  static inline void set_mode(uint8_t mode) {
    if (mode == DIGITAL_INPUT) {
      ModeBit::clear();
    } else if (mode == DIGITAL_OUTPUT || mode == ANALOG_OUTPUT) {
      ModeBit::set();
    }
    if (mode == ANALOG_OUTPUT) {
      PwmChannel::Start();
    } else {
      PwmChannel::Stop();
    }
  }

  // Digital Write.
  static inline void High() {
    if (safe) {
      set_mode(DIGITAL_OUTPUT);
    }
    OutputBit::set();
  };
  static inline void Low() {
    if (safe) {
      set_mode(DIGITAL_OUTPUT);
    }
    OutputBit::clear();
  };
  static inline void set_value(uint8_t value) {
    if (value == 0) {
      Low();
    } else {
      High();
    }
  };
  static inline void set_analog_value(uint8_t value) {
    if (safe) {
      set_mode(ANALOG_OUTPUT);
    }
    if (PwmChannel::analog) {
      PwmChannel::Write(value);
    } else {
      set_value(value);
    }
  }

  // Digital read.
  static inline uint8_t value() {
    if (safe) {
      set_mode(DIGITAL_INPUT);
    }
    return InputBit::value();
  };
};

// A template that will be specialized for each pin, allowing the pin number to
// be specified as a template parameter.
template<int n, bool safe>
struct NumberedPin { };

// Macro to make the pin definitions (template specializations) easier to read.
#define SetupPin(n, port, timer, bit) \
template<bool safe> struct NumberedPin<n, safe> { \
  typedef PinImpl<port, timer, bit, safe> Impl; };

SetupPin(0, PortD, NoPwmChannel, 0);
SetupPin(1, PortD, NoPwmChannel, 1);
SetupPin(2, PortD, NoPwmChannel, 2);
SetupPin(3, PortD, PwmChannel2B, 3);
SetupPin(4, PortD, NoPwmChannel, 4);
SetupPin(5, PortD, PwmChannel0B, 5);
SetupPin(6, PortD, PwmChannel0A, 6);
SetupPin(7, PortD, NoPwmChannel, 7);
SetupPin(8, PortB, NoPwmChannel, 0);
SetupPin(9, PortB, PwmChannel1A, 1);
SetupPin(10, PortB, PwmChannel1B, 2);
SetupPin(11, PortB, PwmChannel2A, 3);
SetupPin(12, PortB, NoPwmChannel, 4);
SetupPin(13, PortB, NoPwmChannel, 5);
SetupPin(14, PortC, NoPwmChannel, 0);
SetupPin(15, PortC, NoPwmChannel, 1);
SetupPin(16, PortC, NoPwmChannel, 2);
SetupPin(17, PortC, NoPwmChannel, 3);
SetupPin(18, PortC, NoPwmChannel, 4);
SetupPin(19, PortC, NoPwmChannel, 5);

// Two specializations of the numbered pin template, one which clears the timer
// for each access to the PWM pins, as does the original Arduino wire lib,
// the other that does not (use with care!).
template<int n, bool safe = false>
struct Pin {
  typedef typename NumberedPin<n, safe>::Impl Impl;
  static void set_mode(uint8_t mode) { Impl::set_mode(mode); }
  static void High() { Impl::High(); }
  static void Low() { Impl::Low(); }
  static void set_value(uint8_t value) { Impl::set_value(value); }
  static void set_analog_value(uint8_t value) { Impl::set_analog_value(value); }
  static uint8_t value() { return Impl::value(); }
  static uint8_t number() { return n; }
};

#else

template<int n>
struct Pin {
  static void set_mode(uint8_t mode) {
    LOG(INFO) << "pin_" << n << "\tmode\t" << int(mode);
  }
  static void High() {
    LOG(INFO) << "pin_" << n << "\tvalue\t1";
  }
  static void Low() {
    LOG(INFO) << "pin_" << n << "\tvalue\t0";
  }
  static void set_value(uint8_t value) {
    LOG(INFO) << "pin_" << n << "\tvalue\t" << int(value ? 1 : 0);
  }
  static uint8_t value() {
    LOG(INFO) << "pin_" << n << "\tread\t1";
  }
  static uint16_t number() {
    return n;
  }
};

#endif  // !__TEST__

template<int pin>
struct DigitalInput {
  enum {
    buffer_size = 0,
    data_size = 1,
  };
  static int8_t Read() {
    Pin<pin>::set_mode(DIGITAL_INPUT);
    return Pin<pin>::value();
  };
};

template<int pin>
struct PwmOutput {
  enum {
    buffer_size = 0,
    data_size = 8,
  };
  static void Init() {
    Pin<pin>::set_mode(ANALOG_OUTPUT);
  }
  static void Write(uint8_t value) {
    return Pin<pin>::set_analog_value(value);
  };
};

}  // namespace hardware_io

#endif   // HARDWARE_IO_PIN_H_
