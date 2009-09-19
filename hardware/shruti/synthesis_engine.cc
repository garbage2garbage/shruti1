// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Main synthesis engine.

#include "hardware/shruti/synthesis_engine.h"

#include <string.h>

#include "hardware/resources/resources_manager.h"
#include "hardware/shruti/oscillator.h"
#include "hardware/utils/random.h"
#include "hardware/utils/op.h"

using hardware_utils::Op;

namespace hardware_shruti {

/* extern */
SynthesisEngine engine;

typedef Oscillator<1, FULL> Osc1;
typedef Oscillator<2, LOW_COMPLEXITY> Osc2;
typedef Oscillator<3, SUB_OSCILLATOR> SubOsc;

/* <static> */
uint8_t SynthesisEngine::modulation_sources_[kNumGlobalModulationSources];

uint8_t SynthesisEngine::oscillator_decimation_;

Patch SynthesisEngine::patch_;
Voice SynthesisEngine::voice_[kNumVoices];
VoiceController SynthesisEngine::controller_;
Lfo SynthesisEngine::lfo_[2];
uint8_t SynthesisEngine::qux_[2];
/* </static> */

/* static */
void SynthesisEngine::Init() {
  controller_.Init(voice_, kNumVoices);
  ResetPatch();
  Reset();
  for (uint8_t i = 0; i < kNumVoices; ++i) {
    voice_[i].Init();
  }
}

void SynthesisEngine::SysExStart() {
  patch_.SysExReceive(0xf0);
}

void SynthesisEngine::SysExByte(uint8_t sysex_byte) {
  patch_.SysExReceive(sysex_byte);
}

void SynthesisEngine::SysExEnd() {
  patch_.SysExReceive(0xf7);
}

static const prog_char empty_patch[] PROGMEM = {
    99,
    WAVEFORM_SAW, WAVEFORM_SQUARE, 0, 50,
    0, 0, 0, 0,
    0, 0, 0, WAVEFORM_SQUARE,
    120, 0, 0, 0,
    20, 0,
    60, 40,
    20, 100,
    60, 40,
    LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_TRIANGLE, 80, 24,
    MOD_SRC_LFO_1, MOD_DST_VCO_1, 0,
    MOD_SRC_LFO_1, MOD_DST_VCO_2, 0,
    MOD_SRC_LFO_2, MOD_DST_PWM_1, 0,
    MOD_SRC_LFO_2, MOD_DST_PWM_2, 0,
    MOD_SRC_LFO_2, MOD_DST_MIX_BALANCE, 0,
    MOD_SRC_NOTE, MOD_DST_FILTER_CUTOFF, 23,
    MOD_SRC_ENV_2, MOD_DST_VCA, 63,
    MOD_SRC_VELOCITY, MOD_DST_VCA, 16,
    MOD_SRC_PITCH_BEND, MOD_DST_VCO_1_2_FINE, 32,
    MOD_SRC_LFO_1, MOD_DST_VCO_1_2_FINE, 16,
    MOD_SRC_ASSIGNABLE_1, MOD_DST_PWM_1, 0,
    MOD_SRC_ASSIGNABLE_2, MOD_DST_FILTER_CUTOFF, 0,
    MOD_SRC_CV_1, MOD_DST_FILTER_CUTOFF, 0,
    MOD_SRC_CV_2, MOD_DST_FILTER_CUTOFF, 0,
    120, 0, 0, 0,
    0x00, 0x00, 0xff, 0xff, 0xcc, 0xcc, 0x44, 0x44,
    0, 0, 0, 1,
    'n', 'e', 'w', ' ', ' ', ' ', ' ', ' ', 16};


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
  if (note - qux_[0] == ((0x29 | 0x15) >> 4)) {
    qux_[1] += ~0xfe;
  } else {
    qux_[1] ^= qux_[1];
  }
  qux_[0] = note;
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
  memset(modulation_sources_, 0, kNumGlobalModulationSources);
  modulation_sources_[MOD_SRC_PITCH_BEND] = 128;
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
  if (parameter_index >= PRM_ENV_ATTACK_1 &&
      parameter_index <= PRM_LFO_RATE_2) {
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
    case PRM_ARP_PATTERN_SIZE:
      controller_.set_pattern_size(parameter_value);
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
void SynthesisEngine::UpdateModulationIncrements() {
  // Update the LFO increments.
  for (uint8_t i = 0; i < 2; ++i) {
    lfo_[i].Update(
        patch_.lfo_wave[i],
        ResourcesManager::Lookup<uint16_t, uint8_t>(
            lut_res_lfo_increments, patch_.lfo_rate[i]));
    for (uint8_t j = 0; j < 1; ++j) {
      voice_[j].mutable_envelope(i)->Update(
          patch_.env_attack[i],
          patch_.env_decay[i],
          patch_.env_sustain[i],
          patch_.env_release[i]);
    }
  }
}

/* static */
void SynthesisEngine::Control() {
  for (uint8_t i = 0; i < 2; ++i) {
    lfo_[i].Increment();
    modulation_sources_[MOD_SRC_LFO_1 + i] = lfo_[i].Render();
  }
  modulation_sources_[MOD_SRC_RANDOM] = Random::state_msb();

  // Update the arpeggiator / step sequencer.
  controller_.Control();
  
  // Read/shift the value of the step sequencer only when it has changed.
  modulation_sources_[MOD_SRC_SEQ] = patch_.sequence_step(controller_.step());
  modulation_sources_[MOD_SRC_STEP] = (
      controller_.has_arpeggiator_note() ? 255 : 0);
  
  for (uint8_t i = 0; i < kNumVoices; ++i) {
    voice_[i].Control();
  }
}

/* static */
void SynthesisEngine::Audio() {
  oscillator_decimation_++;
  oscillator_decimation_ &= 3;
  controller_.Audio();
  for (uint8_t i = 0; i < kNumVoices; ++i) {
    voice_[i].Audio();
  }
}

/* <static> */
Envelope Voice::envelope_[2];
uint8_t Voice::dead_;
int16_t Voice::pitch_increment_;
int16_t Voice::pitch_target_;
int16_t Voice::pitch_value_;
uint8_t Voice::modulation_sources_[kNumVoiceModulationSources];
int8_t Voice::modulation_destinations_[kNumModulationDestinations];
uint8_t Voice::signal_;
uint8_t Voice::noise_sample_;
/* </static> */

/* static */
void Voice::Init() {
  pitch_value_ = 0;
  signal_ = 128;
  envelope_[0].Init();
  envelope_[1].Init();
}

/* static */
void Voice::TriggerEnvelope(uint8_t stage) {
  envelope_[0].Trigger(stage);
  envelope_[1].Trigger(stage);
}

/* static */
void Voice::Trigger(uint8_t note, uint8_t velocity, uint8_t legato) {
  if (!legato) {
    TriggerEnvelope(ATTACK);
    Osc1::Reset();
    Osc2::Reset();
    SubOsc::Reset();
    modulation_sources_[MOD_SRC_VELOCITY - kNumGlobalModulationSources] = velocity << 1;
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
  dead_ = 1;
  for (uint8_t i = 0; i < 2; ++i) {
    envelope_[i].Render();
    dead_ = dead_ && envelope_[i].dead();
  }
  
  pitch_value_ += pitch_increment_;
  if ((pitch_increment_ > 0) ^ (pitch_value_ < pitch_target_)) {
    pitch_value_ = pitch_target_;
    pitch_increment_ = 0;
  }
  
  // Used temporarily, then scaled to modulation_destinations_. This does not
  // need to be static, but if allocated on the heap, we get many push/pops.
  static int16_t dst[kNumModulationDestinations];

  // Update the pre-scaled modulation sources.
  modulation_sources_[MOD_SRC_ENV_1 - kNumGlobalModulationSources] = 
      envelope_[0].value() >> 6;
  modulation_sources_[MOD_SRC_ENV_2 - kNumGlobalModulationSources] = 
      envelope_[1].value() >> 6;
  modulation_sources_[MOD_SRC_NOTE - kNumGlobalModulationSources] =
      uint8_t(pitch_value_ >> 6);
  modulation_sources_[MOD_SRC_GATE - kNumGlobalModulationSources] =
      envelope_[0].stage() >= RELEASE ? 0 : 255;

  // Prepare the work of the modulation matrix, by setting an initial / default
  // value to each modulated parameter.
  dst[MOD_DST_FILTER_CUTOFF] = engine.patch_.filter_cutoff << 7;
  dst[MOD_DST_VCA] = 255;
  dst[MOD_DST_PWM_1] = engine.patch_.osc_parameter[0] << 7;
  dst[MOD_DST_PWM_2] = engine.patch_.osc_parameter[1] << 7;
  dst[MOD_DST_VCO_1_2_FINE] = dst[MOD_DST_VCO_2] = dst[MOD_DST_VCO_1] = 8192;
  dst[MOD_DST_MIX_BALANCE] = engine.patch_.mix_balance << 7;
  dst[MOD_DST_MIX_NOISE] = engine.patch_.mix_noise << 7;
  dst[MOD_DST_MIX_SUB_OSC] = engine.patch_.mix_sub_osc << 7;
  dst[MOD_DST_FILTER_RESONANCE] = engine.patch_.filter_resonance << 7;
  
  // Apply the modulations in the modulation matrix.
  for (uint8_t i = 0; i < kModulationMatrixSize; ++i) {
    int8_t amount = engine.patch_.modulation_matrix.modulation[i].amount;
    if (amount == 0) {
      continue;
    }
    // The last modulation amount is adjusted by the wheel.
    if (i == kSavedModulationMatrixSize - 1) {
      amount = Op::SignedMulScale8(
          amount,
          engine.modulation_sources_[MOD_SRC_WHEEL]);
    }
    uint8_t source = engine.patch_.modulation_matrix.modulation[i].source;
    uint8_t destination =
        engine.patch_.modulation_matrix.modulation[i].destination;
    uint8_t source_value;

    if (source < kNumGlobalModulationSources) {
      // Global sources, read from the engine.
      source_value = engine.modulation_sources_[source];
    } else {
      // Voice specific sources, read from the voice.
      source_value = modulation_sources_[source - kNumGlobalModulationSources];
    }
    if (destination != MOD_DST_VCA) {
      int16_t modulation = dst[destination];
      modulation += Op::SignedUnsignedMul(amount, source_value);
      // For those sources, use relative modulation.
      if (source <= MOD_SRC_LFO_2 ||
          source == MOD_SRC_PITCH_BEND ||
          source == MOD_SRC_NOTE) {
        modulation -= amount << 7;
      }
      dst[destination] = Op::Clip(modulation, 0, 16383);
    } else {
      // The VCA modulation is multiplicative, not additive. Yet another
      // Special case :(.
      if (amount < 0) {
        amount = -amount;
        source_value = 255 - source_value;
      }
      dst[MOD_DST_VCA] = Op::MulScale8(
          dst[MOD_DST_VCA],
          Op::Mix(255, source_value, amount << 2));
    }
  }
  // Hardcoded filter modulations.
  dst[MOD_DST_FILTER_CUTOFF] = Op::Clip(
      dst[MOD_DST_FILTER_CUTOFF] + Op::SignedUnsignedMul(
          engine.patch_.filter_env,
          modulation_sources_[MOD_SRC_ENV_1 - kNumGlobalModulationSources]),
      0,
      16383);
  dst[MOD_DST_FILTER_CUTOFF] = Op::Clip(
      dst[MOD_DST_FILTER_CUTOFF] + Op::SignedUnsignedMul(
          engine.patch_.filter_lfo,
          engine.modulation_sources_[MOD_SRC_LFO_2]) -
      (engine.patch_.filter_lfo << 7),
      0,
      16383);
  
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
    if (i == 0 && engine.patch_.osc_algorithm[0] == WAVEFORM_FM) {
      Osc1::UpdateSecondaryParameter(engine.patch_.osc_range[i] + 12);
    } else {
      pitch += int16_t(engine.patch_.osc_range[i]) << 7;
    }
    // -24 / +24 semitones by the main octave controller.
    pitch += int16_t(engine.patch_.kbd_octave) * kOctave;
    if (i == 1) {
      // 0 / +1 semitones by the detune option for oscillator 2.
      pitch += engine.patch_.osc_option[1];
    }
    // -16 / +16 semitones by the routed modulations.
    pitch += (dst[MOD_DST_VCO_1 + i] - 8192) >> 2;
    // -4 / +4 semitones by the vibrato and pitch bend.
    pitch += (dst[MOD_DST_VCO_1_2_FINE] - 8192) >> 4;

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
    pitch >>= 7;
    if (i == 0) {
      Osc1::Update(modulation_destinations_[MOD_DST_PWM_1], pitch, increment);
      SubOsc::Update(0, pitch - 12, increment >> 1);
    } else {
      Osc2::Update(modulation_destinations_[MOD_DST_PWM_2], pitch, increment);
    }
  }
}

/* static */
void Voice::Audio() {
  if (dead_) {
    signal_ = 128;
    return;
  }

  // Do not recompute the noise sample for every sample.
  if (engine.oscillator_decimation() == 0) {
    Random::Update();
  }
  
  // Update the phase accumulators for the oscillators;
  uint16_t previous_phase = Osc1::phase();

  uint8_t mix = Osc1::Render();
  
  uint8_t osc_2 = Osc2::Render();
  if (engine.patch_.osc_option[0] == RING_MOD) {
    mix = Op::SignedSignedMulScale8(mix + 128, osc_2 + 128) + 128;
  }
  mix = Op::Mix(
      mix,
      osc_2,
      modulation_destinations_[MOD_DST_MIX_BALANCE]);
  
  if (engine.patch_.osc_algorithm[0] != WAVEFORM_SPEECH) {
    uint8_t sub_osc = SubOsc::Render();
    mix = Op::Mix(
        mix,
        sub_osc,
        modulation_destinations_[MOD_DST_MIX_SUB_OSC]);

    mix = Op::Mix(
        mix,
        Random::state_msb(),
        modulation_destinations_[MOD_DST_MIX_NOISE]);
  }

#ifdef SOFTWARE_VCA
  signal_ = Op::SignedMulScale8(
      128 + mix
      modulation_destinations_[MOD_DST_VCA]) + 128;
#else
  signal_ = mix;
#endif

  // If the phase of oscillator 1 has wrapped and if sync is enabled, reset the
  // phase of the second oscillator.
  if (engine.patch_.osc_option[0] == SYNC && Osc1::phase() < previous_phase) {
    Osc2::ResetPhase();
  }
}

}  // hardware_shruti
