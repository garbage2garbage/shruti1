// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Voice manager / arpeggiator.
//
// Routes note messages to a pool of voices, and handles arpeggiation.
// For now, used in monophonic mode only, but it should be fairly easy to use
// the NoteStack instance contained in this class to handle voice stealing for
// polyphonic.
//
// Note that we don't spend enough cycles in this guy to justify the
// "staticification" SynthesisEngine and Voice have gone through.

#ifndef HARDWARE_SHRUTI_VOICE_CONTROLLER_
#define HARDWARE_SHRUTI_VOICE_CONTROLLER_

#include "hardware/shruti/shruti.h"

#include "hardware/shruti/note_stack.h"

namespace hardware_shruti {

static const uint8_t kMidiClockPrescaler = 24 / 4;  // MIDI ppqn / xox ppqn.

enum ArpeggioDirection {
  ARPEGGIO_DIRECTION_UP = 0,
  ARPEGGIO_DIRECTION_DOWN,
  ARPEGGIO_DIRECTION_UP_DOWN,
  ARPEGGIO_DIRECTION_RANDOM,
};

class Voice;

class VoiceController {
 public:
  VoiceController();
  void Init(Voice* voices, uint8_t num_voices_);
  void SetTempo(uint8_t tempo);
  void SetSwing(uint8_t swing);
  void SetPattern(uint8_t pattern);
  void SetOctaves(uint8_t octaves) { octaves_ = octaves; }
  void AllNotesOff();
  void AllSoundOff();
  void Reset();
  void Step();
  void NoteOn(uint8_t note, uint8_t velocity);
  void NoteOff(uint8_t note);
  inline void Audio() { --internal_clock_counter_; }
  inline void ExternalSync() { --midi_clock_counter_; }
  inline uint8_t step() { return pattern_step_; }
  inline uint16_t has_arpeggiator_note() { return pattern_mask_ & pattern_; }
  void Control();

 private:
  void RecomputeStepDurations();
  void ArpeggioDown();
  void ArpeggioUp();
  void ArpeggioLast();
  void ArpeggioFirst();

  int16_t internal_clock_counter_;
  uint8_t midi_clock_counter_;
  int16_t step_duration_[2];
  uint8_t random_byte_;

  // 16 steps x-o-x pattern storing the steps at which a new note is triggered.
  uint16_t pattern_;
  uint16_t pattern_mask_;  // Shift by 1 every 1/16th note, with swing.
  uint8_t pattern_step_;  // Increment by 1 every 1/16th note, with swing.
  
  int8_t arpeggio_step_;  // Incremented/decremented by 1 for up/down pattern.
  int8_t direction_;  // Direction increment.
  int8_t octave_step_;
  int8_t octaves_;  // Number of octaves
  uint8_t mode_;

  NoteStack notes_;
  Voice* voices_;
  uint8_t num_voices_;
  
  uint8_t tempo_;
  uint8_t swing_;
  
  DISALLOW_COPY_AND_ASSIGN(VoiceController);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_VOICE_CONTROLLER_
