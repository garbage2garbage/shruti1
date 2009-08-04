// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Stack of currently held keys.
//
// Currently pressed keys are stored as a linked list. The linked list is used
// as a LIFO stack to allow monosynth-like behaviour. An example of such
// behaviour is:
// player presses and holds C4-> C4 is played.
// player presses and holds C5 (while holding C4) -> C5 is played.
// player presses and holds G4 (while holding C4&C5)-> G4 is played.
// player releases C5 -> G4 is played.
// player releases G4 -> C4 is played.
//
// The nodes used in the linked list are pre-allocated from a pool of 16
// nodes, so the "pointers" (to the root element for example) are not actual
// pointers, but indices of an element in the pool.
//
// Additionally, an array of pointers is stored to allow random access to the
// n-th note, sorted by ascending order of pitch (for arpeggiation).

#ifndef HARDWARE_SHRUTI_NOTE_STACK_H_
#define HARDWARE_SHRUTI_NOTE_STACK_H_

#include "hardware/base/base.h"

static const uint8_t kNoteStackSize = 16;

namespace hardware_shruti {

struct NoteEntry {
  uint8_t note;
  uint8_t velocity;
  uint8_t next_ptr;  // Base 1.
};

class NoteStack {
 public: 
  NoteStack();

  void NoteOn(uint8_t note, uint8_t velocity);
  void NoteOff(uint8_t note);
  void Clear();

  uint8_t size() const { return size_; }
  const NoteEntry& most_recent_note() const { return pool_[root_ptr_]; }
  const NoteEntry& sorted_note(uint8_t index) const {
    return pool_[sorted_ptr_[index]];
  }
  const NoteEntry& dummy() const { return pool_[0]; }
  
 private:
  uint8_t size_;
  NoteEntry pool_[kNoteStackSize + 1];  // First element is a dummy node!
  uint8_t root_ptr_;  // Base 1.
  uint8_t sorted_ptr_[kNoteStackSize];  // Base 1.

  DISALLOW_COPY_AND_ASSIGN(NoteStack);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_NOTE_STACK_H_
