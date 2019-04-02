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
// Patch.

#include "hardware/shruti/patch_metadata.h"

#include <avr/pgmspace.h>

#include "hardware/shruti/patch.h"

namespace hardware_shruti {

static const prog_char raw_parameter_definition[
    kNumEditableParameters * sizeof(ParameterDefinition)] PROGMEM = {
  // Osc 1.
  PRM_OSC_SHAPE_1,
  WAVEFORM_NONE, WAVEFORM_QUAD_SAW_PAD,
  UNIT_WAVEFORM,
  STR_RES_SHAPE, STR_RES_SHAPE,

  PRM_OSC_PARAMETER_1,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_PRM, STR_RES_PARAMETER,

  PRM_OSC_RANGE_1,
  -12, 12,
  UNIT_INT8,
  STR_RES_RNG, STR_RES_RANGE,

  PRM_OSC_OPTION_1,
  SUM, XOR,
  UNIT_OPERATOR,
  STR_RES_OP, STR_RES_OPERATOR,

  // Osc 2.
  PRM_OSC_SHAPE_2,
  WAVEFORM_IMPULSE_TRAIN, WAVEFORM_TRIANGLE,
  UNIT_WAVEFORM,
  STR_RES_SHAPE, STR_RES_SHAPE,

  PRM_OSC_PARAMETER_2,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_PRM, STR_RES_PARAMETER,

  PRM_OSC_RANGE_2,
  -24, 24, 
  UNIT_INT8,
  STR_RES_RNG, STR_RES_RANGE,

  PRM_OSC_OPTION_2,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_TUN, STR_RES_DETUNE,

  // Mix balance.
  PRM_MIX_BALANCE,
  0, 63,
  UNIT_UINT8,
  STR_RES_MIX, STR_RES_OSC_BAL,

  PRM_MIX_SUB_OSC,
  0, 63,
  UNIT_UINT8,
  STR_RES_SUB, STR_RES_SUB_OSC_,

  PRM_MIX_NOISE,
  0, 63,
  UNIT_UINT8,
  STR_RES_NOI, STR_RES_NOISE,

  PRM_MIX_SUB_OSC_SHAPE,
  WAVEFORM_SQUARE, WAVEFORM_TRIANGLE,
  UNIT_WAVEFORM, 
  STR_RES_SHAPE, STR_RES_SHAPE,

  // Filter.
  PRM_FILTER_CUTOFF,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_CUT, STR_RES_CUTOFF,

  PRM_FILTER_RESONANCE,
  0, 63,
  UNIT_UINT8,
  STR_RES_RES, STR_RES_RESONANCE,

  PRM_FILTER_ENV,
  0, 63,
  UNIT_INT8,
  STR_RES_ENV1TVCF, STR_RES_ENV1TVCF,

  PRM_FILTER_LFO,
  0, 63,
  UNIT_INT8,
  STR_RES_LFO2TVCF, STR_RES_LFO2TVCF,

  // Env 1.
  PRM_ENV_ATTACK_1,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_ATK, STR_RES_ATTACK,

  PRM_ENV_DECAY_1,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_DECAY, STR_RES_DECAY,

  PRM_ENV_SUSTAIN_1,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_SUSTAIN, STR_RES_SUSTAIN,

  PRM_ENV_RELEASE_1,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_RELEASE, STR_RES_RELEASE,

  // Env 2.
  PRM_ENV_ATTACK_2,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_ATK, STR_RES_ATTACK,

  PRM_ENV_DECAY_2,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_DECAY, STR_RES_DECAY,

  PRM_ENV_SUSTAIN_2,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_SUSTAIN, STR_RES_SUSTAIN,

  PRM_ENV_RELEASE_2,
  0, 127,
  UNIT_RAW_UINT8,
  STR_RES_RELEASE, STR_RES_RELEASE,

  // Lfo.
  PRM_LFO_WAVE_1,
  LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_STEP_SEQUENCER,
  UNIT_LFO_WAVEFORM,
  STR_RES_WV1, STR_RES_LFO1_WAVE,

  PRM_LFO_RATE_1,
  0, 127 + 16,
  UNIT_LFO_RATE,
  STR_RES_RT1, STR_RES_LFO1_RATE,

  PRM_LFO_WAVE_2,
  LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_STEP_SEQUENCER,
  UNIT_LFO_WAVEFORM,
  STR_RES_WV2, STR_RES_LFO2_WAVE,

  PRM_LFO_RATE_2,
  0, 127 + 16,
  UNIT_LFO_RATE,
  STR_RES_RT2, STR_RES_LFO2_RATE,

  // Modulations.
  PRM_MOD_ROW,
  0, kModulationMatrixSize - 1,
  UNIT_INDEX,
  STR_RES_MOD_, STR_RES_MOD_,

  PRM_MOD_SOURCE,
  0, kNumModulationSources - 1,
  UNIT_MODULATION_SOURCE,
  STR_RES_SRC, STR_RES_SOURCE,

  PRM_MOD_DESTINATION,
  0, kNumModulationDestinations - 1,
  UNIT_MODULATION_DESTINATION,
  STR_RES_DST, STR_RES_DEST_,

  PRM_MOD_AMOUNT,
  -63, 63,
  UNIT_INT8,
  STR_RES_AMT, STR_RES_AMOUNT,

  // Arpeggiator.
  PRM_ARP_TEMPO,
  35, 240 + STR_RES_960 - STR_RES_270 + 1,
  UNIT_TEMPO_WITH_EXTERNAL_CLOCK,
  STR_RES_BPM, STR_RES_TEMPO,

  PRM_ARP_OCTAVE,
  OFF, 4,
  UNIT_UINT8,
  STR_RES_OCTAVE, STR_RES_OCTAVE,

  PRM_ARP_PATTERN,
  0, kNumArpeggiatorPatterns * 4 - 1, 
  UNIT_PATTERN,
  STR_RES_PATTERN, STR_RES_PATTERN,

  PRM_ARP_GROOVE,
  0, 79, 
  UNIT_GROOVE_PATTERN,
  STR_RES_GROOVE, STR_RES_GROOVE,

  // Keyboard settings.
  PRM_KBD_OCTAVE,
  -2, +2,
  UNIT_INT8,
  STR_RES_OCTAVE, STR_RES_OCTAVE,

  PRM_KBD_RAGA,
  0, 32, 
  UNIT_RAGA,
  STR_RES_RAGA, STR_RES_RAGA,

  PRM_KBD_PORTAMENTO,
  -63, 63,
  UNIT_INT8,
  STR_RES_PRT, STR_RES_PORTA,

  PRM_KBD_MIDI_CHANNEL,
  0, 50, 
  UNIT_MIDI_CHANNEL,
  STR_RES_CHN, STR_RES_MIDI_CHAN,

  PRM_ARP_PATTERN_ROTATION,
  0, 15,
  UNIT_UINT8,
  STR_RES_START, STR_RES_START,

  PRM_ARP_PATTERN_SIZE,
  1, 16,
  UNIT_UINT8,
  STR_RES_LENGTH, STR_RES_LENGTH
};

/* static */
ParameterDefinition PatchMetadata::parameter_definition_;

/* static */
uint8_t PatchMetadata::parameter_definition_index_ = 0xff;

/* static */
const ParameterDefinition& PatchMetadata::parameter_definition(uint8_t index) {
  if (index != parameter_definition_index_) {
    parameter_definition_index_ = index;
    ResourcesManager::Load(
        raw_parameter_definition,
        index,
        &parameter_definition_);
  }
  return parameter_definition_;
}

/* static */
uint8_t PatchMetadata::Scale(
    const ParameterDefinition& parameter,
    uint8_t value_7bits) {
  uint8_t scaled_value;
  if (parameter.unit == UNIT_RAW_UINT8) {
    scaled_value = value_7bits;
  } else {
    uint8_t range = parameter.max_value - parameter.min_value + 1;
    scaled_value = (value_7bits * range) >> 7;
    scaled_value += parameter.min_value;
  }
  return scaled_value;
}

}  // hardware_shruti
