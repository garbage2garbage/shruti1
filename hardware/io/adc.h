// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Analog->Digital conversion.

#ifndef HARDWARE_IO_ADC_H_
#define HARDWARE_IO_ADC_H_

#ifndef __TEST__
#include <avr/io.h>
#endif  // !__TEST__

#include "hardware/io/io.h"
#include "hardware/utils/logging.h"

namespace hardware_io {

enum AdcReference {
  ADC_EXTERNAL = 0,
  ADC_DEFAULT = 1,
  ADC_INTERNAL = 3
};

#ifndef __TEST__

IORegister(ADCSRA);
IORegister(ADMUX);
IORegister(ADCL);
IORegister(ADCH);

typedef BitInRegister<ADCSRARegister, ADSC> AdcConvert;
typedef BitInRegister<ADCSRARegister, ADEN> AdcEnabled;

class Adc {
 public:
  static inline void set_prescale_factor(uint8_t factor) {
    *ADCSRARegister::ptr() = (*ADCSRARegister::ptr() & ~0x07) | (factor & 0x07);
  }
  static inline void set_reference(AdcReference reference) {
    reference_ = reference << 6;
  }
  static inline void Enable() {
    AdcEnabled::set();
  }
  static inline void Disabled() {
    AdcEnabled::clear();
  }
  // TODO(oliviergillet): add function to modify sampling rate.
  static inline int16_t Read(uint8_t pin) {
    StartConversion(pin);
    Wait();
    return ReadOut();
  }

 private:
  static uint8_t reference_;
 
  static inline void StartConversion(uint8_t pin) {
    *ADMUXRegister::ptr() = reference_ | (pin & 0x07);
    AdcConvert::set();
  }
  static inline void Wait() {
    while (AdcConvert::value());
  }
  static inline int16_t ReadOut() {
    uint8_t low = *ADCLRegister::ptr();
    uint8_t high = *ADCHRegister::ptr();
    return (high << 8) | low;
  }
  DISALLOW_COPY_AND_ASSIGN(Adc);
};

template<int pin>
struct AnalogInput {
  enum {
    buffer_size = 0,
    data_size = 16,
  };
  static int16_t Read() {
    return Adc::Read(pin);
  };
};

#else

template<int pin>
struct AnalogInput {
  static uint16_t counter_;
  static int16_t Read() {
    LOG(INFO) << "analog_pin_" << pin << "\readout\t" << int(counter_);
    counter_ = (counter_ + 1) & 1023;
    return counter_;
  }
};
template<int pin> AnalogInput::counter_ = 0;

#endif  // !__TEST__

}  // namespace hardware_io

#endif  // HARDWARE_IO_ADC_H_
