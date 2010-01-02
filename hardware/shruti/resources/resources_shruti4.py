#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet.
#
# Author: Olivier Gillet (ol.gillet@gmail.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# -----------------------------------------------------------------------------
#
# Master resources file.

header = """// Copyright 2009 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Resources definitions.
//
// Automatically generated with:
// python arc.py resources.py
"""

namespace = 'hardware_shruti'
target = 'hardware/shruti/shruti4'
modifier = 'PROGMEM'
types = ['uint8_t', 'uint16_t']
includes = """
#include "hardware/base/base.h"

#include <avr/pgmspace.h>
"""
create_specialized_manager = True


import characters
import lookup_tables
import strings
import waveforms


def FilterResources(l):
  return [(k, v) for (k, v, f) in l if f is None or f == 4]


def RemoveFullSrWaveforms(l):
  return [(k, v) for (k, v)  in l if not k.endswith('0')]


resources = [
  (strings.common_strings + lookup_tables.strings + strings.shruti4_strings,
   'string', 'STR_RES', 'prog_char', str, False),
  (FilterResources(lookup_tables.lookup_tables),
   'lookup_table', 'LUT_RES', 'prog_uint16_t', int, False),
  (RemoveFullSrWaveforms(FilterResources(waveforms.waveforms)),
   'waveform', 'WAV_RES', 'prog_uint8_t', int, True),
  (characters.characters, 'character', 'CHR_RES', 'prog_uint8_t', int, True),
]
