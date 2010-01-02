// Copyright 2009 Olivier Gillet.
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
// Main synthesis engine.

#include "hardware/shruti/shruti4/synthesis_engine.h"

#include <string.h>

#include "hardware/shruti/display.h"
#include "hardware/resources/resources_manager.h"
#include "hardware/shruti/shruti4/poly_oscillator.h"
#include "hardware/utils/random.h"
#include "hardware/utils/op.h"

using namespace hardware_utils_op;

namespace hardware_shruti {

/* extern */
SynthesisEngine engine;

typedef PolyOscillator Osc;

/* <static> */
uint8_t SynthesisEngine::modulation_sources_[kNumGlobalModulationSources];

uint8_t SynthesisEngine::oscillator_decimation_;

Patch SynthesisEngine::patch_;
Voice SynthesisEngine::voice_[kNumVoices];
VoiceController SynthesisEngine::controller_;
Lfo SynthesisEngine::lfo_[kNumLfos];
uint8_t SynthesisEngine::qux_[2];
uint8_t SynthesisEngine::nrpn_parameter_number_ = 0xff;
uint8_t SynthesisEngine::num_lfo_reset_steps_;
uint8_t SynthesisEngine::lfo_reset_counter_;
uint8_t SynthesisEngine::lfo_to_reset_;

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

static const prog_char empty_patch[] PROGMEM = {
    99,
    31, 16, 8, 4,
    31, 16, 0, 0,
    DRAWBAR_SQUARE, DRAWBAR_SAW, 31, WAVEFORM_SAW,
    110, 0, 10, 0,
    20, 0,
    60, 40,
    20, 80,
    60, 40,
    LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_TRIANGLE, 96, 3,
    MOD_SRC_LFO_1, MOD_DST_VCO, 0,
    MOD_SRC_LFO_1, MOD_DST_VCO_FINE, 0,
    MOD_SRC_LFO_1, MOD_DST_PWM, 0,
    MOD_SRC_LFO_1, MOD_DST_PWM, 0,
    MOD_SRC_LFO_2, MOD_DST_VCO_FINE, 0,
    // By default, the resonance tracks the note. This value was empirically
    // obtained and it is not clear whether it depends on the positive supply
    // voltage, and if it varies from chip to chip.
    MOD_SRC_NOTE, MOD_DST_FILTER_CUTOFF, 58,
    MOD_SRC_ENV_2, MOD_DST_VCA, 63,
    MOD_SRC_VELOCITY, MOD_DST_VCA, 16,
    MOD_SRC_PITCH_BEND, MOD_DST_VCO_FINE, 63,
    MOD_SRC_LFO_1, MOD_DST_VCO_FINE, 32,
    MOD_SRC_ASSIGNABLE_1, MOD_DST_PWM, 0,
    MOD_SRC_ASSIGNABLE_2, MOD_DST_FILTER_CUTOFF, 0,
    MOD_SRC_CV_1, MOD_DST_FILTER_CUTOFF, 0,
    MOD_SRC_CV_2, MOD_DST_FILTER_CUTOFF, 0,
    120, POLYPHONY_4_UNISON, OFF, OFF,
    0x00, 0x00, 0xff, 0xff, 0xcc, 0xcc, 0x44, 0x44,
    0, 0, 16, 1,
    'n', 'e', 'w', ' ', ' ', ' ', ' ', ' ', 16};

/* static */
void SynthesisEngine::ResetPatch() {
  ResourcesManager::Load(empty_patch, 0, &patch_);
  TouchPatch();
}

/* static */
void SynthesisEngine::NoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
  // If the note controller is not active, we are not currently playing a
  // sequence, so we retrigger the LFOs.
  if (!controller_.active()) {
    lfo_reset_counter_ = num_lfo_reset_steps_ - 1;
  }
  controller_.NoteOn(note, velocity);
#ifdef HAS_EASTER_EGG
  if (note - qux_[0] == ((0x29 | 0x15) >> 4)) {
    qux_[1] += ~0xfe;
  } else {
    qux_[1] ^= qux_[1];
  }
  qux_[0] = note;
#endif  // HAS_EASTER_EGG
}

/* static */
void SynthesisEngine::NoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
  controller_.NoteOff(note);
}

/* static */
void SynthesisEngine::ControlChange(uint8_t channel, uint8_t controller,
                                    uint8_t value) {
  switch (controller) {
    case hardware_midi::kModulationWheelMsb:
      modulation_sources_[MOD_SRC_WHEEL] = (value << 1);
      break;
    case hardware_midi::kDataEntryMsb:
      if (nrpn_parameter_number_ < sizeof(Patch) - 1) {
        SetParameter(nrpn_parameter_number_, value);
      }
      break;
#ifdef SHRUTI1
    case hardware_midi::kPortamentoTimeMsb:
      patch_.kbd_portamento = value;
      break;
#endif  // SHRUTI1
    case hardware_midi::kRelease:
      patch_.env_release[1] = value;
      break;
    case hardware_midi::kAttack:
      patch_.env_attack[1] = value;
      break;
    case hardware_midi::kHarmonicIntensity:
      patch_.filter_resonance = value;
      break;
    case hardware_midi::kBrightness:
      patch_.filter_cutoff = value;
      break;
    case hardware_midi::kNrpnMsb:
      nrpn_parameter_number_ = value;
      break;
  }
}

/* static */
uint8_t SynthesisEngine::CheckChannel(uint8_t channel) {
  return patch_.kbd_midi_channel == 0 ||
         patch_.kbd_midi_channel == (channel + 1);
}

/* static */
void SynthesisEngine::PitchBend(uint8_t channel, uint16_t pitch_bend) {
  modulation_sources_[MOD_SRC_PITCH_BEND] = ShiftRight6(pitch_bend);
}

/* static */
void SynthesisEngine::AllSoundOff(uint8_t channel) {
  controller_.AllSoundOff();
}

/* static */
void SynthesisEngine::AllNotesOff(uint8_t channel) {
  controller_.AllNotesOff();
}

/* static */
void SynthesisEngine::ResetAllControllers(uint8_t channel) {
  modulation_sources_[MOD_SRC_PITCH_BEND] = 128;
  modulation_sources_[MOD_SRC_WHEEL] = 0;
}

// When in Omni mode, disable Omni and enable reception only on the channel on
// which this message has been received.
/* static */
void SynthesisEngine::OmniModeOff(uint8_t channel) {
  patch_.kbd_midi_channel = channel + 1;
}

// Enable Omni mode.
/* static */
void SynthesisEngine::OmniModeOn(uint8_t channel) {
  patch_.kbd_midi_channel = 0;
}

/* static */
void SynthesisEngine::SysExStart() {
  patch_.SysExReceive(0xf0);
}

/* static */
void SynthesisEngine::SysExByte(uint8_t sysex_byte) {
  patch_.SysExReceive(sysex_byte);
}

/* static */
void SynthesisEngine::SysExEnd() {
  patch_.SysExReceive(0xf7);
}

/* static */
void SynthesisEngine::Reset() {
  controller_.Reset();
  controller_.AllSoundOff();
  memset(modulation_sources_, 0, kNumGlobalModulationSources);
  modulation_sources_[MOD_SRC_PITCH_BEND] = 128;
  for (uint8_t i = 0; i < kNumLfos; ++i) {
    lfo_[i].Reset();
  }
}

/* static */
void SynthesisEngine::Clock() {
  controller_.ExternalSync();
}

/* static */
void SynthesisEngine::Start() {
  controller_.Start();
}

/* static */
void SynthesisEngine::Stop() {
  controller_.Stop();
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
  if (parameter_index == PRM_OSC_SHAPE) {
    UpdateOscillatorAlgorithms();
  }
  // A copy of those parameters is stored by the note dispatcher/arpeggiator,
  // so any parameter change must be forwarded to it.
  switch (parameter_index) {
    case PRM_ARP_TEMPO:
      controller_.SetTempo(parameter_value);
      UpdateModulationIncrements();
      break;
    case PRM_ARP_PATTERN_SIZE:
      controller_.set_pattern_size(parameter_value);
      break;
  }
}

/* static */
void SynthesisEngine::UpdateOscillatorAlgorithms() {
  Osc::SetupAlgorithm(patch_.osc_shape);
}

/* static */
void SynthesisEngine::UpdateModulationIncrements() {
  // Update the LFO increments.
  num_lfo_reset_steps_ = 0;
  lfo_to_reset_ = 0;
  for (uint8_t i = 0; i < kNumLfos; ++i) {
    uint16_t increment;
    // The LFO rates 0 to 15 are translated into a multiple of the step
    // sequencer/arpeggiator step size.
    if (patch_.lfo_rate[i] < 16) {
      increment = 65536 / (controller_.estimated_beat_duration() *
                           (1 + patch_.lfo_rate[i]) / 4);
      num_lfo_reset_steps_ = UnsignedUnsignedMul(
          num_lfo_reset_steps_ ? num_lfo_reset_steps_ : 1,
          1 + patch_.lfo_rate[i]);
      lfo_to_reset_ |= _BV(i);
    } else {
      increment = ResourcesManager::Lookup<uint16_t, uint8_t>(
          lut_res_lfo_increments, patch_.lfo_rate[i] - 16);
    }
    lfo_[i].Update(patch_.lfo_wave[i], increment);

    for (uint8_t j = 0; j < kNumVoices; ++j) {
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
  for (uint8_t i = 0; i < kNumLfos; ++i) {
    lfo_[i].Increment();
    modulation_sources_[MOD_SRC_LFO_1 + i] = lfo_[i].Render();
  }
  modulation_sources_[MOD_SRC_RANDOM] = Random::state_msb();

  // Update the arpeggiator / step sequencer.
  if (controller_.Control()) {
    // We need to do a couple of things when the step sequencer advances to the
    // next step:
    // - periodically (eg whenever we move to step 0), recompute the LFO
    // increments from the tempo, (if the LFO follows the tempo), since the
    // tempo might have been modified by the user or the rate of the MIDI clock
    // messages.
    // - Reset the LFO value to 0 every n-th step. Otherwise, there might be a
    // "synchronization drift" because of rounding errors.
    ++lfo_reset_counter_;
    if (lfo_reset_counter_ == num_lfo_reset_steps_) {
      UpdateModulationIncrements();
      for (uint8_t i = 0; i < kNumLfos; ++i) {
        if (lfo_to_reset_ & _BV(i)) {
          lfo_[i].Reset();
        }
      }
      lfo_reset_counter_ = 0;
    }
  }
  
  // Read/shift the value of the step sequencer.
  modulation_sources_[MOD_SRC_SEQ] = patch_.sequence_step(controller_.step());
#ifdef SHRUTI1
  modulation_sources_[MOD_SRC_STEP] = (
      controller_.has_arpeggiator_note() ? 255 : 0);
#else
  modulation_sources_[MOD_SRC_STEP] = (controller_.step() & 0x01 ? 0 : 255);
#endif
  for (uint8_t i = 0; i < kNumVoices; ++i) {
    voice_[i].Control();
  }
}

/* static */
void SynthesisEngine::Audio() {
  // Tick the noise generator.
  oscillator_decimation_ = (oscillator_decimation_ + 1) & 3;
  if (!oscillator_decimation_) {
    Random::Update();
  }
  
  controller_.Audio();
  for (uint8_t i = 0; i < kNumVoices; ++i) {
    voice_[i].Audio();
  }
}

/* <static> */
Envelope Voice::envelope_[2];
uint8_t Voice::dead_;
int16_t Voice::pitch_[kPolyphony];
uint8_t Voice::fast_slop_[kPolyphony];
uint8_t Voice::slow_slop_[kPolyphony];
uint8_t Voice::slop_prescaler_;
uint8_t Voice::modulation_sources_[kNumVoiceModulationSources];
int8_t Voice::modulation_destinations_[kNumModulationDestinations];
uint8_t Voice::signal_;
/* </static> */

/* static */
void Voice::Init() {
  for (uint8_t i = 0; i < kPolyphony; ++i) {
    pitch_[i] = 0; 
    slow_slop_[i] = Random::GetByte();
    fast_slop_[i] = Random::GetByte();
  }
  signal_ = 128;
  for (uint8_t i = 0; i < kNumEnvelopes; ++i) {
    envelope_[i].Init();
  }
}

/* static */
void Voice::TriggerEnvelope(uint8_t stage) {
  for (uint8_t i = 0; i < kNumEnvelopes; ++i) {
    envelope_[i].Trigger(stage);
  }
}

/* static */
void Voice::Trigger(const NoteStack& notes, bool note_on) {
  // When to retrigger the envelope? When this is a note-on, with legato OFF or
  // no pre-existing notes.
  if (note_on && (notes.size() == 1 || !engine.patch_.key_legato)) {
    TriggerEnvelope(ATTACK);
  }
  memset(pitch_, 0, sizeof(uint16_t) * kPolyphony);
  uint8_t unison = engine.patch_.key_num_voices >= POLYPHONY_1_UNISON;
  uint8_t num_notes = engine.patch_.key_num_voices;
  if (unison) {
    num_notes -= POLYPHONY_1_UNISON;
  }
  num_notes = 1 << num_notes;
  num_notes = notes.size() < num_notes ? notes.size() : num_notes;

  const NoteEntry* note_entry = &notes.most_recent_note();
  uint16_t velocity;
  for (uint8_t i = 0; i < num_notes; ++i) {
    pitch_[i] = static_cast<uint16_t>(note_entry->note) << 7;
    velocity += note_entry->velocity;
    if (engine.patch_.kbd_raga) {
      pitch_[i] += ResourcesManager::Lookup<int8_t, uint8_t>(
          ResourceId(LUT_RES_SCALE_JUST + engine.patch_.kbd_raga - 1),
          note_entry->note % 12);
    }
    if (unison) {
      for (uint8_t j = i; j < kPolyphony; j += num_notes) {
        pitch_[j] = pitch_[i];
      }
    }
    note_entry = &notes.note(note_entry->next_ptr);
  }
  if (num_notes) {
    modulation_sources_[MOD_SRC_VELOCITY - kNumGlobalModulationSources] =
        (velocity / num_notes) << 1;
  }
}

/* static */
void Voice::Control() {
  // Update the envelopes.
  dead_ = 1;
  for (uint8_t i = 0; i < kNumEnvelopes; ++i) {
    envelope_[i].Render();
    dead_ = dead_ && envelope_[i].dead();
  }
  ++slop_prescaler_;
  if (!slop_prescaler_) {
    for (uint8_t i = 0; i < kPolyphony; ++i) {
      slow_slop_[i] = Mix(slow_slop_[i], Random::GetByte(), 10);
    }
  }
  
  // Used temporarily, then scaled to modulation_destinations_. This does not
  // need to be static, but if allocated on the heap, we get many push/pops,
  // and the resulting code is slower.
  static int16_t dst[kNumModulationDestinations];

  // Rescale the value of each modulation sources. Envelopes are in the
  // 0-16383 range ; just like pitch. All are scaled to 0-255.
  modulation_sources_[MOD_SRC_ENV_1 - kNumGlobalModulationSources] = 
      ShiftRight6(envelope_[0].value());
  modulation_sources_[MOD_SRC_ENV_2 - kNumGlobalModulationSources] = 
      ShiftRight6(envelope_[1].value());
  modulation_sources_[MOD_SRC_NOTE - kNumGlobalModulationSources] =
      ShiftRight6(pitch_[0]);
  modulation_sources_[MOD_SRC_GATE - kNumGlobalModulationSources] =
      envelope_[0].stage() >= RELEASE ? 0 : 255;
      
  modulation_destinations_[MOD_DST_VCA] = 255;

  // Load and scale to 0-16383 the initial value of each modulated parameter.
  dst[MOD_DST_FILTER_CUTOFF] = engine.patch_.filter_cutoff << 7;
  dst[MOD_DST_SLOP] = engine.patch_.key_slop << 7;
  dst[MOD_DST_PWM] = 0;
  dst[MOD_DST_VCO] = 8192;
  dst[MOD_DST_VCO_FINE] = 8192;
  dst[MOD_DST_FILTER_RESONANCE] = engine.patch_.filter_resonance << 8;
  
  // Apply the modulations in the modulation matrix.
  for (uint8_t i = 0; i < kModulationMatrixSize; ++i) {
    int8_t amount = engine.patch_.modulation_matrix.modulation[i].amount;
    if (!amount) {
      continue;
    }

    // The last modulation amount is adjusted by the wheel.
    if (i == kSavedModulationMatrixSize - 1) {
      amount = SignedMulScale8(
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
      modulation += SignedUnsignedMul(amount, source_value);
      // For those sources, use relative modulation.
      if (source <= MOD_SRC_LFO_2 ||
          source == MOD_SRC_PITCH_BEND ||
          source == MOD_SRC_NOTE) {
        modulation -= amount << 7;
      }
      dst[destination] = Clip(modulation, 0, 16383);
    } else {
      // The VCA modulation is multiplicative, not additive. Yet another
      // Special case :(.
      if (amount < 0) {
        amount = -amount;
        source_value = 255 - source_value;
      }
      modulation_destinations_[MOD_DST_VCA] = MulScale8(
          modulation_destinations_[MOD_DST_VCA],
          Mix(255, source_value, amount << 2));
    }
  }
  // Hardcoded filter modulations.
  dst[MOD_DST_FILTER_CUTOFF] = Clip(
      dst[MOD_DST_FILTER_CUTOFF] + SignedUnsignedMul(
          engine.patch_.filter_env,
          modulation_sources_[MOD_SRC_ENV_1 - kNumGlobalModulationSources]),
      0,
      16383);
  dst[MOD_DST_FILTER_CUTOFF] = Clip(
      dst[MOD_DST_FILTER_CUTOFF] + SignedUnsignedMul(
          engine.patch_.filter_lfo,
          engine.modulation_sources_[MOD_SRC_LFO_2]) -
      (engine.patch_.filter_lfo << 7),
      0,
      16383);
  
  // Store in memory all the updated parameters.
  modulation_destinations_[MOD_DST_FILTER_CUTOFF] = ShiftRight6(
      dst[MOD_DST_FILTER_CUTOFF]);

  modulation_destinations_[MOD_DST_FILTER_RESONANCE] = ShiftRight6(
      dst[MOD_DST_FILTER_RESONANCE]);

  modulation_destinations_[MOD_DST_PWM] = dst[MOD_DST_PWM] >> 7;
  modulation_destinations_[MOD_DST_SLOP] = dst[MOD_DST_SLOP] >> 7;
  
  // Update the oscillator parameters.
  for (uint8_t i = 0; i < kPolyphony; ++i) {
    int16_t pitch = pitch_[i];
    uint16_t increment;
    if (pitch != 0) {
      // -24 / +24 semitones by the main octave controller.
      pitch += static_cast<int16_t>(engine.patch_.kbd_octave) * kOctave;
    
      // -16 / +16 semitones by the routed modulations.
      pitch += (dst[MOD_DST_VCO] - 8192) >> 2;
    
      // -1 / +1 semitones by the vibrato and pitch bend.
      pitch += (dst[MOD_DST_VCO_FINE] - 8192) >> 6;
      // -1 / +1 semitones by the oscillators detuning.
      fast_slop_[i] = Mix(fast_slop_[i], Random::GetByte(), 10);
    
      int16_t slop = SignedUnsignedMul(fast_slop_[i] - 128, 12) +
          SignedUnsignedMul(slow_slop_[i] - 128, 12);
      slop = Clip(slop, -255, 255);
      slop = SignedMulScale8(slop, dst[MOD_DST_SLOP]);
      pitch += slop;
    
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
      increment = ResourcesManager::Lookup<uint16_t, uint16_t>(
          lut_res_oscillator_increments, ref_pitch >> 1);
      // Divide the pitch increment by the number of octaves we had to transpose
      // to get a value in the lookup table.
      increment >>= num_shifts;
    
      // Now the oscillators can recompute all their internal variables!
      pitch >>= 7;
    } else {
      increment = 0;
    }
    Osc::UpdateVoicePitch(i, pitch, increment);
  }
  Osc::UpdateParameter(modulation_destinations_[MOD_DST_PWM]);
}

/* static */
void Voice::Audio() {
  if (dead_) {
    signal_ = 128;
    return;
  }
  signal_ = Osc::Render();
}

}  // namespace hardware_shruti
