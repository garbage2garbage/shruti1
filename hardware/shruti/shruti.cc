// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)

#include "hardware/base/init_arduino.h"
#include "hardware/base/time.h"
#include "hardware/io/adc.h"
#include "hardware/io/audio_output.h"
#include "hardware/io/display.h"
#include "hardware/io/input_array.h"
#include "hardware/io/output_array.h"
#include "hardware/io/pin.h"
#include "hardware/io/pretty_printer.h"
#include "hardware/io/serial.h"
#include "hardware/io/shift_register.h"
#include "hardware/io/timer.h"
#include "hardware/midi/midi.h"
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
PrettyPrinter<DebugOutput> debug;

// Midi input.
typedef Serial<SerialPort0, 31250, BUFFERED, DISABLED> MidiInput;

// Input event handlers.
typedef InputArray<AnalogInput<kPinAnalogInput>, 4, 8> Pots;
typedef InputArray<DigitalInput<kPinDigitalInput>, 1> Switches;

// Shift register for input multiplexer.
typedef ShiftRegister<
    Pin<kPinInputLatch>,
    Pin<kPinClk>,
    Pin<kPinData>, 8, MSB_FIRST> InputMux;

// LED array.
typedef OutputArray<
    Pin<kPinOutputLatch>, 
    Pin<kPinClk>,
    Pin<kPinData>, 11, 4, LSB_FIRST, false> Leds;

// Audio output on pin 3.
typedef AudioOutput<PwmOutput<3>, kAudioBufferSize, kAudioBlockSize> Audio;
uint32_t rendered_blocks = 0;

Display display;
Editor editor;
MidiStreamParser<Engine> midi_parser;

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
          uint8_t current_modulation_source_value = Engine::modulation_source(
              Engine::patch().modulation_matrix.modulation[
                  editor.cursor()].source);
          value = current_modulation_source_value >> 4;
        } else {
          value = 15;
        }
      } else {
        value = 0;
      }
      Leds::set_value(i, value);
    }
    Leds::Out();
    YIELD_VOID;
    
    // Send a character to the LCD display if available.
    display.Update();
    YIELD_VOID;
  }
TASK_END
}

void InputTask() {
  Switches::Event switch_event;
  Pots::Event pot_event;
TASK_BEGIN_NEAR
  while (1) {
    // Read the switches.
    switch_event = Switches::Read();
    
    // Update the editor.
    if (switch_event.event == EVENT_LOWERED && switch_event.time > 100) {
      YIELD_VOID;
      editor.CyclePage();
      YIELD_VOID;
      editor.DisplaySummary();
    }
    YIELD_VOID;
    
    // Select which analog/digital inputs we want to read by a write on the
    // multiplexer register.
    InputMux::Write((Pots::active_input() << 3) | Switches::active_input());
    YIELD_VOID;
    
    // Read the potentiometers.
    pot_event = Pots::Read();
    
    // Update the editor if something happened.
    if (pot_event.event == EVENT_NONE) {
      if (pot_event.time > 1500) {
        YIELD_VOID;
        editor.DisplaySummary();
      }
    } else {
      editor.HandleInput(pot_event.id, pot_event.value);
      YIELD_VOID;
      editor.DisplayDetails();
    }
    YIELD_VOID;
    // TODO(oliviergillet): write VCF control signals to the PWM output.
  }
TASK_END
}

void MidiTask() {
TASK_BEGIN_NEAR
  while (1) {
    for (uint8_t i = 0; i < 4; ++i) {
      if (MidiInput::readable()) {
        midi_parser.PushByte(MidiInput::ImmediateRead());
      }
    }
    YIELD_VOID;
  }
TASK_END
}

void AudioRenderingTask() {
TASK_BEGIN_NEAR
  while (1) {
    if (Audio::writable_block()) {
      rendered_blocks++;
      Engine::Control();
      for (uint8_t i = 0; i < kAudioBlockSize; ++i) {
        Engine::Audio();
        Audio::Overwrite(Engine::signal());
      }
    }
    YIELD_VOID;
  }
TASK_END
}

uint16_t heartbeat_counter = 0;
void HeartbeatTask() {
TASK_BEGIN_NEAR
  while (1) {
    heartbeat_counter++;
    if ((heartbeat_counter & 1023) == 0) {
      debug << Audio::num_glitches() << endl;
    }
    YIELD_VOID;
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
  Audio::EmitSample();
}

void Setup() {
  display.Init();
  
  // 31250kHz, phase correct.
  Timer<2>::set_prescaler(1);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();
  Audio::Init();
  
  display.SetBrightness(29);
  display.SetCustomCharMap(character_table[0], 8);
  
  MidiInput::Init();
  DebugOutput::Init();
  Pots::Init();
  Switches::Init();
  InputMux::Init();
  Leds::Init();  
  
  Engine::Init();
  
  editor.Init(&display);
  // editor.DisplaySplashScreen();
  
  /*Engine::SetParameter(PRM_OSC_ALGORITHM_1, WAVEFORM_CZ);
  Engine::SetParameter(PRM_OSC_PARAMETER_1, 63);
  Engine::SetParameter(PRM_OSC_OPTION_1, 0);
  Engine::SetParameter(PRM_OSC_RANGE_2, 128);
  Engine::SetParameter(PRM_MIX_BALANCE, 0);
  Engine::SetParameter(PRM_MIX_SUB_OSC, 0);
  Engine::SetParameter(PRM_MIX_NOISE, 1);

  Engine::SetParameter(PRM_LFO_RATE_2, 20);
  Engine::SetParameter(PRM_LFO_WAVE_2, LFO_WAVEFORM_TRIANGLE);
  Engine::SetParameter(PRM_MOD_SOURCE, MOD_SRC_LFO_2);
  Engine::SetParameter(PRM_MOD_DESTINATION, MOD_DST_PWM_1);
  Engine::SetParameter(PRM_MOD_AMOUNT, 120);
  Engine::SetParameter(PRM_ARP_OCTAVES, 2);
  Engine::SetParameter(PRM_ARP_PATTERN, 1);
  Engine::SetParameter(PRM_ARP_TEMPO, 120);
  Engine::SetParameter(PRM_ARP_SWING, 0);
  Engine::SetParameter(PRM_KBD_PORTAMENTO, 20);

  for (uint8_t i = 0; i < 4; ++i) {
    Engine::mutable_patch()->sequence[2 * i] = hardware_utils::Random::Byte();
    Engine::mutable_patch()->sequence[2 * i + 1] = Engine::mutable_patch()->sequence[2 * i];
  }
  Engine::NoteOn(0, 36, 120);
  Engine::NoteOn(0, 48, 120);
  Engine::NoteOn(0, 55, 120);*/
  Engine::NoteOn(0, 60, 120);
}

int main(void)
{
	InitArduino();
  Setup();
  ScheduleTasks();
	return 0;
}
