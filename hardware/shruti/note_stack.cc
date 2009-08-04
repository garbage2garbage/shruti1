// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Stack of currently held notes.

#include "hardware/shruti/note_stack.h"

namespace hardware_shruti {

NoteStack::NoteStack() {
  Clear();
}

void NoteStack::NoteOn(uint8_t note, uint8_t velocity) {
  // Remove the note from the list first.
  NoteOff(note);
  // In case of saturation, remove the least recently played note from the
  // stack.
  if (size_ == kNoteStackSize) {
    uint8_t least_recent_note = 0;
    for (int i = 1; i <= kNoteStackSize; ++i) {
      if (pool_[i].next_ptr == 0) {
        least_recent_note = pool_[i].note;
      }
    }
    NoteOff(least_recent_note);
  }
  // Now we are ready to insert the new note. Find a free slot to insert it.
  uint8_t free_slot = 0;
  for (int i = 1; i <= kNoteStackSize; ++i) {
    if (pool_[i].next_ptr == 0 && pool_[i].note == 0) {
      free_slot = i;
    }
  }
  pool_[free_slot].next_ptr = root_ptr_;
  pool_[free_slot].note = note;
  pool_[free_slot].velocity = velocity;
  root_ptr_ = free_slot;
  // The last step consists in inserting the note in the sorted list.
  for (int i = 0; i < size_; ++i) {
    if (pool_[sorted_ptr_[i]].note > note) {
      for (int j = size_; j > i; --j) {
        sorted_ptr_[j] = sorted_ptr_[j - 1];
      }
      sorted_ptr_[i] = free_slot;
      free_slot = 0;
      break;
    }
  }
  if (free_slot) {
    sorted_ptr_[size_] = free_slot;
  }
  ++size_;
}

void NoteStack::NoteOff(uint8_t note) {
  uint8_t current = root_ptr_;
  uint8_t previous = 0;
  while (current) {
    if (pool_[current].note == note) {
      break;
    }
    previous = current;
    current = pool_[current].next_ptr;
  }
  if (current) {
    if (previous) {
      pool_[previous].next_ptr = pool_[current].next_ptr;
    } else {
      root_ptr_ = pool_[current].next_ptr;
    }
    for (int i = 0; i < size_; ++i) {
      if (sorted_ptr_[i] == current) {
        for (int j = i; j < size_ - 1; ++j) {
          sorted_ptr_[j] = sorted_ptr_[j + 1];
        }
        break;
      }
    }
    pool_[current].next_ptr = 0;
    pool_[current].note = 0;
    pool_[current].velocity = 0;
    --size_;
  }
}

void NoteStack::Clear() {
  size_ = 0;
  for (int i = 1; i <= kNoteStackSize; ++i) {
    pool_[i].note = 0;
    pool_[i].velocity = 0;
    pool_[i].next_ptr = 0;
    sorted_ptr_[i] = 0;
  }
  root_ptr_ = 0;  
}

}  // namespace hardware_shruti
