// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)

#include "hardware/base/init_arduino.h"
#include "hardware/base/time.h"
#include "hardware/io/adc.h"
#include "hardware/io/audio_output.h"
#include "hardware/io/devices/74hc595.h"
#include "hardware/io/devices/output_array.h"
#include "hardware/io/input_array.h"
#include "hardware/io/pin.h"
#include "hardware/io/pretty_printer.h"
#include "hardware/io/serial.h"
#include "hardware/io/timer.h"
#include "hardware/midi/midi.h"
#include "hardware/shruti/display.h"
#include "hardware/shruti/editor.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/signal.h"
#include "hardware/utils/task.h"

using namespace hardware_base;
using namespace hardware_io;
using namespace hardware_midi;
using namespace hardware_shruti;

// Midi input.
Serial<SerialPort0, 31250, BUFFERED, DISABLED> midi_input;

// Input event handlers.
typedef InputArray<
    AnalogInput<kPinAnalogInput>,
    kNumEditingPots + kNumAssignablePots,
    8> Pots;

typedef InputArray<
    DigitalInput<kPinDigitalInput>,
    kNumGroupSwitches + 2> Switches;

PwmOutput<kPinVcfCutoffOut> vcf_cutoff_out;
PwmOutput<kPinVcfResonanceOut> vcf_resonance_out;
PwmOutput<kPinVcaOut> vca_out;

Pots pots;
Switches switches;

// Shift register for input multiplexer.
ShiftRegister<
    Pin<kPinInputLatch>,
    Pin<kPinClk>,
    Pin<kPinData>, 8, MSB_FIRST> input_mux;

// LED array.
OutputArray<
    Pin<kPinOutputLatch>, 
    Pin<kPinClk>,
    Pin<kPinData>, kNumPages, 4, LSB_FIRST, false> leds;

// Audio output on pin 3.
AudioOutput<PwmOutput<kPinVcoOut>, kAudioBufferSize, kAudioBlockSize> audio;
uint32_t rendered_blocks = 0;

MidiStreamParser<SynthesisEngine> midi_parser;

void OutputTask() {
TASK_BEGIN_NEAR
  while (1) {
    // Update the LED showing the active page. When the current page is the
    // modulation matrix editor, the LED blinks according to the active
    // modulation source.
    // TODO(pichenettes): which class is responsible for handling that? In
    // any case, this code does not belong here and is not readable.
    for (uint8_t i = 0; i < kNumPages; ++i) {
      uint8_t value = 0;
      if (i == editor.current_page()) {
        if (i == PAGE_MOD_MATRIX) {
          uint8_t current_modulation_source_value = engine.modulation_source(0, 
              engine.patch().modulation_matrix.modulation[
                  editor.subpage()].source);
          value = current_modulation_source_value >> 4;
        } else {
          value = 15;
        }
      } else {
        value = 0;
      }
      leds.set_value(i, value);
    }
    leds.Out();
    TASK_SWITCH;
    
    // Send a character to the LCD display if available.
    display.Update();
    TASK_SWITCH;
  }
TASK_END
}

void InputTask() {
  Switches::Event switch_event;
  Pots::Event pot_event;
  static uint8_t idle;
  static uint8_t target_page_type;
TASK_BEGIN_NEAR
  while (1) {
    idle = 0;
    target_page_type = PAGE_TYPE_ANY;
    
    // Read the switches.
    switch_event = switches.Read();
    
    // If a button was pressed, perform the action. Otherwise, if nothing
    // happened for 1.5s, update the idle flag.
    if (switch_event.event == EVENT_NONE) {
      if (switch_event.time > 1500) {
        idle = 1;
      }
    } else {
      if (switch_event.event == EVENT_RAISED && switch_event.time > 100) {
        uint8_t id = switch_event.id;
        if (id < kNumGroupSwitches) {
          editor.ToggleGroup(id);
          target_page_type = PAGE_TYPE_SUMMARY;
        } else {
          editor.HandleIncrement(2 * id - 2 * kNumGroupSwitches - 1);
          target_page_type = PAGE_TYPE_DETAILS;
        }
      }
    }
    TASK_SWITCH;
    
    // Select which analog/digital inputs we want to read by a write to the
    // multiplexer register.
    input_mux.Write((pots.active_input() << 3) | switches.active_input());
    pot_event = pots.Read();
    
    // Update the editor if something happened.
    // Revert back to the main page when nothing happened for 1.5s.
    if (pot_event.event == EVENT_NONE) {
      if (idle && pot_event.time > 1500) {
        target_page_type = PAGE_TYPE_SUMMARY;
      }
    } else {
      if (pot_event.id < kNumEditingPots) {
        editor.HandleInput(pot_event.id, pot_event.value);
        target_page_type = PAGE_TYPE_DETAILS;
      } else {
        engine.set_assignable_controller(
            pot_event.id - kNumEditingPots, pot_event.value >> 2);
      }
    }
    TASK_SWITCH;
    
    if (target_page_type == PAGE_TYPE_SUMMARY) {
      editor.DisplaySummary();
    } else if (target_page_type == PAGE_TYPE_DETAILS) {
      editor.DisplayDetails();
    }
    TASK_SWITCH;
  }
TASK_END
}

void MidiTask() {
  if (midi_input.readable()) {
    uint8_t first_byte = midi_parser.PushByte(midi_input.ImmediateRead());
    // Display a status indicator on the LCD to indicate that a message has
    // been received. This could be done as well in the synthesis engine code
    // or in the MIDI parser, but I'd rather keep the UI code separate.
    switch (first_byte) {
      case 0x90:
        display.set_status(1);
        break;
      case 0xb0:
        display.set_status(4);
        break;
      case 0xe0:
        display.set_status(3);
        break;
      case 0xf8:
        display.set_status(2);
        break;
    }
  }
}

void AudioRenderingTask() {
  if (audio.writable_block()) {
    rendered_blocks++;
    engine.Control();
    for (uint8_t i = 0; i < kAudioBlockSize; ++i) {
      engine.Audio();
      audio.Overwrite(engine.voice(0).signal());
    }
    vcf_cutoff_out.Write(engine.voice(0).cutoff());
    vcf_resonance_out.Write(engine.voice(0).resonance());
    vca_out.Write(engine.voice(0).vca());
  }
}


void CvTask() {
  engine.set_cv(0, AnalogInput<kPinCvInput>::Read() >> 2);
  engine.set_cv(1, AnalogInput<kPinCvInput + 1>::Read() >> 2);
}

uint16_t previous_num_glitches = 0;
void HeartbeatTask() {
  uint16_t num_glitches = 0;
TASK_BEGIN_NEAR
  while (1) {
    num_glitches = audio.num_glitches();
    if (num_glitches != previous_num_glitches) {
      previous_num_glitches = num_glitches;
      display.set_status('!');
    }
    TASK_SWITCH;
  }
TASK_END
}

void ScheduleTasks() {
  while (1) {
    HeartbeatTask();
    OutputTask();
    AudioRenderingTask();
    MidiTask();
    AudioRenderingTask();
    InputTask();
    AudioRenderingTask();
    MidiTask();
    AudioRenderingTask();
    CvTask();
    AudioRenderingTask();
  }
}

TIMER_2_TICK {
  display.Tick();
  audio.EmitSample();
}

void Setup() {
  display.Init();
  editor.Init();

  // 31.25kHz, phase correct.
  Timer<2>::set_prescaler(1);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();
  Timer<1>::set_prescaler(1);
  Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);
  
  audio.Init();
  
  vcf_cutoff_out.Init();
  vcf_resonance_out.Init();
  vca_out.Init();
  
  
  // display.SetBrightness(29);
  // display.SetCustomCharMap(character_table[0], 8);
  // editor.DisplaySplashScreen();
  
  midi_input.Init();
  pots.Init();
  switches.Init();
  // Set the digital input to high to enable the pull-up resistor.
  Pin<kPinDigitalInput>::High();
  input_mux.Init();
  leds.Init();  
  
  engine.Init();
}

int main(void) {
  InitArduino();
  Setup();
  ScheduleTasks();
  return 0;
}
