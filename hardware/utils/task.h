// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Implementation of multitasking by coroutines.

#ifndef HARDWARE_UTILS_TASK_H_
#define HARDWARE_UTILS_TASK_H_

#define TASK_BEGIN static uint16_t state = 0; \
    switch(state) { \
case 0:;

// This is very unreliable because it assumes the line numbers will fit in an
// uint8_t. Don't use this unless you want to save a hundred bytes by having
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

#endif  // HARDWARE_UTILS_TASK_H_
