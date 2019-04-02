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
//
// Routes note messages to a pool of voices, and handles arpeggiation.
// For now, used in monophonic mode only, but it should be fairly easy to use
// the NoteStack instance contained in this class to handle voice stealing for
// a polyphonic synth.
//
// Two instances of this guy will be needed for multitimbrality. Since there is
// no plan to support multitimbrality, this class is implemented as a "static
// singleton". This does not yield a code size gain, but this is coherent with
// what was done with NoteStack.

#ifndef HARDWARE_SHRUTI_VOICE_CONTROLLER_
#define HARDWARE_SHRUTI_VOICE_CONTROLLER_

#include "hardware/shruti/shruti.h"

#include "hardware/shruti/note_stack.h"

namespace hardware_shruti {

static const uint8_t kNumSteps = 16;

enum ArpeggioDirection {
  ARPEGGIO_DIRECTION_UP = 0,
  ARPEGGIO_DIRECTION_DOWN,
  ARPEGGIO_DIRECTION_UP_DOWN,
  ARPEGGIO_DIRECTION_RANDOM,
};

class Patch;
class Voice;

class VoiceController {
 public:
  VoiceController() { }
  static void Init(Voice* voices, uint8_t num_voices_);
  static void AllNotesOff();
  static void AllSoundOff();
  static void Reset();
  static void Step();
  static void NoteOn(uint8_t note, uint8_t velocity);
  static void NoteOff(uint8_t note);
  static void UpdateArpeggiatorParameters(const Patch&);
  static inline void ExternalSync() { --midi_clock_counter_; }
  static inline uint8_t step() { return pattern_step_; }
  static inline uint8_t active() { return active_; }
  static inline uint16_t has_arpeggiator_note() {
    return pattern_mask_ & pattern_;
  }
  // Returns 1 when the clock has advanced.
  static uint8_t Control();
  static uint16_t estimated_beat_duration() {
    return estimated_beat_duration_;
  }
  // (for external sync).
  static void Stop() {
    active_ = 0;
  }
  static void Start() {
    Reset();
    active_ = 1;
  }

 private:
  static void ArpeggioStep();
  static void ArpeggioStart();

  static int16_t internal_clock_counter_;
  static int8_t midi_clock_counter_;
  static uint8_t midi_clock_prescaler_;
  static int16_t average_step_duration_;
  static int16_t step_duration_[kNumSteps];

  // 16 steps x-o-x pattern storing the steps at which a new note is triggered.
  static uint16_t pattern_;
  // Shift by 1 every 1/16th note, with swing.
  static uint16_t pattern_mask_;
  // Increment by 1 every 1/16th note, with swing.
  static uint8_t pattern_step_;
  static uint8_t pattern_size_;
  
  // Incremented/decremented by 1 for up/down pattern.
  static int8_t arpeggio_step_;
  // Direction increment.
  static int8_t direction_;
  static int8_t octave_step_;
  // Number of octaves
  static int8_t octaves_;
  static uint8_t mode_;

  static NoteStack notes_;
  static Voice* voices_;
  static uint8_t num_voices_;
  
  // After 4 beats without event, the sequencer is not active. The LED stops
  // blinking and the sequencer will restart from the first note in the pattern. 
  static uint8_t active_;
  static uint8_t inactive_steps_;
  
  // In order to sync the LFOs to an external MIDI clock, we need to estimate at
  // which BPM the master MIDI clock is running. This attemps to track this by
  // counting the number of control rate cycles in a beat.
  static uint16_t step_duration_estimator_num_;
  static uint8_t step_duration_estimator_den_;
  static uint16_t estimated_beat_duration_;
  
  DISALLOW_COPY_AND_ASSIGN(VoiceController);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_VOICE_CONTROLLER_
