// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Definition of the synthesis parameters.

#ifndef HARDWARE_SHRUTI_PARAMETERS_H_
#define HARDWARE_SHRUTI_PARAMETERS_H_

#include "hardware/base/base.h"

namespace hardware_shruti {

enum ModulationSource {
  /* First the modulation sources common to all notes. */
  MOD_SRC_LFO_1 = 0,
  MOD_SRC_LFO_2,
  MOD_SRC_SEQ,
  MOD_SRC_STEP,
  MOD_SRC_WHEEL,
  MOD_SRC_PITCH_BEND,
  MOD_SRC_ASSIGNABLE_1,
  MOD_SRC_ASSIGNABLE_2,
  /* Then those which are different for each note. */
  MOD_SRC_ENV,
  MOD_SRC_VELOCITY,
  MOD_SRC_NOTE,
  MOD_SRC_GATE,
};

static const uint8_t kNumModulationSources = 12;

enum ModulationDestination {
  MOD_DST_FILTER_CUTOFF = 0,
  MOD_DST_VCA,
  MOD_DST_PWM_1,
  MOD_DST_PWM_2,
  MOD_DST_VCO_1,
  MOD_DST_VCO_2,
  MOD_DST_MIX_BALANCE,
  MOD_DST_MIX_NOISE,
  MOD_DST_MIX_SUB_OSC,
  MOD_DST_FILTER_RESONANCE
};

static const uint8_t kNumModulationDestinations = 10;

enum Parameter {
  PRM_OSC_ALGORITHM_1,
  PRM_OSC_ALGORITHM_2,
  PRM_OSC_PARAMETER_1,
  PRM_OSC_PARAMETER_2,
  PRM_OSC_RANGE_1,
  PRM_OSC_RANGE_2,
  PRM_OSC_OPTION_1,
  PRM_OSC_OPTION_2,

  PRM_MIX_BALANCE,
  PRM_MIX_SUB_OSC,
  PRM_MIX_NOISE,
  PRM_MIX_SUB_OSC_ALGORITHM,

  PRM_FILTER_CUTOFF,
  PRM_FILTER_RESONANCE,
  PRM_FILTER_ENV,
  PRM_FILTER_LFO,

  PRM_ENV_ATTACK,
  PRM_ENV_DECAY,
  PRM_ENV_SUSTAIN,
  PRM_ENV_RELEASE,

  PRM_LFO_WAVE_1,
  PRM_LFO_WAVE_2,
  PRM_LFO_RATE_1,
  PRM_LFO_RATE_2,

  PRM_MOD_SOURCE = 24,
  PRM_MOD_DESTINATION = 25,
  PRM_MOD_AMOUNT = 26,
  PRM_MOD_ROW = 27,

  PRM_ARP_TEMPO = 48,
  PRM_ARP_OCTAVES,
  PRM_ARP_PATTERN,
  PRM_ARP_SWING,
  
  PRM_SEQUENCE = 52,

  PRM_KBD_OCTAVE = 60,
  PRM_KBD_RAGA,
  PRM_KBD_PORTAMENTO,
  PRM_KBD_MIDI_CHANNEL,
  
  PRM_NAME = 64,
};

enum OscillatorAlgorithm {
  WAVEFORM_IMPULSE_TRAIN,
  WAVEFORM_SAW,
  WAVEFORM_SQUARE,
  WAVEFORM_TRIANGLE,
  WAVEFORM_CZ,
  WAVEFORM_FM,
  WAVEFORM_8BITLAND,
  WAVEFORM_SPEECH,
  WAVEFORM_WAVETABLE,
  WAVEFORM_ANALOG_WAVETABLE,
};

enum LfoWave {
  // For oscillators.
  LFO_WAVEFORM_TRIANGLE,
  LFO_WAVEFORM_SQUARE,
  LFO_WAVEFORM_S_H,
  LFO_WAVEFORM_SAW,
};

enum Status {
  OFF = 0,
  ON
};

enum OPERATOR {
  SUM = 0,
  SYNC = 1,
  RING_MOD = 2
};

static const uint8_t kNumEditableParameters = 36;

}  // namespace hardware_shruti

#endif   // HARDWARE_SHRUTI_PARAMETERS_H_
