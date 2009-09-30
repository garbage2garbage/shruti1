// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Base header.

#ifndef HARDWARE_BASE_BASE_H_
#define HARDWARE_BASE_BASE_H_

#include <inttypes.h>

#ifndef NULL
#define NULL 0
#endif

static const uint16_t kEepromSize = 1024;

#define abs(x) ((x) > 0 ? (x) : -(x))

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif  // HARDWARE_BASE_BASE_H_
