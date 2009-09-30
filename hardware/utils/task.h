// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Implementation of multitasking by coroutines, and naive deterministic
// scheduler.

#ifndef HARDWARE_UTILS_TASK_H_
#define HARDWARE_UTILS_TASK_H_

#include "hardware/base/base.h"

#define TASK_BEGIN static uint16_t state = 0; \
    switch(state) { \
case 0:;

// This is very unreliable because it assumes the line numbers will fit in an
// uint8_t. Don't use this unless you want to save a couple of bytes by using
// 8 bits comparisons instead of 16 bits comparisons.
#define TASK_BEGIN_NEAR static uint8_t state = 0; \
    switch(state) { \
case 0:;

#define TASK_RETURN(value) \
    do { \
      state = __LINE__; \
      return (value); \
case __LINE__:; \
    } while (0)

#define TASK_SWITCH \
    do { \
      state = __LINE__; \
      return; \
case __LINE__:; \
    } while (0)

#define TASK_END } return;

namespace hardware_utils {

typedef struct {
  void (*code)();
  uint8_t priority;
} Task;

// This naive deterministic scheduler stores an array of "slots", each element
// of which stores a 0 (nop) or a task id. During initialization, the array is
// filled in such a way that $task.priority occurrences of a task are present in
// the array, and are roughly evenly spaced.
// For example if the tasks/priority are:
// Task 1: 8
// Task 2: 4
// Task 3: 3
// Task 4: 1
//
// The slots will contain:
// 1 2 1 3 1 2 1 4 1 2 1 3 2 3 0 0
//
// And the scheduler will execute the tasks in this sequence.
template<uint8_t num_slots>
class NaiveScheduler {
 public:
  void Init()  {
    uint8_t slot = 0;

    // For a given task, occupy $priority available slots, spaced apart by
    // #total slots / $priority.
    for (uint8_t i = 0; i < sizeof(slots_); ++i) {
      slots_[i] = 0;
    }

    for (uint8_t i = 0; i < sizeof(tasks_) / sizeof(Task); ++i) {
      for (uint8_t j = 0; j < tasks_[i].priority; ++j) {
        // Search for the next available slot.
        while (1) {
          if (slot >= sizeof(slots_)) {
            slot = 0;
          }
          if (slots_[slot] == 0) {
            break;
          }
          slot++;
        }
        slots_[slot] = i + 1;
        slot += sizeof(slots_) / tasks_[i].priority;
      }
    }
  }

  void Run() {
    while (1) {
      if (slots_[current_slot_]) {
        tasks_[slots_[current_slot_] - 1].code();
      }
      current_slot_++;
      if (current_slot_ >= sizeof(slots_)) {
        current_slot_ = 0;
      }
    }
  }

 private:
  static Task tasks_[];
  static uint8_t slots_[num_slots];
  static uint8_t current_slot_;
};

template<uint8_t num_slots>
uint8_t NaiveScheduler<num_slots>::slots_[num_slots];

template<uint8_t num_slots>
uint8_t NaiveScheduler<num_slots>::current_slot_ = 0;

}  // namespace hardware_utils

#endif  // HARDWARE_UTILS_TASK_H_
