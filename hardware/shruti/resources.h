// Copyright 2009 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
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
// Resources definitions.
//
// Automatically generated with:
// make resources


#ifndef HARDWARE_SHRUTI_RESOURCES_H_
#define HARDWARE_SHRUTI_RESOURCES_H_


#include "hardware/base/base.h"

#include <avr/pgmspace.h>


#include "hardware/resources/resources_manager.h"
namespace hardware_shruti {

typedef uint8_t ResourceId;

extern const prog_char* string_table[];

extern const prog_uint16_t* lookup_table_table[];

extern const prog_uint8_t* waveform_table[];

extern const prog_uint8_t* character_table[];

extern const prog_uint16_t lut_res_lfo_increments[] PROGMEM;
extern const prog_uint16_t lut_res_env_portamento_increments[] PROGMEM;
extern const prog_uint16_t lut_res_oscillator_increments[] PROGMEM;
extern const prog_uint16_t lut_res_fm_frequency_ratios[] PROGMEM;
extern const prog_uint16_t lut_res_scale_just[] PROGMEM;
extern const prog_uint16_t lut_res_scale_pythagorean[] PROGMEM;
extern const prog_uint16_t lut_res_scale_1_4_eb[] PROGMEM;
extern const prog_uint16_t lut_res_scale_1_4_e[] PROGMEM;
extern const prog_uint16_t lut_res_scale_1_4_ea[] PROGMEM;
extern const prog_uint16_t lut_res_scale_bhairav[] PROGMEM;
extern const prog_uint16_t lut_res_scale_gunakri[] PROGMEM;
extern const prog_uint16_t lut_res_scale_marwa[] PROGMEM;
extern const prog_uint16_t lut_res_scale_shree[] PROGMEM;
extern const prog_uint16_t lut_res_scale_purvi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_bilawal[] PROGMEM;
extern const prog_uint16_t lut_res_scale_yaman[] PROGMEM;
extern const prog_uint16_t lut_res_scale_kafi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_bhimpalasree[] PROGMEM;
extern const prog_uint16_t lut_res_scale_darbari[] PROGMEM;
extern const prog_uint16_t lut_res_scale_rageshree[] PROGMEM;
extern const prog_uint16_t lut_res_scale_khamaj[] PROGMEM;
extern const prog_uint16_t lut_res_scale_mimal[] PROGMEM;
extern const prog_uint16_t lut_res_scale_parameshwari[] PROGMEM;
extern const prog_uint16_t lut_res_scale_rangeshwari[] PROGMEM;
extern const prog_uint16_t lut_res_scale_gangeshwari[] PROGMEM;
extern const prog_uint16_t lut_res_scale_kameshwari[] PROGMEM;
extern const prog_uint16_t lut_res_scale_palas_kafi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_natbhairav[] PROGMEM;
extern const prog_uint16_t lut_res_scale_m_kauns[] PROGMEM;
extern const prog_uint16_t lut_res_scale_bairagi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_b_todi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_chandradeep[] PROGMEM;
extern const prog_uint16_t lut_res_scale_kaushik_todi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_jogeshwari[] PROGMEM;
extern const prog_uint16_t lut_res_arpeggiator_patterns[] PROGMEM;
extern const prog_uint16_t lut_res_turbo_tempi[] PROGMEM;
extern const prog_uint16_t lut_res_groove_swing[] PROGMEM;
extern const prog_uint16_t lut_res_groove_shuffle[] PROGMEM;
extern const prog_uint16_t lut_res_groove_push[] PROGMEM;
extern const prog_uint16_t lut_res_groove_lag[] PROGMEM;
extern const prog_uint16_t lut_res_groove_human[] PROGMEM;
extern const prog_uint8_t wav_res_formant_sine[] PROGMEM;
extern const prog_uint8_t wav_res_formant_square[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_0[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_6[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_0[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_0[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_5[] PROGMEM;
extern const prog_uint8_t wav_res_wavetable[] PROGMEM;
extern const prog_uint8_t wav_res_vowel_data[] PROGMEM;
extern const prog_uint8_t chr_res_special_characters[] PROGMEM;
#define STR_RES_PRM 0  // prm
#define STR_RES_RNG 1  // rng
#define STR_RES_OP 2  // op
#define STR_RES_TUN 3  // tun
#define STR_RES_PRT 4  // prt
#define STR_RES_PORTA 5  // porta
#define STR_RES_PARAMETER 6  // parameter
#define STR_RES_RANGE 7  // range
#define STR_RES_OPERATOR 8  // operator
#define STR_RES_DETUNE 9  // detune
#define STR_RES_OSC_BAL 10  // osc bal
#define STR_RES_SUB_OSC_ 11  // sub osc.
#define STR_RES_PATTERN 12  // pattern
#define STR_RES_GROOVE 13  // groove
#define STR_RES_OSCILLATOR_1 14  // oscillator 1
#define STR_RES_OSCILLATOR_2 15  // oscillator 2
#define STR_RES_ARPEGGIO 16  // arpeggio
#define STR_RES_PERFORMANCE 17  // performance
#define STR_RES_NONE 18  // none
#define STR_RES_BLIT 19  // blit
#define STR_RES_SAW 20  // saw
#define STR_RES_SQUARE 21  // square
#define STR_RES_TRIANG 22  // triang
#define STR_RES_CZ 23  // cz
#define STR_RES_FM 24  // fm
#define STR_RES_8BITS 25  // 8bits
#define STR_RES_PWM 26  // pwm
#define STR_RES_NOISE 27  // noise
#define STR_RES_VOWEL 28  // vowel
#define STR_RES_DRONE 29  // drone
#define STR_RES_SWEEP 30  // sweep
#define STR_RES_ZSYNC 31  // zsync
#define STR_RES_PAD 32  // pad
#define STR_RES_1S2 33  // 1+2
#define STR_RES_1_2 34  // 1>2
#define STR_RES_1P2 35  // 1*2
#define STR_RES_1X2 36  // 1^2
#define STR_RES_CUT 37  // cut
#define STR_RES_VCA 38  // vca
#define STR_RES_PW1 39  // pw1
#define STR_RES_PW2 40  // pw2
#define STR_RES_51 41  // 1
#define STR_RES_52 42  // 2
#define STR_RES_5 43  // 
#define STR_RES_MIX 44  // mix
#define STR_RES_NOI 45  // noi
#define STR_RES_SUB 46  // sub
#define STR_RES_RES 47  // res
#define STR_RES_CUTOFF 48  // cutoff
#define STR_RES__VCA 49  //  vca
#define STR_RES_PWM1 50  // pwm1
#define STR_RES_PWM2 51  // pwm2
#define STR_RES_OSC1 52  // osc1
#define STR_RES_OSC2 53  // osc2
#define STR_RES_OSC1S2 54  // osc1+2
#define STR_RES__MIX 55  //  mix
#define STR_RES__NOISE 56  //  noise
#define STR_RES_SUBOSC 57  // subosc
#define STR_RES_RESO 58  // reso
#define STR_RES_ATK 59  // atk
#define STR_RES_WV1 60  // wv1
#define STR_RES_RT1 61  // rt1
#define STR_RES_WV2 62  // wv2
#define STR_RES_RT2 63  // rt2
#define STR_RES_SRC 64  // src
#define STR_RES_DST 65  // dst
#define STR_RES_AMT 66  // amt
#define STR_RES_CHN 67  // chn
#define STR_RES_BPM 68  // bpm
#define STR_RES_SWG 69  // swg
#define STR_RES_SHAPE 70  // shape
#define STR_RES_ENV1TVCF 71  // env1~vcf
#define STR_RES_LFO2TVCF 72  // lfo2~vcf
#define STR_RES_RESONANCE 73  // resonance
#define STR_RES_ENVELOPE_1 74  // envelope 1
#define STR_RES_ENVELOPE_2 75  // envelope 2
#define STR_RES_SEQUENCER 76  // sequencer
#define STR_RES_ATTACK 77  // attack
#define STR_RES_DECAY 78  // decay
#define STR_RES_SUSTAIN 79  // sustain
#define STR_RES_RELEASE 80  // release
#define STR_RES_LFO1_WAVE 81  // lfo1 wave
#define STR_RES_LFO1_RATE 82  // lfo1 rate
#define STR_RES_LFO2_WAVE 83  // lfo2 wave
#define STR_RES_LFO2_RATE 84  // lfo2 rate
#define STR_RES_MOD_ 85  // mod.
#define STR_RES_SOURCE 86  // source
#define STR_RES_DEST_ 87  // dest.
#define STR_RES_AMOUNT 88  // amount
#define STR_RES_OCTAVE 89  // octave
#define STR_RES_RAGA 90  // raga
#define STR_RES_MIDI_CHAN 91  // midi chan
#define STR_RES_TEMPO 92  // tempo
#define STR_RES_MIXER 93  // mixer
#define STR_RES_FILTER 94  // filter
#define STR_RES_LFOS 95  // lfos
#define STR_RES_MODULATION 96  // modulation
#define STR_RES_KEYBOARD 97  // keyboard
#define STR_RES_OFF 98  // off
#define STR_RES_ON 99  // on
#define STR_RES_TRI 100  // tri
#define STR_RES_SQR 101  // sqr
#define STR_RES_S_H 102  // s&h
#define STR_RES_3 103  // 
#define STR_RES__SEQ 104  //  seq
#define STR_RES_LF1 105  // lf1
#define STR_RES_LF2 106  // lf2
#define STR_RES_SEQ 107  // seq
#define STR_RES_ARP 108  // arp
#define STR_RES_WHL 109  // whl
#define STR_RES_BND 110  // bnd
#define STR_RES_OFS 111  // ofs
#define STR_RES_CV1 112  // cv1
#define STR_RES_CV2 113  // cv2
#define STR_RES_CV3 114  // cv3
#define STR_RES_RND 115  // rnd
#define STR_RES_EN1 116  // en1
#define STR_RES_EN2 117  // en2
#define STR_RES_VEL 118  // vel
#define STR_RES_NOT 119  // not
#define STR_RES_GAT 120  // gat
#define STR_RES_LFO1 121  // lfo1
#define STR_RES_LFO2 122  // lfo2
#define STR_RES_STPSEQ 123  // stpseq
#define STR_RES__ARP 124  //  arp
#define STR_RES_MWHEEL 125  // mwheel
#define STR_RES_BENDER 126  // bender
#define STR_RES_OFFSET 127  // offset
#define STR_RES__CV1 128  //  cv1
#define STR_RES__CV2 129  //  cv2
#define STR_RES__CV3 130  //  cv3
#define STR_RES_RANDOM 131  // random
#define STR_RES_ENV1 132  // env1
#define STR_RES_ENV2 133  // env2
#define STR_RES_VELO 134  // velo
#define STR_RES_NOTE 135  // note
#define STR_RES_GATE 136  // gate
#define STR_RES_270 137  // 270
#define STR_RES_300 138  // 300
#define STR_RES_360 139  // 360
#define STR_RES_480 140  // 480
#define STR_RES_720 141  // 720
#define STR_RES_960 142  // 960
#define STR_RES_TOUCH_A_KNOB_TO 143  // touch a knob to
#define STR_RES_ASSIGN_PARAMETER 144  // assign parameter
#define STR_RES_READY 145  // ready
#define STR_RES_FOR_OS_UPDATE 146  // for os update
#define STR_RES_P_ORLEANS_21_MN 147  // P.ORLEANS: 21 MN
#define STR_RES_P_BERCY_____S1H 148  // P.BERCY:     +1H
#define STR_RES_PATCH_BANK 149  // patch bank
#define STR_RES_STEP_SEQUENCER 150  // step sequencer
#define STR_RES_LOAD 151  // load
#define STR_RES_ 152  // ----
#define STR_RES_SAVE 153  // save
#define STR_RES_EXTERN 154  // extern
#define STR_RES_MUTABLE____V0_56 155  // mutable    v0.56
#define STR_RES_INSTRUMENTS_671 156  // instruments -1
#define STR_RES_EQUAL 157  // equal
#define STR_RES_JUST 158  // just
#define STR_RES_PYTHAG 159  // pythag
#define STR_RES_1_4_EB 160  // 1/4 eb
#define STR_RES_1_4_E 161  // 1/4 e
#define STR_RES_1_4_EA 162  // 1/4 ea
#define STR_RES_BHAIRA 163  // bhaira
#define STR_RES_GUNAKR 164  // gunakr
#define STR_RES_MARWA 165  // marwa
#define STR_RES_SHREE 166  // shree
#define STR_RES_PURVI 167  // purvi
#define STR_RES_BILAWA 168  // bilawa
#define STR_RES_YAMAN 169  // yaman
#define STR_RES_KAFI 170  // kafi
#define STR_RES_BHIMPA 171  // bhimpa
#define STR_RES_DARBAR 172  // darbar
#define STR_RES_BAGESH 173  // bagesh
#define STR_RES_RAGESH 174  // ragesh
#define STR_RES_KHAMAJ 175  // khamaj
#define STR_RES_MIMAL 176  // mi'mal
#define STR_RES_PARAME 177  // parame
#define STR_RES_RANGES 178  // ranges
#define STR_RES_GANGES 179  // ganges
#define STR_RES_KAMESH 180  // kamesh
#define STR_RES_PALAS_ 181  // palas 
#define STR_RES_NATBHA 182  // natbha
#define STR_RES_M_KAUN 183  // m.kaun
#define STR_RES_BAIRAG 184  // bairag
#define STR_RES_B_TODI 185  // b.todi
#define STR_RES_CHANDR 186  // chandr
#define STR_RES_KAUSHI 187  // kaushi
#define STR_RES_JOGESH 188  // jogesh
#define STR_RES_RASIA 189  // rasia
#define LUT_RES_LFO_INCREMENTS 0
#define LUT_RES_LFO_INCREMENTS_SIZE 128
#define LUT_RES_ENV_PORTAMENTO_INCREMENTS 1
#define LUT_RES_ENV_PORTAMENTO_INCREMENTS_SIZE 128
#define LUT_RES_OSCILLATOR_INCREMENTS 2
#define LUT_RES_OSCILLATOR_INCREMENTS_SIZE 768
#define LUT_RES_FM_FREQUENCY_RATIOS 3
#define LUT_RES_FM_FREQUENCY_RATIOS_SIZE 25
#define LUT_RES_SCALE_JUST 4
#define LUT_RES_SCALE_JUST_SIZE 12
#define LUT_RES_SCALE_PYTHAGOREAN 5
#define LUT_RES_SCALE_PYTHAGOREAN_SIZE 12
#define LUT_RES_SCALE_1_4_EB 6
#define LUT_RES_SCALE_1_4_EB_SIZE 12
#define LUT_RES_SCALE_1_4_E 7
#define LUT_RES_SCALE_1_4_E_SIZE 12
#define LUT_RES_SCALE_1_4_EA 8
#define LUT_RES_SCALE_1_4_EA_SIZE 12
#define LUT_RES_SCALE_BHAIRAV 9
#define LUT_RES_SCALE_BHAIRAV_SIZE 12
#define LUT_RES_SCALE_GUNAKRI 10
#define LUT_RES_SCALE_GUNAKRI_SIZE 12
#define LUT_RES_SCALE_MARWA 11
#define LUT_RES_SCALE_MARWA_SIZE 12
#define LUT_RES_SCALE_SHREE 12
#define LUT_RES_SCALE_SHREE_SIZE 12
#define LUT_RES_SCALE_PURVI 13
#define LUT_RES_SCALE_PURVI_SIZE 12
#define LUT_RES_SCALE_BILAWAL 14
#define LUT_RES_SCALE_BILAWAL_SIZE 12
#define LUT_RES_SCALE_YAMAN 15
#define LUT_RES_SCALE_YAMAN_SIZE 12
#define LUT_RES_SCALE_KAFI 16
#define LUT_RES_SCALE_KAFI_SIZE 12
#define LUT_RES_SCALE_BHIMPALASREE 17
#define LUT_RES_SCALE_BHIMPALASREE_SIZE 12
#define LUT_RES_SCALE_DARBARI 18
#define LUT_RES_SCALE_DARBARI_SIZE 12
#define LUT_RES_SCALE_BAGESHREE 19
#define LUT_RES_SCALE_BAGESHREE_SIZE 12
#define LUT_RES_SCALE_RAGESHREE 20
#define LUT_RES_SCALE_RAGESHREE_SIZE 12
#define LUT_RES_SCALE_KHAMAJ 21
#define LUT_RES_SCALE_KHAMAJ_SIZE 12
#define LUT_RES_SCALE_MIMAL 22
#define LUT_RES_SCALE_MIMAL_SIZE 12
#define LUT_RES_SCALE_PARAMESHWARI 23
#define LUT_RES_SCALE_PARAMESHWARI_SIZE 12
#define LUT_RES_SCALE_RANGESHWARI 24
#define LUT_RES_SCALE_RANGESHWARI_SIZE 12
#define LUT_RES_SCALE_GANGESHWARI 25
#define LUT_RES_SCALE_GANGESHWARI_SIZE 12
#define LUT_RES_SCALE_KAMESHWARI 26
#define LUT_RES_SCALE_KAMESHWARI_SIZE 12
#define LUT_RES_SCALE_PALAS_KAFI 27
#define LUT_RES_SCALE_PALAS_KAFI_SIZE 12
#define LUT_RES_SCALE_NATBHAIRAV 28
#define LUT_RES_SCALE_NATBHAIRAV_SIZE 12
#define LUT_RES_SCALE_M_KAUNS 29
#define LUT_RES_SCALE_M_KAUNS_SIZE 12
#define LUT_RES_SCALE_BAIRAGI 30
#define LUT_RES_SCALE_BAIRAGI_SIZE 12
#define LUT_RES_SCALE_B_TODI 31
#define LUT_RES_SCALE_B_TODI_SIZE 12
#define LUT_RES_SCALE_CHANDRADEEP 32
#define LUT_RES_SCALE_CHANDRADEEP_SIZE 12
#define LUT_RES_SCALE_KAUSHIK_TODI 33
#define LUT_RES_SCALE_KAUSHIK_TODI_SIZE 12
#define LUT_RES_SCALE_JOGESHWARI 34
#define LUT_RES_SCALE_JOGESHWARI_SIZE 12
#define LUT_RES_SCALE_RASIA 35
#define LUT_RES_SCALE_RASIA_SIZE 12
#define LUT_RES_ARPEGGIATOR_PATTERNS 36
#define LUT_RES_ARPEGGIATOR_PATTERNS_SIZE 15
#define LUT_RES_TURBO_TEMPI 37
#define LUT_RES_TURBO_TEMPI_SIZE 6
#define LUT_RES_GROOVE_SWING 38
#define LUT_RES_GROOVE_SWING_SIZE 16
#define LUT_RES_GROOVE_SHUFFLE 39
#define LUT_RES_GROOVE_SHUFFLE_SIZE 16
#define LUT_RES_GROOVE_PUSH 40
#define LUT_RES_GROOVE_PUSH_SIZE 16
#define LUT_RES_GROOVE_LAG 41
#define LUT_RES_GROOVE_LAG_SIZE 16
#define LUT_RES_GROOVE_HUMAN 42
#define LUT_RES_GROOVE_HUMAN_SIZE 16
#define WAV_RES_FORMANT_SINE 0
#define WAV_RES_FORMANT_SINE_SIZE 256
#define WAV_RES_FORMANT_SQUARE 1
#define WAV_RES_FORMANT_SQUARE_SIZE 256
#define WAV_RES_BANDLIMITED_SQUARE_0 2
#define WAV_RES_BANDLIMITED_SQUARE_0_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_1 3
#define WAV_RES_BANDLIMITED_SQUARE_1_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_2 4
#define WAV_RES_BANDLIMITED_SQUARE_2_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_3 5
#define WAV_RES_BANDLIMITED_SQUARE_3_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_4 6
#define WAV_RES_BANDLIMITED_SQUARE_4_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_5 7
#define WAV_RES_BANDLIMITED_SQUARE_5_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_6 8
#define WAV_RES_BANDLIMITED_SQUARE_6_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_0 9
#define WAV_RES_BANDLIMITED_SAW_0_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_1 10
#define WAV_RES_BANDLIMITED_SAW_1_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_2 11
#define WAV_RES_BANDLIMITED_SAW_2_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_3 12
#define WAV_RES_BANDLIMITED_SAW_3_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_4 13
#define WAV_RES_BANDLIMITED_SAW_4_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_5 14
#define WAV_RES_BANDLIMITED_SAW_5_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_6 15
#define WAV_RES_BANDLIMITED_SAW_6_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_0 16
#define WAV_RES_BANDLIMITED_TRIANGLE_0_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_1 17
#define WAV_RES_BANDLIMITED_TRIANGLE_1_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_2 18
#define WAV_RES_BANDLIMITED_TRIANGLE_2_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_3 19
#define WAV_RES_BANDLIMITED_TRIANGLE_3_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_4 20
#define WAV_RES_BANDLIMITED_TRIANGLE_4_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_5 21
#define WAV_RES_BANDLIMITED_TRIANGLE_5_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_6 22
#define WAV_RES_BANDLIMITED_TRIANGLE_6_SIZE 257
#define WAV_RES_WAVETABLE 23
#define WAV_RES_WAVETABLE_SIZE 2193
#define WAV_RES_VOWEL_DATA 24
#define WAV_RES_VOWEL_DATA_SIZE 45
#define CHR_RES_SPECIAL_CHARACTERS 0
#define CHR_RES_SPECIAL_CHARACTERS_SIZE 64
typedef hardware_resources::ResourcesManager<
    ResourceId,
    hardware_resources::ResourcesTables<
        string_table,
        lookup_table_table> > ResourcesManager; 

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_RESOURCES_H_
