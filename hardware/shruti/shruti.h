// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Parameter editor.

#ifndef HARDWARE_SHRUTI_SHRUTI_H_
#define HARDWARE_SHRUTI_SHRUTI_H_

#include "hardware/base/base.h"

#ifndef __TEST__
#define USE_OPTIMIZED_OP
#endif  // !__TEST__

namespace hardware_shruti {

// Set this flag to 6 to get a nice polysynth (humour).
static const uint8_t kNumVoices = 1;

static const uint8_t kNumEditingPots = 4;
static const uint8_t kNumAssignablePots = 2;
static const uint8_t kNumGroupSwitches = 5;

// Rate of the main timer. For now, 1 sample is generated per tick, but we might
// want to do something different to achieve other sample rates
// (20kHz or 16kHz).
static const uint16_t kMainTimerRate = 31250;
static const uint16_t kSampleRate = 31250;

static const uint16_t kDisplayBaudRate = 2400;


// One control signal sample is generated for each 32 audio sample.
static const uint8_t kControlRate = 32;

// The latency is 1ms, with a buffer storing 4ms of audio.
static const uint8_t kAudioBlockSize = kControlRate;
static const uint8_t kAudioBufferSize = kAudioBlockSize * 4;

// ---- Wirings ----------------------------------------------------------------

// Serial/UART output.
static const uint8_t kPinMidiInRx = 0;
static const uint8_t kPinMidiThruTx = 1;
static const uint8_t kPinLcdTx = 2;

// Shift register / muxes.
static const uint8_t kPinData = 4;
static const uint8_t kPinDigitalInput = 8;
static const uint8_t kPinOutputLatch = 6;
static const uint8_t kPinInputLatch = 7;
static const uint8_t kPinClk = 5;

// PWM/audio output.
static const uint8_t kPinVcoOut = 3;
static const uint8_t kPinVcaOut = 11;
static const uint8_t kPinVcfCutoffOut = 9;
static const uint8_t kPinVcfResonanceOut = 10;

static const uint8_t kPinAnalogInput = 0;
static const uint8_t kPinCvInput = 1;

// ---- LCD display type -------------------------------------------------------

static const uint8_t kLcdWidth = 16;
static const uint8_t kLcdHeight = 2;

// ---- Scheduler configuration ------------------------------------------------

static const uint8_t kSchedulerNumSlots = 32;

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_SHRUTI_H_
