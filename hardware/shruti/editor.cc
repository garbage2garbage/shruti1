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
// Patch editor.
//
// I really hate this code. Work on a "UI" framework to avoid such horrible
// things in progress...

#include "hardware/shruti/editor.h"
#include "hardware/shruti/display.h"
#include "hardware/shruti/patch_metadata.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/string.h"
#include "hardware/hal/watchdog_timer.h"

using namespace hardware_hal;
using namespace hardware_utils;

using hardware_hal::kLcdNoCursor;

namespace hardware_shruti {
  
/* extern */
Editor editor;

static const prog_char arp_pattern_prefix[4] PROGMEM = {
  0x03, 0x04, 0x05, '?'  // Up, Down, UpDown, Random
};

static const prog_char arp_groove_template_prefix[5] PROGMEM = {
  's', 'f', 'p', 'l', 'h'  // swing, shuffle, push, lag, human
};

static const prog_char units_definitions[UNIT_MIDI_CHANNEL + 1]
    PROGMEM = {
  0,
  0,
  0,
  STR_RES_OFF,
  STR_RES_NONE,
  STR_RES_1S2,
  STR_RES_TRI,
  0,
  0,
  STR_RES_LFO1,
  STR_RES_CUTOFF,
  0,
  STR_RES_EQUAL,
  0,
  0,
  0,
};

/* static */
const UiHandler Editor::ui_handler_[] = {
  { &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput, &Editor::HandleEditIncrement },
  { &Editor::DisplayStepSequencerPage, &Editor::DisplayStepSequencerPage,
    &Editor::HandleStepSequencerInput, &Editor::HandleStepSequencerIncrement },
  { &Editor::DisplayLoadSavePage, &Editor::DisplayLoadSavePage,
    &Editor::HandleLoadSaveInput, &Editor::HandleLoadSaveIncrement },
};

/* static */
const PageDefinition Editor::page_definition_[] = {
  { PAGE_OSC_OSC_1, PAGE_OSC_OSC_2, GROUP_OSC,
    STR_RES_OSCILLATOR_1, PARAMETER_EDITOR, 0 },
  { PAGE_OSC_OSC_2, PAGE_OSC_OSC_MIX, GROUP_OSC,
    STR_RES_OSCILLATOR_2, PARAMETER_EDITOR, 4 },
  { PAGE_OSC_OSC_MIX, PAGE_OSC_OSC_1, GROUP_OSC,
    STR_RES_MIXER, PARAMETER_EDITOR, 8 },
  { PAGE_FILTER_FILTER, PAGE_FILTER_FILTER, GROUP_FILTER,
    STR_RES_FILTER, PARAMETER_EDITOR, 12 },
  { PAGE_MOD_ENV_1, PAGE_MOD_ENV_2, GROUP_MOD,
    STR_RES_ENVELOPE_1, PARAMETER_EDITOR, 16 },
  { PAGE_MOD_ENV_2, PAGE_MOD_LFO, GROUP_MOD,
    STR_RES_ENVELOPE_2, PARAMETER_EDITOR, 20 },
  { PAGE_MOD_LFO, PAGE_MOD_MATRIX, GROUP_MOD,
    STR_RES_LFOS, PARAMETER_EDITOR, 24 },
  { PAGE_MOD_MATRIX, PAGE_MOD_ENV_1, GROUP_MOD,
    STR_RES_MODULATION, PARAMETER_EDITOR, 28 },
  { PAGE_PLAY_ARP, PAGE_PLAY_STEP_SEQUENCER, GROUP_PLAY,
    STR_RES_ARPEGGIO, PARAMETER_EDITOR, 32 },
  { PAGE_PLAY_STEP_SEQUENCER, PAGE_PLAY_KBD, GROUP_PLAY,
    STR_RES_SEQUENCER, STEP_SEQUENCER, 40 },
  { PAGE_PLAY_KBD, PAGE_PLAY_ARP, GROUP_PLAY,
    STR_RES_KEYBOARD, PARAMETER_EDITOR, 36 },
  { PAGE_LOAD_SAVE, PAGE_LOAD_SAVE, GROUP_LOAD_SAVE,
    STR_RES_PATCH_BANK, LOAD_SAVE, 0 },
  { PAGE_PERFORMANCE, PAGE_PERFORMANCE, GROUP_PERFORMANCE,
    STR_RES_PERFORMANCE, PARAMETER_EDITOR, 0 },
};

/* <static> */
uint8_t Editor::current_display_type_;

ParameterPage Editor::current_page_;
ParameterPage Editor::last_visited_page_[kNumGroups] = {
    PAGE_OSC_OSC_1,
    PAGE_FILTER_FILTER,
    PAGE_MOD_ENV_1,
    PAGE_PLAY_ARP,
    PAGE_LOAD_SAVE,
    PAGE_PERFORMANCE
};
uint8_t Editor::current_knob_;
uint8_t Editor::last_visited_subpage_ = 0;

char Editor::line_buffer_[kLcdWidth * kLcdHeight + 1];

uint8_t Editor::cursor_;
uint8_t Editor::subpage_;
uint8_t Editor::action_;
uint8_t Editor::current_patch_number_ = 0;
uint8_t Editor::previous_patch_number_ = 0;
uint8_t Editor::test_note_playing_ = 0;
uint8_t Editor::assign_in_progress_ = 0;
ParameterAssignment Editor::assigned_parameters_[kNumEditingPots] = {
  { 1, 0 },
  { PRM_FILTER_CUTOFF, 0 },
  { PRM_FILTER_ENV, 0 },
  { 25, 0 },
};
ParameterAssignment Editor::parameter_to_assign_;
/* </static> */

/* static */
void Editor::Init() {
  current_page_ = PAGE_FILTER_FILTER;
  // Check that the pages are in the right order in the PageDefinition
  // structure. Otherwise we won't be able to do fancy addressing.
  for (uint8_t i = 0; i < kNumPages; ++i) {
    CHECK_EQ(page_definition_[i].id, i);
  }
  line_buffer_[kLcdWidth] = '\0';
}

/* static */
void Editor::DoShiftFunction(ParameterGroup group, uint8_t hold_time) {
  switch (group) {
    case GROUP_OSC:
      if (current_page_ == PAGE_LOAD_SAVE) {
        engine.ResetPatch();
      } else {
        ToggleGroup(GROUP_PERFORMANCE);
      }
      break;
      
    case GROUP_FILTER:
      if (current_page_ == PAGE_LOAD_SAVE) {
        RandomizePatch();
      } else if (current_page_ <= PAGE_PLAY_KBD) {
        parameter_to_assign_.id = page_definition_[
            current_page_].first_parameter_index + current_knob_;
        parameter_to_assign_.subpage = subpage_;
        DisplaySplashScreen(STR_RES_TOUCH_A_KNOB_TO);
        assign_in_progress_ = 1;
      }
      break;
      
    case GROUP_MOD:
      if (hold_time > 8 /* 1.5 SECONDS */) {
        if (current_page_ == PAGE_LOAD_SAVE) {
          display.set_status('O');
          display.ToggleSplashScreen();
          display.set_status('@');
        } else {
          engine.AllSoundOff(0);
          test_note_playing_ = 0;
          display.Init();
          current_display_type_ = PAGE_TYPE_DETAILS;
          DisplaySummary();
        }
      }
      break;

    case GROUP_PLAY:
      engine.NoteOn(0, 48, test_note_playing_ ? 0 : 100);
      test_note_playing_ ^= 1;
      break;
      
    case GROUP_LOAD_SAVE:
      if (hold_time > 8 /* 2.048 seconds */) {
        DisplaySplashScreen(STR_RES_READY);
        SystemReset(WDTO_500MS);
      }
      break;
  }
}

/* static */
void Editor::ToggleGroup(ParameterGroup group) {
  cursor_ = 0;
  subpage_ = 0;
  assign_in_progress_ = 0;
  display.set_cursor_position(kLcdNoCursor);
  current_display_type_ = PAGE_TYPE_DETAILS;
  // Special case for the load/save page.
  if (group == GROUP_LOAD_SAVE) {
    current_page_ = PAGE_LOAD_SAVE;
    EnterLoadSaveMode();
  } else {
    // Make sure that we won't confirm a save when moving back to the
    // Load/save page.
    action_ = ACTION_EXIT;
    // If we move to another group, go to the last visited page in this group.
    if (group != page_definition_[current_page_].group) {
      current_page_ = last_visited_page_[group];
    } else {
      current_page_ = page_definition_[current_page_].next;
    }
    // When switching to the modulation matrix page, go back to the previously
    // edited modulation.
    if (current_page_ == PAGE_MOD_MATRIX) {
        subpage_ = last_visited_subpage_;
    }
    last_visited_page_[group] = current_page_;
  }
}

/* static */
void Editor::RandomizeParameter(uint8_t subpage, uint8_t parameter_index) {
  const ParameterDefinition& parameter = PatchMetadata::parameter_definition(
      parameter_index);
  uint8_t range = parameter.max_value - parameter.min_value + 1;
  uint8_t value = Random::GetByte();
  while (value >= range) {
    value -= range;
  }
  value += parameter.min_value;
  engine.SetParameter(parameter.id + subpage * 3, value);
}

/* static */
void Editor::RandomizePatch() {
  // Randomize all the main parameters
  for (uint8_t parameter = 0; parameter < 28; ++parameter) {
    RandomizeParameter(0, parameter);
  }
  for (uint8_t slot = 0; slot < kModulationMatrixSize; ++slot) {
    for (uint8_t parameter = 29; parameter < 32; ++parameter) {
      RandomizeParameter(slot, parameter);
    }
  }
}

/* static */
void Editor::HandleInput(uint8_t knob_index, uint16_t value) {
  (*ui_handler_[page_definition_[current_page_].ui_type].input_handler)(
      knob_index, value);
}

/* static */
void Editor::HandleIncrement(int8_t direction) {
  (*ui_handler_[page_definition_[current_page_].ui_type].increment_handler)(
      direction);
}

/* static */
void Editor::DisplaySummary() {
  // No need to render the summary page twice.
  if (current_display_type_ == PAGE_TYPE_SUMMARY) {
    return;
  }
  (*ui_handler_[page_definition_[current_page_].ui_type].summary_page)();
  current_display_type_ = PAGE_TYPE_SUMMARY;
}

/* static */
void Editor::DisplayDetails() {
  current_display_type_ = PAGE_TYPE_DETAILS;
  (*ui_handler_[page_definition_[current_page_].ui_type].details_page)();
}

/* static */
void Editor::EnterLoadSaveMode() {
  if (current_page_ == PAGE_LOAD_SAVE && action_ == ACTION_SAVE) {
    // The Load/save button has been pressed twice, we were in the load/save
    // mode, and the action was set to "save": all the conditions are met to
    // overwrite the patch.
    engine.mutable_patch()->EepromSave(current_patch_number_);
    engine.mutable_patch()->SysExSend();
  }
  current_page_ = PAGE_LOAD_SAVE;
  previous_patch_number_ = current_patch_number_;
  engine.mutable_patch()->Backup();
  action_ = ACTION_EXIT;
}

/* static */
void Editor::HandleLoadSaveInput(uint8_t knob_index, uint16_t value) {
  switch (knob_index) {
    case 0:
      {
        const uint8_t num_patches = kEepromSize / kSerializedPatchSize;
        uint8_t new_patch = value * num_patches / 1024;
        if (new_patch != current_patch_number_ && action_ == ACTION_LOAD) {
          engine.mutable_patch()->EepromLoad(new_patch);
          engine.TouchPatch();
        }
        if (action_ != ACTION_EXIT) {
          current_patch_number_ = new_patch;
        }
      }
      break;
    case 1:
      if (action_ == ACTION_SAVE) {
        cursor_ = value * kPatchNameSize / 1024;
      }
      break;
    case 2:
      if (action_ == ACTION_SAVE) {
        value += (value << 1);
        engine.mutable_patch()->name[cursor_] = 32 + (value >> 5);
      }
      break;
    case 3:
      if (value < 64) {
        action_ = ACTION_LOAD;
      } else {
        // We are leaving the load mode - restore the previously saved patch.
        if (action_ == ACTION_LOAD) {
          current_patch_number_ = previous_patch_number_;
          engine.mutable_patch()->Restore();
          engine.TouchPatch();
        }
        action_ = value >= 960 ? ACTION_SAVE : ACTION_EXIT;
      }
      break;
  }  
}

/* static */
void Editor::HandleLoadSaveIncrement(int8_t direction) {
  if (action_ == ACTION_SAVE) {
    uint8_t value = engine.patch().name[cursor_];
    value += direction;
    if (value >= 32 && value <= 128) {
      engine.mutable_patch()->name[cursor_] = value;
    }
  }
}

/* static */
void Editor::DisplayLoadSavePage() {
  // 0123456789abcdef
  // load/save patch
  // 32 barbpapa save 
  ResourcesManager::LoadStringResource(
      STR_RES_PATCH_BANK,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display.Print(0, line_buffer_);
  
  UnsafeItoa<int16_t>(current_patch_number_ + 1, 2, line_buffer_);
  AlignRight(line_buffer_, 2);
  line_buffer_[2] = ' ';
  memcpy(line_buffer_ + 3, engine.patch().name, kPatchNameSize);
  line_buffer_[11] = ' ';
  if (action_ == ACTION_SAVE) {
    display.set_cursor_position(kLcdWidth + 3 + cursor_);
  } else {
    display.set_cursor_position(kLcdNoCursor);
  }
  ResourcesManager::LoadStringResource(
      action_ + STR_RES_LOAD,
      line_buffer_ + 12,
      kColumnWidth);
  display.Print(1, line_buffer_);
}

/* static */
void Editor::DisplayStepSequencerPage() {
  // 0123456789abcdef
  // step sequencer
  // 0000ffff44449999
  ResourcesManager::LoadStringResource(
      STR_RES_STEP_SEQUENCER,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display.Print(0, line_buffer_);
  for (uint8_t i = 0; i < 16; ++i) {
    uint8_t value = engine.patch().sequence_step(i) >> 4;
    line_buffer_[i] = i < engine.patch().pattern_size ?
        NibbleToAscii(value) : ' ';
  }
  display.Print(1, line_buffer_);
  display.set_cursor_position(kLcdWidth + cursor_);
}

/* static */
void Editor::HandleStepSequencerInput(
    uint8_t knob_index,
    uint16_t value) {
  if (assign_in_progress_) {
    assigned_parameters_[knob_index] = parameter_to_assign_;
    assign_in_progress_ = 0;
    ToggleGroup(GROUP_PERFORMANCE);
  } else {
    switch (knob_index) {
      case 0:
        engine.mutable_patch()->pattern_rotation = value >> 6;
        current_knob_ = 0;
        break;
      case 1:
        {
          cursor_ = value >> 6;
          uint8_t max_position = engine.GetParameter(PRM_ARP_PATTERN_SIZE) - 1;
          if (cursor_ > max_position) {
            cursor_ = max_position;
          }
        }
        break;
      case 2:
        engine.mutable_patch()->set_sequence_step(cursor_, value >> 2);
        break;
      case 3:
        {
          uint8_t new_size = (value >> 6) + 1;
          if (cursor_ >= new_size) {
            cursor_ = new_size - 1;
          }
          engine.SetParameter(PRM_ARP_PATTERN_SIZE, new_size);
          current_knob_ = 1;
        }
        break;
    }
  }
}

/* static */
void Editor::HandleStepSequencerIncrement(int8_t direction) {
  engine.mutable_patch()->set_sequence_step(cursor_, 
      engine.patch().sequence_step(cursor_) + (direction << 4));
}

/* static */
void Editor::DisplayEditSummaryPage() {
  // 0123456789abcdef
  // foo bar baz bad
  //  63 127   0   0
  for (uint8_t i = 0; i < kNumEditingPots; ++i) {
    uint8_t index = KnobIndexToParameterId(i);
    const ParameterDefinition& parameter = PatchMetadata::parameter_definition(
        index);
    ResourcesManager::LoadStringResource(
        parameter.short_name,
        line_buffer_ + i * kColumnWidth,
        kColumnWidth - 1);
    line_buffer_[i * kColumnWidth + kColumnWidth - 1] = '\0';
    AlignRight(line_buffer_ + i * kColumnWidth, kColumnWidth);
    PrettyPrintParameterValue(
        parameter,
        line_buffer_ + i * kColumnWidth + kLcdWidth + 1,
        kColumnWidth - 1);
    line_buffer_[i * kColumnWidth + kColumnWidth + kLcdWidth] = '\0';
    AlignRight(line_buffer_ + i * kColumnWidth + kLcdWidth + 1, kColumnWidth);
  }
  display.Print(0, line_buffer_);
  display.Print(1, line_buffer_ + kLcdWidth + 1);
}

/* static */
void Editor::DisplayEditDetailsPage() {  
  // 0123456789abcdef
  // filter
  // cutoff       127
  //
  // OR
  //
  // mod src>dst
  // amount        63
  if (current_page_ == PAGE_MOD_MATRIX) {
    const ParameterDefinition& current_source = (
        PatchMetadata::parameter_definition(
            page_definition_[PAGE_MOD_MATRIX].first_parameter_index + 1));
    PrettyPrintParameterValue(
        current_source,
        line_buffer_ + 4,
        kColumnWidth - 1);
    const ParameterDefinition& current_destination = (
        PatchMetadata::parameter_definition(
            page_definition_[PAGE_MOD_MATRIX].first_parameter_index + 2));
    PrettyPrintParameterValue(
        current_destination,
        line_buffer_ + kColumnWidth + 4,
        kColumnWidth);
    line_buffer_[0] = 'm';
    line_buffer_[1] = 'o';
    line_buffer_[2] = 'd';
    line_buffer_[3] = ' ';
    line_buffer_[kColumnWidth + 3] = '>';
    AlignLeft(line_buffer_ + kColumnWidth + 4, kLcdWidth - kColumnWidth - 4);
    display.Print(0, line_buffer_);
  }
  uint8_t index = KnobIndexToParameterId(current_knob_);
  const ParameterDefinition& parameter = PatchMetadata::parameter_definition(
      index);
  const PageDefinition& page = page_definition_[current_page_];

  if (current_page_ != PAGE_MOD_MATRIX) {
    ResourcesManager::LoadStringResource(
        page.name,
        line_buffer_,
        kLcdWidth);
    AlignLeft(line_buffer_, kLcdWidth);
    display.Print(0, line_buffer_);
  }
  
  ResourcesManager::LoadStringResource(
      parameter.long_name,
      line_buffer_,
      kCaptionWidth);
  AlignLeft(line_buffer_, kCaptionWidth);
  
  PrettyPrintParameterValue(
      parameter,
      line_buffer_ + kCaptionWidth,
      kValueWidth);
  AlignRight(line_buffer_ + kCaptionWidth, kValueWidth);
  display.Print(1, line_buffer_);
}

/* static */
uint8_t Editor::KnobIndexToParameterId(uint8_t knob_index) {
  if (current_page_ == PAGE_PERFORMANCE) {
    subpage_ = assigned_parameters_[knob_index].subpage;
    return assigned_parameters_[knob_index].id;
  } else {
    return page_definition_[current_page_].first_parameter_index + \
        knob_index;
  }
}

/* static */
void Editor::HandleEditInput(uint8_t knob_index, uint16_t value) {
  if (assign_in_progress_) {
    assigned_parameters_[knob_index] = parameter_to_assign_;
    assign_in_progress_ = 0;
    ToggleGroup(GROUP_PERFORMANCE);
  } else {
    uint8_t value_7bits = value >> 3;
    uint8_t index = KnobIndexToParameterId(knob_index);
    const ParameterDefinition& parameter = PatchMetadata::parameter_definition(
        index);
    SetParameterValue(
        parameter.id,
        PatchMetadata::Scale(parameter, value_7bits));
    current_knob_ = knob_index;
  }
}

/* static */
void Editor::HandleEditIncrement(int8_t direction) {
  uint8_t index = KnobIndexToParameterId(current_knob_);
  const ParameterDefinition& parameter = PatchMetadata::parameter_definition(
      index);
  
  int16_t value = GetParameterValue(parameter.id);
  if (parameter.unit == UNIT_INT8) {
    value = static_cast<int16_t>(static_cast<int8_t>(value));
    value += direction;
    if (value >= static_cast<int8_t>(parameter.min_value) &&
        value <= static_cast<int8_t>(parameter.max_value)) {
      SetParameterValue(parameter.id, value);
    }
  } else {
    value += direction;
    if (value >= parameter.min_value && value <= parameter.max_value) {
      SetParameterValue(parameter.id, value);
    }
  }
}

/* static */
void Editor::SetParameterValue(uint8_t id, uint8_t value) {
  // Dirty hack for the modulation page.
  if (current_page_ == PAGE_MOD_MATRIX && id == PRM_MOD_ROW) {
    subpage_ = value;
    last_visited_subpage_ = value;
  } else {
    engine.SetParameter(id + subpage_ * 3, value);
  }
}

/* static */
uint8_t Editor::GetParameterValue(uint8_t id) {
  uint8_t value;
  if (current_page_ == PAGE_MOD_MATRIX && id == PRM_MOD_ROW) {
    value = subpage_;
  } else {
    value = engine.GetParameter(id + subpage_ * 3);
  }
  return value;
}

/* static */
void Editor::DisplaySplashScreen(ResourceId first_line) {
  // 0123456789abcdef
  // mutable 
  // instruments sh-1
  for (uint8_t i = 0; i < 2; ++i) {
    ResourcesManager::LoadStringResource(
        first_line + i,
        line_buffer_,
        kLcdWidth);
    AlignLeft(line_buffer_, kLcdWidth);
    display.Print(i, line_buffer_);
  }
}

/* static */
void Editor::PrettyPrintParameterValue(const ParameterDefinition& parameter,
                                       char* buffer, uint8_t width) {
  int16_t value = GetParameterValue(parameter.id);
  ResourceId text = ResourcesManager::Lookup<uint8_t, uint8_t>(
      units_definitions,
      parameter.unit);
  char prefix = '\0';
  switch (parameter.unit) {
    case UNIT_INT8:
      value = int16_t(int8_t(value));
      break;
    case UNIT_INDEX:
      ++value;
      break;
    case UNIT_MODULATION_SOURCE:
      if (width <= 4) {
        text = STR_RES_LF1;
      }
      break;
    case UNIT_MODULATION_DESTINATION:
      if (width <= 4) {
        text = STR_RES_CUT;
      }
      break;
    case UNIT_LFO_RATE:
      if (value >= 16) {
        value = value - 16;
      } else {
        ++value;
        prefix = 'x';
      }
      break;
    case UNIT_PATTERN:
      prefix = ResourcesManager::Lookup<uint8_t, uint8_t>(
          arp_pattern_prefix,
          value & 0x03);
      value = (value >> 2) + 1;
      break;
    case UNIT_TEMPO_WITH_EXTERNAL_CLOCK:
      if (value < 40) {
        value = value - 35;
        text = STR_RES_EXTERN;
      } else if (value > 240) {
        value = ResourcesManager::Lookup<uint16_t, uint8_t>(
            lut_res_turbo_tempi, value - 240 - 1);
      }
      break;
    case UNIT_GROOVE_PATTERN:
      prefix = ResourcesManager::Lookup<uint8_t, uint8_t>(
          arp_groove_template_prefix,
          value >> 4);
      value = value & 0xf;
      break;
    case UNIT_MIDI_CHANNEL:
      if (value >= 34) {
        prefix = '>';
        value -= 34;
      }
      else if (value >= 17) {
        prefix = '+';
        value -= 17;
      }
      break;
  }
  if (prefix) {
    *buffer++ = prefix;
    --width;
  }
  if (text) {
    ResourcesManager::LoadStringResource(text + value, buffer, width);
  } else {
    UnsafeItoa<int16_t>(value, width, buffer);
  }
}

}  // namespace hardware_shruti
