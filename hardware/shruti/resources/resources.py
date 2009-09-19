import numpy

header = """// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Resources definitions.
//
// Automatically generated with:
// python arc.py resources.py
"""

namespace = 'hardware_shruti'
target = 'hardware/shruti'
modifier = 'PROGMEM'
types = ['uint8_t', 'uint16_t']

includes = """
#include "hardware/base/base.h"

#ifdef __TEST__
#define prog_char char
#define prog_uint16_t uint16_t
#define prog_uint8_t uint8_t
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif
"""

import characters
import lookup_tables
import strings
import waveforms

resources = [
  (strings.strings + lookup_tables.strings, 'string', 'STR_RES', 'prog_char',
   str, False),
  (lookup_tables.lookup_tables, 'lookup_table', 'LUT_RES', 'prog_uint16_t',
   int, False),
  (waveforms.waveforms, 'waveform', 'WAV_RES', 'prog_uint8_t', int, True),
  (characters.characters, 'character', 'CHR_RES', 'prog_uint8_t', int, True),
]

create_specialized_manager = True
