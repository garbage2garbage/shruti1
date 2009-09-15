// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
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
extern const prog_uint8_t wav_res_bandlimited_saw_6[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_0[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_1[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_2[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_3[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_4[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_5[] PROGMEM;
extern const prog_uint8_t wav_res_bandlimited_triangle_6[] PROGMEM;
extern const prog_uint8_t wav_res_wavetable[] PROGMEM;
extern const prog_uint8_t wav_res_speech_data[] PROGMEM;
extern const prog_uint8_t chr_res_special_characters[] PROGMEM;
#define STR_RES_ALG 0  // alg
#define STR_RES_PRM 1  // prm
#define STR_RES_RNG 2  // rng
#define STR_RES_OP 3  // op
#define STR_RES_TUN 4  // tun
#define STR_RES_ENV 5  // env
#define STR_RES_LFO 6  // lfo
#define STR_RES_ATK 7  // atk
#define STR_RES_DEC 8  // dec
#define STR_RES_SUS 9  // sus
#define STR_RES_REL 10  // rel
#define STR_RES_WV1 11  // wv1
#define STR_RES_RT1 12  // rt1
#define STR_RES_WV2 13  // wv2
#define STR_RES_RT2 14  // rt2
#define STR_RES_MOD 15  // mod
#define STR_RES_SRC 16  // src
#define STR_RES_DST 17  // dst
#define STR_RES_AMT 18  // amt
#define STR_RES_OCT 19  // oct
#define STR_RES_RAG 20  // rag
#define STR_RES_PRT 21  // prt
#define STR_RES_CHN 22  // chn
#define STR_RES_BPM 23  // bpm
#define STR_RES_PAT 24  // pat
#define STR_RES_SWG 25  // swg
#define STR_RES_ALGORITHM 26  // algorithm
#define STR_RES_PARAMETER 27  // parameter
#define STR_RES_RANGE 28  // range
#define STR_RES_OPERATOR 29  // operator
#define STR_RES_DETUNE 30  // detune
#define STR_RES_ENVTVCF 31  // env~vcf
#define STR_RES_LFO2TVCF 32  // lfo2~vcf
#define STR_RES_OSC_BAL 33  // osc bal
#define STR_RES_SUB_OSC_ 34  // sub osc.
#define STR_RES_RESONANCE 35  // resonance
#define STR_RES_ENVELOPE_1 36  // envelope 1
#define STR_RES_ENVELOPE_2 37  // envelope 2
#define STR_RES_SEQUENCER 38  // sequencer
#define STR_RES_ATTACK 39  // attack
#define STR_RES_DECAY 40  // decay
#define STR_RES_SUSTAIN 41  // sustain
#define STR_RES_RELEASE 42  // release
#define STR_RES_LFO1_WAVE 43  // lfo1 wave
#define STR_RES_LFO1_RATE 44  // lfo1 rate
#define STR_RES_LFO2_WAVE 45  // lfo2 wave
#define STR_RES_LFO2_RATE 46  // lfo2 rate
#define STR_RES_MOD_ 47  // mod.
#define STR_RES_SOURCE 48  // source
#define STR_RES_DEST_ 49  // dest.
#define STR_RES_AMOUNT 50  // amount
#define STR_RES_OCTAVE 51  // octave
#define STR_RES_RAGA 52  // raga
#define STR_RES_PORTA 53  // porta
#define STR_RES_MIDI_CHAN 54  // midi chan
#define STR_RES_TEMPO 55  // tempo
#define STR_RES_PATTERN 56  // pattern
#define STR_RES_SWING 57  // swing
#define STR_RES_OSCILLATOR_1 58  // oscillator 1
#define STR_RES_OSCILLATOR_2 59  // oscillator 2
#define STR_RES_MIXER 60  // mixer
#define STR_RES_FILTER 61  // filter
#define STR_RES_LFOS 62  // lfos
#define STR_RES_MODULATION 63  // modulation
#define STR_RES_KEYBOARD 64  // keyboard
#define STR_RES_ARPEGGIO 65  // arpeggio
#define STR_RES_OFF 66  // off
#define STR_RES_ON 67  // on
#define STR_RES_TRI 68  // tri
#define STR_RES_SQR 69  // sqr
#define STR_RES_S_H 70  // s&h
#define STR_RES_RAMP 71  // ramp
#define STR_RES_BLIT 72  // blit
#define STR_RES_SAW 73  // saw
#define STR_RES_SQUARE 74  // square
#define STR_RES_TRIANG 75  // triang
#define STR_RES_CZ 76  // cz
#define STR_RES_FM 77  // fm
#define STR_RES_8BITS 78  // 8bits
#define STR_RES_VOICE 79  // voice
#define STR_RES_TABLE 80  // table
#define STR_RES_SWEEP 81  // sweep
#define STR_RES_1S2 82  // 1+2
#define STR_RES_1_2 83  // 1>2
#define STR_RES_1P2 84  // 1*2
#define STR_RES_LF1 85  // lf1
#define STR_RES_LF2 86  // lf2
#define STR_RES_SEQ 87  // seq
#define STR_RES_ARP 88  // arp
#define STR_RES_WHL 89  // whl
#define STR_RES_BND 90  // bnd
#define STR_RES_AS1 91  // as1
#define STR_RES_AS2 92  // as2
#define STR_RES_CV1 93  // cv1
#define STR_RES_CV2 94  // cv2
#define STR_RES_RND 95  // rnd
#define STR_RES_EN1 96  // en1
#define STR_RES_EN2 97  // en2
#define STR_RES_VEL 98  // vel
#define STR_RES_NOT 99  // not
#define STR_RES_GAT 100  // gat
#define STR_RES_LFO1 101  // lfo1
#define STR_RES_LFO2 102  // lfo2
#define STR_RES_STPSEQ 103  // stpseq
#define STR_RES__ARP 104  //  arp
#define STR_RES_MWHEEL 105  // mwheel
#define STR_RES_BENDER 106  // bender
#define STR_RES_ASSGN1 107  // assgn1
#define STR_RES_ASSGN2 108  // assgn2
#define STR_RES__CV1 109  //  cv1
#define STR_RES__CV2 110  //  cv2
#define STR_RES_RANDOM 111  // random
#define STR_RES_ENV1 112  // env1
#define STR_RES_ENV2 113  // env2
#define STR_RES_VELO 114  // velo
#define STR_RES_NOTE 115  // note
#define STR_RES_GATE 116  // gate
#define STR_RES_CUT 117  // cut
#define STR_RES_VCA 118  // vca
#define STR_RES_PW1 119  // pw1
#define STR_RES_PW2 120  // pw2
#define STR_RES_41 121  // 1
#define STR_RES_42 122  // 2
#define STR_RES_4 123  // 
#define STR_RES_MIX 124  // mix
#define STR_RES_NOI 125  // noi
#define STR_RES_SUB 126  // sub
#define STR_RES_RES 127  // res
#define STR_RES_CUTOFF 128  // cutoff
#define STR_RES__VCA 129  //  vca
#define STR_RES_PWM1 130  // pwm1
#define STR_RES_PWM2 131  // pwm2
#define STR_RES_OSC1 132  // osc1
#define STR_RES_OSC2 133  // osc2
#define STR_RES_OSC 134  // osc
#define STR_RES__MIX 135  //  mix
#define STR_RES_NOISE 136  // noise
#define STR_RES_SUBOSC 137  // subosc
#define STR_RES_RESO 138  // reso
#define STR_RES_51 139  // 1
#define STR_RES_61 140  // 1
#define STR_RES_71 141  // 1
#define STR_RES__1 142  // ?1
#define STR_RES_52 143  // 2
#define STR_RES_62 144  // 2
#define STR_RES_72 145  // 2
#define STR_RES__2 146  // ?2
#define STR_RES_53 147  // 3
#define STR_RES_63 148  // 3
#define STR_RES_73 149  // 3
#define STR_RES__3 150  // ?3
#define STR_RES_54 151  // 4
#define STR_RES_64 152  // 4
#define STR_RES_74 153  // 4
#define STR_RES__4 154  // ?4
#define STR_RES_55 155  // 5
#define STR_RES_65 156  // 5
#define STR_RES_75 157  // 5
#define STR_RES__5 158  // ?5
#define STR_RES_56 159  // 6
#define STR_RES_66 160  // 6
#define STR_RES_76 161  // 6
#define STR_RES__6 162  // ?6
#define STR_RES___MUTABLE 163  // > mutable
#define STR_RES___INSTRUMENTS___ 164  //   instruments  <
#define STR_RES_LOAD_SAVE_PATCH 165  // load/save patch
#define STR_RES_STEP_SEQUENCER 166  // step sequencer
#define STR_RES_LOAD 167  // load
#define STR_RES_EXIT 168  // exit
#define STR_RES_SAVE 169  // save
#define STR_RES_EXTERN 170  // extern
#define STR_RES_EQUAL 171  // equal
#define STR_RES_JUST 172  // just
#define STR_RES_PYTHAG 173  // pythag
#define STR_RES_1_4_EB 174  // 1/4 eb
#define STR_RES_1_4_E 175  // 1/4 e
#define STR_RES_1_4_EA 176  // 1/4 ea
#define STR_RES_KANAKA 177  // kanaka
#define STR_RES_RATNAN 178  // ratnan
#define STR_RES_GANAMU 179  // ganamu
#define STR_RES_VANASP 180  // vanasp
#define STR_RES_MANAVA 181  // manava
#define STR_RES_TANARU 182  // tanaru
#define STR_RES_SENAVA 183  // senava
#define STR_RES_HANUMA 184  // hanuma
#define STR_RES_DHENUK 185  // dhenuk
#define STR_RES_NATAKA 186  // nataka
#define STR_RES_KOKILA 187  // kokila
#define STR_RES_RUPAVA 188  // rupava
#define STR_RES_GAYAKA 189  // gayaka
#define STR_RES_VAKULA 190  // vakula
#define STR_RES_MAYAMA 191  // mayama
#define STR_RES_CHAKRA 192  // chakra
#define STR_RES_SURYAK 193  // suryak
#define STR_RES_HATAKA 194  // hataka
#define STR_RES_JHANKA 195  // jhanka
#define STR_RES_NATABH 196  // natabh
#define STR_RES_KEERAV 197  // keerav
#define STR_RES_KHARAH 198  // kharah
#define STR_RES_GOURIM 199  // gourim
#define STR_RES_VARUNA 200  // varuna
#define STR_RES_MARARA 201  // marara
#define STR_RES_CHARUK 202  // charuk
#define STR_RES_SARASA 203  // sarasa
#define STR_RES_HARIKA 204  // harika
#define STR_RES_DHEERA 205  // dheera
#define STR_RES_NAGANA 206  // nagana
#define STR_RES_YAGAPR 207  // yagapr
#define STR_RES_RAGAVA 208  // ragava
#define STR_RES_GANGEY 209  // gangey
#define STR_RES_VAGADH 210  // vagadh
#define STR_RES_SHULIN 211  // shulin
#define STR_RES_CHALAN 212  // chalan
#define STR_RES_SALAGA 213  // salaga
#define STR_RES_JALARN 214  // jalarn
#define STR_RES_JHALAV 215  // jhalav
#define STR_RES_NAVANE 216  // navane
#define STR_RES_PAVANI 217  // pavani
#define STR_RES_RAGHUP 218  // raghup
#define STR_RES_GAVAMB 219  // gavamb
#define STR_RES_BHAVAP 220  // bhavap
#define STR_RES_SHUBHA 221  // shubha
#define STR_RES_SHADVI 222  // shadvi
#define STR_RES_SUVARN 223  // suvarn
#define STR_RES_DIVYAM 224  // divyam
#define STR_RES_DHAVAL 225  // dhaval
#define STR_RES_NAMANA 226  // namana
#define STR_RES_KAMAVA 227  // kamava
#define STR_RES_RAMAPR 228  // ramapr
#define STR_RES_GAMANA 229  // gamana
#define STR_RES_VISHWA 230  // vishwa
#define STR_RES_SHAMAL 231  // shamal
#define STR_RES_SHANMU 232  // shanmu
#define STR_RES_SIMHEN 233  // simhen
#define STR_RES_HEMAVA 234  // hemava
#define STR_RES_DHARMA 235  // dharma
#define STR_RES_NEETIM 236  // neetim
#define STR_RES_KANTAM 237  // kantam
#define STR_RES_RISHAB 238  // rishab
#define STR_RES_LATANG 239  // latang
#define STR_RES_VACHAS 240  // vachas
#define STR_RES_MECHAK 241  // mechak
#define STR_RES_CHITRA 242  // chitra
#define STR_RES_SUCHAR 243  // suchar
#define STR_RES_JYOTIS 244  // jyotis
#define STR_RES_DHATUV 245  // dhatuv
#define STR_RES_NASIKA 246  // nasika
#define STR_RES_KOSALA 247  // kosala
#define STR_RES_RASIKA 248  // rasika
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
#define LUT_RES_ARPEGGIATOR_PATTERNS 80
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
#define WAV_RES_SPEECH_DATA 30
#define WAV_RES_SPEECH_DATA_SIZE 45
#define CHR_RES_SPECIAL_CHARACTERS 0
#define CHR_RES_SPECIAL_CHARACTERS_SIZE 64
typedef hardware_resources::ResourcesManager<
    ResourceId,
    hardware_resources::ResourcesTables<
        string_table,
        lookup_table_table> > ResourcesManager; 

}  // namespace hardware_shruti

#endif  // HARDWARE_SHRUTI_RESOURCES_H_
