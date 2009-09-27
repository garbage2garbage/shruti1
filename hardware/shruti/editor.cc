// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Parameter editor.
//
// I really hate this code. Work on a "UI" framework to avoid such horrible
// things in progress...

#include "hardware/shruti/editor.h"
#include "hardware/shruti/display.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/string.h"

using namespace hardware_io;
using namespace hardware_utils;

namespace hardware_shruti {
  
/* extern */
Editor editor;

static const prog_char raw_parameter_definition[
    kNumEditableParameters * sizeof(ParameterDefinition)] PROGMEM = {
  // Osc 1.
  PRM_OSC_ALGORITHM_1,
  WAVEFORM_NONE, WAVEFORM_ANALOG_WAVETABLE,
  PAGE_OSC_OSC_1, UNIT_WAVEFORM,
  STR_RES_ALGORITHM, STR_RES_ALGORITHM,
  
  PRM_OSC_PARAMETER_1,
  0, 127,
  PAGE_OSC_OSC_1, UNIT_RAW_UINT8,
  STR_RES_PRM, STR_RES_PARAMETER,
  
  PRM_OSC_RANGE_1,
  -12, 12,
  PAGE_OSC_OSC_1, UNIT_INT8,
  STR_RES_RNG, STR_RES_RANGE,
  
  PRM_OSC_OPTION_1,
  SUM, RING_MOD,
  PAGE_OSC_OSC_1, UNIT_OPERATOR,
  STR_RES_OP, STR_RES_OPERATOR,

  // Osc 2.
  PRM_OSC_ALGORITHM_2,
  WAVEFORM_IMPULSE_TRAIN, WAVEFORM_TRIANGLE,
  PAGE_OSC_OSC_2, UNIT_WAVEFORM,
  STR_RES_ALGORITHM, STR_RES_ALGORITHM,
  
  PRM_OSC_PARAMETER_2,
  0, 127,
  PAGE_OSC_OSC_2, UNIT_RAW_UINT8,
  STR_RES_PRM, STR_RES_PARAMETER,
  
  PRM_OSC_RANGE_2,
  -24, 24, 
  PAGE_OSC_OSC_2, UNIT_INT8,
  STR_RES_RNG, STR_RES_RANGE,
  
  PRM_OSC_OPTION_2,
  0, 127,
  PAGE_OSC_OSC_2, UNIT_RAW_UINT8,
  STR_RES_TUN, STR_RES_DETUNE,

  // Mix balance.
  PRM_MIX_BALANCE,
  0, 127,
  PAGE_OSC_OSC_MIX, UNIT_RAW_UINT8,
  STR_RES_MIX, STR_RES_OSC_BAL,

  PRM_MIX_SUB_OSC,
  0, 127,
  PAGE_OSC_OSC_MIX, UNIT_UINT8,
  STR_RES_SUB, STR_RES_SUB_OSC_,
  
  PRM_MIX_NOISE,
  0, 127,
  PAGE_OSC_OSC_MIX, UNIT_UINT8,
  STR_RES_NOI, STR_RES_NOISE,

  PRM_MIX_SUB_OSC_ALGORITHM,
  WAVEFORM_SQUARE, WAVEFORM_TRIANGLE,
  PAGE_OSC_OSC_MIX, UNIT_WAVEFORM, 
  STR_RES_ALGORITHM, STR_RES_ALGORITHM,
  
  // Filter.
  PRM_FILTER_CUTOFF,
  0, 127,
  PAGE_FILTER_FILTER, UNIT_RAW_UINT8,
  STR_RES_CUT, STR_RES_CUTOFF,
  
  PRM_FILTER_RESONANCE,
  0, 127,
  PAGE_FILTER_FILTER, UNIT_RAW_UINT8,
  STR_RES_RES, STR_RES_RESONANCE,
  
  PRM_FILTER_ENV,
  0, 64,
  PAGE_FILTER_FILTER, UNIT_INT8,
  STR_RES_ENVTVCF, STR_RES_ENVTVCF,
  
  PRM_FILTER_LFO,
  0, 64,
  PAGE_FILTER_FILTER, UNIT_INT8,
  STR_RES_LFO2TVCF, STR_RES_LFO2TVCF,

  // Env 1.
  PRM_ENV_ATTACK_1,
  0, 127,
  PAGE_MOD_ENV_1, UNIT_RAW_UINT8,
  STR_RES_ATK, STR_RES_ATTACK,
  
  PRM_ENV_DECAY_1,
  0, 127,
  PAGE_MOD_ENV_1, UNIT_RAW_UINT8,
  STR_RES_DECAY, STR_RES_DECAY,
  
  PRM_ENV_SUSTAIN_1,
  0, 127,
  PAGE_MOD_ENV_1, UNIT_RAW_UINT8,
  STR_RES_SUSTAIN, STR_RES_SUSTAIN,
  
  PRM_ENV_RELEASE_1,
  0, 127,
  PAGE_MOD_ENV_1, UNIT_RAW_UINT8,
  STR_RES_RELEASE, STR_RES_RELEASE,

  // Env 2.
  PRM_ENV_ATTACK_2,
  0, 127,
  PAGE_MOD_ENV_2, UNIT_RAW_UINT8,
  STR_RES_ATK, STR_RES_ATTACK,
  
  PRM_ENV_DECAY_2,
  0, 127,
  PAGE_MOD_ENV_2, UNIT_RAW_UINT8,
  STR_RES_DECAY, STR_RES_DECAY,
  
  PRM_ENV_SUSTAIN_2,
  0, 127,
  PAGE_MOD_ENV_2, UNIT_RAW_UINT8,
  STR_RES_SUSTAIN, STR_RES_SUSTAIN,
  
  PRM_ENV_RELEASE_2,
  0, 127,
  PAGE_MOD_ENV_2, UNIT_RAW_UINT8,
  STR_RES_RELEASE, STR_RES_RELEASE,

  // Lfo.
  PRM_LFO_WAVE_1,
  LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_RAMP,
  PAGE_MOD_LFO, UNIT_LFO_WAVEFORM,
  STR_RES_WV1, STR_RES_LFO1_WAVE,
  
  PRM_LFO_RATE_1,
  0, 127 + 16,
  PAGE_MOD_LFO, UNIT_LFO_RATE,
  STR_RES_RT1, STR_RES_LFO1_RATE,
  
  PRM_LFO_WAVE_2,
  LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_RAMP,
  PAGE_MOD_LFO, UNIT_LFO_WAVEFORM,
  STR_RES_WV2, STR_RES_LFO2_WAVE,
  
  PRM_LFO_RATE_2,
  0, 127 + 16,
  PAGE_MOD_LFO, UNIT_LFO_RATE,
  STR_RES_RT2, STR_RES_LFO2_RATE,
  
  // Modulation
  PRM_MOD_ROW,
  0, kModulationMatrixSize - 1,
  PAGE_MOD_MATRIX, UNIT_INDEX,
  STR_RES_MOD_, STR_RES_MOD_,

  PRM_MOD_SOURCE,
  0, kNumModulationSources - 1,
  PAGE_MOD_MATRIX, UNIT_MODULATION_SOURCE,
  STR_RES_SRC, STR_RES_SOURCE,
  
  PRM_MOD_DESTINATION,
  0, kNumModulationDestinations - 1,
  PAGE_MOD_MATRIX, UNIT_MODULATION_DESTINATION,
  STR_RES_DST, STR_RES_DEST_,
  
  PRM_MOD_AMOUNT,
  -64, 64,
  PAGE_MOD_MATRIX, UNIT_INT8,
  STR_RES_AMT, STR_RES_AMOUNT,

  // Arpeggiator.
  PRM_ARP_TEMPO,
  24, 240,
  PAGE_PLAY_ARP, UNIT_TEMPO_WITH_EXTERNAL_CLOCK,
  STR_RES_BPM, STR_RES_TEMPO,
  
  PRM_ARP_OCTAVES,
  OFF, 4,
  PAGE_PLAY_ARP, UNIT_UINT8,
  STR_RES_OCTAVE, STR_RES_OCTAVE,
  
  PRM_ARP_PATTERN,
  0, 23, 
  PAGE_PLAY_ARP, UNIT_PATTERN,
  STR_RES_PATTERN, STR_RES_PATTERN,
  
  PRM_ARP_SWING,
  0, 127, 
  PAGE_PLAY_ARP, UNIT_RAW_UINT8,
  STR_RES_SWG, STR_RES_SWING,
  
  // Keyboard management.
  PRM_KBD_OCTAVE,
  -2, +2,
  PAGE_PLAY_KBD, UNIT_INT8,
  STR_RES_OCTAVE, STR_RES_OCTAVE,
  
  PRM_KBD_RAGA,
  0, 77, 
  PAGE_PLAY_KBD, UNIT_RAGA,
  STR_RES_RAGA, STR_RES_RAGA,
  
  PRM_KBD_PORTAMENTO,
  0, 127,
  PAGE_PLAY_KBD, UNIT_RAW_UINT8,
  STR_RES_PRT, STR_RES_PORTA,
  
  PRM_KBD_MIDI_CHANNEL,
  0, 16, 
  PAGE_PLAY_KBD, UNIT_UINT8,
  STR_RES_CHN, STR_RES_MIDI_CHAN
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
  { PAGE_OSC_OSC_1, GROUP_OSC, STR_RES_OSCILLATOR_1, PARAMETER_EDITOR },
  { PAGE_OSC_OSC_2, GROUP_OSC, STR_RES_OSCILLATOR_2, PARAMETER_EDITOR },
  { PAGE_OSC_OSC_MIX, GROUP_OSC, STR_RES_MIXER, PARAMETER_EDITOR },
  { PAGE_FILTER_FILTER, GROUP_FILTER, STR_RES_FILTER, PARAMETER_EDITOR },
  { PAGE_MOD_ENV_1, GROUP_MOD, STR_RES_ENVELOPE_1, PARAMETER_EDITOR },
  { PAGE_MOD_ENV_2, GROUP_MOD, STR_RES_ENVELOPE_2, PARAMETER_EDITOR },
  { PAGE_MOD_LFO, GROUP_MOD, STR_RES_LFOS, PARAMETER_EDITOR },
  { PAGE_MOD_MATRIX, GROUP_MOD, STR_RES_MODULATION, PARAMETER_EDITOR },
  { PAGE_PLAY_ARP, GROUP_PLAY, STR_RES_ARPEGGIO, PARAMETER_EDITOR },
  { PAGE_PLAY_STEP_SEQUENCER, GROUP_PLAY, STR_RES_SEQUENCER, STEP_SEQUENCER },
  { PAGE_PLAY_KBD, GROUP_PLAY, STR_RES_KEYBOARD, PARAMETER_EDITOR },
  { PAGE_LOAD_SAVE, GROUP_LOAD_SAVE, STR_RES_LOAD_SAVE_PATCH, LOAD_SAVE }
};

/* <static> */
ParameterDefinition Editor::parameter_definition_;
uint8_t Editor::parameter_definition_index_;
uint8_t Editor::current_display_type_;

ParameterPage Editor::current_page_;
ParameterPage Editor::last_visited_page_[kNumGroups];
uint8_t Editor::current_controller_;
uint8_t Editor::parameter_definition_offset_[kNumPages][kNumControllers];

char Editor::line_buffer_[kLcdWidth * kLcdHeight + 1];

uint8_t Editor::cursor_;
uint8_t Editor::subpage_;
uint8_t Editor::action_;
uint8_t Editor::current_patch_number_;
uint8_t Editor::previous_patch_number_;
/* </static> */

/* static */
void Editor::Init() {
  parameter_definition_index_ = 0xff;
  current_page_ = PAGE_FILTER_FILTER;
  current_controller_ = 0;
  last_visited_page_[GROUP_OSC] = PAGE_OSC_OSC_1;
  last_visited_page_[GROUP_FILTER] = PAGE_FILTER_FILTER;
  last_visited_page_[GROUP_MOD] = PAGE_MOD_ENV_1;
  last_visited_page_[GROUP_PLAY] = PAGE_PLAY_ARP;
  last_visited_page_[GROUP_LOAD_SAVE] = PAGE_LOAD_SAVE;
  for (uint8_t i = 0; i < kNumEditableParameters; ++i) {
    parameter_definition_offset_[parameter_definition(i).page]
        [i % kNumControllers] = i;
  }
  // Check that the pages are in the right order in the PageDefinition
  // structure. Otherwise we won't be able to do fancy addressing.
  for (uint8_t i = 0; i < kNumPages; ++i) {
    CHECK_EQ(page_definition_[i].id, i);
  }
  line_buffer_[kLcdWidth] = '\0';
  previous_patch_number_ = 0;
  current_patch_number_ = 0;
}

/* static */
void Editor::ToggleGroup(ParameterGroup group) {
  cursor_ = 0;
  subpage_ = 0;
  display.set_cursor_position(kLcdNoCursor);
  current_display_type_ = PAGE_TYPE_DETAILS;
  // Special case for the load/save page.
  if (group == GROUP_LOAD_SAVE) {
    current_page_ = PAGE_LOAD_SAVE;
    EnterLoadSaveMode();
  } else {
    uint8_t first_in_group = 255;
    uint8_t last_in_group = 255;
  
    // Get the first and last page in the requested group.
    for (uint8_t i = 0; i < kNumPages; ++i) {
      if (page_definition_[i].group == group) {
        if (first_in_group == 255) {
          first_in_group = i;
        }
        last_in_group = i;
      }
    }
    if (group != page_definition_[current_page_].group) {
      // If we move to another group, go to the last visited page in this group.
      current_page_ = last_visited_page_[group];
    } else {
      // Otherwise, cycle the pages in the current group.
      if (current_page_ == last_in_group) {
        current_page_ = first_in_group;
      } else {
        current_page_ = current_page_ + 1;
      }
    }
    last_visited_page_[group] = current_page_;
  }
}

/* static */
void Editor::HandleInput(uint8_t controller_index, uint16_t value) {
  (*ui_handler_[page_definition_[current_page_].ui_type].input_handler)(
      controller_index, value);
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
void Editor::HandleLoadSaveInput(uint8_t controller_index, uint16_t value) {
  switch (controller_index) {
    case 0:
      {
        const uint8_t num_patches = kEepromSize / kSerializedPatchSize;
        uint8_t new_patch = value * num_patches / 1024;
        if (new_patch != current_patch_number_ && action_ == ACTION_LOAD) {
          engine.mutable_patch()->EepromLoad(new_patch);
          engine.TouchPatch();
        }
        current_patch_number_ = new_patch;
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
      STR_RES_LOAD_SAVE_PATCH,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display.Print(0, line_buffer_);
  
  Itoa<int16_t>(current_patch_number_ + 1, 2, line_buffer_);
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
    uint8_t controller_index,
    uint16_t value) {
  switch (controller_index) {
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
      }
      break;
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
  for (uint8_t i = 0; i < kNumControllers; ++i) {
    uint8_t index = parameter_definition_offset_[current_page_][i];
    const ParameterDefinition& parameter = parameter_definition(index);
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
    const ParameterDefinition& current_source = parameter_definition(
        parameter_definition_offset_[PAGE_MOD_MATRIX][1]);
    PrettyPrintParameterValue(
        current_source,
        line_buffer_ + 4,
        kColumnWidth - 1);
    const ParameterDefinition& current_destination = parameter_definition(
        parameter_definition_offset_[PAGE_MOD_MATRIX][2]);
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
  uint8_t index =
      parameter_definition_offset_[current_page_][current_controller_];
  const ParameterDefinition& parameter = parameter_definition(index);
  const PageDefinition& page = page_definition_[parameter.page];

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
void Editor::HandleEditInput(uint8_t controller_index, uint16_t value) {
  uint8_t new_value;
  uint8_t index = parameter_definition_offset_[current_page_][controller_index];
  const ParameterDefinition& parameter = parameter_definition(index);

  // Handle the simple case when the parameter can only take one value.
  if (parameter.min_value == parameter.max_value) {
    new_value = parameter.min_value;
  } else if (parameter.unit == UNIT_RAW_UINT8) {
    new_value = (value >> 3);
  } else {
    uint8_t range = parameter.max_value - parameter.min_value + 1;
    new_value = ((value >> 3) * range) >> 7;
    new_value += parameter.min_value;
  }
  SetParameterWithHacks(parameter.id, new_value);
  current_controller_ = controller_index;
}

/* static */
void Editor::SetParameterWithHacks(uint8_t id, uint8_t value) {
  // Set the tempo to 0 for external clock.
  if (id == PRM_ARP_TEMPO) {
    if (value < 40) {
      value = 0;
    }
  }
  
  // Dirty hack for the modulation page.
  if (current_page_ == PAGE_MOD_MATRIX) {
    if (id == PRM_MOD_ROW) {
      subpage_ = value;
    } else {
      engine.SetParameter(id + subpage_ * 3, value);
    }
  } else {
    engine.SetParameter(id, value);
  }
}

/* static */
uint8_t Editor::GetParameterWithHacks(uint8_t id) {
  if (current_page_ == PAGE_MOD_MATRIX) {
    if (id == PRM_MOD_ROW) {
      return subpage_;
    } else {
      return engine.GetParameter(id + subpage_ * 3);
    }
  } else {
    uint8_t value = engine.GetParameter(id);
    if (id == PRM_ARP_TEMPO && value == 0) {
      value = 39;
    }
    return value;
  }
}

/* static */
void Editor::HandleEditIncrement(int8_t direction) {
  uint8_t index = parameter_definition_offset_[current_page_][
      current_controller_];
  const ParameterDefinition& parameter = parameter_definition(index);
  
  int16_t value = GetParameterWithHacks(parameter.id);
  if (parameter.unit == UNIT_INT8) {
    value = int16_t(int8_t(value));
    value += direction;
    if (value >= int8_t(parameter.min_value) &&
        value <= int8_t(parameter.max_value)) {
      SetParameterWithHacks(parameter.id, value);
    }
  } else {
    value += direction;
    if (value >= parameter.min_value && value <= parameter.max_value) {
      SetParameterWithHacks(parameter.id, value);
    }
  }
}

/* static */
void Editor::DisplaySplashScreen(ResourceId first_line) {
  // 0123456789abcdef
  //     mutable 
  //   instruments
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
  int16_t value = GetParameterWithHacks(parameter.id);
  ResourceId base = 0;
  char prefix = '\0';
  switch (parameter.unit) {
    case UNIT_INT8:
      value = int16_t(int8_t(value));
      break;
    case UNIT_BOOLEAN:
      base = STR_RES_OFF;
      break;
    case UNIT_OPERATOR:
      base = STR_RES_1S2;
      break;
    case UNIT_WAVEFORM:
      base = STR_RES_NONE;
      break;
    case UNIT_LFO_WAVEFORM:
      base = STR_RES_TRI;
      break;
    case UNIT_RAGA:
      base = STR_RES_EQUAL;
      break;
    case UNIT_INDEX:
      value++;
      break;
    case UNIT_PATTERN:
      base = STR_RES_3_1;
      break;
    case UNIT_MODULATION_SOURCE:
      base = (width <= 4) ? STR_RES_LF1 : STR_RES_LFO1;
      break;
    case UNIT_MODULATION_DESTINATION:
      base = (width <= 4) ? STR_RES_CUT : STR_RES_CUTOFF;
      break;
    case UNIT_LFO_RATE:
      if (value >= 16) {
        value = value - 16;
      } else {
        ++value;
        prefix = '/';
      }
      break;
    case UNIT_TEMPO_WITH_EXTERNAL_CLOCK:
      if (value == 39) {
        value = 0;
        base = STR_RES_EXTERN;
      }
  }
  if (prefix) {
    *buffer++ = prefix;
  }
  if (base) {
    ResourcesManager::LoadStringResource(base + value, buffer, width);
  } else {
    Itoa<int16_t>(value, width, buffer);
  }
}

/* static */
void Editor::ResetPatch() {
  engine.ResetPatch();
}

/* static */
const ParameterDefinition& Editor::parameter_definition(uint8_t index) {
  if (index != parameter_definition_index_) {
    parameter_definition_index_ = index;
    ResourcesManager::Load(
        raw_parameter_definition,
        index,
        &parameter_definition_);
  }
  return parameter_definition_;
}

}  // namespace hardware_shruti
