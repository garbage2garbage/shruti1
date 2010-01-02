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


#ifndef HARDWARE_SHRUTI_SHRUTI4_RESOURCES_H_
#define HARDWARE_SHRUTI_SHRUTI4_RESOURCES_H_


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
extern const prog_uint8_t wav_res_bandlimited_square_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_square_6[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_saw_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_1_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_1_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_1_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_1_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_1_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_2_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_2_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_2_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_2_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_2_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_reed_2_6[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_1_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_1_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_1_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_1_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_1_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_1_6[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_2_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_2_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_2_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_2_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_2_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_voice_2_6[] PROGMEM;
extern const prog_uint8_t chr_res_special_characters[] PROGMEM;
#define STR_RES_ATK 0  // atk
#define STR_RES_WV1 1  // wv1
#define STR_RES_RT1 2  // rt1
#define STR_RES_WV2 3  // wv2
#define STR_RES_RT2 4  // rt2
#define STR_RES_SRC 5  // src
#define STR_RES_DST 6  // dst
#define STR_RES_AMT 7  // amt
#define STR_RES_CHN 8  // chn
#define STR_RES_BPM 9  // bpm
#define STR_RES_SWG 10  // swg
#define STR_RES_SHAPE 11  // shape
#define STR_RES_ENV1TVCF 12  // env1~vcf
#define STR_RES_LFO2TVCF 13  // lfo2~vcf
#define STR_RES_RESONANCE 14  // resonance
#define STR_RES_ENVELOPE_1 15  // envelope 1
#define STR_RES_ENVELOPE_2 16  // envelope 2
#define STR_RES_SEQUENCER 17  // sequencer
#define STR_RES_ATTACK 18  // attack
#define STR_RES_DECAY 19  // decay
#define STR_RES_SUSTAIN 20  // sustain
#define STR_RES_RELEASE 21  // release
#define STR_RES_LFO1_WAVE 22  // lfo1 wave
#define STR_RES_LFO1_RATE 23  // lfo1 rate
#define STR_RES_LFO2_WAVE 24  // lfo2 wave
#define STR_RES_LFO2_RATE 25  // lfo2 rate
#define STR_RES_MOD_ 26  // mod.
#define STR_RES_SOURCE 27  // source
#define STR_RES_DEST_ 28  // dest.
#define STR_RES_AMOUNT 29  // amount
#define STR_RES_OCTAVE 30  // octave
#define STR_RES_RAGA 31  // raga
#define STR_RES_MIDI_CHAN 32  // midi chan
#define STR_RES_TEMPO 33  // tempo
#define STR_RES_MIXER 34  // mixer
#define STR_RES_FILTER 35  // filter
#define STR_RES_LFOS 36  // lfos
#define STR_RES_MODULATION 37  // modulation
#define STR_RES_KEYBOARD 38  // keyboard
#define STR_RES_OFF 39  // off
#define STR_RES_ON 40  // on
#define STR_RES_TRI 41  // tri
#define STR_RES_SQR 42  // sqr
#define STR_RES_S_H 43  // s&h
#define STR_RES_3 44  // 
#define STR_RES_LF1 45  // lf1
#define STR_RES_LF2 46  // lf2
#define STR_RES_SEQ 47  // seq
#define STR_RES_ARP 48  // arp
#define STR_RES_WHL 49  // whl
#define STR_RES_BND 50  // bnd
#define STR_RES_AS1 51  // as1
#define STR_RES_AS2 52  // as2
#define STR_RES_CV1 53  // cv1
#define STR_RES_CV2 54  // cv2
#define STR_RES_RND 55  // rnd
#define STR_RES_EN1 56  // en1
#define STR_RES_EN2 57  // en2
#define STR_RES_VEL 58  // vel
#define STR_RES_NOT 59  // not
#define STR_RES_GAT 60  // gat
#define STR_RES_LFO1 61  // lfo1
#define STR_RES_LFO2 62  // lfo2
#define STR_RES_STPSEQ 63  // stpseq
#define STR_RES__ARP 64  //  arp
#define STR_RES_MWHEEL 65  // mwheel
#define STR_RES_BENDER 66  // bender
#define STR_RES_ASSGN1 67  // assgn1
#define STR_RES_ASSGN2 68  // assgn2
#define STR_RES__CV1 69  //  cv1
#define STR_RES__CV2 70  //  cv2
#define STR_RES_RANDOM 71  // random
#define STR_RES_ENV1 72  // env1
#define STR_RES_ENV2 73  // env2
#define STR_RES_VELO 74  // velo
#define STR_RES_NOTE 75  // note
#define STR_RES_GATE 76  // gate
#define STR_RES_P_ORLEANS_21_MN 77  // P.ORLEANS: 21 MN
#define STR_RES_P_BERCY_____S1H 78  // P.BERCY:     +1H
#define STR_RES_LOAD_SAVE_PATCH 79  // load/save patch
#define STR_RES_STEP_SEQUENCER 80  // step sequencer
#define STR_RES_LOAD 81  // load
#define STR_RES_EXIT 82  // exit
#define STR_RES_SAVE 83  // save
#define STR_RES_EXTERN 84  // extern
#define STR_RES_EQUAL 85  // equal
#define STR_RES_JUST 86  // just
#define STR_RES_PYTHAG 87  // pythag
#define STR_RES_1_4_EB 88  // 1/4 eb
#define STR_RES_1_4_E 89  // 1/4 e
#define STR_RES_1_4_EA 90  // 1/4 ea
#define STR_RES_KANAKA 91  // kanaka
#define STR_RES_RATNAN 92  // ratnan
#define STR_RES_GANAMU 93  // ganamu
#define STR_RES_VANASP 94  // vanasp
#define STR_RES_MANAVA 95  // manava
#define STR_RES_TANARU 96  // tanaru
#define STR_RES_SENAVA 97  // senava
#define STR_RES_HANUMA 98  // hanuma
#define STR_RES_DHENUK 99  // dhenuk
#define STR_RES_NATAKA 100  // nataka
#define STR_RES_KOKILA 101  // kokila
#define STR_RES_RUPAVA 102  // rupava
#define STR_RES_GAYAKA 103  // gayaka
#define STR_RES_VAKULA 104  // vakula
#define STR_RES_MAYAMA 105  // mayama
#define STR_RES_CHAKRA 106  // chakra
#define STR_RES_SURYAK 107  // suryak
#define STR_RES_HATAKA 108  // hataka
#define STR_RES_JHANKA 109  // jhanka
#define STR_RES_NATABH 110  // natabh
#define STR_RES_KEERAV 111  // keerav
#define STR_RES_KHARAH 112  // kharah
#define STR_RES_GOURIM 113  // gourim
#define STR_RES_VARUNA 114  // varuna
#define STR_RES_MARARA 115  // marara
#define STR_RES_CHARUK 116  // charuk
#define STR_RES_SARASA 117  // sarasa
#define STR_RES_HARIKA 118  // harika
#define STR_RES_DHEERA 119  // dheera
#define STR_RES_NAGANA 120  // nagana
#define STR_RES_YAGAPR 121  // yagapr
#define STR_RES_RAGAVA 122  // ragava
#define STR_RES_GANGEY 123  // gangey
#define STR_RES_VAGADH 124  // vagadh
#define STR_RES_SHULIN 125  // shulin
#define STR_RES_CHALAN 126  // chalan
#define STR_RES_SALAGA 127  // salaga
#define STR_RES_JALARN 128  // jalarn
#define STR_RES_JHALAV 129  // jhalav
#define STR_RES_NAVANE 130  // navane
#define STR_RES_PAVANI 131  // pavani
#define STR_RES_RAGHUP 132  // raghup
#define STR_RES_GAVAMB 133  // gavamb
#define STR_RES_BHAVAP 134  // bhavap
#define STR_RES_SHUBHA 135  // shubha
#define STR_RES_SHADVI 136  // shadvi
#define STR_RES_SUVARN 137  // suvarn
#define STR_RES_DIVYAM 138  // divyam
#define STR_RES_DHAVAL 139  // dhaval
#define STR_RES_NAMANA 140  // namana
#define STR_RES_KAMAVA 141  // kamava
#define STR_RES_RAMAPR 142  // ramapr
#define STR_RES_GAMANA 143  // gamana
#define STR_RES_VISHWA 144  // vishwa
#define STR_RES_SHAMAL 145  // shamal
#define STR_RES_SHANMU 146  // shanmu
#define STR_RES_SIMHEN 147  // simhen
#define STR_RES_HEMAVA 148  // hemava
#define STR_RES_DHARMA 149  // dharma
#define STR_RES_NEETIM 150  // neetim
#define STR_RES_KANTAM 151  // kantam
#define STR_RES_RISHAB 152  // rishab
#define STR_RES_LATANG 153  // latang
#define STR_RES_VACHAS 154  // vachas
#define STR_RES_MECHAK 155  // mechak
#define STR_RES_CHITRA 156  // chitra
#define STR_RES_SUCHAR 157  // suchar
#define STR_RES_JYOTIS 158  // jyotis
#define STR_RES_DHATUV 159  // dhatuv
#define STR_RES_NASIKA 160  // nasika
#define STR_RES_KOSALA 161  // kosala
#define STR_RES_RASIKA 162  // rasika
#define STR_RES_16_MIX 163  // 16' mix
#define STR_RES__8_MIX 164  //  8' mix
#define STR_RES__4_MIX 165  //  4' mix
#define STR_RES__2_MIX 166  //  2' mix
#define STR_RES_SH1 167  // sh1
#define STR_RES_SHAPE_1 168  // shape 1
#define STR_RES_SH2 169  // sh2
#define STR_RES_SHAPE_2 170  // shape 2
#define STR_RES_BALANCE 171  // balance
#define STR_RES_SINE 172  // sine
#define STR_RES_SQUARE 173  // square
#define STR_RES_SAW 174  // saw
#define STR_RES_1_REED 175  // 1 reed
#define STR_RES_2_REED 176  // 2 reed
#define STR_RES_OOH 177  // ooh
#define STR_RES_AAH 178  // aah
#define STR_RES_NONE 179  // none
#define STR_RES_DRAWBR 180  // drawbr
#define STR_RES_SINE_ 181  // sine 
#define STR_RES_SQUARE_ 182  // square 
#define STR_RES_SAW_ 183  // saw 
#define STR_RES_1_REED_ 184  // 1 reed 
#define STR_RES_2_REED_ 185  // 2 reed 
#define STR_RES_OOH_ 186  // ooh 
#define STR_RES_AAH_ 187  // aah 
#define STR_RES_DSAW 188  // dsaw
#define STR_RES_DSQR 189  // dsqr
#define STR_RES_DRAWBARS_1 190  // drawbars 1
#define STR_RES_DRAWBARS_2 191  // drawbars 2
#define STR_RES_OSCILLATOR 192  // oscillator
#define STR_RES_VOICES 193  // voices
#define STR_RES_1 194  // 1
#define STR_RES_2 195  // 2
#define STR_RES_4 196  // 4
#define STR_RES_1_UNI 197  // 1 uni
#define STR_RES_2_UNI 198  // 2 uni
#define STR_RES_4_UNI 199  // 4 uni
#define STR_RES_LEGATO 200  // legato
#define STR_RES_RETRIGGER 201  // retrigger
#define STR_RES_CUT 202  // cut
#define STR_RES_VCA 203  // vca
#define STR_RES_5 204  // 
#define STR_RES_VIB 205  // vib
#define STR_RES_SLP 206  // slp
#define STR_RES_PWM 207  // pwm
#define STR_RES_RES 208  // res
#define STR_RES_CUTOFF 209  // cutoff
#define STR_RES__VCA 210  //  vca
#define STR_RES_SLOP 211  // slop
#define STR_RES_PITCH 212  // pitch
#define STR_RES_VIBRTO 213  // vibrto
#define STR_RES__PWM 214  //  pwm
#define STR_RES_RESO 215  // reso
#define STR_RES_MUTABLE 216  // mutable
#define STR_RES_INSTRUMENTS_674 217  // instruments -4
#define LUT_RES_LFO_INCREMENTS 0
#define LUT_RES_LFO_INCREMENTS_SIZE 128
#define LUT_RES_ENV_PORTAMENTO_INCREMENTS 1
#define LUT_RES_ENV_PORTAMENTO_INCREMENTS_SIZE 128
#define LUT_RES_OSCILLATOR_INCREMENTS 2
#define LUT_RES_OSCILLATOR_INCREMENTS_SIZE 768
#define LUT_RES_SCALE_JUST 3
#define LUT_RES_SCALE_JUST_SIZE 12
#define LUT_RES_SCALE_PYTHAGOREAN 4
#define LUT_RES_SCALE_PYTHAGOREAN_SIZE 12
#define LUT_RES_SCALE_1_4_EB 5
#define LUT_RES_SCALE_1_4_EB_SIZE 12
#define LUT_RES_SCALE_1_4_E 6
#define LUT_RES_SCALE_1_4_E_SIZE 12
#define LUT_RES_SCALE_1_4_EA 7
#define LUT_RES_SCALE_1_4_EA_SIZE 12
#define LUT_RES_SCALE_KANAKANGI 8
#define LUT_RES_SCALE_KANAKANGI_SIZE 12
#define LUT_RES_SCALE_RATNANGI 9
#define LUT_RES_SCALE_RATNANGI_SIZE 12
#define LUT_RES_SCALE_GANAMURTI 10
#define LUT_RES_SCALE_GANAMURTI_SIZE 12
#define LUT_RES_SCALE_VANASPATI 11
#define LUT_RES_SCALE_VANASPATI_SIZE 12
#define LUT_RES_SCALE_MANAVATI 12
#define LUT_RES_SCALE_MANAVATI_SIZE 12
#define LUT_RES_SCALE_TANARUPI 13
#define LUT_RES_SCALE_TANARUPI_SIZE 12
#define LUT_RES_SCALE_SENAVATI 14
#define LUT_RES_SCALE_SENAVATI_SIZE 12
#define LUT_RES_SCALE_HANUMATODI 15
#define LUT_RES_SCALE_HANUMATODI_SIZE 12
#define LUT_RES_SCALE_DHENUKA 16
#define LUT_RES_SCALE_DHENUKA_SIZE 12
#define LUT_RES_SCALE_NATAKAPRIYA 17
#define LUT_RES_SCALE_NATAKAPRIYA_SIZE 12
#define LUT_RES_SCALE_KOKILAPRIYA 18
#define LUT_RES_SCALE_KOKILAPRIYA_SIZE 12
#define LUT_RES_SCALE_RUPAVATI 19
#define LUT_RES_SCALE_RUPAVATI_SIZE 12
#define LUT_RES_SCALE_GAYAKAPRIYA 20
#define LUT_RES_SCALE_GAYAKAPRIYA_SIZE 12
#define LUT_RES_SCALE_VAKULABHARANAM 21
#define LUT_RES_SCALE_VAKULABHARANAM_SIZE 12
#define LUT_RES_SCALE_MAYAMALAVAGOWLA 22
#define LUT_RES_SCALE_MAYAMALAVAGOWLA_SIZE 12
#define LUT_RES_SCALE_CHAKRAVAKAM 23
#define LUT_RES_SCALE_CHAKRAVAKAM_SIZE 12
#define LUT_RES_SCALE_SURYAKANTAM 24
#define LUT_RES_SCALE_SURYAKANTAM_SIZE 12
#define LUT_RES_SCALE_HATAKAMBARI 25
#define LUT_RES_SCALE_HATAKAMBARI_SIZE 12
#define LUT_RES_SCALE_JHANKARADHWANI 26
#define LUT_RES_SCALE_JHANKARADHWANI_SIZE 12
#define LUT_RES_SCALE_NATABHAIRAVI 27
#define LUT_RES_SCALE_NATABHAIRAVI_SIZE 12
#define LUT_RES_SCALE_KEERAVANI 28
#define LUT_RES_SCALE_KEERAVANI_SIZE 12
#define LUT_RES_SCALE_KHARAHARAPRIYA 29
#define LUT_RES_SCALE_KHARAHARAPRIYA_SIZE 12
#define LUT_RES_SCALE_GOURIMANOHARI 30
#define LUT_RES_SCALE_GOURIMANOHARI_SIZE 12
#define LUT_RES_SCALE_VARUNAPRIYA 31
#define LUT_RES_SCALE_VARUNAPRIYA_SIZE 12
#define LUT_RES_SCALE_MARARANJANI 32
#define LUT_RES_SCALE_MARARANJANI_SIZE 12
#define LUT_RES_SCALE_CHARUKESI 33
#define LUT_RES_SCALE_CHARUKESI_SIZE 12
#define LUT_RES_SCALE_SARASANGI 34
#define LUT_RES_SCALE_SARASANGI_SIZE 12
#define LUT_RES_SCALE_HARIKAMBHOJI 35
#define LUT_RES_SCALE_HARIKAMBHOJI_SIZE 12
#define LUT_RES_SCALE_DHEERASANKARABHARANAM 36
#define LUT_RES_SCALE_DHEERASANKARABHARANAM_SIZE 12
#define LUT_RES_SCALE_NAGANANDINI 37
#define LUT_RES_SCALE_NAGANANDINI_SIZE 12
#define LUT_RES_SCALE_YAGAPRIYA 38
#define LUT_RES_SCALE_YAGAPRIYA_SIZE 12
#define LUT_RES_SCALE_RAGAVARDHINI 39
#define LUT_RES_SCALE_RAGAVARDHINI_SIZE 12
#define LUT_RES_SCALE_GANGEYABHUSHANI 40
#define LUT_RES_SCALE_GANGEYABHUSHANI_SIZE 12
#define LUT_RES_SCALE_VAGADHEESWARI 41
#define LUT_RES_SCALE_VAGADHEESWARI_SIZE 12
#define LUT_RES_SCALE_SHULINI 42
#define LUT_RES_SCALE_SHULINI_SIZE 12
#define LUT_RES_SCALE_CHALANATA 43
#define LUT_RES_SCALE_CHALANATA_SIZE 12
#define LUT_RES_SCALE_SALAGAM 44
#define LUT_RES_SCALE_SALAGAM_SIZE 12
#define LUT_RES_SCALE_JALARNAVAM 45
#define LUT_RES_SCALE_JALARNAVAM_SIZE 12
#define LUT_RES_SCALE_JHALAVARALI 46
#define LUT_RES_SCALE_JHALAVARALI_SIZE 12
#define LUT_RES_SCALE_NAVANEETAM 47
#define LUT_RES_SCALE_NAVANEETAM_SIZE 12
#define LUT_RES_SCALE_PAVANI 48
#define LUT_RES_SCALE_PAVANI_SIZE 12
#define LUT_RES_SCALE_RAGHUPRIYA 49
#define LUT_RES_SCALE_RAGHUPRIYA_SIZE 12
#define LUT_RES_SCALE_GAVAMBHODI 50
#define LUT_RES_SCALE_GAVAMBHODI_SIZE 12
#define LUT_RES_SCALE_BHAVAPRIYA 51
#define LUT_RES_SCALE_BHAVAPRIYA_SIZE 12
#define LUT_RES_SCALE_SHUBHAPANTUVARALI 52
#define LUT_RES_SCALE_SHUBHAPANTUVARALI_SIZE 12
#define LUT_RES_SCALE_SHADVIDAMARGINI 53
#define LUT_RES_SCALE_SHADVIDAMARGINI_SIZE 12
#define LUT_RES_SCALE_SUVARNANGI 54
#define LUT_RES_SCALE_SUVARNANGI_SIZE 12
#define LUT_RES_SCALE_DIVYAMANI 55
#define LUT_RES_SCALE_DIVYAMANI_SIZE 12
#define LUT_RES_SCALE_DHAVALAMBARI 56
#define LUT_RES_SCALE_DHAVALAMBARI_SIZE 12
#define LUT_RES_SCALE_NAMANARAYANI 57
#define LUT_RES_SCALE_NAMANARAYANI_SIZE 12
#define LUT_RES_SCALE_KAMAVARDANI 58
#define LUT_RES_SCALE_KAMAVARDANI_SIZE 12
#define LUT_RES_SCALE_RAMAPRIYA 59
#define LUT_RES_SCALE_RAMAPRIYA_SIZE 12
#define LUT_RES_SCALE_GAMANASHRAMA 60
#define LUT_RES_SCALE_GAMANASHRAMA_SIZE 12
#define LUT_RES_SCALE_VISHWAMBARI 61
#define LUT_RES_SCALE_VISHWAMBARI_SIZE 12
#define LUT_RES_SCALE_SHAMALANGI 62
#define LUT_RES_SCALE_SHAMALANGI_SIZE 12
#define LUT_RES_SCALE_SHANMUKHAPRIYA 63
#define LUT_RES_SCALE_SHANMUKHAPRIYA_SIZE 12
#define LUT_RES_SCALE_SIMHENDRAMADHYAMAM 64
#define LUT_RES_SCALE_SIMHENDRAMADHYAMAM_SIZE 12
#define LUT_RES_SCALE_HEMAVATI 65
#define LUT_RES_SCALE_HEMAVATI_SIZE 12
#define LUT_RES_SCALE_DHARMAVATI 66
#define LUT_RES_SCALE_DHARMAVATI_SIZE 12
#define LUT_RES_SCALE_NEETIMATI 67
#define LUT_RES_SCALE_NEETIMATI_SIZE 12
#define LUT_RES_SCALE_KANTAMANI 68
#define LUT_RES_SCALE_KANTAMANI_SIZE 12
#define LUT_RES_SCALE_RISHABHAPRIYA 69
#define LUT_RES_SCALE_RISHABHAPRIYA_SIZE 12
#define LUT_RES_SCALE_LATANGI 70
#define LUT_RES_SCALE_LATANGI_SIZE 12
#define LUT_RES_SCALE_VACHASPATI 71
#define LUT_RES_SCALE_VACHASPATI_SIZE 12
#define LUT_RES_SCALE_MECHAKALYANI 72
#define LUT_RES_SCALE_MECHAKALYANI_SIZE 12
#define LUT_RES_SCALE_CHITRAMBARI 73
#define LUT_RES_SCALE_CHITRAMBARI_SIZE 12
#define LUT_RES_SCALE_SUCHARITRA 74
#define LUT_RES_SCALE_SUCHARITRA_SIZE 12
#define LUT_RES_SCALE_JYOTISWARUPINI 75
#define LUT_RES_SCALE_JYOTISWARUPINI_SIZE 12
#define LUT_RES_SCALE_DHATUVARDANI 76
#define LUT_RES_SCALE_DHATUVARDANI_SIZE 12
#define LUT_RES_SCALE_NASIKABHUSHANI 77
#define LUT_RES_SCALE_NASIKABHUSHANI_SIZE 12
#define LUT_RES_SCALE_KOSALAM 78
#define LUT_RES_SCALE_KOSALAM_SIZE 12
#define LUT_RES_SCALE_RASIKAPRIYA 79
#define LUT_RES_SCALE_RASIKAPRIYA_SIZE 12
#define WAV_RES_BANDLIMITED_SQUARE_1 0
#define WAV_RES_BANDLIMITED_SQUARE_1_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_2 1
#define WAV_RES_BANDLIMITED_SQUARE_2_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_3 2
#define WAV_RES_BANDLIMITED_SQUARE_3_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_4 3
#define WAV_RES_BANDLIMITED_SQUARE_4_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_5 4
#define WAV_RES_BANDLIMITED_SQUARE_5_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_6 5
#define WAV_RES_BANDLIMITED_SQUARE_6_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_1 6
#define WAV_RES_BANDLIMITED_SAW_1_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_2 7
#define WAV_RES_BANDLIMITED_SAW_2_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_3 8
#define WAV_RES_BANDLIMITED_SAW_3_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_4 9
#define WAV_RES_BANDLIMITED_SAW_4_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_5 10
#define WAV_RES_BANDLIMITED_SAW_5_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_6 11
#define WAV_RES_BANDLIMITED_SAW_6_SIZE 257
#define WAV_RES_BANDLIMITED_REED_1_1 12
#define WAV_RES_BANDLIMITED_REED_1_1_SIZE 257
#define WAV_RES_BANDLIMITED_REED_1_2 13
#define WAV_RES_BANDLIMITED_REED_1_2_SIZE 257
#define WAV_RES_BANDLIMITED_REED_1_3 14
#define WAV_RES_BANDLIMITED_REED_1_3_SIZE 257
#define WAV_RES_BANDLIMITED_REED_1_4 15
#define WAV_RES_BANDLIMITED_REED_1_4_SIZE 257
#define WAV_RES_BANDLIMITED_REED_1_5 16
#define WAV_RES_BANDLIMITED_REED_1_5_SIZE 257
#define WAV_RES_BANDLIMITED_REED_1_6 17
#define WAV_RES_BANDLIMITED_REED_1_6_SIZE 257
#define WAV_RES_BANDLIMITED_REED_2_1 18
#define WAV_RES_BANDLIMITED_REED_2_1_SIZE 257
#define WAV_RES_BANDLIMITED_REED_2_2 19
#define WAV_RES_BANDLIMITED_REED_2_2_SIZE 257
#define WAV_RES_BANDLIMITED_REED_2_3 20
#define WAV_RES_BANDLIMITED_REED_2_3_SIZE 257
#define WAV_RES_BANDLIMITED_REED_2_4 21
#define WAV_RES_BANDLIMITED_REED_2_4_SIZE 257
#define WAV_RES_BANDLIMITED_REED_2_5 22
#define WAV_RES_BANDLIMITED_REED_2_5_SIZE 257
#define WAV_RES_BANDLIMITED_REED_2_6 23
#define WAV_RES_BANDLIMITED_REED_2_6_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_1_1 24
#define WAV_RES_BANDLIMITED_VOICE_1_1_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_1_2 25
#define WAV_RES_BANDLIMITED_VOICE_1_2_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_1_3 26
#define WAV_RES_BANDLIMITED_VOICE_1_3_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_1_4 27
#define WAV_RES_BANDLIMITED_VOICE_1_4_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_1_5 28
#define WAV_RES_BANDLIMITED_VOICE_1_5_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_1_6 29
#define WAV_RES_BANDLIMITED_VOICE_1_6_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_2_1 30
#define WAV_RES_BANDLIMITED_VOICE_2_1_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_2_2 31
#define WAV_RES_BANDLIMITED_VOICE_2_2_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_2_3 32
#define WAV_RES_BANDLIMITED_VOICE_2_3_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_2_4 33
#define WAV_RES_BANDLIMITED_VOICE_2_4_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_2_5 34
#define WAV_RES_BANDLIMITED_VOICE_2_5_SIZE 257
#define WAV_RES_BANDLIMITED_VOICE_2_6 35
#define WAV_RES_BANDLIMITED_VOICE_2_6_SIZE 257
#define CHR_RES_SPECIAL_CHARACTERS 0
#define CHR_RES_SPECIAL_CHARACTERS_SIZE 64
typedef hardware_resources::ResourcesManager<
    ResourceId,
    hardware_resources::ResourcesTables<
        string_table,
        lookup_table_table> > ResourcesManager; 

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_SHRUTI4_RESOURCES_H_
