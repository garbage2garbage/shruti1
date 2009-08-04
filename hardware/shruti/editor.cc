// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Parameter editor.
//
// I really hate this code. Work on a "UI" framework to avoid such horrible
// things in progress...

#include "hardware/shruti/editor.h"

#include "hardware/io/pretty_printer.h"
#include "hardware/io/serial.h"
#include "hardware/shruti/patch_memory.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/string_utils.h"

using namespace hardware_io;
using namespace hardware_utils;

namespace hardware_shruti {

static const prog_char raw_parameter_definition[
    kNumEditableParameters * sizeof(ParameterDefinition)] PROGMEM = {
  // Osc 1.
  PRM_OSC_ALGORITHM_1,
  WAVEFORM_IMPULSE_TRAIN, WAVEFORM_ANALOG_WAVETABLE,
  PAGE_OSC_OSC_1, UNIT_WAVEFORM,
  STR_RES_ALG, STR_RES_ALGORITHM,
  
  PRM_OSC_PARAMETER_1,
  0, 127,
  PAGE_OSC_OSC_1, UNIT_RAW_UINT8,
  STR_RES_PRM, STR_RES_PARAMETER,
  
  PRM_OSC_RANGE_1,
  128 - 48, 128 + 48,
  PAGE_OSC_OSC_1, UNIT_INT8,
  STR_RES_RNG, STR_RES_RANGE,
  
  PRM_OSC_OPTION_1,
  OFF, ON,
  PAGE_OSC_OSC_1, UNIT_BOOLEAN,
  STR_RES_SYN, STR_RES_SYNC,

  // Osc 2.
  PRM_OSC_ALGORITHM_2,
  WAVEFORM_IMPULSE_TRAIN, WAVEFORM_TRIANGLE,
  PAGE_OSC_OSC_2, UNIT_WAVEFORM,
  STR_RES_ALG, STR_RES_ALGORITHM,
  
  PRM_OSC_PARAMETER_2,
  0, 127,
  PAGE_OSC_OSC_2, UNIT_RAW_UINT8,
  STR_RES_PRM, STR_RES_PARAMETER,
  
  PRM_OSC_RANGE_2,
  128 - 48, 128 + 48, 
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
  STR_RES_ALG, STR_RES_ALGORITHM,
  
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
  0, 127,
  PAGE_FILTER_FILTER, UNIT_RAW_UINT8,
  STR_RES_ENV, STR_RES_ENVTVCF,
  
  PRM_FILTER_LFO,
  0, 127,
  PAGE_FILTER_FILTER, UNIT_RAW_UINT8,
  STR_RES_LFO, STR_RES_LFO2TVCF,

  // Env.
  PRM_ENV_ATTACK,
  0, 127,
  PAGE_MOD_ENV, UNIT_RAW_UINT8,
  STR_RES_ATK, STR_RES_ATTACK,
  
  PRM_ENV_DECAY,
  0, 127,
  PAGE_MOD_ENV, UNIT_RAW_UINT8,
  STR_RES_DEC, STR_RES_DECAY,
  
  PRM_ENV_SUSTAIN,
  0, 127,
  PAGE_MOD_ENV, UNIT_RAW_UINT8,
  STR_RES_SUS, STR_RES_SUSTAIN,
  
  PRM_ENV_RELEASE,
  0, 127,
  PAGE_MOD_ENV, UNIT_RAW_UINT8,
  STR_RES_REL, STR_RES_RELEASE,

  // Lfo.
  PRM_LFO_WAVE_1,
  LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_SAW,
  PAGE_MOD_LFO, UNIT_LFO_WAVEFORM,
  STR_RES_WV1, STR_RES_LFO1_WAVE,
  
  PRM_LFO_RATE_1,
  0, 127,
  PAGE_MOD_LFO, UNIT_RAW_UINT8,
  STR_RES_RT1, STR_RES_LFO1_RATE,
  
  PRM_LFO_WAVE_2,
  LFO_WAVEFORM_TRIANGLE, LFO_WAVEFORM_SAW,
  PAGE_MOD_LFO, UNIT_LFO_WAVEFORM,
  STR_RES_WV2, STR_RES_LFO2_WAVE,
  
  PRM_LFO_RATE_2,
  0, 127,
  PAGE_MOD_LFO, UNIT_RAW_UINT8,
  STR_RES_RT2, STR_RES_LFO1_RATE,
  
  // Modulation
  PRM_MOD_ROW,
  0, 7,
  PAGE_MOD_MATRIX, UNIT_UINT8,
  STR_RES_MOD, STR_RES_MOD_,

  PRM_MOD_SOURCE,
  MOD_SRC_LFO_1, MOD_SRC_GATE,
  PAGE_MOD_MATRIX, UNIT_MODULATION_SOURCE,
  STR_RES_SRC, STR_RES_SOURCE,
  
  PRM_MOD_DESTINATION,
  MOD_DST_FILTER_CUTOFF, MOD_DST_FILTER_RESONANCE,
  PAGE_MOD_MATRIX, UNIT_MODULATION_DESTINATION,
  STR_RES_DST, STR_RES_DEST_,
  
  PRM_MOD_AMOUNT,
  0, 127,
  PAGE_MOD_MATRIX, UNIT_RAW_UINT8,
  STR_RES_AMT, STR_RES_AMOUNT,

  // Arpeggiator.
  PRM_ARP_TEMPO,
  24, 240,
  PAGE_PLAY_ARP, UNIT_TEMPO_WITH_EXTERNAL_CLOCK,
  STR_RES_BPM, STR_RES_TEMPO,
  
  PRM_ARP_OCTAVES,
  OFF, 4,
  PAGE_PLAY_ARP, UNIT_UINT8,
  STR_RES_OCT, STR_RES_OCTAVE,
  
  PRM_ARP_PATTERN,
  0, 23, 
  PAGE_PLAY_ARP, UNIT_PATTERN,
  STR_RES_PAT, STR_RES_PATTERN,
  
  PRM_ARP_SWING,
  0, 127, 
  PAGE_PLAY_ARP, UNIT_RAW_UINT8,
  STR_RES_SWG, STR_RES_SWING,
  
  // Keyboard management.
  PRM_KBD_OCTAVE,
  128 - 2, 128 + 2,
  PAGE_PLAY_KBD, UNIT_INT8,
  STR_RES_OCT, STR_RES_OCTAVE,
  
  PRM_KBD_RAGA,
  0, 77, 
  PAGE_PLAY_KBD, UNIT_RAGA,
  STR_RES_RAG, STR_RES_RAGA,
  
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
const PageDefinition Editor::page_definition_[] = {
  { PAGE_OSC_OSC_1, GROUP_OSC, STR_RES_OSCILLATOR_1,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput},
  { PAGE_OSC_OSC_2, GROUP_OSC, STR_RES_OSCILLATOR_2,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_OSC_OSC_MIX, GROUP_OSC, STR_RES_MIXER,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_FILTER_FILTER, GROUP_FILTER, STR_RES_FILTER,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_MOD_ENV, GROUP_MOD, STR_RES_ENVELOPE,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_MOD_LFO, GROUP_MOD, STR_RES_LFOS,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_MOD_MATRIX, GROUP_MOD, STR_RES_MODULATION,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_PLAY_ARP, GROUP_PLAY, STR_RES_ARPEGGIO,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_PLAY_STEP_SEQUENCER, GROUP_PLAY, STR_RES_SEQUENCER,
    &Editor::DisplayStepSequencerPage, &Editor::DisplayStepSequencerPage,
    &Editor::HandleStepSequencerInput },
  { PAGE_PLAY_KBD, GROUP_PLAY, STR_RES_KEYBOARD,
    &Editor::DisplayEditSummaryPage, &Editor::DisplayEditDetailsPage,
    &Editor::HandleEditInput },
  { PAGE_LOAD_SAVE, GROUP_LOAD_SAVE, STR_RES_LOAD_SAVE_PATCH, 
    &Editor::DisplayLoadSavePage, &Editor::DisplayLoadSavePage,
    &Editor::HandleLoadSaveInput },
};

Editor::Editor()
  : parameter_definition_index_(0xff),
    display_(NULL),
    current_page_(PAGE_FILTER_FILTER),
    current_controller_(0) {
  last_visited_page_[GROUP_OSC] = PAGE_OSC_OSC_1;
  last_visited_page_[GROUP_FILTER] = PAGE_FILTER_FILTER;
  last_visited_page_[GROUP_MOD] = PAGE_MOD_ENV;
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
  flip_ = 0;
}

void Editor::Init(Display* display) {
  display_ = display;
}

void Editor::CyclePage() {
  current_page_++;
  cursor_ = 0;
  display_->set_cursor_position(kLcdNoCursor);
  current_display_type_ = PAGE_TYPE_DETAILS;
  if (current_page_ == kNumPages) {
    current_page_ = 0;
  }
  if (current_page_ == PAGE_LOAD_SAVE) {
    EnterLoadSaveMode();
  }
}

void Editor::ToggleGroup(ParameterGroup group) {
  cursor_ = 0;
  display_->set_cursor_position(kLcdNoCursor);
  // Special case for the load/save page.
  if (group == GROUP_LOAD_SAVE) {
    EnterLoadSaveMode();
  } else {
    uint8_t first_in_group = 255;
    uint8_t last_in_group = 255;
  
    // Get the first and last page in the requested group.
    for (uint8_t i = 0; i < kNumPages; i++) {
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
void Editor::HandleInput(uint8_t controller_index, uint16_t value) {
  (this->*page_definition_[current_page_].input_handler)(
      controller_index, value);
}

void Editor::DisplaySummary() {
  // No need to render the summary page twice.
  if (current_display_type_ == PAGE_TYPE_SUMMARY) {
    return;
  }
  (this->*page_definition_[current_page_].summary_page)();
  current_display_type_ = PAGE_TYPE_SUMMARY;
}

void Editor::DisplayDetails() {
  current_display_type_ = PAGE_TYPE_DETAILS;
  (this->*page_definition_[current_page_].details_page)();
}

void Editor::EnterLoadSaveMode() {
  if (current_page_ == PAGE_LOAD_SAVE && action_ == ACTION_SAVE) {
    // The Load/save button has been pressed twice, we were in the load/save
    // mode, and the action was set to "save": all the conditions are met to
    // overwrite the patch.
    Engine::patch().Pack(patch_buffer_);
    PatchMemory::Write(current_patch_number_, patch_buffer_,
                       kSerializedPatchSize);
  }
  current_page_ = PAGE_LOAD_SAVE;
  previous_patch_number_ = current_patch_number_;
  Engine::patch().Pack(patch_undo_buffer_);
  action_ = ACTION_EXIT;
}

void Editor::HandleLoadSaveInput(uint8_t controller_index, uint16_t value) {
  switch (controller_index) {
    case 0:
      {
        const uint8_t num_patches = kEepromSize / kSerializedPatchSize;
        uint8_t new_patch = value * num_patches / 1024;
        if (new_patch != current_patch_number_ && action_ == ACTION_LOAD) {
          PatchMemory::Read(new_patch, kSerializedPatchSize, patch_buffer_);
          if (Patch::Check(patch_buffer_)) {
            Engine::mutable_patch()->Unpack(patch_buffer_);
            Engine::TouchPatch();
          } else {
            Engine::mutable_patch()->name[0] = '?';
          }
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
        Engine::mutable_patch()->name[cursor_] = 32 + (value >> 5);
      }
      break;
    case 3:
      if (value < 16) {
        action_ = ACTION_LOAD;
      } else {
        // We are leaving the load mode - restore the previously saved patch.
        if (action_ == ACTION_LOAD) {
          current_patch_number_ = previous_patch_number_;
          Engine::mutable_patch()->Unpack(patch_undo_buffer_);
          Engine::TouchPatch();
        }
        action_ = value >= 1008 ? ACTION_SAVE : ACTION_EXIT;
      }
      break;
  }  
}

void Editor::DisplayLoadSavePage() {
  // 0123456789abcdef
  // load/save patch
  // 32 barbpapa save 
  ResourcesManager::LoadStringResource(
      STR_RES_LOAD_SAVE_PATCH,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display_->Print(0, line_buffer_);
  
  Itoa<int16_t>(current_patch_number_ + 1, 2, line_buffer_);
  AlignRight(line_buffer_, 2);
  line_buffer_[2] = ' ';
  memcpy(line_buffer_ + 3, Engine::patch().name, kPatchNameSize);
  line_buffer_[11] = ' ';
  if (action_ == ACTION_SAVE) {
    display_->set_cursor_position(kLcdWidth + 3 + cursor_);
  } else {
    display_->set_cursor_position(kLcdNoCursor);
  }
  ResourcesManager::LoadStringResource(
      action_ + STR_RES_LOAD,
      line_buffer_ + 12,
      kColumnWidth);
  display_->Print(1, line_buffer_);
}

void Editor::DisplayStepSequencerPage() {
  // 0123456789abcdef
  // step sequencer
  // 0000ffff44449999
  ResourcesManager::LoadStringResource(
      STR_RES_STEP_SEQUENCER,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display_->Print(0, line_buffer_);
  for (uint8_t i = 0; i < 16; ++i) {
    uint8_t value = Engine::patch().sequence_step(i) >> 4;
    line_buffer_[i] = value < 10 ? value + 48 : value + 87;
  }
  display_->Print(1, line_buffer_);
  display_->set_cursor_position(kLcdWidth + cursor_);
}

void Editor::HandleStepSequencerInput(
    uint8_t controller_index,
    uint16_t value) {
  switch (controller_index) {
    case 1:
      cursor_ = value >> 6;
      break;
    case 2:
      Engine::mutable_patch()->set_sequence_step(cursor_, value >> 2);
      break;
  }
}

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
        kColumnWidth);
    AlignRight(line_buffer_ + i * kColumnWidth, kColumnWidth);
  }
  display_->Print(0, line_buffer_);
  
  for (uint8_t i = 0; i < kNumControllers; ++i) {
    uint8_t index = parameter_definition_offset_[current_page_][i];
    const ParameterDefinition& parameter = parameter_definition(index);
    PrettyPrintParameterValue(
        parameter,
        line_buffer_ + i * kColumnWidth,
        kColumnWidth - 1);
    line_buffer_[i * kColumnWidth + kColumnWidth - 1] = '\0';
    AlignRight(line_buffer_ + i * kColumnWidth, kColumnWidth);
  }
  display_->Print(1, line_buffer_);
}

void Editor::DisplayEditDetailsPage() {  
  // 0123456789abcdef
  // filter
  // cutoff       127
  uint8_t index =
      parameter_definition_offset_[current_page_][current_controller_];
  const ParameterDefinition& parameter = parameter_definition(index);
  const PageDefinition& page = page_definition_[parameter.page];

  ResourcesManager::LoadStringResource(
      page.name,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display_->Print(0, line_buffer_);
  
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
  display_->Print(1, line_buffer_);
}

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
  // TODO(oliviergillet): this is not a very pretty way of doing this...
  if (parameter.unit == UNIT_TEMPO_WITH_EXTERNAL_CLOCK) {
    if (new_value < 40) {
      new_value = 0;
    }
  }
  
  // TODO(oliviergillet): dirty hack to get the modulation page working.
  if (current_page_ == PAGE_MOD_MATRIX) {
    if (parameter.id == PRM_MOD_ROW) {
      cursor_ = new_value;
    } else {
      Engine::SetParameter(parameter.id + cursor_ * 3, new_value);
    }
  } else {
    Engine::SetParameter(parameter.id, new_value);
  }
  current_controller_ = controller_index;
}

void Editor::DisplaySplashScreen() {
  // 0123456789abcdef
  //     mutable 
  //   instruments
  ResourcesManager::LoadStringResource(
      STR_RES_MUTABLE,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display_->Print(0, line_buffer_);
  ResourcesManager::LoadStringResource(
      STR_RES_INSTRUMENTS,
      line_buffer_,
      kLcdWidth);
  AlignLeft(line_buffer_, kLcdWidth);
  display_->Print(1, line_buffer_);
}

void Editor::PrettyPrintParameterValue(const ParameterDefinition& parameter,
                                       char* buffer, uint8_t width) {
  int16_t value;
  // TODO(oliviergillet): dirty hack to get the modulation page working.
  if (current_page_ == PAGE_MOD_MATRIX) {
    if (parameter.id == PRM_MOD_ROW) {
      value = cursor_ + 1;
    } else {
      value = Engine::GetParameter(parameter.id + cursor_ * 3);
    }
  } else {
    value = Engine::GetParameter(parameter.id);
  }
  ResourceId base = 0;
  switch (parameter.unit) {
    case UNIT_INT8:
      value = value - 128;
      break;
    case UNIT_BOOLEAN:
      base = STR_RES_OFF;
      break;
    case UNIT_WAVEFORM:
      base = STR_RES_BLIT;
      break;
    case UNIT_LFO_WAVEFORM:
      base = STR_RES_TRI;
      break;
    case UNIT_RAGA:
      base = STR_RES_EQUAL;
      break;
    case UNIT_PATTERN:
      base = STR_RES_51;
      break;
    case UNIT_MODULATION_SOURCE:
      base = (width <= 4) ? STR_RES_LF1 : STR_RES_LFO1;
      break;
    case UNIT_MODULATION_DESTINATION:
      base = (width <= 4) ? STR_RES_CUT : STR_RES_CUTOFF;
      break;
    case UNIT_TEMPO_WITH_EXTERNAL_CLOCK:
      if (value == 0) {
        base = STR_RES_EXTERN;
      }
  }
  if (base) {
    ResourcesManager::LoadStringResource(base + value, buffer, width);
  } else {
    Itoa<int16_t>(value, width, buffer);
  }
}

void Editor::ResetPatch() {
  Engine::ResetPatch();
}

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
