// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Main synthesis engine.

#include "hardware/shruti/synthesis_engine.h"

#include <string.h>

#include "hardware/resources/resources_manager.h"
#include "hardware/shruti/lfo.h"
#include "hardware/shruti/oscillator.h"
#include "hardware/shruti/parameters.h"
#include "hardware/utils/random.h"
#include "hardware/utils/signal.h"

using hardware_utils::Signal;

namespace hardware_shruti {

/* extern */
SynthesisEngine engine;

typedef Oscillator<1, FULL> Osc1;
typedef Oscillator<2, LOW_COMPLEXITY> Osc2;
typedef Oscillator<3, SUB_OSCILLATOR> SubOsc;

/* <static> */
int16_t SynthesisEngine::envelope_increment_[5];
int16_t SynthesisEngine::envelope_target_[5];
uint8_t SynthesisEngine::modulation_sources_[MOD_SRC_ASSIGNABLE_2 + 1];

uint8_t SynthesisEngine::oscillator_decimation_;

Patch SynthesisEngine::patch_;
Voice SynthesisEngine::voice_;
VoiceController SynthesisEngine::controller_;
Lfo SynthesisEngine::lfo_[2];
/* </static> */

/* static */
void SynthesisEngine::Init() {
  envelope_target_[ATTACK] = 16383;
  envelope_target_[RELEASE] = 0;
  envelope_target_[DEAD] = -1;
  envelope_increment_[DEAD] = 0;
  envelope_increment_[SUSTAIN] = 0;
  controller_.Init(&voice_, 1);  // "Monophonic shit" - Mr Oizo.
  ResetPatch();
  Reset();
  voice_.Init();
}

static const prog_char empty_patch[] PROGMEM = {
    /*108,
    WAVEFORM_SAW, WAVEFORM_SAW, 0, 0,
    128, 128, 0, 0,
    0, 0, 0, WAVEFORM_TRIANGLE,
    120, 0, 0, 0,
    0, 40, 100, 40,
    LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_TRIANGLE, 64, 16,
    MOD_SRC_LFO_1, MOD_DST_VCO_1, 0,
    MOD_SRC_LFO_1, MOD_DST_VCO_2, 0,
    MOD_SRC_LFO_2, MOD_DST_PWM_1, 0,
    MOD_SRC_LFO_2, MOD_DST_PWM_2, 0,
    MOD_SRC_NOTE, MOD_DST_FILTER_CUTOFF, 48,
    MOD_SRC_ENV, MOD_DST_VCA, 127,
    MOD_SRC_VELOCITY, MOD_DST_VCA, 31,
    MOD_SRC_LFO_2, MOD_DST_MIX_BALANCE, 0,
    120, 0, 0, 0,
    136, 136, 136, 136, 136, 136, 136, 136,
    128, 0, 0, 1,
    'n', 'e', 'w', ' ', ' ', ' ', ' ', ' '};*/

    99,
    WAVEFORM_SAW, WAVEFORM_SQUARE, 0, 50,
    128, 128, 0, 0,
    4, 4, 4, WAVEFORM_SQUARE,
    120, 0, 0, 0,
    0, 40, 100, 40,
    LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_TRIANGLE, 64, 16,
    MOD_SRC_LFO_1, MOD_DST_VCO_1, 0,
    MOD_SRC_LFO_1, MOD_DST_VCO_2, 0,
    MOD_SRC_LFO_2, MOD_DST_PWM_1, 0,
    MOD_SRC_LFO_2, MOD_DST_PWM_2, 0,
    MOD_SRC_NOTE, MOD_DST_FILTER_CUTOFF, 48,
    MOD_SRC_ENV, MOD_DST_VCA, 127,
    MOD_SRC_VELOCITY, MOD_DST_VCA, 31,
    MOD_SRC_LFO_2, MOD_DST_MIX_BALANCE, 0,
    120, 0, 0, 0,
    136, 136, 136, 136, 136, 136, 136, 136,
    128, 0, 0, 1,
    'n', 'e', 'w', ' ', ' ', ' ', ' ', ' '};


/* static */
void SynthesisEngine::ResetPatch() {
  ResourcesManager::Load(empty_patch, 0, &patch_);
  TouchPatch();
}

#define IGNORE_OTHER_CHANNELS \
  if (patch_.kbd_midi_channel != 0 && \
      patch_.kbd_midi_channel != (channel + 1)) { \
    return; \
  }

/* static */
void SynthesisEngine::NoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  IGNORE_OTHER_CHANNELS;
  controller_.NoteOn(note, velocity);
}

/* static */
void SynthesisEngine::NoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  IGNORE_OTHER_CHANNELS;
  controller_.NoteOff(note);
}

/* static */
void SynthesisEngine::ControlChange(uint8_t channel, uint8_t controller,
                                    uint8_t value) {
  IGNORE_OTHER_CHANNELS;
  switch (controller) {
    case hardware_midi::kModulationWheelMsb:
      // TODO(pichenettes): improve the MIDI implementation.
      modulation_sources_[MOD_SRC_WHEEL] = (value << 1);
      break;
  }
}

/* static */
void SynthesisEngine::PitchBend(uint8_t channel, uint16_t pitch_bend) {
  IGNORE_OTHER_CHANNELS;
  modulation_sources_[MOD_SRC_PITCH_BEND] = pitch_bend >> 6;
}

/* static */
void SynthesisEngine::AllSoundOff(uint8_t channel) {
  IGNORE_OTHER_CHANNELS;
  controller_.AllSoundOff();
}

/* static */
void SynthesisEngine::AllNotesOff(uint8_t channel) {
  IGNORE_OTHER_CHANNELS;
  controller_.AllNotesOff();
}

/* static */
void SynthesisEngine::ResetAllControllers(uint8_t channel) {
  IGNORE_OTHER_CHANNELS;
  modulation_sources_[MOD_SRC_PITCH_BEND] = 128;
  modulation_sources_[MOD_SRC_WHEEL] = 0;
}

// When in Omni mode, disable Omni and enable reception only on the channel on
// which this message has been received.
void SynthesisEngine::OmniModeOff(uint8_t channel) {
  IGNORE_OTHER_CHANNELS;
  patch_.kbd_midi_channel = channel + 1;
}

// Enable Omni mode.
void SynthesisEngine::OmniModeOn(uint8_t channel) {
  IGNORE_OTHER_CHANNELS;
  patch_.kbd_midi_channel = 0;
}

/* static */
void SynthesisEngine::Reset() {
  controller_.Reset();
  controller_.AllSoundOff();
  modulation_sources_[MOD_SRC_LFO_1] = 128;
  modulation_sources_[MOD_SRC_LFO_2] = 128;
  modulation_sources_[MOD_SRC_PITCH_BEND] = 128;
  modulation_sources_[MOD_SRC_WHEEL] = 0;
  modulation_sources_[MOD_SRC_ASSIGNABLE_1] = 0;
  modulation_sources_[MOD_SRC_ASSIGNABLE_2] = 0;
  lfo_[0].ResetPhase();
  lfo_[1].ResetPhase();
}

/* static */
void SynthesisEngine::Clock() {
  controller_.ExternalSync();
}

/* static */
void SynthesisEngine::SetParameter(
    uint8_t parameter_index,
    uint8_t parameter_value) {
  uint8_t* base = &patch_.keep_me_at_the_top;
  base[parameter_index + 1] = parameter_value;
  if (parameter_index >= PRM_ENV_ATTACK && parameter_index <= PRM_LFO_RATE_2) {
    UpdateModulationIncrements();
  }
  if ((parameter_index <= PRM_OSC_ALGORITHM_2) ||
      (parameter_index == PRM_MIX_SUB_OSC_ALGORITHM)) {
    UpdateOscillatorAlgorithms();
  }
  // A copy of those parameters is stored by the note dispatcher/arpeggiator,
  // so any parameter change must be forwarded to it.
  switch (parameter_index) {
    case PRM_ARP_TEMPO:
      controller_.SetTempo(parameter_value);
      break;
    case PRM_ARP_OCTAVES:
      controller_.SetOctaves(parameter_value);
      break;
    case PRM_ARP_PATTERN:
      controller_.SetPattern(parameter_value);
      break;
    case PRM_ARP_SWING:
      controller_.SetSwing(parameter_value);
      break;
  }
}

/* static */
void SynthesisEngine::UpdateOscillatorAlgorithms() {
  Osc1::SetupAlgorithm(patch_.osc_algorithm[0]);
  Osc2::SetupAlgorithm(patch_.osc_algorithm[1]);
  SubOsc::SetupAlgorithm(patch_.mix_sub_osc_algorithm);
}

/* static */
uint16_t SynthesisEngine::ScaleEnvelopeIncrement(uint8_t time, uint8_t scale) {
  uint16_t increment = ResourcesManager::Lookup<uint16_t, uint8_t>(
      lut_res_env_portamento_increments, time);
  increment = (uint32_t(increment) * scale) >> 8;
  if (increment == 0) {
    increment = 1;
  }
  return increment;
};

/* static */
void SynthesisEngine::UpdateModulationIncrements() {
  // Update the LFO increments.
  for (uint8_t i = 0; i < 2; ++i) {
    lfo_[i].Update(
        patch_.lfo_wave[i],
        ResourcesManager::Lookup<uint16_t, uint8_t>(
            lut_res_lfo_increments, patch_.lfo_rate[i]));
  }
    
  // Update the envelope increments and targets.
  envelope_increment_[ATTACK] = ScaleEnvelopeIncrement(
      patch_.env_attack, 127);
  envelope_increment_[DECAY] = -ScaleEnvelopeIncrement(
      patch_.env_decay,
      127 - patch_.env_sustain);
  envelope_target_[DECAY] = int16_t(patch_.env_sustain) << 7;
}

/* static */
void SynthesisEngine::Control() {
  for (uint8_t i = 0; i < 2; ++i) {
    lfo_[i].Increment();
    modulation_sources_[MOD_SRC_LFO_1 + i] = lfo_[i].Render();
  }
    
  // Update the arpeggiator / step sequencer.
  controller_.Control();
  
  // Read/shift the value of the step sequencer only when it has changed.
  modulation_sources_[MOD_SRC_SEQ] = patch_.sequence_step(controller_.step());
  modulation_sources_[MOD_SRC_STEP] = (
      controller_.has_arpeggiator_note() ? 255 : 0);
  
  voice_.Control();
}

/* static */
void SynthesisEngine::Audio() {
  oscillator_decimation_++;
  controller_.Audio();
  voice_.Audio();
}

/* <static> */
uint8_t Voice::envelope_stage_;
int16_t Voice::envelope_value_;
int16_t Voice::envelope_increment_;
int16_t Voice::envelope_target_;
int16_t Voice::pitch_increment_;
int16_t Voice::pitch_target_;
int16_t Voice::pitch_value_;
uint8_t Voice::modulation_sources_[MOD_SRC_GATE - MOD_SRC_ENV + 1];
int8_t Voice::modulation_destinations_[MOD_DST_FILTER_RESONANCE + 1];
uint8_t Voice::signal_;
uint8_t Voice::noise_sample_;
/* </static> */

/* static */
void Voice::Init() {
  pitch_value_ = 0;
  envelope_stage_ = DEAD;
  signal_ = 128;
}

/* static */
void Voice::TriggerEnvelope(uint8_t stage) {
  envelope_stage_ = stage;
  // The note might be released at any moment, so we need to figure out
  // the right slope to make it reach 0 within the release time.
  if (stage == RELEASE) {
    envelope_increment_ = -SynthesisEngine::ScaleEnvelopeIncrement(
        engine.patch_.env_release,
        envelope_value_ >> 7);
    if (envelope_increment_ == 0) {
      envelope_increment_ = -1;
    }
  } else {
    envelope_increment_ = engine.envelope_increment_[stage];
  }
  envelope_target_ = engine.envelope_target_[stage];
}

/* static */
void Voice::Trigger(uint8_t note, uint8_t velocity, uint8_t legato) {
  if (!legato) {
    TriggerEnvelope(ATTACK);
    Osc1::Reset();
    Osc2::Reset();
    SubOsc::Reset();
    modulation_sources_[MOD_SRC_VELOCITY - MOD_SRC_ENV] = velocity << 1;
  }
  pitch_target_ = uint16_t(note) << 7;
  if (engine.patch_.kbd_raga) {
    pitch_target_ += ResourcesManager::Lookup<int8_t, uint8_t>(
        ResourceId(LUT_RES_SCALE_JUST + engine.patch_.kbd_raga - 1),
        note % 12);
  }
  if (pitch_value_ == 0) {
    pitch_value_ = pitch_target_;
  }
  int16_t delta = pitch_target_ - pitch_value_;
  int32_t increment = ResourcesManager::Lookup<uint16_t, uint8_t>(
      lut_res_env_portamento_increments,
      engine.patch_.kbd_portamento);
  pitch_increment_ = (delta * increment) >> 15;
  if (pitch_increment_ == 0) {
    if (delta < 0) {
      pitch_increment_ = -1;
    } else {
      pitch_increment_ = 1;
    }
  }
}

/* static */
void Voice::Control() {
  // Update the envelopes.
  envelope_value_ += envelope_increment_;
  if ((envelope_stage_ == ATTACK) ^ (envelope_value_ < envelope_target_)) {
    envelope_value_ = envelope_target_;
    ++envelope_stage_;
    TriggerEnvelope(envelope_stage_);
  }
  pitch_value_ += pitch_increment_;
  if ((pitch_increment_ > 0) ^ (pitch_value_ < pitch_target_)) {
    pitch_value_ = pitch_target_;
    pitch_increment_ = 0;
  }
  
  // Used temporarily, then scaled to modulation_destinations_
  int16_t dst[kNumModulationDestinations];

  // Update the pre-scaled modulation sources.
  modulation_sources_[MOD_SRC_ENV - MOD_SRC_ENV] = 
      envelope_value_ >> 6;
  modulation_sources_[MOD_SRC_NOTE - MOD_SRC_ENV] =
      uint8_t(pitch_value_ >> 6);
  modulation_sources_[MOD_SRC_GATE - MOD_SRC_ENV] =
      envelope_stage_ >= RELEASE ? 0 : 255;

  // Prepare the work of the modulation matrix, by setting an initial / default
  // value to each modulated parameter.
  dst[MOD_DST_FILTER_CUTOFF] = engine.patch_.filter_cutoff << 7;
  dst[MOD_DST_VCA] = 255;
  dst[MOD_DST_PWM_1] = engine.patch_.osc_parameter[0] << 7;
  dst[MOD_DST_PWM_2] = engine.patch_.osc_parameter[1] << 7;
  // The connection from the pitch-bend to the oscillators is hardcoded.
  dst[MOD_DST_VCO_1] = 8192 - 1024 + (
      engine.modulation_sources_[MOD_SRC_PITCH_BEND] << 3);
  dst[MOD_DST_VCO_2] = dst[MOD_DST_VCO_1];  
  dst[MOD_DST_MIX_BALANCE] = engine.patch_.mix_balance << 7;
  dst[MOD_DST_MIX_NOISE] = engine.patch_.mix_noise << 7;
  dst[MOD_DST_MIX_SUB_OSC] = engine.patch_.mix_sub_osc << 7;
  dst[MOD_DST_FILTER_RESONANCE] = engine.patch_.filter_resonance << 7;
  
  // Apply the modulations in the modulation matrix + 2 hardcoded modulations
  // for the filter (envelope and LFO2).
  for (uint8_t i = 0; i < kModulationMatrixSize + 2; ++i) {
    uint8_t source;
    uint8_t destination = MOD_DST_FILTER_CUTOFF;
    uint8_t source_value;
    uint8_t amount;
    if (i < kModulationMatrixSize) {
      source = engine.patch_.modulation_matrix.modulation[i].source;
      destination = engine.patch_.modulation_matrix.modulation[i].destination;
      amount = engine.patch_.modulation_matrix.modulation[i].amount;
    } else if (i == kModulationMatrixSize) {
      source = MOD_SRC_ENV;
      amount = engine.patch_.filter_env;
    } else if (i == kModulationMatrixSize + 1) {
      source = MOD_SRC_LFO_2;
      amount = engine.patch_.filter_lfo;
    }
    if (amount == 0) {
      continue;
    }
    if (source <= MOD_SRC_ASSIGNABLE_2) {
      // Global sources, read from the engine.
      source_value = engine.modulation_sources_[source];
    } else {
      // Voice specific sources, read from the voice.
      source_value = modulation_sources_[source - MOD_SRC_ENV];
    }
    if (destination != MOD_DST_VCA) {
      int16_t modulation = dst[destination];
      modulation += Signal::MulScale1(source_value, amount);
      // For those sources, use relative modulation.
      if (source <= MOD_SRC_LFO_2 ||
          source == MOD_SRC_PITCH_BEND ||
          source == MOD_SRC_NOTE) {
        modulation -= amount << 6;
      }
      dst[destination] = Signal::Clip(modulation, 0, 16383);
    } else {
      // The VCA modulation is multiplicative, not additive. Yet another
      // Special case :(.
      dst[MOD_DST_VCA] = Signal::MulScale8(
          dst[MOD_DST_VCA],
          Signal::Mix(255, source_value, amount << 1));
    }
  }
  
  // Store in memory all the updated parameters.
  modulation_destinations_[MOD_DST_FILTER_CUTOFF] = \
      dst[MOD_DST_FILTER_CUTOFF] >> 6;

  modulation_destinations_[MOD_DST_FILTER_RESONANCE] = \
      dst[MOD_DST_FILTER_RESONANCE] >> 6;

  modulation_destinations_[MOD_DST_PWM_1] = \
      dst[MOD_DST_PWM_1] >> 7;

  modulation_destinations_[MOD_DST_PWM_2] = \
      dst[MOD_DST_PWM_2] >> 7;

  modulation_destinations_[MOD_DST_VCA] = dst[MOD_DST_VCA];
  modulation_destinations_[MOD_DST_MIX_BALANCE] = dst[MOD_DST_MIX_BALANCE] >> 6;
  modulation_destinations_[MOD_DST_MIX_NOISE] = dst[MOD_DST_MIX_NOISE] >> 8;
  modulation_destinations_[MOD_DST_MIX_SUB_OSC] = dst[MOD_DST_MIX_SUB_OSC] >> 7;
  
  // Update the oscillator parameters.
  for (uint8_t i = 0; i < 2; ++i) {
    int16_t pitch = pitch_value_;
    // -24 / +24 semitones by the range controller.
    pitch += (int16_t(engine.patch_.osc_range[i]) - 128) << 7;
    // -24 / +24 semitones by the main octave controller.
    pitch += int16_t(engine.patch_.kbd_octave - 128) * kOctave;
    if (i == 1) {
      // 0 / +1 semitones by the detune option for oscillator 2.
      pitch += engine.patch_.osc_option[1];
    }
    // -16 / +16 semitones by the routed modulations.
    pitch += (dst[MOD_DST_VCO_1 + i] - 8192) >> 2;

    // Wrap pitch to a reasonable range.
    while (pitch < kLowestNote) {
      pitch += kOctave;
    }
    while (pitch >= kHighestNote) {
      pitch -= kOctave;
    }
    // Extract the pitch increment from the pitch table.
    int16_t ref_pitch = pitch - kPitchTableStart;
    uint8_t num_shifts = 0;
    while (ref_pitch < 0) {
      ref_pitch += kOctave;
      ++num_shifts;
    }
    uint16_t increment = ResourcesManager::Lookup<uint16_t, uint16_t>(
        lut_res_oscillator_increments, ref_pitch >> 1);
    // Divide the pitch increment by the number of octaves we had to transpose
    // to get a value in the lookup table.
    increment >>= num_shifts;
    
    // Now the oscillators can recompute all their internal variables!
    if (i == 0) {
      Osc1::Update(modulation_destinations_[MOD_DST_PWM_1], pitch >> 7,
                   increment);
      SubOsc::Update(0, (pitch >> 7) - 12, increment >> 1);
    } else {
      Osc2::Update(modulation_destinations_[MOD_DST_PWM_2], pitch >> 7,
                   increment);
    }
  }
}

/* static */
void Voice::Audio() {
  if (dead()) {
    signal_ = 128;
    return;
  }
  
  // Update the phase accumulators for the oscillators;
  uint16_t previous_phase = Osc1::phase();

  uint8_t mix = Osc1::Render();
  
  uint8_t osc_2 = Osc2::Render();
  if (engine.patch_.osc_option[0] == RING_MOD) {
    mix = Signal::SignedSignedMulScale8(mix + 128, osc_2 + 128) + 128;
  }
  mix = Signal::Mix(mix, osc_2, modulation_destinations_[MOD_DST_MIX_BALANCE]);
  
  uint8_t sub_osc = SubOsc::Render();
  mix = Signal::Mix(mix, sub_osc,
                    modulation_destinations_[MOD_DST_MIX_SUB_OSC]);

  // Do not recompute the noise sample for every sample.
  if ((engine.oscillator_decimation() & 3) == 0) {
    noise_sample_ = Random::Byte();
  }
  mix = Signal::Mix(mix, noise_sample_,
                    modulation_destinations_[MOD_DST_MIX_NOISE]);

#ifdef SOFTWARE_VCA
  signal_ = Signal::SignedMulScale8(128 + mix
                                    modulation_destinations_[MOD_DST_VCA]);
  signal_ = signal_ + 128;
#else
  signal_ = mix;
#endif
 
  // If the phase of oscillator 1 has wrapped and if sync is enabled, reset the
  // phase of the second oscillator.
  if ((Osc1::phase() < previous_phase) && engine.patch_.osc_option[0] == SYNC) {
    Osc2::ResetPhase();
  }
}

}  // hardware_shruti