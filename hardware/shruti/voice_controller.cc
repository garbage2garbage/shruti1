// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Voice manager / arpeggiator.

#include "hardware/shruti/voice_controller.h"

#include "hardware/shruti/resources.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/random.h"

using hardware_utils::Random;

namespace hardware_shruti {
  
/* <static> */
int16_t VoiceController::internal_clock_counter_;
uint8_t VoiceController::midi_clock_counter_;
int16_t VoiceController::step_duration_[2];
uint8_t VoiceController::random_byte_;

uint16_t VoiceController::pattern_;
uint16_t VoiceController::pattern_mask_;
uint8_t VoiceController::pattern_step_;
  
int8_t VoiceController::arpeggio_step_;
int8_t VoiceController::direction_;
int8_t VoiceController::octave_step_;
int8_t VoiceController::octaves_;
uint8_t VoiceController::mode_;

NoteStack VoiceController::notes_;
Voice* VoiceController::voices_;
uint8_t VoiceController::num_voices_;
  
uint8_t VoiceController::tempo_;
uint8_t VoiceController::swing_;
uint8_t VoiceController::pattern_size_;
/* </static> */

/* static */
void VoiceController::Init(Voice* voices, uint8_t num_voices) {
  voices_ = voices;
  num_voices_ = num_voices;
  notes_.Clear();
  voices_ = NULL;
  num_voices_ = 0;
  tempo_ = 120;
  swing_ = 0;
  step_duration_[0] = (kSampleRate * 60L / 4) / 120;
  step_duration_[1] = (kSampleRate * 60L / 4) / 120;
  octaves_ = 0;
  pattern_ = 0x5555;
  mode_ = 0;
  internal_clock_counter_ = step_duration_[0];
  midi_clock_counter_ = 6;
  pattern_size_ = 16;
  Reset();
}

/* static */
void VoiceController::Reset() {
  pattern_mask_ = 255;
  pattern_step_ = 15;
  direction_ = mode_ == ARPEGGIO_DIRECTION_DOWN ? -1 : 1; 
  internal_clock_counter_ = 0;
  midi_clock_counter_ = 6;
  if (mode_ == ARPEGGIO_DIRECTION_DOWN) {
    // Move to the first note, so that we'll start from the last note at the
    // next clock tick.
    ArpeggioFirst();
  } else {
    // Move to the last note, so that we'll start from the first note at the
    // next clock tick.
    ArpeggioLast();
  }
}

/* static */
void VoiceController::AllSoundOff() {
  notes_.Clear();
  for (uint8_t i = 0; i < num_voices_; ++i) {
    voices_[i].Kill();
  }
}

/* static */
void VoiceController::AllNotesOff() {
  notes_.Clear();
  for (uint8_t i = 0; i < num_voices_; ++i) {
    voices_[i].Release();
  }
}

/* static */
void VoiceController::SetTempo(uint8_t tempo) {
  tempo_ = tempo;
  RecomputeStepDurations();
}

/* static */
void VoiceController::SetSwing(uint8_t swing) {
  swing_ = swing;
  RecomputeStepDurations();
}

/* static */
void VoiceController::RecomputeStepDurations() {
  if (tempo_) {
    step_duration_[0] = (kSampleRate * 60L / 4) / long(tempo_);
    step_duration_[1] = step_duration_[0];
    int16_t swing = (step_duration_[0] * long(swing_)) >> 9;
    step_duration_[0] += swing;
    step_duration_[1] -= swing;
  } else {
    // We work at 4ppqn, while the MIDI real-time clock uses 24 ppqn.
    step_duration_[0] = kMidiClockPrescaler;
    step_duration_[1] = kMidiClockPrescaler;
  }
}

/* static */
void VoiceController::SetPattern(uint8_t pattern) {
  pattern_ = ResourcesManager::Lookup<uint16_t, uint8_t>(
      lut_res_arpeggiator_patterns, pattern >> 2);
  mode_ = pattern & 0x03;
  direction_ = mode_ == ARPEGGIO_DIRECTION_DOWN ? -1 : 1;
}

/* static */
void VoiceController::NoteOn(uint8_t note, uint8_t velocity) {
  random_byte_ += note;
  random_byte_ += velocity;
  if (velocity == 0) {
    NoteOff(note);
  } else {
    // If no notes were present in the stack, reset the arpeggiator step count.
    if (notes_.size() == 0) {
      Reset();
    }
    // Add a note to the stack. If the arpeggiator is running, it will trigger
    // itself. Otherwise, trigger it manually.
    notes_.NoteOn(note, velocity);
    if (octaves_ == 0) {
      voices_[0].Trigger(note, velocity, false);
    }
  }
}

/* static */
void VoiceController::NoteOff(uint8_t note) {
  notes_.NoteOff(note);

  // If no note is remaining, play the release phase of the envelope.
  if (notes_.size() == 0) {
    voices_[0].Release();
  } else {
    // Otherwise retrigger the previously played note, or let the arpeggiator
    // do it.
    if (octaves_ == 0) {
      voices_[0].Trigger(notes_.most_recent_note().note, 0, true);
    }
  }
}

/* static */
void VoiceController::Control() {
  if ((tempo_ && internal_clock_counter_ > 0) ||
      (!tempo_ && midi_clock_counter_ > 0)) {
    return;
  }
  // Move to the next step in the x-o-x pattern.
  pattern_mask_ <<= 1;
  pattern_step_ += 1;
  if (pattern_step_ >= pattern_size_) {
    pattern_mask_ = 1;
    pattern_step_ = 0;
  }
  
  // Update the value of the counter depending on which steps we are on.
  // Steps 1, 2 are longer than steps 3, 4 if swing is enabled.
  if (tempo_) {
    if (pattern_step_ & 2) {
      internal_clock_counter_ += step_duration_[1];
    } else {
      internal_clock_counter_ += step_duration_[0];
    }
  } else {
    midi_clock_counter_ += 6;
  }
  
  if (notes_.size() == 0 || octaves_ == 0) {
    return;
  }
  
  // If the bit is set in the x-o-x pattern, move to the next note.
  if (has_arpeggiator_note()) {
    Step();
  }
}

/* static */
void VoiceController::ArpeggioFirst() {
  octave_step_ = 0;
  arpeggio_step_ = 0;
}

/* static */
void VoiceController::ArpeggioLast() {
  octave_step_ = octaves_ - 1;
  arpeggio_step_ = notes_.size() - 1;
}

/* static */
void VoiceController::ArpeggioUp() {
  uint8_t num_notes = notes_.size();
  
  // Move to the next note.
  ++arpeggio_step_;
  
  // If we have just played the last note.
  if (arpeggio_step_ >= num_notes) {
    arpeggio_step_ = 0;
    ++octave_step_;
    if (octave_step_ >= octaves_) {
      if (mode_ == ARPEGGIO_DIRECTION_UP_DOWN) {
        direction_ = -1;
        ArpeggioLast();
        if (num_notes > 1 || octaves_ > 1) {
          ArpeggioDown();
        }
      } else {
        ArpeggioFirst();
      }
    }
  }
}

/* static */
void VoiceController::ArpeggioDown() {
  uint8_t num_notes = notes_.size();
  
  // Move to the next note.
  --arpeggio_step_;
  
  // If we have just played the last note.
  if (arpeggio_step_ < 0) {
    arpeggio_step_ = num_notes - 1;
    --octave_step_;
    if (octave_step_ < 0) {
      if (mode_ == ARPEGGIO_DIRECTION_UP_DOWN) {
        direction_ = +1;
        ArpeggioFirst();
        if (num_notes > 1 || octaves_ > 1) {
          ArpeggioUp();
        }
      } else {
        ArpeggioLast();
      }
    }
  }
}

/* static */
void VoiceController::Step() {
  uint8_t num_notes = notes_.size();
  if (mode_ == ARPEGGIO_DIRECTION_RANDOM) {
    random_byte_ = Random::state_msb();
    octave_step_ = random_byte_ & 0xf;
    arpeggio_step_ = (random_byte_ & 0xf0) >> 4;
    while (octave_step_ >= octaves_) {
      octave_step_ -= octaves_;
    }
    while (arpeggio_step_ >= num_notes) {
      arpeggio_step_ -= num_notes;
    }
  } else {
    if (direction_ == 1) {
      ArpeggioUp();
    } else if (direction_ == -1) {
      ArpeggioDown();
    }
  }
  uint8_t note = notes_.sorted_note(arpeggio_step_).note;
  note += 12 * octave_step_;
  while (note > 127) {
    note -= 12;
  }
  voices_[0].Trigger(note, notes_.sorted_note(arpeggio_step_).velocity, false);
}    

}  // namespace hardware_shruti
