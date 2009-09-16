// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Parameter editor.

#ifndef HARDWARE_SHRUTI_EDITOR_H_
#define HARDWARE_SHRUTI_EDITOR_H_

#include <stdio.h>

#include "hardware/base/base.h"
#include "hardware/io/devices/sparkfun_ser_lcd.h"
#include "hardware/io/pin.h"
#include "hardware/shruti/patch.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/shruti.h"

using hardware_io::kLcdNoCursor;
using hardware_io::Display;
using hardware_io::Pin;

namespace hardware_shruti {

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
  PAGE_END = 255
};

enum Unit {
  UNIT_RAW_UINT8,
  UNIT_UINT8,
  UNIT_INT8,
  UNIT_BOOLEAN,
  UNIT_WAVEFORM,
  UNIT_OPERATOR,
  UNIT_LFO_WAVEFORM,
  UNIT_INDEX,
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

static const uint8_t kNumPages = 12;
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

enum PageUi {
  PARAMETER_EDITOR = 0,
  STEP_SEQUENCER = 1,
  LOAD_SAVE = 2,
};

struct UiHandler {
  void (*summary_page)();
  void (*details_page)();
  void (*input_handler)(uint8_t controller_index, uint16_t value);
  void (*increment_handler)(int8_t direction);  
};

struct PageDefinition {
  ParameterPage id;
  ParameterGroup group;
  ResourceId name;
  uint8_t ui_type;
};

class Editor {
 public:
  Editor() { }
  static void Init();
  static void ToggleGroup(ParameterGroup group);

  static void HandleInput(uint8_t controller_index, uint16_t value);
  static void HandleIncrement(int8_t direction);
  static void DisplaySummary();
  static void DisplayDetails();
  static void DisplaySplashScreen(ResourceId first_line);
  
  static void ResetPatch();
  static inline ParameterPage current_page() { return current_page_; }
  static inline uint8_t cursor() { return cursor_; }
  static inline uint8_t subpage() { return subpage_; }

 private:
  static void PrettyPrintParameterValue(const ParameterDefinition& parameter,
                                 char* buffer, uint8_t width);
  
  // Output and Input handling for all the different category of pages.
  static void DisplayEditSummaryPage();
  static void DisplayEditDetailsPage();
  static void HandleEditInput(uint8_t controller_index, uint16_t value);
  static void HandleEditIncrement(int8_t direction);
  // A bunch of hacks for special values/pages.
  static void SetParameterWithHacks(uint8_t id, uint8_t value);
  static uint8_t GetParameterWithHacks(uint8_t id);
  
  static void DisplayLoadSavePage();
  static void HandleLoadSaveInput(uint8_t controller_index, uint16_t value);
  static void EnterLoadSaveMode();
  static void HandleLoadSaveIncrement(int8_t direction);
  static void DumpCurrentPatch();
  
  static void DisplayStepSequencerPage();
  static void HandleStepSequencerInput(uint8_t controller_index, uint16_t value);
  static void HandleStepSequencerIncrement(int8_t direction);

  static const ParameterDefinition& parameter_definition(uint8_t index);

  static const PageDefinition page_definition_[];
  static const UiHandler ui_handler_[];

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

  // Load/Save related stuff. Cursor is also used for the step sequencer step.
  static uint8_t cursor_;
  static uint8_t subpage_;
  static uint8_t action_;
  static uint8_t current_patch_number_;
  static uint8_t previous_patch_number_;

  DISALLOW_COPY_AND_ASSIGN(Editor);
};

extern Editor editor;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_EDITOR_H_
