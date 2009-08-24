// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Controller for an array of analog or digital inputs behind a multiplexer
// controlled by a shift register. Stores an array reflecting the current value
// of each input. A readout returns an "Event" object with the latest value of
// the controller, a flag indicating how it changed with respect to the
// previous value, and a time.
//
// When event = EVENT_NONE, the time is the idle time (how many ms since an
// event occurred).
// When event != EVENT_NONE, the time is the time spent in the previous state.
//
// The "threshold" template parameters indicate the minimum magnitude of a
// change in the ADC value that will raise an EVENT_CHANGED. This can be used,
// for example, to avoid a costly update of internal variables to reflect a
// change in the value read by a potentiometer - a change which turns out to be
// only noise.

#ifndef HARDWARE_IO_INPUT_ARRAY_H_
#define HARDWARE_IO_INPUT_ARRAY_H_

#include "hardware/base/time.h"
#include "hardware/io/size_to_type.h"

namespace hardware_io {

enum InputEvent {
  EVENT_NONE = 0,
  EVENT_CHANGED = 1,
  EVENT_RAISED = 2,
  EVENT_LOWERED = 3
};

template<typename Input, uint8_t num_inputs, uint8_t threshold = 1>
class InputArray {
 public:
  InputArray() { }
  typedef typename DataTypeForSize<Input::data_size>::Type T;
  typedef struct {
    uint8_t id;
    uint8_t event;  // Could have been InputEvent but I want the extra byte.
    T value;
    uint32_t time;
   } Event;
  static void Init() {
    // No need to initialize anything - the first cycle of readouts will take
    // care of this.
    // TODO(pichenettes): remove this initialization code.
    active_input_ = 0;
    starting_up_ = 1;
    Input::Init();
  }
  static Event Read() {
    Event e;
    e.id = active_input_;
    e.value = Input::Read();
    uint8_t same;
    if (threshold == 1) {
      same = values_[active_input_] == e.value;
    } else {
      same = abs(values_[active_input_] - e.value) < threshold;
    }
    e.time = hardware_base::milliseconds() - last_event_time_;
    if (same) {
      e.event = EVENT_NONE;
    } else {
      values_[active_input_] = e.value;
      last_event_time_ = hardware_base::milliseconds();
      if (Input::data_size == 1) {
        e.event = (e.value == 1) ? EVENT_RAISED : EVENT_LOWERED;
      } else {
        e.event = EVENT_CHANGED;
      }
    }
    ++active_input_;
    if (active_input_ == num_inputs) {
      active_input_ = 0;
    }
    // During the first cycle, do not raise any event - just record the values.
    if (starting_up_) {
      if (active_input_ == 0) {
        starting_up_ = 0;
      }
      e.event = EVENT_NONE;
      e.time = 0;
    }
    return e;
  }
  static uint8_t active_input() { return active_input_; }
 private:
  static T values_[num_inputs];
  static uint8_t active_input_;
  static uint8_t starting_up_;
  static uint32_t last_event_time_;
  
  DISALLOW_COPY_AND_ASSIGN(InputArray);
};

template<typename Input, uint8_t num_inputs, uint8_t robustness>
typename InputArray<Input, num_inputs, robustness>::T
InputArray<Input, num_inputs, robustness>::values_[num_inputs];

template<typename Input, uint8_t num_inputs,  uint8_t robustness>
uint8_t InputArray<Input, num_inputs, robustness>::active_input_;

template<typename Input, uint8_t num_inputs, uint8_t robustness>
uint32_t InputArray<Input, num_inputs, robustness>::last_event_time_;

template<typename Input, uint8_t num_inputs, uint8_t robustness>
uint8_t InputArray<Input, num_inputs, robustness>::starting_up_;

}  // namespace hardware_io

#endif   // HARDWARE_IO_INPUT_ARRAY_H_
