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
// python arc.py resources.py


#ifndef HARDWARE_SHRUTI_RESOURCES_H_
#define HARDWARE_SHRUTI_RESOURCES_H_


#include "hardware/base/base.h"

#ifdef __TEST__
#define prog_char char
#define prog_uint16_t uint16_t
#define prog_uint8_t uint8_t
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif


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
extern const prog_uint16_t lut_res_scale_kanakangi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_vanaspati[] PROGMEM;
extern const prog_uint16_t lut_res_scale_tanarupi[] PROGMEM;
extern const prog_uint16_t lut_res_scale_jhankaradhwani[] PROGMEM;
extern const prog_uint16_t lut_res_scale_kharaharapriya[] PROGMEM;
extern const prog_uint16_t lut_res_scale_varunapriya[] PROGMEM;
extern const prog_uint16_t lut_res_scale_yagapriya[] PROGMEM;
extern const prog_uint16_t lut_res_scale_vagadheeswari[] PROGMEM;
extern const prog_uint16_t lut_res_scale_chalanata[] PROGMEM;
extern const prog_uint16_t lut_res_arpeggiator_patterns[] PROGMEM;
extern const prog_uint8_t wav_res_formant_sine[] PROGMEM;
extern const prog_uint8_t wav_res_formant_square[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_pulse_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_pulse_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_pulse_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_pulse_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_pulse_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_pulse_6[] PROGMEM;
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
#define STR_RES_ATK 4  // atk
#define STR_RES_WV1 5  // wv1
#define STR_RES_RT1 6  // rt1
#define STR_RES_WV2 7  // wv2
#define STR_RES_RT2 8  // rt2
#define STR_RES_SRC 9  // src
#define STR_RES_DST 10  // dst
#define STR_RES_AMT 11  // amt
#define STR_RES_PRT 12  // prt
#define STR_RES_CHN 13  // chn
#define STR_RES_BPM 14  // bpm
#define STR_RES_SWG 15  // swg
#define STR_RES_SHAPE 16  // shape
#define STR_RES_PARAMETER 17  // parameter
#define STR_RES_RANGE 18  // range
#define STR_RES_OPERATOR 19  // operator
#define STR_RES_DETUNE 20  // detune
#define STR_RES_ENV1TVCF 21  // env1~vcf
#define STR_RES_LFO2TVCF 22  // lfo2~vcf
#define STR_RES_OSC_BAL 23  // osc bal
#define STR_RES_SUB_OSC_ 24  // sub osc.
#define STR_RES_RESONANCE 25  // resonance
#define STR_RES_ENVELOPE_1 26  // envelope 1
#define STR_RES_ENVELOPE_2 27  // envelope 2
#define STR_RES_SEQUENCER 28  // sequencer
#define STR_RES_ATTACK 29  // attack
#define STR_RES_DECAY 30  // decay
#define STR_RES_SUSTAIN 31  // sustain
#define STR_RES_RELEASE 32  // release
#define STR_RES_LFO1_WAVE 33  // lfo1 wave
#define STR_RES_LFO1_RATE 34  // lfo1 rate
#define STR_RES_LFO2_WAVE 35  // lfo2 wave
#define STR_RES_LFO2_RATE 36  // lfo2 rate
#define STR_RES_MOD_ 37  // mod.
#define STR_RES_SOURCE 38  // source
#define STR_RES_DEST_ 39  // dest.
#define STR_RES_AMOUNT 40  // amount
#define STR_RES_OCTAVE 41  // octave
#define STR_RES_RAGA 42  // raga
#define STR_RES_PORTA 43  // porta
#define STR_RES_MIDI_CHAN 44  // midi chan
#define STR_RES_TEMPO 45  // tempo
#define STR_RES_PATTERN 46  // pattern
#define STR_RES_SWING 47  // swing
#define STR_RES_OSCILLATOR_1 48  // oscillator 1
#define STR_RES_OSCILLATOR_2 49  // oscillator 2
#define STR_RES_MIXER 50  // mixer
#define STR_RES_FILTER 51  // filter
#define STR_RES_LFOS 52  // lfos
#define STR_RES_MODULATION 53  // modulation
#define STR_RES_KEYBOARD 54  // keyboard
#define STR_RES_ARPEGGIO 55  // arpeggio
#define STR_RES_OFF 56  // off
#define STR_RES_ON 57  // on
#define STR_RES_TRI 58  // tri
#define STR_RES_SQR 59  // sqr
#define STR_RES_S_H 60  // s&h
#define STR_RES_3 61  // 
#define STR_RES_NONE 62  // none
#define STR_RES_BLIT 63  // blit
#define STR_RES_SAW 64  // saw
#define STR_RES_SQUARE 65  // square
#define STR_RES_TRIANG 66  // triang
#define STR_RES_CZ 67  // cz
#define STR_RES_FM 68  // fm
#define STR_RES_8BITS 69  // 8bits
#define STR_RES_VOWEL 70  // vowel
#define STR_RES_TABLE 71  // table
#define STR_RES_SWEEP 72  // sweep
#define STR_RES_1S2 73  // 1+2
#define STR_RES_1_2 74  // 1>2
#define STR_RES_1P2 75  // 1*2
#define STR_RES_1X2 76  // 1^2
#define STR_RES_LF1 77  // lf1
#define STR_RES_LF2 78  // lf2
#define STR_RES_SEQ 79  // seq
#define STR_RES_ARP 80  // arp
#define STR_RES_WHL 81  // whl
#define STR_RES_BND 82  // bnd
#define STR_RES_AS1 83  // as1
#define STR_RES_AS2 84  // as2
#define STR_RES_CV1 85  // cv1
#define STR_RES_CV2 86  // cv2
#define STR_RES_RND 87  // rnd
#define STR_RES_EN1 88  // en1
#define STR_RES_EN2 89  // en2
#define STR_RES_VEL 90  // vel
#define STR_RES_NOT 91  // not
#define STR_RES_GAT 92  // gat
#define STR_RES_LFO1 93  // lfo1
#define STR_RES_LFO2 94  // lfo2
#define STR_RES_STPSEQ 95  // stpseq
#define STR_RES__ARP 96  //  arp
#define STR_RES_MWHEEL 97  // mwheel
#define STR_RES_BENDER 98  // bender
#define STR_RES_ASSGN1 99  // assgn1
#define STR_RES_ASSGN2 100  // assgn2
#define STR_RES__CV1 101  //  cv1
#define STR_RES__CV2 102  //  cv2
#define STR_RES_RANDOM 103  // random
#define STR_RES_ENV1 104  // env1
#define STR_RES_ENV2 105  // env2
#define STR_RES_VELO 106  // velo
#define STR_RES_NOTE 107  // note
#define STR_RES_GATE 108  // gate
#define STR_RES_CUT 109  // cut
#define STR_RES_VCA 110  // vca
#define STR_RES_PW1 111  // pw1
#define STR_RES_PW2 112  // pw2
#define STR_RES_51 113  // 1
#define STR_RES_52 114  // 2
#define STR_RES_5 115  // 
#define STR_RES_MIX 116  // mix
#define STR_RES_NOI 117  // noi
#define STR_RES_SUB 118  // sub
#define STR_RES_RES 119  // res
#define STR_RES_CUTOFF 120  // cutoff
#define STR_RES__VCA 121  //  vca
#define STR_RES_PWM1 122  // pwm1
#define STR_RES_PWM2 123  // pwm2
#define STR_RES_OSC1 124  // osc1
#define STR_RES_OSC2 125  // osc2
#define STR_RES_OSC1S2 126  // osc1+2
#define STR_RES__MIX 127  //  mix
#define STR_RES_NOISE 128  // noise
#define STR_RES_SUBOSC 129  // subosc
#define STR_RES_RESO 130  // reso
#define STR_RES_3_1 131  //  1
#define STR_RES_4_1 132  //  1
#define STR_RES_5_1 133  //  1
#define STR_RES___1 134  // ? 1
#define STR_RES_3_2 135  //  2
#define STR_RES_4_2 136  //  2
#define STR_RES_5_2 137  //  2
#define STR_RES___2 138  // ? 2
#define STR_RES_3_3 139  //  3
#define STR_RES_4_3 140  //  3
#define STR_RES_5_3 141  //  3
#define STR_RES___3 142  // ? 3
#define STR_RES_3_4 143  //  4
#define STR_RES_4_4 144  //  4
#define STR_RES_5_4 145  //  4
#define STR_RES___4 146  // ? 4
#define STR_RES_3_5 147  //  5
#define STR_RES_4_5 148  //  5
#define STR_RES_5_5 149  //  5
#define STR_RES___5 150  // ? 5
#define STR_RES_3_6 151  //  6
#define STR_RES_4_6 152  //  6
#define STR_RES_5_6 153  //  6
#define STR_RES___6 154  // ? 6
#define STR_RES_MUTABLE 155  // mutable
#define STR_RES_INSTRUMENTS_671 156  // instruments -1
#define STR_RES_P_ORLEANS_21_MN 157  // P.ORLEANS: 21 MN
#define STR_RES_P_BERCY_____S1H 158  // P.BERCY:     +1H
#define STR_RES_LOAD_SAVE_PATCH 159  // load/save patch
#define STR_RES_STEP_SEQUENCER 160  // step sequencer
#define STR_RES_LOAD 161  // load
#define STR_RES_EXIT 162  // exit
#define STR_RES_SAVE 163  // save
#define STR_RES_EXTERN 164  // extern
#define STR_RES_EQUAL 165  // equal
#define STR_RES_JUST 166  // just
#define STR_RES_PYTHAG 167  // pythag
#define STR_RES_1_4_EB 168  // 1/4 eb
#define STR_RES_1_4_E 169  // 1/4 e
#define STR_RES_1_4_EA 170  // 1/4 ea
#define STR_RES_KANAKA 171  // kanaka
#define STR_RES_RATNAN 172  // ratnan
#define STR_RES_GANAMU 173  // ganamu
#define STR_RES_VANASP 174  // vanasp
#define STR_RES_MANAVA 175  // manava
#define STR_RES_TANARU 176  // tanaru
#define STR_RES_SENAVA 177  // senava
#define STR_RES_HANUMA 178  // hanuma
#define STR_RES_DHENUK 179  // dhenuk
#define STR_RES_NATAKA 180  // nataka
#define STR_RES_KOKILA 181  // kokila
#define STR_RES_RUPAVA 182  // rupava
#define STR_RES_GAYAKA 183  // gayaka
#define STR_RES_VAKULA 184  // vakula
#define STR_RES_MAYAMA 185  // mayama
#define STR_RES_CHAKRA 186  // chakra
#define STR_RES_SURYAK 187  // suryak
#define STR_RES_HATAKA 188  // hataka
#define STR_RES_JHANKA 189  // jhanka
#define STR_RES_NATABH 190  // natabh
#define STR_RES_KEERAV 191  // keerav
#define STR_RES_KHARAH 192  // kharah
#define STR_RES_GOURIM 193  // gourim
#define STR_RES_VARUNA 194  // varuna
#define STR_RES_MARARA 195  // marara
#define STR_RES_CHARUK 196  // charuk
#define STR_RES_SARASA 197  // sarasa
#define STR_RES_HARIKA 198  // harika
#define STR_RES_DHEERA 199  // dheera
#define STR_RES_NAGANA 200  // nagana
#define STR_RES_YAGAPR 201  // yagapr
#define STR_RES_RAGAVA 202  // ragava
#define STR_RES_GANGEY 203  // gangey
#define STR_RES_VAGADH 204  // vagadh
#define STR_RES_SHULIN 205  // shulin
#define STR_RES_CHALAN 206  // chalan
#define STR_RES_SALAGA 207  // salaga
#define STR_RES_JALARN 208  // jalarn
#define STR_RES_JHALAV 209  // jhalav
#define STR_RES_NAVANE 210  // navane
#define STR_RES_PAVANI 211  // pavani
#define STR_RES_RAGHUP 212  // raghup
#define STR_RES_GAVAMB 213  // gavamb
#define STR_RES_BHAVAP 214  // bhavap
#define STR_RES_SHUBHA 215  // shubha
#define STR_RES_SHADVI 216  // shadvi
#define STR_RES_SUVARN 217  // suvarn
#define STR_RES_DIVYAM 218  // divyam
#define STR_RES_DHAVAL 219  // dhaval
#define STR_RES_NAMANA 220  // namana
#define STR_RES_KAMAVA 221  // kamava
#define STR_RES_RAMAPR 222  // ramapr
#define STR_RES_GAMANA 223  // gamana
#define STR_RES_VISHWA 224  // vishwa
#define STR_RES_SHAMAL 225  // shamal
#define STR_RES_SHANMU 226  // shanmu
#define STR_RES_SIMHEN 227  // simhen
#define STR_RES_HEMAVA 228  // hemava
#define STR_RES_DHARMA 229  // dharma
#define STR_RES_NEETIM 230  // neetim
#define STR_RES_KANTAM 231  // kantam
#define STR_RES_RISHAB 232  // rishab
#define STR_RES_LATANG 233  // latang
#define STR_RES_VACHAS 234  // vachas
#define STR_RES_MECHAK 235  // mechak
#define STR_RES_CHITRA 236  // chitra
#define STR_RES_SUCHAR 237  // suchar
#define STR_RES_JYOTIS 238  // jyotis
#define STR_RES_DHATUV 239  // dhatuv
#define STR_RES_NASIKA 240  // nasika
#define STR_RES_KOSALA 241  // kosala
#define STR_RES_RASIKA 242  // rasika
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
#define LUT_RES_SCALE_KANAKANGI 9
#define LUT_RES_SCALE_KANAKANGI_SIZE 12
#define LUT_RES_SCALE_RATNANGI 10
#define LUT_RES_SCALE_RATNANGI_SIZE 12
#define LUT_RES_SCALE_GANAMURTI 11
#define LUT_RES_SCALE_GANAMURTI_SIZE 12
#define LUT_RES_SCALE_VANASPATI 12
#define LUT_RES_SCALE_VANASPATI_SIZE 12
#define LUT_RES_SCALE_MANAVATI 13
#define LUT_RES_SCALE_MANAVATI_SIZE 12
#define LUT_RES_SCALE_TANARUPI 14
#define LUT_RES_SCALE_TANARUPI_SIZE 12
#define LUT_RES_SCALE_SENAVATI 15
#define LUT_RES_SCALE_SENAVATI_SIZE 12
#define LUT_RES_SCALE_HANUMATODI 16
#define LUT_RES_SCALE_HANUMATODI_SIZE 12
#define LUT_RES_SCALE_DHENUKA 17
#define LUT_RES_SCALE_DHENUKA_SIZE 12
#define LUT_RES_SCALE_NATAKAPRIYA 18
#define LUT_RES_SCALE_NATAKAPRIYA_SIZE 12
#define LUT_RES_SCALE_KOKILAPRIYA 19
#define LUT_RES_SCALE_KOKILAPRIYA_SIZE 12
#define LUT_RES_SCALE_RUPAVATI 20
#define LUT_RES_SCALE_RUPAVATI_SIZE 12
#define LUT_RES_SCALE_GAYAKAPRIYA 21
#define LUT_RES_SCALE_GAYAKAPRIYA_SIZE 12
#define LUT_RES_SCALE_VAKULABHARANAM 22
#define LUT_RES_SCALE_VAKULABHARANAM_SIZE 12
#define LUT_RES_SCALE_MAYAMALAVAGOWLA 23
#define LUT_RES_SCALE_MAYAMALAVAGOWLA_SIZE 12
#define LUT_RES_SCALE_CHAKRAVAKAM 24
#define LUT_RES_SCALE_CHAKRAVAKAM_SIZE 12
#define LUT_RES_SCALE_SURYAKANTAM 25
#define LUT_RES_SCALE_SURYAKANTAM_SIZE 12
#define LUT_RES_SCALE_HATAKAMBARI 26
#define LUT_RES_SCALE_HATAKAMBARI_SIZE 12
#define LUT_RES_SCALE_JHANKARADHWANI 27
#define LUT_RES_SCALE_JHANKARADHWANI_SIZE 12
#define LUT_RES_SCALE_NATABHAIRAVI 28
#define LUT_RES_SCALE_NATABHAIRAVI_SIZE 12
#define LUT_RES_SCALE_KEERAVANI 29
#define LUT_RES_SCALE_KEERAVANI_SIZE 12
#define LUT_RES_SCALE_KHARAHARAPRIYA 30
#define LUT_RES_SCALE_KHARAHARAPRIYA_SIZE 12
#define LUT_RES_SCALE_GOURIMANOHARI 31
#define LUT_RES_SCALE_GOURIMANOHARI_SIZE 12
#define LUT_RES_SCALE_VARUNAPRIYA 32
#define LUT_RES_SCALE_VARUNAPRIYA_SIZE 12
#define LUT_RES_SCALE_MARARANJANI 33
#define LUT_RES_SCALE_MARARANJANI_SIZE 12
#define LUT_RES_SCALE_CHARUKESI 34
#define LUT_RES_SCALE_CHARUKESI_SIZE 12
#define LUT_RES_SCALE_SARASANGI 35
#define LUT_RES_SCALE_SARASANGI_SIZE 12
#define LUT_RES_SCALE_HARIKAMBHOJI 36
#define LUT_RES_SCALE_HARIKAMBHOJI_SIZE 12
#define LUT_RES_SCALE_DHEERASANKARABHARANAM 37
#define LUT_RES_SCALE_DHEERASANKARABHARANAM_SIZE 12
#define LUT_RES_SCALE_NAGANANDINI 38
#define LUT_RES_SCALE_NAGANANDINI_SIZE 12
#define LUT_RES_SCALE_YAGAPRIYA 39
#define LUT_RES_SCALE_YAGAPRIYA_SIZE 12
#define LUT_RES_SCALE_RAGAVARDHINI 40
#define LUT_RES_SCALE_RAGAVARDHINI_SIZE 12
#define LUT_RES_SCALE_GANGEYABHUSHANI 41
#define LUT_RES_SCALE_GANGEYABHUSHANI_SIZE 12
#define LUT_RES_SCALE_VAGADHEESWARI 42
#define LUT_RES_SCALE_VAGADHEESWARI_SIZE 12
#define LUT_RES_SCALE_SHULINI 43
#define LUT_RES_SCALE_SHULINI_SIZE 12
#define LUT_RES_SCALE_CHALANATA 44
#define LUT_RES_SCALE_CHALANATA_SIZE 12
#define LUT_RES_SCALE_SALAGAM 45
#define LUT_RES_SCALE_SALAGAM_SIZE 12
#define LUT_RES_SCALE_JALARNAVAM 46
#define LUT_RES_SCALE_JALARNAVAM_SIZE 12
#define LUT_RES_SCALE_JHALAVARALI 47
#define LUT_RES_SCALE_JHALAVARALI_SIZE 12
#define LUT_RES_SCALE_NAVANEETAM 48
#define LUT_RES_SCALE_NAVANEETAM_SIZE 12
#define LUT_RES_SCALE_PAVANI 49
#define LUT_RES_SCALE_PAVANI_SIZE 12
#define LUT_RES_SCALE_RAGHUPRIYA 50
#define LUT_RES_SCALE_RAGHUPRIYA_SIZE 12
#define LUT_RES_SCALE_GAVAMBHODI 51
#define LUT_RES_SCALE_GAVAMBHODI_SIZE 12
#define LUT_RES_SCALE_BHAVAPRIYA 52
#define LUT_RES_SCALE_BHAVAPRIYA_SIZE 12
#define LUT_RES_SCALE_SHUBHAPANTUVARALI 53
#define LUT_RES_SCALE_SHUBHAPANTUVARALI_SIZE 12
#define LUT_RES_SCALE_SHADVIDAMARGINI 54
#define LUT_RES_SCALE_SHADVIDAMARGINI_SIZE 12
#define LUT_RES_SCALE_SUVARNANGI 55
#define LUT_RES_SCALE_SUVARNANGI_SIZE 12
#define LUT_RES_SCALE_DIVYAMANI 56
#define LUT_RES_SCALE_DIVYAMANI_SIZE 12
#define LUT_RES_SCALE_DHAVALAMBARI 57
#define LUT_RES_SCALE_DHAVALAMBARI_SIZE 12
#define LUT_RES_SCALE_NAMANARAYANI 58
#define LUT_RES_SCALE_NAMANARAYANI_SIZE 12
#define LUT_RES_SCALE_KAMAVARDANI 59
#define LUT_RES_SCALE_KAMAVARDANI_SIZE 12
#define LUT_RES_SCALE_RAMAPRIYA 60
#define LUT_RES_SCALE_RAMAPRIYA_SIZE 12
#define LUT_RES_SCALE_GAMANASHRAMA 61
#define LUT_RES_SCALE_GAMANASHRAMA_SIZE 12
#define LUT_RES_SCALE_VISHWAMBARI 62
#define LUT_RES_SCALE_VISHWAMBARI_SIZE 12
#define LUT_RES_SCALE_SHAMALANGI 63
#define LUT_RES_SCALE_SHAMALANGI_SIZE 12
#define LUT_RES_SCALE_SHANMUKHAPRIYA 64
#define LUT_RES_SCALE_SHANMUKHAPRIYA_SIZE 12
#define LUT_RES_SCALE_SIMHENDRAMADHYAMAM 65
#define LUT_RES_SCALE_SIMHENDRAMADHYAMAM_SIZE 12
#define LUT_RES_SCALE_HEMAVATI 66
#define LUT_RES_SCALE_HEMAVATI_SIZE 12
#define LUT_RES_SCALE_DHARMAVATI 67
#define LUT_RES_SCALE_DHARMAVATI_SIZE 12
#define LUT_RES_SCALE_NEETIMATI 68
#define LUT_RES_SCALE_NEETIMATI_SIZE 12
#define LUT_RES_SCALE_KANTAMANI 69
#define LUT_RES_SCALE_KANTAMANI_SIZE 12
#define LUT_RES_SCALE_RISHABHAPRIYA 70
#define LUT_RES_SCALE_RISHABHAPRIYA_SIZE 12
#define LUT_RES_SCALE_LATANGI 71
#define LUT_RES_SCALE_LATANGI_SIZE 12
#define LUT_RES_SCALE_VACHASPATI 72
#define LUT_RES_SCALE_VACHASPATI_SIZE 12
#define LUT_RES_SCALE_MECHAKALYANI 73
#define LUT_RES_SCALE_MECHAKALYANI_SIZE 12
#define LUT_RES_SCALE_CHITRAMBARI 74
#define LUT_RES_SCALE_CHITRAMBARI_SIZE 12
#define LUT_RES_SCALE_SUCHARITRA 75
#define LUT_RES_SCALE_SUCHARITRA_SIZE 12
#define LUT_RES_SCALE_JYOTISWARUPINI 76
#define LUT_RES_SCALE_JYOTISWARUPINI_SIZE 12
#define LUT_RES_SCALE_DHATUVARDANI 77
#define LUT_RES_SCALE_DHATUVARDANI_SIZE 12
#define LUT_RES_SCALE_NASIKABHUSHANI 78
#define LUT_RES_SCALE_NASIKABHUSHANI_SIZE 12
#define LUT_RES_SCALE_KOSALAM 79
#define LUT_RES_SCALE_KOSALAM_SIZE 12
#define LUT_RES_SCALE_RASIKAPRIYA 80
#define LUT_RES_SCALE_RASIKAPRIYA_SIZE 12
#define LUT_RES_ARPEGGIATOR_PATTERNS 81
#define LUT_RES_ARPEGGIATOR_PATTERNS_SIZE 6
#define WAV_RES_FORMANT_SINE 0
#define WAV_RES_FORMANT_SINE_SIZE 256
#define WAV_RES_FORMANT_SQUARE 1
#define WAV_RES_FORMANT_SQUARE_SIZE 256
#define WAV_RES_BANDLIMITED_PULSE_1 2
#define WAV_RES_BANDLIMITED_PULSE_1_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_2 3
#define WAV_RES_BANDLIMITED_PULSE_2_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_3 4
#define WAV_RES_BANDLIMITED_PULSE_3_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_4 5
#define WAV_RES_BANDLIMITED_PULSE_4_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_5 6
#define WAV_RES_BANDLIMITED_PULSE_5_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_6 7
#define WAV_RES_BANDLIMITED_PULSE_6_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_0 8
#define WAV_RES_BANDLIMITED_SQUARE_0_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_1 9
#define WAV_RES_BANDLIMITED_SQUARE_1_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_2 10
#define WAV_RES_BANDLIMITED_SQUARE_2_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_3 11
#define WAV_RES_BANDLIMITED_SQUARE_3_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_4 12
#define WAV_RES_BANDLIMITED_SQUARE_4_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_5 13
#define WAV_RES_BANDLIMITED_SQUARE_5_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_6 14
#define WAV_RES_BANDLIMITED_SQUARE_6_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_0 15
#define WAV_RES_BANDLIMITED_SAW_0_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_1 16
#define WAV_RES_BANDLIMITED_SAW_1_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_2 17
#define WAV_RES_BANDLIMITED_SAW_2_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_3 18
#define WAV_RES_BANDLIMITED_SAW_3_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_4 19
#define WAV_RES_BANDLIMITED_SAW_4_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_5 20
#define WAV_RES_BANDLIMITED_SAW_5_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_6 21
#define WAV_RES_BANDLIMITED_SAW_6_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_0 22
#define WAV_RES_BANDLIMITED_TRIANGLE_0_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_1 23
#define WAV_RES_BANDLIMITED_TRIANGLE_1_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_2 24
#define WAV_RES_BANDLIMITED_TRIANGLE_2_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_3 25
#define WAV_RES_BANDLIMITED_TRIANGLE_3_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_4 26
#define WAV_RES_BANDLIMITED_TRIANGLE_4_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_5 27
#define WAV_RES_BANDLIMITED_TRIANGLE_5_SIZE 257
#define WAV_RES_BANDLIMITED_TRIANGLE_6 28
#define WAV_RES_BANDLIMITED_TRIANGLE_6_SIZE 257
#define WAV_RES_WAVETABLE 29
#define WAV_RES_WAVETABLE_SIZE 2145
#define WAV_RES_VOWEL_DATA 30
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
