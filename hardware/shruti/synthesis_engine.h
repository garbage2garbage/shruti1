// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Main synthesis engine. Everything is in static methods because this allows
// the compiler to compute in advance the address of all objects, and use direct
// addressing like:
// lds r18, 0x04e4
//
// instead of ugly indirect addressing like:
// subi	r28, 0xF1
// sbci	r29, 0xFD
// ld	r18, Y
// ldd	r19, Y+1	; 0x01
//
// As a result, the code looks ugly. Sorry... At least, I applied this
// "optimization" here only.

#ifndef HARDWARE_SHRUTI_SYNTHESIS_ENGINE_H_
#define HARDWARE_SHRUTI_SYNTHESIS_ENGINE_H_

#include "hardware/shruti/shruti.h"

#include "hardware/midi/midi.h"
#include "hardware/shruti/lfo.h"
#include "hardware/shruti/parameters.h"
#include "hardware/shruti/patch.h"
#include "hardware/shruti/voice_controller.h"

namespace hardware_shruti {

// Used for MIDI -> oscillator increment conversion.
static const uint16_t kLowestNote = 24 * 128;
static const uint16_t kOctave = 12 * 128;
static const uint16_t kPitchTableStart = 96 * 128;
static const uint16_t kHighestNote = 108 * 128;

enum EnvelopeStage {
  ATTACK = 0,
  DECAY = 1,
  SUSTAIN = 2,
  RELEASE = 3,
  DEAD = 4
};

class Voice {
 public:
  Voice() { };
  static void Init();

  // Called whenever a new note is played, manually or through the arpeggiator.
  static void Trigger(uint8_t note, uint8_t velocity, uint8_t legato);

  // Move this voice to the release stage.
  static void Release() { TriggerEnvelope(RELEASE); }

  // Move this voice to the release stage.
  static void Kill() { envelope_stage_ = DEAD; }

  static void Audio();
  static void Control();

  // Called whenever a write to the CV analog outputs has to be made.
  static inline uint8_t vcf()  {
    return modulation_destinations_[MOD_DST_FILTER_CUTOFF];
  }
  static inline uint8_t vca()  {
    return modulation_destinations_[MOD_DST_VCA];
  }
  static inline uint8_t resonance()  {
    return modulation_destinations_[MOD_DST_FILTER_RESONANCE];
  }
  static inline uint8_t dead()  { return envelope_stage_ == DEAD; }
  static inline uint8_t signal()  { return signal_; }
  static inline uint8_t modulation_source(uint8_t i) {
    return modulation_sources_[i - MOD_SRC_ENV];
  }
  
  static void TriggerEnvelope(uint8_t envelope);
  
 private:
  // Counters/phases for the envelope generator.
  static uint8_t envelope_stage_;
  static int16_t envelope_value_;
  static int16_t envelope_increment_;
  static int16_t envelope_target_;

  // Counters/phases for the pitch envelope generator (portamento).
  // Pitches are stored on 14 bits, the 7 highest bits are the MIDI note value,
  // the 7 lowest bits are used for fine-tuning.
  static int16_t pitch_increment_;
  static int16_t pitch_target_;
  static int16_t pitch_value_;

  // The voice-specific modulation sources are from MOD_SRC_ENV to
  // MOD_SRC_GATE.
  static uint8_t modulation_sources_[MOD_SRC_GATE - MOD_SRC_ENV + 1];

  // Value of all the stuff controlled by the modulators, scaled to the value
  // they will be used for. MOD_DST_FILTER_RESONANCE is the last entry
  // in the modulation destinations enum.
  static int8_t modulation_destinations_[MOD_DST_FILTER_RESONANCE + 1];
  
  static uint8_t signal_;
  
  DISALLOW_COPY_AND_ASSIGN(Voice);
};

class SynthesisEngine : public hardware_midi::MidiDevice {
  friend class Voice;

 public:
  SynthesisEngine() { };
  static void Init();

  // Forwarded to the controller.
  static void NoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
  static void NoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
  
  // Handled.
  static void ControlChange(uint8_t channel, uint8_t controller, uint8_t value);
  static void PitchBend(uint8_t channel, uint16_t pitch_bend);
  static void AllSoundOff(uint8_t channel);
  static void ResetAllControllers(uint8_t channel);
  static void AllNotesOff(uint8_t channel);
  static void OmniModeOff(uint8_t channel);
  static void OmniModeOn(uint8_t channel);
  static void Reset();
  static void Clock();
  
  static void Audio();
  static void Control();
  
  // Patch manipulation stuff.
  static void SetParameter(uint8_t parameter_index, uint8_t parameter_value);
  static inline uint8_t GetParameter(uint8_t parameter_index) {
    const uint8_t* base = &patch_.keep_me_at_the_top;
    return base[parameter_index + 1];
  }
  static uint8_t sequence_step(uint8_t step) {
    return patch_.sequence_step(step);
  }
  static void set_sequence_step(uint8_t step, uint8_t value) {
    patch_.set_sequence_step(step, value);
  }
  static void set_assignable_controller(uint8_t controller, uint8_t value) {
    modulation_sources_[MOD_SRC_ASSIGNABLE_1 + controller] = value;
  }
  static void ResetPatch();
  // Variables dependent on parameters (increments) are recomputed in
  // SetParameter when the related parameter is modified. Sometimes, the patch
  // is modified all at once without any call to SetParameter (for example when
  // loading a patch from the EEPROM)... so in this case we need to recompute
  // all the related variables.
  static inline void TouchPatch() {
    RecomputeModulationIncrements();
    UpdateOscillatorAlgorithms();
  }
  static void set_patch(const uint8_t* packed_patch) {
    patch_.Unpack(packed_patch);
  }
  static inline const Patch& patch() { return patch_; }
  static inline Patch* mutable_patch() { return &patch_; }
  
  // These variables are sent to I/O pins, and are made accessible here.
  static inline uint8_t vcf() { return voice_.vcf(); }
  static inline uint8_t vca() { return voice_.vca(); }
  static inline uint8_t resonance() { return voice_.resonance(); }
  static inline uint8_t signal() { return voice_.signal(); }
  static inline uint8_t modulation_source(uint8_t i) {
    if (i < MOD_SRC_ENV) {
      return modulation_sources_[i];
    } else {
      return voice_.modulation_source(i);
    }
  }
  
 private:
  // Increment and stage info for the envelopes.
  static int16_t envelope_increment_[4];
  static int16_t envelope_target_[4];
  
  // Value of global modulation parameters, scaled to 0-255;
  static uint8_t modulation_sources_[MOD_SRC_ASSIGNABLE_2 + 1];
  
  static Patch patch_;
  static Lfo lfo_[2];
  static Voice voice_;  // Monophonic.
  static VoiceController controller_;

  // Called whenever a parameter related to LFOs/envelopes is modified (for now
  // everytime a parameter is modified by the user).
  static void RecomputeModulationIncrements();
  // Called whenever a parameter related to oscillators is called.
  static void UpdateOscillatorAlgorithms();

  // Computes the increment for a given envelope stage.
  static uint16_t ScaleEnvelopeIncrement(uint8_t time, uint8_t scale);
  
  DISALLOW_COPY_AND_ASSIGN(SynthesisEngine);
};

extern SynthesisEngine engine;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_SYNTHESIS_ENGINE_H_
