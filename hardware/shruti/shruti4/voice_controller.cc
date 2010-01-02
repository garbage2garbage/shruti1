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
// Voice manager / arpeggiator.

#include "hardware/shruti/shruti4/voice_controller.h"
#include "hardware/shruti/shruti4/synthesis_engine.h"

namespace hardware_shruti {
  
/* <static> */
int16_t VoiceController::internal_clock_counter_;
uint8_t VoiceController::midi_clock_counter_;
int16_t VoiceController::step_duration_;

uint8_t VoiceController::pattern_step_;
  
NoteStack VoiceController::notes_;
Voice* VoiceController::voices_;
uint8_t VoiceController::num_voices_;
  
uint8_t VoiceController::tempo_;
uint8_t VoiceController::pattern_size_;
uint8_t VoiceController::active_;
uint8_t VoiceController::inactive_steps_;

uint16_t VoiceController::step_duration_estimator_num_;
uint8_t VoiceController::step_duration_estimator_den_;
uint16_t VoiceController::estimated_beat_duration_;
/* </static> */

/* static */
void VoiceController::Init(Voice* voices, uint8_t num_voices) {
  voices_ = voices;
  num_voices_ = num_voices;
  notes_.Clear();
  tempo_ = 120;
  step_duration_ = (kSampleRate * 60L / 4) / 120;
  pattern_size_ = 16;
  inactive_steps_ = 0;
  active_ = 0;
  Reset();
}

/* static */
void VoiceController::Reset() {
  // If 4 beats has elapsed without event, the sequencer will restart from
  // the first step when a key is pressed. Otherwise, the pattern will continue
  // moving on.
  // Note that all this active/reset stuff is only for internal clock. For
  // external clock there's little we can do as we don't know, for example,
  // if not receiving a clock event at t means "we're slowing down" or "we have
  // stopped".
  if (!active_) {
    midi_clock_counter_ = kMidiClockPrescaler;
    if (tempo_) {
      estimated_beat_duration_ = step_duration_ / (kControlRate / 4);
    }
    internal_clock_counter_ = 0;
    step_duration_estimator_num_ = 0xffff;
    step_duration_estimator_den_ = 0xff;
    pattern_step_ = pattern_size_ - 1;
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
void VoiceController::RecomputeStepDurations() {
  step_duration_ = (kSampleRate * 60L / 4) / static_cast<int32_t>(tempo_);
  estimated_beat_duration_ = step_duration_ / (kControlRate / 4);
}

/* static */
void VoiceController::NoteOn(uint8_t note, uint8_t velocity) {
  if (velocity == 0) {
    NoteOff(note);
  } else {
    notes_.NoteOn(note, velocity);
    Start();
    voices_[0].Trigger(notes_, true);
  }
}

/* static */
void VoiceController::NoteOff(uint8_t note) {
  notes_.NoteOff(note);

  // If no note is remaining, the oscillators continue running, but we play the
  // release phase of the envelope.
  if (notes_.size() == 0) {
    voices_[0].Release();
  }
}

/* static */
uint8_t VoiceController::Control() {
  ++step_duration_estimator_num_;
  if ((tempo_ && internal_clock_counter_ > 0) ||
      (!tempo_ && midi_clock_counter_ > 0)) {
    return 0;
  }
  ++step_duration_estimator_den_;
  
  // Move to the next step in the x-o-x pattern.
  pattern_step_ += 1;
  if (pattern_step_ >= pattern_size_) {
    pattern_step_ = 0;
  }
  if (notes_.size() == 0 && tempo_) {
    ++inactive_steps_;
    if (inactive_steps_ >= pattern_size_) {
      Stop();
      inactive_steps_ = 0;
    }
  }
  
  // Update the value of the counter depending on which steps we are on.
  // Steps 1, 2 are longer than steps 3, 4 if swing is enabled.
  if (tempo_) {
    internal_clock_counter_ += step_duration_;
  } else {
    midi_clock_counter_ += kMidiClockPrescaler;
  }
  if (step_duration_estimator_den_ == 4) {
    estimated_beat_duration_ = step_duration_estimator_num_;
    step_duration_estimator_den_ = 0;
    step_duration_estimator_num_ = 0;
  }

  return 1;
}

}  // namespace hardware_shruti
