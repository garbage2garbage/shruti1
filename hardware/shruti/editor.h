// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Parameter editor.

#ifndef HARDWARE_SHRUTI_EDITOR_H_
#define HARDWARE_SHRUTI_EDITOR_H_

#include <stdio.h>

#include "hardware/base/base.h"
#include "hardware/io/display.h"
#include "hardware/io/pin.h"
#include "hardware/shruti/parameters.h"
#include "hardware/shruti/patch.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/shruti.h"

using hardware_io::kLcdNoCursor;
using hardware_io::Display;
using hardware_io::Pin;

namespace hardware_shruti {

enum CurrentDisplayType {
  PAGE_TYPE_SUMMARY,
  PAGE_TYPE_DETAILS
};

enum Group {
  GROUP_OSC,
  GROUP_FILTER,
  GROUP_MOD,
  GROUP_PLAY,
  GROUP_LOAD_SAVE,
};

enum Page {
  PAGE_OSC_OSC_1,
  PAGE_OSC_OSC_2,
  PAGE_OSC_OSC_MIX,
  PAGE_FILTER_FILTER,
  PAGE_MOD_ENV,
  PAGE_MOD_LFO,
  PAGE_MOD_MATRIX,
  PAGE_PLAY_ARP,
  PAGE_PLAY_STEP_SEQUENCER,
  PAGE_PLAY_KBD,
  PAGE_LOAD_SAVE,
  PAGE_END = 255
};

enum Unit {
  UNIT_RAW_UINT8,
  UNIT_UINT8,
  UNIT_INT8,
  UNIT_BOOLEAN,
  UNIT_WAVEFORM,
  UNIT_LFO_WAVEFORM,
  UNIT_MODULATION_SOURCE,
  UNIT_MODULATION_DESTINATION,
  UNIT_PATTERN,
  UNIT_RAGA,
  UNIT_TEMPO_WITH_EXTERNAL_CLOCK,
};

enum Action {
  ACTION_LOAD,
  ACTION_EXIT,
  ACTION_SAVE
};

// We do not use enums here because they take 2 bytes, not 1.
typedef uint8_t ParameterGroup;
typedef uint8_t ParameterPage;
typedef uint8_t ParameterUnit;

static const uint8_t kNumPages = 11;
static const uint8_t kNumGroups = 6;
static const uint8_t kNumControllers = 4;

// Size (in char) of the display elements.
static const uint8_t kCaptionWidth = 10;
static const uint8_t kValueWidth = 6;
static const uint8_t kColumnWidth = 4;

struct ParameterDefinition {
  uint8_t id;
  uint8_t min_value;
  uint8_t max_value;
  ParameterPage page;
  ParameterUnit unit;
  ResourceId short_name;
  ResourceId long_name;
};

class Editor;

struct PageDefinition {
  ParameterPage id;
  ParameterGroup group;
  ResourceId name;
  void (*summary_page)();
  void (*details_page)();
  void (*input_handler)(uint8_t controller_index, uint16_t value);
};

class SynthesisEngine;

class Editor {
 public:
  Editor() { }
  static void Init();
  static void ToggleGroup(ParameterGroup group);

  static void HandleInput(uint8_t controller_index, uint16_t value);
  static void DisplaySummary();
  static void DisplayDetails();
  static void DisplaySplashScreen();
  
  static void ResetPatch();
  static inline ParameterPage current_page() { return current_page_; }
  static inline uint8_t cursor() { return cursor_; }

 private:
  static void PrettyPrintParameterValue(const ParameterDefinition& parameter,
                                 char* buffer, uint8_t width);
  
  // Output and Input handling for all the different category of pages.
  static void DisplayEditSummaryPage();
  static void DisplayEditDetailsPage();
  static void HandleEditInput(uint8_t controller_index, uint16_t value);
  
  static void DisplayLoadSavePage();
  static void HandleLoadSaveInput(uint8_t controller_index, uint16_t value);
  static void EnterLoadSaveMode();
  
  static void DisplayStepSequencerPage();
  static void HandleStepSequencerInput(uint8_t controller_index, uint16_t value);

  static const ParameterDefinition& parameter_definition(uint8_t index);

  static const PageDefinition page_definition_[];

  // Parameter definitions are stored in program memory and need to be copied
  // in SRAM when read. This temporary storage space holds them.
  static ParameterDefinition parameter_definition_;
  static uint8_t parameter_definition_index_;
  static uint8_t current_display_type_;  // 0 for summary, 1 for details.

  static ParameterPage current_page_;
  static ParameterPage last_visited_page_[kNumGroups];
  static uint8_t current_controller_;
  static uint8_t parameter_definition_offset_[kNumPages][kNumControllers];

  static char line_buffer_[kLcdWidth * kLcdHeight + 1];

  // Load/Save related stuff. Cursor is also used for the step sequencer, and
  // for storing the modulation matrix entry being edited. You're warned!
  static uint16_t cursor_;
  static uint8_t flip_;  // Used for blinking cursor.
  static uint8_t action_;
  static uint8_t current_patch_number_;
  static uint8_t previous_patch_number_;
  static uint8_t patch_buffer_[kSerializedPatchSize];
  // Buffer used to allow the user to undo the loading of a patch (similar to
  // the "compare" function on some synths).
  static uint8_t patch_undo_buffer_[kSerializedPatchSize];
  
  DISALLOW_COPY_AND_ASSIGN(Editor);
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_EDITOR_H_
