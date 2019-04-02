// Copyright 2009 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
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
// Patch metadata.

#ifndef HARDWARE_SHRUTI_PATCH_METADATA_H_
#define HARDWARE_SHRUTI_PATCH_METADATA_H_

#include "hardware/base/base.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/shruti.h"

namespace hardware_shruti {

enum Unit {
  UNIT_RAW_UINT8,
  UNIT_UINT8,
  UNIT_INT8,
  UNIT_BOOLEAN,
  UNIT_WAVEFORM,
  UNIT_OPERATOR,
  UNIT_LFO_WAVEFORM,
  UNIT_LFO_RATE,
  UNIT_INDEX,
  UNIT_MODULATION_SOURCE,
  UNIT_MODULATION_DESTINATION,
  UNIT_PATTERN,
  UNIT_RAGA,
  UNIT_TEMPO_WITH_EXTERNAL_CLOCK,
  UNIT_GROOVE_PATTERN,
  UNIT_MIDI_CHANNEL,
};

typedef uint8_t ParameterUnit;

struct ParameterDefinition {
  uint8_t id;
  uint8_t min_value;
  uint8_t max_value;
  ParameterUnit unit;
  ResourceId short_name;
  ResourceId long_name;
};

class PatchMetadata {
 public:
  PatchMetadata() { }
  static const ParameterDefinition& parameter_definition(uint8_t index);
  static uint8_t Scale(const ParameterDefinition& parameter, uint8_t value);
  
 private:
  static ParameterDefinition parameter_definition_;
  static uint8_t parameter_definition_index_;
  
  DISALLOW_COPY_AND_ASSIGN(PatchMetadata);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_PATCH_METADATA_H_
