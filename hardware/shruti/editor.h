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
// Parameter editor.

#ifndef HARDWARE_SHRUTI_EDITOR_H_
#define HARDWARE_SHRUTI_EDITOR_H_

#include "hardware/base/base.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/shruti.h"

namespace hardware_shruti {

class ParameterDefinition;

enum CurrentDisplayType {
  PAGE_TYPE_SUMMARY,
  PAGE_TYPE_DETAILS,
  PAGE_TYPE_ANY,
};

enum Group {
  GROUP_OSC,
  GROUP_FILTER,
  GROUP_MOD,
  GROUP_PLAY,
  GROUP_LOAD_SAVE,
  GROUP_PERFORMANCE,
};

enum Page {
  PAGE_OSC_OSC_1,
  PAGE_OSC_OSC_2,
  PAGE_OSC_OSC_MIX,
  PAGE_FILTER_FILTER,
  PAGE_MOD_ENV_1,
  PAGE_MOD_ENV_2,
  PAGE_MOD_LFO,
  PAGE_MOD_MATRIX,
  PAGE_PLAY_ARP,
  PAGE_PLAY_STEP_SEQUENCER,
  PAGE_PLAY_KBD,
  PAGE_LOAD_SAVE,
  PAGE_PERFORMANCE,
};

enum Action {
  ACTION_LOAD,
  ACTION_EXIT,
  ACTION_SAVE
};

// We do not use enums here because they take 2 bytes, not 1.
typedef uint8_t ParameterGroup;
typedef uint8_t ParameterPage;

static const uint8_t kNumPages = 12;
static const uint8_t kNumGroups = 6;

// Size (in char) of the display elements.
static const uint8_t kCaptionWidth = 10;
static const uint8_t kValueWidth = 6;
static const uint8_t kColumnWidth = 4;

enum PageUiType {
  PARAMETER_EDITOR = 0,
  STEP_SEQUENCER = 1,
  LOAD_SAVE = 2,
};

typedef uint8_t UiType;

struct PageDefinition {
  ParameterPage id;
  ParameterPage next;
  ParameterGroup group;
  ResourceId name;
  UiType ui_type;
  uint8_t first_parameter_index;
};

struct ParameterAssignment {
  uint8_t id;
  uint8_t subpage;
};

// For each type of page (basic parameter editor, step sequencer, load/save
// dialog, 4 functions must be defined:
// - a function displaying the "overview" page.
// - a function displaying a specific parameter value ("details").
// - a function handling a change in one of the 4 editing pots.
// - a function handling presses on the increment/decrement buttons.
struct UiHandler {
  void (*summary_page)();
  void (*details_page)();
  void (*input_handler)(uint8_t knob_index, uint16_t value);
  void (*increment_handler)(int8_t direction);  
};

class Editor {
 public:
  Editor() { }
  static void Init();
  
  // Handles a press on one of the "parameter groups" buttons. Go back to the
  // most recently visited page in this group, or cycle through the pages in
  // the current group.
  static void ToggleGroup(ParameterGroup group);
  
  // Handles a long press on one of the "parameter groups" buttons. This
  // triggers a lot of useful hacky functions.
  static void DoShiftFunction(ParameterGroup group, uint8_t hold_time);

  // Handles the modification of one of the editing pots.
  static void HandleInput(uint8_t knob_index, uint16_t value);
  
  // Handles a press on the inc/dec buttons.
  static void HandleIncrement(int8_t direction);
  
  // Displays variants of the current page.
  static void DisplaySummary();
  static void DisplayDetails();
  
  // Displays two lines of text read from a resource.
  static void DisplaySplashScreen(ResourceId first_line);
  
  static inline ParameterPage current_page() { return current_page_; }
  static inline uint8_t cursor() { return cursor_; }
  static inline uint8_t subpage() { return subpage_; }

 private:
  static void PrettyPrintParameterValue(const ParameterDefinition& parameter,
                                        char* buffer, uint8_t width);
  
  // Output and Input handling for all the different category of pages.
  static void DisplayEditSummaryPage();
  static void DisplayEditDetailsPage();
  static void HandleEditInput(uint8_t knob_index, uint16_t value);
  static void HandleEditIncrement(int8_t direction);
  // A bunch of hacks for special values/pages.
  static void SetParameterValue(uint8_t id, uint8_t value);
  static uint8_t GetParameterValue(uint8_t id);
  
  // Returns the parameter id of the parameter that should be edited when
  // touching knob #knob_index.
  static uint8_t KnobIndexToParameterId(uint8_t knob_index);
  
  static void DisplayLoadSavePage();
  static void HandleLoadSaveInput(uint8_t knob_index, uint16_t value);
  static void EnterLoadSaveMode();
  static void HandleLoadSaveIncrement(int8_t direction);
  static void DumpCurrentPatch();
  
  static void DisplayStepSequencerPage();
  static void HandleStepSequencerInput(uint8_t knob_index, uint16_t value);
  static void HandleStepSequencerIncrement(int8_t direction);
  
  static void RandomizeParameter(uint8_t subpage, uint8_t parameter_index);
  static void RandomizePatch();


  static const PageDefinition page_definition_[];
  static const UiHandler ui_handler_[];

  // Parameter definitions are stored in program memory and need to be copied
  // in SRAM when read. This temporary storage space holds them.
  static uint8_t current_display_type_;  // 0 for summary, 1 for details.

  static ParameterPage current_page_;
  static ParameterPage last_visited_page_[kNumGroups];
  // Used for the modulation matrix page only.
  static uint8_t last_visited_subpage_;
  static uint8_t current_knob_;

  static char line_buffer_[kLcdWidth * kLcdHeight + 1];

  // Load/Save related stuff. Cursor is also used for the step sequencer step.
  static uint8_t cursor_;
  static uint8_t subpage_;
  static uint8_t action_;
  static uint8_t current_patch_number_;
  static uint8_t previous_patch_number_;

  static uint8_t assign_in_progress_; 
  static uint8_t test_note_playing_;
  static ParameterAssignment assigned_parameters_[kNumEditingPots];
  static ParameterAssignment parameter_to_assign_;

  DISALLOW_COPY_AND_ASSIGN(Editor);
};

extern Editor editor;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_EDITOR_H_
