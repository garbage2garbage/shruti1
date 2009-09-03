// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Oscillators. Note that the code of each oscillator is duplicated/specialized,
// for a noticeable performance boost.
// Another performance optimization consists in rendering the oscillator at
// a sample rate which is the half of the base sample rate - this is
// particularly useful for oscillators which do not have a rich frequency
// content, but which are very computationnally expensive.
//
// The template parameter mode determines the features supported by
// an oscillator:
//
//          full      low complexity   sub oscillator
// Blit     sr/2      sr/2             n/a
// Square   sr        sr/2             sr/4
// Pulse    sr/2      sr/2             n/a
// Saw      sr        sr/2             n/a
// Triangle sr        sr/2             sr/4
// CZ       sr/2      n/a              n/a
// FM       sr/2      n/a              n/a
// 8-bits   sr        n/a              n/a
// Voice    sr/2      n/a              n/a
// Table    sr/2      n/a              n/a
// Sweep    sr/2      n/a              n/a
//
// TODO(pichenettes): This code is horrible, find a way to get the generate the
// 3 specialized variants in a more efficient way.

#ifndef HARDWARE_SHRUTI_OSCILLATOR_H_
#define HARDWARE_SHRUTI_OSCILLATOR_H_

#include "hardware/base/base.h"
#include "hardware/shruti/shruti.h"

#include "hardware/shruti/parameters.h"
#include "hardware/shruti/resources.h"
#include "hardware/utils/random.h"
#include "hardware/utils/signal.h"

using hardware_utils::Signal;
using hardware_utils::Random;

namespace hardware_shruti {

#define WAV_RES_SINE WAV_RES_BANDLIMITED_TRIANGLE_6
#define HALF_SAMPLE_RATE if (engine.oscillator_decimation() & 1) return;
#define FOURTH_SAMPLE_RATE if (engine.oscillator_decimation()) return;

enum OscillatorMode {
  FULL = 0,
  LOW_COMPLEXITY = 1,
  SUB_OSCILLATOR = 2
};

static const uint8_t kSpeechControlRateDecimation = 4;
static const uint8_t kNumZonesFullSampleRate = 6;
static const uint8_t kNumZonesHalfSampleRate = 5;

struct PulseSquareOscillatorData {
  const prog_uint8_t* wave[2];
  uint8_t balance;
  uint16_t shift;
  int8_t square;
  uint8_t leak;
};

// Interpolates between two 256-samples wavetables.
struct SawTriangleOscillatorData {
  const prog_uint8_t* wave[2];
  uint8_t balance;
};

// Interpolates between two different points in a 64-samples wavetable.
struct Wavetable64OscillatorData {
  int16_t smooth_parameter;
};

struct CzOscillatorData {
  uint16_t formant_phase;
  uint16_t formant_phase_increment;
};

struct FmOscillatorData {
  uint16_t modulator_phase;
  uint16_t modulator_phase_increment;
};

struct SpeechSynthesizerData {
  uint16_t formant_increment[3];
  uint16_t formant_phase[3];
  uint8_t formant_amplitude[3];
  uint8_t noise_modulation;
  uint8_t update;  // Update only every kSpeechControlRateDecimation-th call.
};

union OscillatorData {
  PulseSquareOscillatorData bl;
  SawTriangleOscillatorData wv;
  CzOscillatorData cz;
  FmOscillatorData fm;
  SpeechSynthesizerData sp;
  Wavetable64OscillatorData wt;
};

struct AlgorithmFn {
  void (*update)();
  void (*render)();
};

template<int id, OscillatorMode mode>
class Oscillator {
 public:
   // Called whenever the parameters of the oscillator change. Can be used
   // to pre-compute parameters, set tables, etc.
   static inline void SetupAlgorithm(uint8_t algorithm) {
     if (algorithm != algorithm_) {
       if (mode == LOW_COMPLEXITY) {
         fn_ = fn_table_low_complexity_[algorithm & 1];
       } else if (mode == FULL) {
         fn_ = fn_table_[algorithm];
       }
       algorithm_ = algorithm;
       Reset();
       if (mode == FULL) {
         sweeping_ = algorithm_ == WAVEFORM_ANALOG_WAVETABLE;
       }
     }
  }
  static inline uint8_t Render() {
    // Updating the phase by the increment here is a bad idea, because it will
    // be reloaded from memory anyway in the oscillator code @@, and because we
    // might use a different increment (FM), or because we might need to
    // check if we have completed a cycle to sync to another waveform.
    if (mode == SUB_OSCILLATOR) {
      RenderSub();
    } else {
      (*fn_.render)();
    }
    return held_sample_;
  }
  static inline void Reset() {
    ResetPhase();
    if (algorithm_ == WAVEFORM_WAVETABLE) {
      data_.wt.smooth_parameter = parameter_ * 64;
    }
  }
  static inline void Update(
      uint8_t parameter,
      uint8_t note,
      uint16_t increment) {
    note_ = note;

    if (mode == SUB_OSCILLATOR) {
      phase_increment_ = increment << 2;
      UpdateSub();
    } else {
      if (mode == FULL && sweeping_) {
        algorithm_ = parameter >> 5;
        fn_ = fn_table_[algorithm_];
        parameter = (parameter & 0x1f) << 2;
      }
      parameter_ = parameter;
      if (mode == FULL) {
        phase_increment_ = increment;
      }
      phase_increment_2_ = increment << 1;
      if (fn_.update) {
        (*fn_.update)();
      }
    }
  }
  static inline uint16_t phase() { return phase_; }
  static inline void ResetPhase() { phase_ = 0;  }

 private:
  // Current phase of the oscillator.
  static uint16_t phase_;
  
  // Phase increment (and phase increment x 2, for low-sr oscillators).
  static uint16_t phase_increment_;
  static uint16_t phase_increment_2_;
  
  // Copy of the algorithm used by this oscillator. Change along with the
  // Setup/Update/Render pointers.
  static uint8_t algorithm_;
  // Whether we are sweeping through the algorithms.
  static uint8_t sweeping_;
  
  // Current value of the oscillator parameter.
  static uint8_t parameter_;
  
  // Sample generated in the previous full call.
  static uint8_t held_sample_;
  
  // Current MIDI note (used for wavetable selection).
  static uint8_t note_;
  
  // Union of state data used by each algorithm.
  static OscillatorData data_;
  
  // A trio of pointers to the setup/update/render functions. update/render
  // function might be NULL.
  static AlgorithmFn fn_;
  
  static AlgorithmFn fn_table_[];
  static AlgorithmFn fn_table_low_complexity_[];
  
  static inline uint8_t ReadSample(const prog_uint8_t* table, uint16_t phase) {
    return ResourcesManager::Lookup<uint8_t, uint8_t>(table, phase >> 8);
  }
#ifdef FAST_SIGNAL_PROCESSING
  static inline uint8_t InterpolateSample(
      const prog_uint8_t* table,
      uint16_t phase) {
    uint8_t result;
    asm(
      "movw r30, %A1"           "\n\t"  // copy base address to r30:r31
      "add r30, %B2"            "\n\t"  // increment table address by phaseH
      "adc r31, r1"             "\n\t"  // just carry
      "lpm %0, z+"              "\n\t"  // load sample[n]
      "lpm r1, z+"              "\n\t"  // load sample[n+1]
      "mul %A2, r1"             "\n\t"  // multiply second sample by phaseL
      "movw r30, r0"            "\n\t"  // result to accumulator
      "com %A2"                 "\n\t"  // 255 - phaseL -> phaseL
      "mul %A2, %0"             "\n\t"  // multiply first sample by phaseL
      "com %A2"                 "\n\t"  // 255 - phaseL -> phaseL
      "add r30, r0"             "\n\t"  // accumulate L
      "adc r31, r1"             "\n\t"  // accumulate H
      "eor r1, r1"              "\n\t"  // reset r1 after multiplication
      "mov %0, r31"             "\n\t"  // use sum H as output
      : "=r" (result)
      : "a" (table), "a" (phase)
      : "r30", "r31"
    );
    return result;
  }
#else
  static inline uint8_t InterpolateSample(const prog_uint8_t* table,
                                          uint16_t phase) {
    return Signal::Mix(
        ResourcesManager::Lookup<uint8_t, uint8_t>(table, phase >> 8),
        ResourcesManager::Lookup<uint8_t, uint8_t>(table, 1 + (phase >> 8)),
        phase & 0xff);
  }
#endif  // FAST_SIGNAL_PROCESSING

  static inline uint8_t InterpolateTwoTables(
      const prog_uint8_t* table_a, const prog_uint8_t* table_b,
      uint16_t phase, uint8_t balance) {
    return Signal::Mix(InterpolateSample(table_a, phase),
                       InterpolateSample(table_b, phase),
                       balance & 0xff);
  }
  
  // ------- Band-limited waveforms with variable pulse width -----------------.
  static void UpdatePulseSquare() {
    uint8_t note = Signal::Swap4(note_ - 24);
    uint8_t wave_index = note & 0xf;
    // If the parameter is different from 0, use the PWM generator - otherwise,
    // use directly the wavetable. This will never happen for the
    // sub oscillator.
    if (parameter_ != 0 || algorithm_ == WAVEFORM_IMPULSE_TRAIN) {
      // TODO(pichenettes): find better formula for leaky integrator constant.
      data_.bl.leak = 255 - ((note_ - 24) >> 3);
      data_.bl.wave[0] =
          waveform_table[WAV_RES_BANDLIMITED_PULSE_1 + wave_index];
      data_.bl.shift = uint16_t(parameter_ + 127) << 8;
    } else {
      if (mode == LOW_COMPLEXITY) {
        wave_index = Signal::AddClip(wave_index, 1, kNumZonesFullSampleRate);
      }
      data_.bl.wave[0] =
          waveform_table[WAV_RES_BANDLIMITED_SQUARE_0 + wave_index];
      wave_index = Signal::AddClip(wave_index, 1, kNumZonesFullSampleRate);
      data_.bl.wave[1] =
          waveform_table[WAV_RES_BANDLIMITED_SQUARE_0 + wave_index];
      // Leak is set to 0 - this will be used by the rendering code to know that
      // it should render a pure square wave, by not trying to integrate a
      // blit, but instead directly reading from the wavetable.
      data_.bl.leak = 0;
      data_.bl.balance = note & 0xf0;
    }
  }
  static void RenderPulseSquare() {
    if (data_.bl.leak == 0 && mode == FULL) {
      phase_ += phase_increment_;
    } else {
      HALF_SAMPLE_RATE;
      phase_ += phase_increment_2_;
    }
    if (data_.bl.leak) {
      int16_t blit = InterpolateSample(data_.bl.wave[0], phase_);
      blit -= InterpolateSample(data_.bl.wave[0], phase_ + data_.bl.shift);
      if (algorithm_ == WAVEFORM_IMPULSE_TRAIN) {
        held_sample_ = Signal::Clip8(blit + 128);
      } else {
        int8_t square = Signal::SignedClip8(
            Signal::SignedMulScale8(data_.bl.square, data_.bl.leak) + blit);
        data_.bl.square = square;
        held_sample_ = square + 128;
      }
    } else {
      held_sample_ = InterpolateTwoTables(
          data_.bl.wave[0], data_.bl.wave[1],
          phase_, data_.bl.balance);
    }
  }
  
  // ------- Minimal version of the square and triangle oscillators ------------
  static void UpdateSub() {
    uint8_t note = Signal::Swap4(note_ - 12);
    uint8_t wave_index = note & 0xf;
    uint8_t base_resource_id = algorithm_ == WAVEFORM_SQUARE ?
        WAV_RES_BANDLIMITED_SQUARE_1 :
        WAV_RES_BANDLIMITED_TRIANGLE_1;

    wave_index = Signal::AddClip(wave_index, 1, kNumZonesHalfSampleRate);
    data_.wv.wave[0] = waveform_table[base_resource_id + wave_index];
    wave_index = Signal::AddClip(wave_index, 1, kNumZonesHalfSampleRate);
    data_.wv.wave[1] = waveform_table[base_resource_id + wave_index];
    data_.wv.balance = note & 0xf0;
  }
  static void RenderSub() {
    FOURTH_SAMPLE_RATE;
    phase_ += phase_increment_;
    held_sample_ = InterpolateTwoTables(
        data_.wv.wave[0], data_.wv.wave[1],
        phase_, data_.wv.balance);
  }

  // ------- Interpolation between two waveforms from two wavetables -----------
  static void UpdateSawTriangle() {
    uint8_t note = Signal::Swap4(note_ - 24);
    uint8_t wave_index = note & 0xf;
    uint8_t base_resource_id = algorithm_ == WAVEFORM_SAW ?
        WAV_RES_BANDLIMITED_SAW_0 :
        WAV_RES_BANDLIMITED_TRIANGLE_0;
      
    if (mode == LOW_COMPLEXITY) {
      wave_index = Signal::AddClip(wave_index, 1, kNumZonesFullSampleRate);
    }
    data_.wv.wave[0] = waveform_table[base_resource_id + wave_index];
    wave_index = Signal::AddClip(wave_index, 1, kNumZonesFullSampleRate);
    data_.wv.wave[1] = waveform_table[base_resource_id + wave_index];
    data_.wv.balance = note & 0xf0;
  }
  static void RenderSawTriangle() {
    if (mode == FULL) {
      phase_ += phase_increment_;
    } else {
      HALF_SAMPLE_RATE;
      phase_ += phase_increment_2_;
    }
    held_sample_ = InterpolateTwoTables(
        data_.wv.wave[0], data_.wv.wave[1],
        phase_, data_.wv.balance);

    // To produce pulse width-modulated variants, we shift (saw) or set to
    // a constant (triangle) a portion of the waveform within an increasingly
    // large fraction of the period. Note that this is pure waveshapping - the
    // phase information is not used to determine when/where to shift.
    //
    //     /|            /|          /\             /\
    //    / |           / |         /  \           /  \
    //   /  |    =>  /|/  |        /    \  =>  ___/    \
    //  /   |       /     |/      /      \
    // /    |/                   /        \
    //
    if (held_sample_ < parameter_) {
      if (algorithm_ == WAVEFORM_SAW) {
        // Add a discontinuity.
        held_sample_ += parameter_ >> 1;
      } else {
        // Clip.
        held_sample_ = parameter_;
      }
    }
  }

  // ------- Interpolation between two offsets of a wavetable ------------------
  // 64 samples per cycle.
  static void UpdateWavetable64() {
    // Since the wavetable is very crowded (32 waveforms) and the parameter
    // value has a low resolution (4 positions between each waveform), the
    // parameter value is smoothed to avoid rough stepping.
    int16_t target_parameter = parameter_ * 64;
    int16_t increment = target_parameter - data_.wt.smooth_parameter >> 4;
    if (increment == 0) {
      if (target_parameter < data_.wt.smooth_parameter) {
        increment = -1;
      } else if (target_parameter > data_.wt.smooth_parameter) {
        increment = +1;
      }
    }
    data_.wt.smooth_parameter += increment;
  }
  static void RenderWavetable64() {
    HALF_SAMPLE_RATE;
    phase_ += phase_increment_2_;
    uint8_t p = data_.wt.smooth_parameter >> 8;
    uint16_t offset_a = (p << 6) + p;  // p * 65
    offset_a += phase_ >> 10;
    const prog_uint8_t* wav_res_wavetable = waveform_table[WAV_RES_WAVETABLE];
    wav_res_wavetable += offset_a;
    uint8_t lo = (phase_ >> 2) & 0xff;
    held_sample_ = InterpolateTwoTables(
        wav_res_wavetable,
        wav_res_wavetable + 65,
        lo,
        data_.wt.smooth_parameter & 0xff);
  }
  
  // ------- Casio CZ-like synthesis ------------------------------------------.
  static void UpdateCz() {
    data_.cz.formant_phase_increment = phase_increment_2_ + (
        (phase_increment_2_ * uint32_t(parameter_)) >> 3);
  }
  static void RenderCz() {
    HALF_SAMPLE_RATE;
    
    uint16_t last_phase = phase_;
    phase_ += phase_increment_2_;
    if (phase_ < last_phase) {
      data_.cz.formant_phase = 0;
    }
    // TODO(pichenettes): limit increment to avoid aliasing (this will be
    // equivalent to clipping a VCF control signal).
    data_.cz.formant_phase += data_.cz.formant_phase_increment;
    uint8_t result = InterpolateSample(
        waveform_table[WAV_RES_SINE],
        data_.cz.formant_phase);
    held_sample_ = Signal::MulScale8(result, 255 - (phase_ >> 8));
  }
  
  // ------- FM ---------------------------------------------------------------.
  static void UpdateFm() {
    uint16_t increment = phase_increment_2_;
    uint8_t triangle = parameter_ & 0x1f;
    if (parameter_ & 0x10) { 
      triangle ^= 0x1f;
    }
    increment += increment * triangle;
    data_.fm.modulator_phase_increment = increment >> 3;
  }
  static void RenderFm() {
    HALF_SAMPLE_RATE
    
    data_.fm.modulator_phase += data_.fm.modulator_phase_increment;
    int8_t modulator = ReadSample(waveform_table[WAV_RES_SINE],
                                  data_.fm.modulator_phase) - 128;
    int16_t modulation = Signal::SignedMulScale4(modulator, parameter_);
    phase_ += phase_increment_2_ + modulation;
    held_sample_ = InterpolateSample(waveform_table[WAV_RES_SINE], phase_);
  }
  
  // ------- 8-bit land -------------------------------------------------------.
  static void Render8BitLand() {
    phase_ += phase_increment_;
    uint8_t x = parameter_;
    held_sample_ = (((phase_ >> 8) ^ (x << 1)) & (~x)) + (x >> 1);
  }
  
  // ------- Speech -----------------------------------------------------------.
  static void UpdateSpeech() {
    data_.sp.update++;
    if (data_.sp.update == kSpeechControlRateDecimation) {
      data_.sp.update = 255;
    } else {
      return;
    }
    
    uint8_t offset_1 = Signal::ShiftRight4(parameter_);
    offset_1 = (offset_1 << 2) + offset_1;  // offset_1 * 5
    uint8_t offset_2 = offset_1 + 5;
    uint8_t balance = parameter_ & 15;
    for (uint8_t i = 0; i < 3; ++i) {
      data_.sp.formant_increment[i] = Signal::UnscaledMix4(
          ResourcesManager::Lookup<uint8_t, uint8_t>(
              waveform_table[WAV_RES_SPEECH_DATA], offset_1 + i),
          ResourcesManager::Lookup<uint8_t, uint8_t>(
              waveform_table[WAV_RES_SPEECH_DATA], offset_2 + i),
          balance);
      data_.sp.formant_increment[i] <<= 3;
    }
    for (uint8_t i = 0; i < 2; ++i) {
      uint8_t amplitude_a = ResourcesManager::Lookup<uint8_t, uint8_t>(
          waveform_table[WAV_RES_SPEECH_DATA],
          offset_1 + 3 + i);
      uint8_t amplitude_b = ResourcesManager::Lookup<uint8_t, uint8_t>(
          waveform_table[WAV_RES_SPEECH_DATA],
          offset_2 + 3 + i);

      data_.sp.formant_amplitude[2 * i + 1] = Signal::Mix4(
          amplitude_a & 0xf,
          amplitude_b & 0xf, balance);
      amplitude_a = Signal::ShiftRight4(amplitude_a);
      amplitude_b = Signal::ShiftRight4(amplitude_b);
      data_.sp.formant_amplitude[2 * i] = Signal::Mix4(
          amplitude_a,
          amplitude_b, balance);
    }
  }
  static void RenderSpeech() {
    HALF_SAMPLE_RATE;
    
    int8_t result = 0;
    for (uint8_t i = 0; i < 3; ++i) {
      data_.sp.formant_phase[i] += data_.sp.formant_increment[i];
      result += ResourcesManager::Lookup<uint8_t, uint8_t>(
          i == 2 ? waveform_table[WAV_RES_FORMANT_SQUARE] :
                   waveform_table[WAV_RES_FORMANT_SINE],
          ((data_.sp.formant_phase[i] >> 8) & 0xf0) |
            data_.sp.formant_amplitude[i]);
    }
    result = Signal::SignedMulScale8(result, 255 - (phase_ >> 8));

    phase_ += phase_increment_;
    int16_t phase_noise = int8_t(Random::state_msb()) *
        int8_t(data_.sp.noise_modulation);
    if ((phase_ + phase_noise) < phase_increment_) {
      data_.sp.formant_phase[0] = 0;
      data_.sp.formant_phase[1] = 0;
      data_.sp.formant_phase[2] = 0;
    }
    held_sample_ = result + 128;
  }
  
  DISALLOW_COPY_AND_ASSIGN(Oscillator);
};

#define Os Oscillator<id, mode>

template<int id, OscillatorMode mode>
uint16_t Oscillator<id, mode>::phase_increment_;
template<int id, OscillatorMode mode>
uint16_t Oscillator<id, mode>::phase_increment_2_;

template<int id, OscillatorMode mode> uint16_t Oscillator<id, mode>::phase_;
template<int id, OscillatorMode mode> uint8_t Oscillator<id, mode>::algorithm_;
template<int id, OscillatorMode mode> uint8_t Oscillator<id, mode>::parameter_;
template<int id, OscillatorMode mode> uint8_t Oscillator<id, mode>::note_;
template<int id, OscillatorMode mode> uint8_t Oscillator<id, mode>::sweeping_;
template<int id, OscillatorMode mode> uint8_t Oscillator<id, mode>::held_sample_;
template<int id, OscillatorMode mode> OscillatorData Oscillator<id, mode>::data_;
template<int id, OscillatorMode mode> AlgorithmFn Oscillator<id, mode>::fn_;
template<int id, OscillatorMode mode>
AlgorithmFn Oscillator<id, mode>::fn_table_low_complexity_[] = {
  { &Os::UpdatePulseSquare, &Os::RenderPulseSquare },
  { &Os::UpdateSawTriangle, &Os::RenderSawTriangle },
};
template<int id, OscillatorMode mode>
AlgorithmFn Oscillator<id, mode>::fn_table_[] = {
  { &Os::UpdatePulseSquare, &Os::RenderPulseSquare },
  { &Os::UpdateSawTriangle, &Os::RenderSawTriangle },
  { &Os::UpdatePulseSquare, &Os::RenderPulseSquare },
  { &Os::UpdateSawTriangle, &Os::RenderSawTriangle },
  { &Os::UpdateCz, &Os::RenderCz },
  { &Os::UpdateFm, &Os::RenderFm },
  { NULL, &Os::Render8BitLand },
  { &Os::UpdateSpeech, &Os::RenderSpeech },
  { &Os::UpdateWavetable64, &Os::RenderWavetable64 },
  { &Os::UpdateSawTriangle, &Os::RenderSawTriangle },
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_OSCILLATOR_H_
