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
// Voice manager / arpeggiator.

#include "hardware/shruti/voice_controller.h"
#include "hardware/shruti/patch.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/op.h"
#include "hardware/utils/random.h"

using hardware_utils::Random;
using namespace hardware_utils_op;

namespace hardware_shruti {
  
static const prog_uint8_t midi_clock_scale[5] PROGMEM = {
  6 /* normal */, 3 /* x2 */, 12, 24, 48 /* x2, x4, x8 */
};

/* <static> */
int16_t VoiceController::internal_clock_counter_;
int8_t VoiceController::midi_clock_counter_;
uint8_t VoiceController::midi_clock_prescaler_;
int16_t VoiceController::step_duration_[kNumSteps];
int16_t VoiceController::average_step_duration_;

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
  step_duration_[0] = step_duration_[1] = (kSampleRate * 60L / 4) / 120;
  octaves_ = 0;
  pattern_size_ = 16;
  pattern_ = 0x5555;
  mode_ = 0;
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
    if (midi_clock_prescaler_ == 0) {
      estimated_beat_duration_ = step_duration_[0] / (kControlRate / 4);
    }
    pattern_mask_ = 255;
    internal_clock_counter_ = 0;
    midi_clock_counter_ = 0;
    step_duration_estimator_num_ = 0xffff;
    step_duration_estimator_den_ = 0xff;
    pattern_step_ = pattern_size_ - 1;
    direction_ = mode_ == ARPEGGIO_DIRECTION_DOWN ? -1 : 1; 
    ArpeggioStart();
  }
}

/* static */
void VoiceController::AllSoundOff() {
  notes_.Clear();
  for (uint8_t i = 0; i < num_voices_; ++i) {
    voices_[i].Kill();
  }
  active_ = 0;
}

/* static */
void VoiceController::AllNotesOff() {
  notes_.Clear();
  for (uint8_t i = 0; i < num_voices_; ++i) {
    voices_[i].Release();
  }
}

/* static */
void VoiceController::UpdateArpeggiatorParameters(const Patch& patch) {
  pattern_ = ResourcesManager::Lookup<uint16_t, uint8_t>(
      lut_res_arpeggiator_patterns, patch.arp_pattern >> 2);
  mode_ = patch.arp_pattern & 0x03;
  direction_ = mode_ == ARPEGGIO_DIRECTION_DOWN ? -1 : 1;
  octaves_ = patch.arp_octave;
  pattern_size_ = patch.pattern_size;
  if (patch.arp_tempo < 40) {
    midi_clock_prescaler_ = ResourcesManager::Lookup<uint8_t, uint8_t>(
        midi_clock_scale, patch.arp_tempo - 35);
  } else {
    midi_clock_prescaler_ = 0;
  }
  average_step_duration_ = (kSampleRate * 60L / 4) / static_cast<int32_t>(
      patch.arp_tempo <= 240
          ? patch.arp_tempo
          : ResourcesManager::Lookup<uint16_t, uint8_t>(
              lut_res_turbo_tempi, patch.arp_tempo - 240 - 1));
  uint8_t groove_amount = Swap4(patch.arp_groove);
  uint8_t groove_template = groove_amount & 0x0f;
  for (uint8_t i = 0; i < kNumSteps; ++i) {
    int32_t swing_direction = ResourcesManager::Lookup<int16_t, uint8_t>(
        LUT_RES_GROOVE_SWING + groove_template, i);
    swing_direction *= average_step_duration_;
    swing_direction *= groove_amount;
    int16_t swing = swing_direction >> 17;
    step_duration_[i] = average_step_duration_ + swing;
  }
  estimated_beat_duration_ = average_step_duration_ / (kControlRate / 4);
}

/* static */
void VoiceController::NoteOn(uint8_t note, uint8_t velocity) {
  if (velocity == 0) {
    NoteOff(note);
  } else {
    notes_.NoteOn(note, velocity);
    // In case we haven't played something for a while, reset all the
    // sequencer/arpeggiator stuff.
    Start();
    // Trigger the note.
    if (octaves_ == 0) {
      voices_[0].Trigger(note, velocity, notes_.size() > 1);
    }
  }
}

/* static */
void VoiceController::NoteOff(uint8_t note) {
  // Get the currently playing note.
  uint8_t top_note = notes_.most_recent_note().note;
  notes_.NoteOff(note);

  // If no note is remaining, play the release phase of the envelope.
  if (notes_.size() == 0) {
    voices_[0].Release();
  } else {
    // Otherwise retrigger the previously played note, or let the arpeggiator
    // do it. No need to retrigger if we just removed notes different from
    // the one currently played.
    if (octaves_ == 0) {
      if (top_note == note) {
        voices_[0].Trigger(notes_.most_recent_note().note, 0, true);
      }
    }
  }
}

/* static */
uint8_t VoiceController::Control() {
  ++step_duration_estimator_num_;
  internal_clock_counter_ -= kControlRate;
  if ((!midi_clock_prescaler_ && internal_clock_counter_ > 0) ||
      (midi_clock_prescaler_ && midi_clock_counter_ > 0)) {
    return 0;
  }
  ++step_duration_estimator_den_;
  
  // Move to the next step in the x-o-x pattern.
  pattern_mask_ <<= 1;
  pattern_step_ += 1;
  if (pattern_step_ >= pattern_size_) {
    pattern_mask_ = 1;
    pattern_step_ = 0;
  }
  if (notes_.size() == 0 && !midi_clock_prescaler_ && active_) {
    ++inactive_steps_;
    if (inactive_steps_ >= pattern_size_) {
      Stop();
      inactive_steps_ = 0;
    }
  }
  
  // Update the value of the counter depending on which steps we are on.
  // Steps 1, 2 are longer than steps 3, 4 if swing is enabled.
  if (!midi_clock_prescaler_) {
    internal_clock_counter_ += step_duration_[pattern_step_ & 0x0f];
  } else {
    midi_clock_counter_ += midi_clock_prescaler_;
  }
  if (step_duration_estimator_den_ == 4) {
    estimated_beat_duration_ = step_duration_estimator_num_;
    step_duration_estimator_den_ = 0;
    step_duration_estimator_num_ = 0;
  }

  if (notes_.size() == 0 || octaves_ == 0) {
    return 1;
  }
  
  // If the bit is set in the x-o-x pattern, move to the next note.
  if (has_arpeggiator_note()) {
    Step();
  }
  return 2;
}

/* static */
void VoiceController::ArpeggioStart() {
  if (direction_ == 1) {
    octave_step_ = 0;
    arpeggio_step_ = 0; 
  } else {
    octave_step_ = octaves_ - 1;
    arpeggio_step_ = notes_.size() - 1;
  }
}

/* static */
void VoiceController::ArpeggioStep() {
  uint8_t num_notes = notes_.size();
  arpeggio_step_ += direction_;
  uint8_t next_octave = 0;
  if (arpeggio_step_ >= num_notes) {
    arpeggio_step_ = 0;
    next_octave = 1;
  } else if (arpeggio_step_ < 0) {
    arpeggio_step_ = num_notes - 1;
    next_octave = 1;
  }
  if (next_octave) {
    octave_step_ += direction_;
    if (octave_step_ >= octaves_ || octave_step_ < 0) {
      if (mode_ == ARPEGGIO_DIRECTION_UP_DOWN) {
        direction_ = -direction_;
        ArpeggioStart();
        if (num_notes > 1 || octaves_ > 1) {
          ArpeggioStep();
        }
      } else {
        ArpeggioStart();
      }
    }
  }
}

/* static */
void VoiceController::Step() {
  uint8_t num_notes = notes_.size();
  if (mode_ == ARPEGGIO_DIRECTION_RANDOM) {
    uint8_t random_byte = Random::state_msb();
    octave_step_ = random_byte & 0xf;
    arpeggio_step_ = (random_byte & 0xf0) >> 4;
    while (octave_step_ >= octaves_) {
      octave_step_ -= octaves_;
    }
    while (arpeggio_step_ >= num_notes) {
      arpeggio_step_ -= num_notes;
    }
  } else {
    ArpeggioStep();
  }
  uint8_t note = notes_.sorted_note(arpeggio_step_).note;
  note += 12 * octave_step_;
  while (note > 127) {
    note -= 12;
  }
  voices_[0].Trigger(note, notes_.sorted_note(arpeggio_step_).velocity, false);
}    

}  // namespace hardware_shruti
