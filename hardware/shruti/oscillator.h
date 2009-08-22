// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Oscillators. Note that the code of each oscillator is duplicated/specialized,
// for a noticeable performance boost.

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

const uint8_t kSpeechControlRateDecimation = 4;

struct BandLimitedOscillatorData {
  int8_t square;
  uint16_t shift;
  uint8_t leak;
  uint8_t pure_square_shortcut;
  const prog_uint8_t* pulse_tbl;
};

struct HqWavetableOscillatorData {
  uint8_t balance;
  const prog_uint8_t* wave_a;
  const prog_uint8_t* wave_b;
};

struct DirtyWavetableOscillatorData {
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
  BandLimitedOscillatorData bl;
  HqWavetableOscillatorData wv;
  CzOscillatorData cz;
  FmOscillatorData fm;
  SpeechSynthesizerData sp;
  DirtyWavetableOscillatorData wt;
};

typedef uint8_t (*RenderFn)();
typedef void (*UpdateFn)();
typedef void (*ResetFn)();

struct AlgorithmFn {
  ResetFn reset;
  UpdateFn update;
  RenderFn render;
};

template<int id, bool stripped_down = false>
class Oscillator {
 public:
   // Called whenever the parameters of the oscillator change. Can be used
   // to pre-compute parameters, set tables, etc.
   static inline void SetupAlgorithm(uint8_t algorithm) {
     if (algorithm != algorithm_) {
       if (stripped_down) {
         fn_ = fn_tablet_[algorithm & 1];
       } else {
         fn_ = fn_table_[algorithm];
       }
       algorithm_ = algorithm;
       Reset();
       if (!stripped_down) {
         sweeping_ = algorithm_ == WAVEFORM_ANALOG_WAVETABLE;
       }
     }
  }
  static inline uint8_t Render() {
    // Updating the phase by the increment here is a bad idea, because it will
    // be reloaded from memory anyway in the oscillator code @@, because we
    // might use a different increment (FM), or because we might need to
    // check if we have completed a cycle to sync to another waveform.
    return (*fn_.render)();
  }
  static inline void Reset() {
    ResetPhase();
    if (fn_.reset) {
      (*fn_.reset)();
    }
  }
  static inline void Update(
      uint8_t parameter,
      uint8_t note,
      uint16_t increment) {
    if (!stripped_down && sweeping_) {
      algorithm_ = parameter >> 5;
      // Sweeping through the triangle table is boring.
      if (parameter >= 96) {
        ++algorithm_;
      }
      fn_ = fn_table_[algorithm_];
      parameter = (parameter & 0x1f) << 2;
    }
    parameter_ = parameter;
    phase_increment_ = increment;    
    note_ = note;
    if (fn_.update) {
      (*fn_.update)();
    }
  }
  static inline uint16_t phase() { return phase_; }
  static inline void ResetPhase() { phase_ = 0;  }

 private:
  // Current phase of the oscillator.
  static uint16_t phase_;
  
  // Phase increment.
  static uint16_t phase_increment_;
  
  // Copy of the algorithm used by this oscillator. Change along with the
  // Setup/Update/Render pointers.
  static uint8_t algorithm_;
  // Whether we are sweeping through the algorithms.
  static uint8_t sweeping_;
  
  // Current value of the oscillator parameter.
  static uint8_t parameter_;
  
  // Current MIDI note (used for wavetable selection).
  static uint8_t note_;
  
  // Union of state data used by each algorithm.
  static OscillatorData data_;
  
  // A trio of pointers to the setup/update/render functions. update/render
  // function might be NULL.
  static AlgorithmFn fn_;
  
  static AlgorithmFn fn_table_[];
  static AlgorithmFn fn_tablet_[];
  
  static inline uint8_t ReadSample(const prog_uint8_t* table, uint16_t phase) {
    return ResourcesManager::Lookup<uint8_t, uint8_t>(table, phase >> 8);
  }
#ifdef __FAST_SIGNAL_PROCESSING__
  static inline uint8_t InterpolateSample(const prog_uint8_t* table,
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
      : "+r" (result)
      : "a" (table), "a" (phase)
      : "r30", "r31"
    );
    return result;
  }
#else 
  static inline uint8_t InterpolateSample(const prog_uint8_t* table,
                                          uint16_t phase) {
    return Mix(
        ResourcesManager::Lookup<uint8_t, uint8_t>(table, phase >> 8),
        ResourcesManager::Lookup<uint8_t, uint8_t>(table, 1 + (phase >> 8)),
        phase & 0xff);
  }
#endif  // __FAST_SIGNAL_PROCESSING__
  
  // ------- Band-limited waveforms with variable pulse width -----------------.
  static void ResetBandLimited() {
    data_.bl.square = 0;
    data_.bl.leak = 255;
    data_.bl.pulse_tbl = waveform_table[WAV_RES_BANDLIMITED_PULSE_0];
  }
  static void UpdateBandLimited() {
    uint8_t note = Signal::Shift4(note_ - 24);
    if (parameter_ != 0 || algorithm_ == WAVEFORM_IMPULSE_TRAIN) {
      // TODO(oliviergillet) : find better formula for leaky integrator constant.
      data_.bl.leak = 255 - ((note_ - 24) >> 3);
      data_.bl.pulse_tbl = waveform_table[WAV_RES_BANDLIMITED_PULSE_0 + note];
      data_.bl.shift = uint16_t(parameter_ + 127) << 8;
      data_.bl.pure_square_shortcut = 0;
    } else {
      data_.bl.pulse_tbl = waveform_table[WAV_RES_BANDLIMITED_SQUARE_0 + note];
      data_.bl.pure_square_shortcut = 1;
    }
  }
  static uint8_t RenderBandLimited() {
    phase_ += phase_increment_;
    // The pure square wave is precomputed!
    if (data_.bl.pure_square_shortcut) {
      return InterpolateSample(data_.bl.pulse_tbl, phase_);
    } else {
      int16_t blit = InterpolateSample(data_.bl.pulse_tbl, phase_);
      blit -= InterpolateSample(data_.bl.pulse_tbl, phase_ + data_.bl.shift);
      if (algorithm_ == WAVEFORM_IMPULSE_TRAIN) {
        return Signal::Clip8(blit + 128);
      } else {
        int8_t square = Signal::SignedClip8(
            Signal::SignedMulScale8(data_.bl.square, data_.bl.leak) + blit);
        data_.bl.square = square;
        return square + 128;
      }
    }
  }
  
  // ------- Interpolation between two waveforms in a wavetable ---------------.
  // 256 samples per cycle.
  static void ResetHqWave() {
    data_.wv.balance = 0;
    data_.wv.wave_a = waveform_table[WAV_RES_BANDLIMITED_SAW_0];
    data_.wv.wave_b = waveform_table[WAV_RES_BANDLIMITED_SAW_0];
  }
  static void UpdateHqWave() {
    uint8_t note = Signal::Shift4(note_ - 24);
    switch (algorithm_) {
      case WAVEFORM_SAW:
        data_.wv.wave_a = waveform_table[WAV_RES_BANDLIMITED_SAW_0 + note];
        data_.wv.wave_b = waveform_table[WAV_RES_BANDLIMITED_TRIANGLE_0 + note];
        data_.wv.balance = parameter_ << 1;
        break;
      case WAVEFORM_TRIANGLE:
        data_.wv.wave_a = waveform_table[WAV_RES_BANDLIMITED_TRIANGLE_0 + note];
        data_.wv.wave_b = waveform_table[WAV_RES_BANDLIMITED_TRIANGLE_5];
        data_.wv.balance = parameter_ << 1;
        break;
    }
  }
  static uint8_t RenderHqWave() {
    phase_ += phase_increment_;
    uint8_t a = InterpolateSample(data_.wv.wave_a, phase_);
    if (data_.wv.balance) {
      uint8_t b = InterpolateSample(data_.wv.wave_b, phase_);
      return Signal::Mix(a, b, data_.wv.balance);
    } else {
      return a;
    }
  }

  // ------- Cheaper and dirtier wavetable ------------------------------------.
  // Uses 64 samples per cycle (not 256) - and the samples are not particularly
  // band-limited.
  static void ResetDirtyWave() {
    data_.wt.smooth_parameter = 0;
  }
  static void UpdateDirtyWave() {
    // Since the wavetable is very crowded (32 waveforms) and the parameter
    // value has a low resolution (4 positions between each waveform), the
    // parameter value is smoothed.
    int16_t target_parameter = parameter_ * 64;
    int16_t increment = (target_parameter - data_.wt.smooth_parameter) >> 4;
    if (increment == 0) {
      if (target_parameter < data_.wt.smooth_parameter) {
        increment = -1;
      } else if (target_parameter > data_.wt.smooth_parameter) {
        increment = +1;
      }
    }
    data_.wt.smooth_parameter += increment;
  }
  static uint8_t RenderDirtyWave() {
    phase_ += phase_increment_;
    uint8_t p = data_.wt.smooth_parameter >> 8;
    uint16_t offset_a = (p << 6) + p;  // p * 65
    offset_a += phase_ >> 10;
    const prog_uint8_t* wav_res_wavetable = waveform_table[WAV_RES_WAVETABLE];
    wav_res_wavetable += offset_a;
    uint8_t lo = (phase_ >> 2) & 0xff;
    uint8_t wave_a = InterpolateSample(wav_res_wavetable, lo);
    uint8_t wave_b = InterpolateSample(wav_res_wavetable, 0x4100 | lo);
    return Signal::Mix(wave_a, wave_b, data_.wt.smooth_parameter & 0xff);
  }
  
  // ------- Casio CZ-like synthesis ------------------------------------------.
  static void UpdateCz() {
    data_.cz.formant_phase_increment = phase_increment_ + (
        (phase_increment_ * uint32_t(parameter_)) >> 3);
  }
  static uint8_t RenderCz() {
    uint16_t last_phase = phase_;
    phase_ += phase_increment_;
    if (phase_ < last_phase) {
      data_.cz.formant_phase = 0;
    }
    data_.cz.formant_phase += data_.cz.formant_phase_increment;
    uint8_t result = InterpolateSample(
        waveform_table[WAV_RES_SINE],
        data_.cz.formant_phase);
    result = Signal::MulScale8(result, 255 - (phase_ >> 8));
    return result;
  }
  
  // ------- FM ---------------------------------------------------------------.
  static void UpdateFm() {
    uint16_t increment = phase_increment_;
    uint8_t triangle = parameter_ & 0x1f;
    if (parameter_ & 0x10) { 
      triangle ^= 0x1f;
    }
    increment += increment * triangle;
    data_.fm.modulator_phase_increment = increment >> 3;
  }
  static uint8_t RenderFm() {
    data_.fm.modulator_phase += data_.fm.modulator_phase_increment;
    int8_t modulator = ReadSample(waveform_table[WAV_RES_SINE],
                                  data_.fm.modulator_phase) - 128;
    int16_t modulation = Signal::SignedMulScale4(modulator, parameter_);
    phase_ += phase_increment_ + modulation;
    return InterpolateSample(waveform_table[WAV_RES_SINE], phase_);
  }
  
  // ------- 8-bit land -------------------------------------------------------.
  static uint8_t Render8BitLand() {
    phase_ += phase_increment_;
    uint8_t x = parameter_;
    return (((phase_ >> 8) ^ (x << 1)) & (~x)) + (x >> 1);
  }
  
  // ------- Speech -----------------------------------------------------------.
  static void UpdateSpeech() {
    data_.sp.update++;
    if (data_.sp.update == kSpeechControlRateDecimation) {
      data_.sp.update = 0;
    }
    
    if (data_.sp.update != 1) {
      return;
    }
    
    uint8_t offset_1 = Signal::Shift4(parameter_);
    offset_1 = (offset_1 << 2) + offset_1;  // offset_1 * 5
    uint8_t offset_2 = offset_1 + 5;
    uint8_t mix = parameter_ & 15;
    for (uint8_t i = 0; i < 3; ++i) {
      data_.sp.formant_increment[i] = Signal::UnscaledMix4(
          ResourcesManager::Lookup<uint8_t, uint8_t>(
              waveform_table[WAV_RES_SPEECH_DATA], offset_1 + i),
          ResourcesManager::Lookup<uint8_t, uint8_t>(
              waveform_table[WAV_RES_SPEECH_DATA], offset_2 + i),
          mix);
      data_.sp.formant_increment[i] <<= 2;
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
          amplitude_b & 0xf, mix);
      amplitude_a = Signal::Shift4(amplitude_a);
      amplitude_b = Signal::Shift4(amplitude_b);
      data_.sp.formant_amplitude[2 * i] = Signal::Mix4(
          amplitude_a,
          amplitude_b, mix);
    }
  }
  static uint8_t RenderSpeech() {
    int16_t phase_noise = 0;
    if (data_.sp.noise_modulation) {
      phase_noise = int8_t(Random::Byte()) * int8_t(data_.sp.noise_modulation);
    }

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
    if ((phase_ + phase_noise) < phase_increment_) {
      data_.sp.formant_phase[0] = 0;
      data_.sp.formant_phase[1] = 0;
      data_.sp.formant_phase[2] = 0;
    }
    return result + 128;
  }
  
  DISALLOW_COPY_AND_ASSIGN(Oscillator);
};

#define Os Oscillator<id, lite>

template<int id, bool lite> uint16_t Oscillator<id, lite>::phase_;
template<int id, bool lite> uint16_t Oscillator<id, lite>::phase_increment_;
template<int id, bool lite> uint8_t Oscillator<id, lite>::algorithm_;
template<int id, bool lite> uint8_t Oscillator<id, lite>::parameter_;
template<int id, bool lite> uint8_t Oscillator<id, lite>::note_;
template<int id, bool lite> uint8_t Oscillator<id, lite>::sweeping_;
template<int id, bool lite> OscillatorData Oscillator<id, lite>::data_;
template<int id, bool lite> AlgorithmFn Oscillator<id, lite>::fn_;
template<int id, bool lite> AlgorithmFn Oscillator<id, lite>::fn_tablet_[] = {
  { &Os::ResetBandLimited, &Os::UpdateBandLimited, &Os::RenderBandLimited },
  { &Os::ResetHqWave, &Os::UpdateHqWave, &Os::RenderHqWave },
};
template<int id, bool lite> AlgorithmFn Oscillator<id, lite>::fn_table_[] = {
  { &Os::ResetBandLimited, &Os::UpdateBandLimited, &Os::RenderBandLimited },
  { &Os::ResetHqWave, &Os::UpdateHqWave, &Os::RenderHqWave },
  { &Os::ResetBandLimited, &Os::UpdateBandLimited, &Os::RenderBandLimited },
  { &Os::ResetHqWave, &Os::UpdateHqWave, &Os::RenderHqWave },
  { NULL, &Os::UpdateCz, &Os::RenderCz },  // cz
  { NULL, &Os::UpdateFm, &Os::RenderFm },  // fm
  { NULL, NULL, &Os::Render8BitLand },
  { NULL, &Os::UpdateSpeech, &Os::RenderSpeech },  // speech
  { &Os::ResetDirtyWave, &Os::UpdateDirtyWave, &Os::RenderDirtyWave },
  { &Os::ResetHqWave, &Os::UpdateHqWave, &Os::RenderHqWave },
};

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_OSCILLATOR_H_
