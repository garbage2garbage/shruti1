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

#include "hardware/hal/adc.h"
#include "hardware/hal/audio_output.h"
#include "hardware/hal/devices/output_array.h"
#include "hardware/hal/devices/shift_register.h"
#include "hardware/hal/gpio.h"
#include "hardware/hal/init_atmega.h"
#include "hardware/hal/input_array.h"
#include "hardware/hal/serial.h"
#include "hardware/hal/time.h"
#include "hardware/hal/timer.h"
#include "hardware/midi/midi.h"
#include "hardware/shruti/display.h"
#include "hardware/shruti/editor.h"
#include "hardware/shruti/synthesis_engine.h"
#include "hardware/utils/task.h"

using namespace hardware_hal;
using namespace hardware_midi;
using namespace hardware_shruti;

using hardware_utils::NaiveScheduler;
using hardware_utils::Task;

// Midi input.
Serial<SerialPort0, 31250, BUFFERED, POLLED> midi_io;

// Input event handlers.
typedef InputArray<
    AnalogInput<kPinAnalogInput>,
    kNumEditingPots,
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
    Gpio<kPinInputLatch>,
    Gpio<kPinClk>,
    Gpio<kPinData>, 8, MSB_FIRST> input_mux;

// LED array.
OutputArray<
    Gpio<kPinOutputLatch>, 
    Gpio<kPinClk>,
    Gpio<kPinData>, kNumPages, 4, MSB_FIRST, false> leds;

// Audio output on pin 3.
AudioOutput<PwmOutput<kPinVcoOut>, kAudioBufferSize, kAudioBlockSize> audio_out;

MidiStreamParser<SynthesisEngine> midi_parser;

static const uint16_t kDetailsPageDelay = 900;

// What follows is a list of "tasks" - short functions handling a particular
// aspect of the synth (rendering audio, updating the LCD display, etc). they
// are called in sequence, with some tasks being more frequently called than
// others, by the Scheduler.
void UpdateLedsTask() {
  leds.Clear();
  if (editor.current_page() == PAGE_MOD_MATRIX) {
    uint8_t current_modulation_source_value = engine.modulation_source(0,
        engine.patch().modulation_matrix.modulation[
            editor.subpage()].source);
    leds.set_value(PAGE_MOD_MATRIX, current_modulation_source_value >> 4);
  } else if (editor.current_page() == PAGE_PERFORMANCE) {
    for (uint8_t i = 0; i < kNumModulationDestinations; ++i) {
      leds.set_value(i, engine.voice(0).modulation_destination(i) >> 4);
    }
  } else {
    leds.set_value(editor.current_page(), 15);
  }
  // The led of the arpeggiator page flashes strongly on the 0-th step and
  // weakly on the other steps which are a multiple of 4.
  if (engine.voice_controller().active()) {
    if (!(engine.voice_controller().step() & 3)) {
      leds.set_value(PAGE_LOAD_SAVE, engine.voice_controller().step() ? 1 : 15);
    }
  }
  leds.Output();
}

void UpdateDisplayTask() {
  display.Update();
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
      if (switch_event.time > kDetailsPageDelay) {
        idle = 1;
      }
    } else {
      if (switch_event.event == EVENT_RAISED && switch_event.time > 100) {
        uint8_t id = switch_event.id;
        uint8_t hold_time = static_cast<uint16_t>(switch_event.time) >> 8;
        if (id < kNumGroupSwitches) {
          if (hold_time >= 3) {  // 0.768 seconds
            editor.DoShiftFunction(id, hold_time);
          } else {
            editor.ToggleGroup(id);
          }
          target_page_type = PAGE_TYPE_SUMMARY;
        } else {
          editor.HandleIncrement(id == kNumGroupSwitches ? -1 : 1);
          target_page_type = PAGE_TYPE_DETAILS;
        }
      }
    }
    
    // Select which analog/digital inputs we want to read by a write to the
    // multiplexer register.
    input_mux.Write((pots.active_input() << 3) | switches.active_input());    
    pot_event = pots.Read();
    
    // Update the editor if something happened.
    // Revert back to the main page when nothing happened for 1.5s.
    if (pot_event.event == EVENT_NONE) {
      if (idle && pot_event.time > kDetailsPageDelay) {
        target_page_type = PAGE_TYPE_SUMMARY;
      }
    } else {
      editor.HandleInput(pot_event.id, pot_event.value);
      target_page_type = PAGE_TYPE_DETAILS;
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

uint8_t current_cv;

void CvTask() {
  ++current_cv;
  if (current_cv >= kNumCvInputs) {
    current_cv = 0;
  }
  engine.set_cv(current_cv, Adc::Read(kPinCvInput + current_cv) >> 2);
}

void MidiTask() {
  // Always flush the MIDI buffer before continuing. This makes unlikely the
  // situation where MIDI bytes are dropped... at the cost of a more glitchy
  // audio output in case of MIDI overloading.
  uint8_t status = 0;
  while (midi_io.readable()) {
    uint8_t value = midi_io.ImmediateRead();
    
    // Copy the byte to the MIDI output (thru). We could use Overwrite here
    // since the output rate is the same as the input rate, which ensures that
    // 0.32ms have elapsed between the writes.
    midi_io.Write(value);
    
    // Also, parse the message.
    status = midi_parser.PushByte(value);
    if (engine.patch().kbd_midi_channel >= 17) {
      break;
    }
  }
  // Display a status indicator on the LCD to indicate that a message has
  // been received. This could be done as well in the synthesis engine code
  // or in the MIDI parser, but I'd rather keep the UI code separate.
  switch (status & 0xf0) {
    // Note on/off.
    case 0x90:
      display.set_status('\x01');
      break;
    // Controller.
    case 0xb0:
      display.set_status('\x05');
      break;
    // Bender.
    case 0xe0:
      display.set_status('\x02');
      break;
    // Special messages.
    case 0xf0:
      // Display a status indicator to monitor SysEx patch reception.
      if (status == 0xf0 || status == 0xf7) {
        switch (engine.patch().sysex_reception_state()) {
          case RECEIVING_DATA:
            display.set_status('~');
            break;
          case RECEPTION_OK:
            display.set_status('+');
            engine.TouchPatch();
            break;
          case RECEPTION_ERROR:
            display.set_status('#');
            break;
        }
      }
      break;
  }
}

void AudioRenderingTask() {
  if (audio_out.writable_block()) {
    engine.Control();
    if (engine.voice(0).dead()) {
      for (uint8_t i = kAudioBlockSize; i > 0 ; --i) {
        audio_out.Overwrite(128);
      }
    } else {
      for (uint8_t i = kAudioBlockSize; i > 0 ; --i) {
        engine.Audio();
        audio_out.Overwrite(engine.voice(0).signal());
      }
    }
    vcf_cutoff_out.Write(engine.voice(0).cutoff());
    vcf_resonance_out.Write(engine.voice(0).resonance());
    vca_out.Write(engine.voice(0).vca());
  }
}

uint16_t previous_num_glitches;

// This task displays a '!' in the status area of the LCD displays whenever
// a discontinuity occurred in the audio rendering. Even if the code is
// optimized in such a way that it never occurs, I'd rather keep it here in
// case new features are implemented and need performance monitoring.
// This code uses 42 bytes.
void AudioGlitchMonitoringTask() {
  uint16_t num_glitches = audio_out.num_glitches();
  if (num_glitches != previous_num_glitches) {
    previous_num_glitches = num_glitches;
    display.set_status('!');
  }
}

typedef NaiveScheduler<kSchedulerNumSlots> Scheduler;

Scheduler scheduler;

/* static */
template<>
Task Scheduler::tasks_[] = {
    { &AudioRenderingTask, 16 },
    { &MidiTask, 6 },
    { &UpdateLedsTask, 4 },
    { &UpdateDisplayTask, 2 },
#ifdef HAS_GLITCH_MONITORING
    { &AudioGlitchMonitoringTask, 1 },
#endif  // HAS_GLITCH_MONITORING
    { &InputTask, 2 },
    { &CvTask, 1 },
};

TIMER_2_TICK {
  display.Tick();
  audio_out.EmitSample();
}

void Init() {
  scheduler.Init();
  display.Init();
  editor.Init();
  audio_out.Init();

  // Initialize all the PWM outputs, in 31.25kHz, phase correct mode.
  Timer<1>::set_prescaler(1);
  Timer<1>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::set_prescaler(1);
  Timer<2>::set_mode(TIMER_PWM_PHASE_CORRECT);
  Timer<2>::Start();
  vcf_cutoff_out.Init();
  vcf_resonance_out.Init();
  vca_out.Init();
  
  display.SetBrightness(29);
  display.SetCustomCharMap(character_table[0], 8);
  editor.DisplaySplashScreen(STR_RES_MUTABLE____V0_59);
  
  midi_io.Init();
  pots.Init();
  switches.Init();
  DigitalInput<kPinDigitalInput>::EnablePullUpResistor();
  input_mux.Init();
  leds.Init();  
  
  engine.Init();
}

int main(void) {
  InitAtmega(false);  // Do not initialize timers 1 & 2.
  Init();
  scheduler.Run();
}
