// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)

#include "hardware/base/init_arduino.h"
#include "hardware/base/time.h"
#include "hardware/io/adc.h"
#include "hardware/io/audio_output.h"
#include "hardware/io/input_array.h"
#include "hardware/io/output_array.h"
#include "hardware/io/pin.h"
#include "hardware/io/pretty_printer.h"
#include "hardware/io/serial.h"
#include "hardware/io/shift_register.h"
#include "hardware/io/timer.h"
#include "hardware/midi/midi.h"
#include "hardware/shruti/display.h"
#include "hardware/shruti/editor.h"
#include "hardware/shruti/resources.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/random.h"
#include "hardware/utils/task.h"

using namespace hardware_base;
using namespace hardware_io;
using namespace hardware_midi;
using namespace hardware_shruti;

// Serial object used for debugging.
typedef Serial<SerialPort0, 38400, DISABLED, POLLED> DebugOutput;
DebugOutput debug_output;
PrettyPrinter<DebugOutput> debug;

// Midi input.
Serial<SerialPort0, 31250, BUFFERED, DISABLED> midi_input;

// Input event handlers.
typedef InputArray<AnalogInput<kPinAnalogInput>, 4, 8> Pots;
typedef InputArray<DigitalInput<kPinDigitalInput>, 5> Switches;
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
    Pin<kPinData>, 11, 4, LSB_FIRST, false> leds;

// Audio output on pin 3.
AudioOutput<PwmOutput<3>, kAudioBufferSize, kAudioBlockSize> audio;
uint32_t rendered_blocks = 0;

MidiStreamParser<SynthesisEngine> midi_parser;

void OutputTask() {
TASK_BEGIN_NEAR
  while (1) {
    // Update the LED showing the active page. When the current page is the
    // modulation matrix editor, the LED blinks according to the active
    // modulation source.
    // TODO(oliviergillet): which class is responsible for handling that? In
    // any case, this code does not belong here and is not readable.
    for (uint8_t i = 0; i < kNumPages; ++i) {
      uint8_t value = 0;
      if (i == editor.current_page()) {
        if (i == PAGE_MOD_MATRIX) {
          uint8_t current_modulation_source_value = engine.modulation_source(
              engine.patch().modulation_matrix.modulation[
                  editor.cursor()].source);
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
  static uint8_t id;
TASK_BEGIN_NEAR
  while (1) {
    // Read the switches.
    switch_event = switches.Read();
    
    // Update the editor.
    if (switch_event.event == EVENT_RAISED && switch_event.time > 100) {
      id = switch_event.id;
      TASK_SWITCH;
      editor.ToggleGroup(id);
      TASK_SWITCH;
      editor.DisplaySummary();
    }
    TASK_SWITCH;
    
    // Select which analog/digital inputs we want to read by a write to the
    // multiplexer register.
    input_mux.Write((pots.active_input() << 3) | switches.active_input());
    TASK_SWITCH;
    
    // Read the potentiometers.
    pot_event = pots.Read();
    
    // Update the editor if something happened.
    if (pot_event.event == EVENT_NONE) {
      // Nothing happened. If nothing happened for a long time, display the
      // summary page instead of the details.
      if (pot_event.time > 1500) {
        TASK_SWITCH;
        editor.DisplaySummary();
      }
    } else {
      editor.HandleInput(pot_event.id, pot_event.value);
      TASK_SWITCH;
      editor.DisplayDetails();
    }
    TASK_SWITCH;
    // TODO(oliviergillet): write VCF control signals to the PWM output.
  }
TASK_END
}

void MidiTask() {
TASK_BEGIN_NEAR
  while (1) {
    for (uint8_t i = 0; i < 4; ++i) {
      if (midi_input.readable()) {
        midi_parser.PushByte(midi_input.ImmediateRead());
      }
    }
    TASK_SWITCH;
  }
TASK_END
}

void AudioRenderingTask() {
TASK_BEGIN_NEAR
  while (1) {
    if (audio.writable_block()) {
      rendered_blocks++;
      engine.Control();
      for (uint8_t i = 0; i < kAudioBlockSize; ++i) {
        engine.Audio();
        audio.Overwrite(engine.signal());
      }
    }
    TASK_SWITCH;
  }
TASK_END
}

uint16_t heartbeat_counter = 0;
void HeartbeatTask() {
TASK_BEGIN_NEAR
  while (1) {
    heartbeat_counter++;
    if ((heartbeat_counter & 1023) == 0) {
      debug << audio.num_glitches() << endl;
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
  }
}

TIMER_2_TICK {
  display.Tick();
  audio.EmitSample();
}

void Setup() {
  display.Init();
  editor.Init();
  
  // 31250kHz, phase correct.
  Timer<2>::set_prescaler(1);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();
  audio.Init();
  
  display.SetBrightness(29);
  display.SetCustomCharMap(character_table[0], 8);
  editor.DisplaySplashScreen();
  
  midi_input.Init();
  debug_output.Init();
  pots.Init();
  switches.Init();
  // Set the digital input to high to enable the pull-up resistor.
  Pin<kPinDigitalInput>::High();
  input_mux.Init();
  leds.Init();  
  
  engine.Init();
  engine.NoteOn(0, 48, 120);
}

int main(void)
{
  InitArduino();
  Setup();
  ScheduleTasks();
  return 0;
}
