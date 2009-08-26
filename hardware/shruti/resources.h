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
extern const prog_uint8_t wav_res_sine[] PROGMEM;
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
#define STR_RES_LFO 5  // lfo
#define STR_RES_ATK 6  // atk
#define STR_RES_DEC 7  // dec
#define STR_RES_SUS 8  // sus
#define STR_RES_REL 9  // rel
#define STR_RES_WV1 10  // wv1
#define STR_RES_RT1 11  // rt1
#define STR_RES_WV2 12  // wv2
#define STR_RES_RT2 13  // rt2
#define STR_RES_MOD 14  // mod
#define STR_RES_SRC 15  // src
#define STR_RES_DST 16  // dst
#define STR_RES_AMT 17  // amt
#define STR_RES_OCT 18  // oct
#define STR_RES_RAG 19  // rag
#define STR_RES_PRT 20  // prt
#define STR_RES_CHN 21  // chn
#define STR_RES_BPM 22  // bpm
#define STR_RES_PAT 23  // pat
#define STR_RES_SWG 24  // swg
#define STR_RES_ALGORITHM 25  // algorithm
#define STR_RES_PARAMETER 26  // parameter
#define STR_RES_RANGE 27  // range
#define STR_RES_OPERATOR 28  // operator
#define STR_RES_DETUNE 29  // detune
#define STR_RES_ENVTVCF 30  // env~vcf
#define STR_RES_LFO2TVCF 31  // lfo2~vcf
#define STR_RES_OSC_BAL 32  // osc bal
#define STR_RES_SUB_OSC_ 33  // sub osc.
#define STR_RES_RESONANCE 34  // resonance
#define STR_RES_ENVELOPE 35  // envelope
#define STR_RES_SEQUENCER 36  // sequencer
#define STR_RES_ATTACK 37  // attack
#define STR_RES_DECAY 38  // decay
#define STR_RES_SUSTAIN 39  // sustain
#define STR_RES_RELEASE 40  // release
#define STR_RES_LFO1_WAVE 41  // lfo1 wave
#define STR_RES_LFO1_RATE 42  // lfo1 rate
#define STR_RES_LFO2_WAVE 43  // lfo2 wave
#define STR_RES_LFO2_RATE 44  // lfo2 rate
#define STR_RES_MOD_ 45  // mod.
#define STR_RES_SOURCE 46  // source
#define STR_RES_DEST_ 47  // dest.
#define STR_RES_AMOUNT 48  // amount
#define STR_RES_OCTAVE 49  // octave
#define STR_RES_RAGA 50  // raga
#define STR_RES_PORTA 51  // porta
#define STR_RES_MIDI_CHAN 52  // midi chan
#define STR_RES_TEMPO 53  // tempo
#define STR_RES_PATTERN 54  // pattern
#define STR_RES_SWING 55  // swing
#define STR_RES_OSCILLATOR_1 56  // oscillator 1
#define STR_RES_OSCILLATOR_2 57  // oscillator 2
#define STR_RES_MIXER 58  // mixer
#define STR_RES_FILTER 59  // filter
#define STR_RES_LFOS 60  // lfos
#define STR_RES_MODULATION 61  // modulation
#define STR_RES_KEYBOARD 62  // keyboard
#define STR_RES_ARPEGGIO 63  // arpeggio
#define STR_RES_OFF 64  // off
#define STR_RES_ON 65  // on
#define STR_RES_TRI 66  // tri
#define STR_RES_SQR 67  // sqr
#define STR_RES_S_H 68  // s&h
#define STR_RES_RAMP 69  // ramp
#define STR_RES_BLIT 70  // blit
#define STR_RES_SAW 71  // saw
#define STR_RES_SQUARE 72  // square
#define STR_RES_TRIANG 73  // triang
#define STR_RES_CZ 74  // cz
#define STR_RES_FM 75  // fm
#define STR_RES_8BITS 76  // 8bits
#define STR_RES_VOICE 77  // voice
#define STR_RES_TABLE 78  // table
#define STR_RES_SWEEP 79  // sweep
#define STR_RES_1S2 80  // 1+2
#define STR_RES_1_2 81  // 1>2
#define STR_RES_1P2 82  // 1*2
#define STR_RES_LF1 83  // lf1
#define STR_RES_LF2 84  // lf2
#define STR_RES_SEQ 85  // seq
#define STR_RES_ARP 86  // arp
#define STR_RES_WHL 87  // whl
#define STR_RES_BND 88  // bnd
#define STR_RES_AS1 89  // as1
#define STR_RES_AS2 90  // as2
#define STR_RES_ENV 91  // env
#define STR_RES_VEL 92  // vel
#define STR_RES_NOT 93  // not
#define STR_RES_GAT 94  // gat
#define STR_RES_LFO1 95  // lfo1
#define STR_RES_LFO2 96  // lfo2
#define STR_RES_STPSEQ 97  // stpseq
#define STR_RES__ARP 98  //  arp
#define STR_RES_MWHEEL 99  // mwheel
#define STR_RES_BENDER 100  // bender
#define STR_RES_ASSGN1 101  // assgn1
#define STR_RES_ASSGN2 102  // assgn2
#define STR_RES_ENVLPE 103  // envlpe
#define STR_RES_VELO 104  // velo
#define STR_RES_NOTE 105  // note
#define STR_RES_GATE 106  // gate
#define STR_RES_CUT 107  // cut
#define STR_RES_VCA 108  // vca
#define STR_RES_PW1 109  // pw1
#define STR_RES_PW2 110  // pw2
#define STR_RES_41 111  // 1
#define STR_RES_42 112  // 2
#define STR_RES_MIX 113  // mix
#define STR_RES_NOI 114  // noi
#define STR_RES_SUB 115  // sub
#define STR_RES_RES 116  // res
#define STR_RES_CUTOFF 117  // cutoff
#define STR_RES__VCA 118  //  vca
#define STR_RES_PWM1 119  // pwm1
#define STR_RES_PWM2 120  // pwm2
#define STR_RES_OSC1 121  // osc1
#define STR_RES_OSC2 122  // osc2
#define STR_RES__MIX 123  //  mix
#define STR_RES_NOISE 124  // noise
#define STR_RES_SUBOSC 125  // subosc
#define STR_RES_RESO 126  // reso
#define STR_RES_51 127  // 1
#define STR_RES_61 128  // 1
#define STR_RES_71 129  // 1
#define STR_RES__1 130  // ?1
#define STR_RES_52 131  // 2
#define STR_RES_62 132  // 2
#define STR_RES_72 133  // 2
#define STR_RES__2 134  // ?2
#define STR_RES_53 135  // 3
#define STR_RES_63 136  // 3
#define STR_RES_73 137  // 3
#define STR_RES__3 138  // ?3
#define STR_RES_54 139  // 4
#define STR_RES_64 140  // 4
#define STR_RES_74 141  // 4
#define STR_RES__4 142  // ?4
#define STR_RES_55 143  // 5
#define STR_RES_65 144  // 5
#define STR_RES_75 145  // 5
#define STR_RES__5 146  // ?5
#define STR_RES_56 147  // 6
#define STR_RES_66 148  // 6
#define STR_RES_76 149  // 6
#define STR_RES__6 150  // ?6
#define STR_RES_____MUTABLE 151  //     mutable
#define STR_RES___INSTRUMENTS 152  //   instruments
#define STR_RES_LOAD_SAVE_PATCH 153  // load/save patch
#define STR_RES_STEP_SEQUENCER 154  // step sequencer
#define STR_RES_LOAD 155  // load
#define STR_RES_EXIT 156  // exit
#define STR_RES_SAVE 157  // save
#define STR_RES_EXTERN 158  // extern
#define STR_RES_EQUAL 159  // equal
#define STR_RES_JUST 160  // just
#define STR_RES_PYTHAG 161  // pythag
#define STR_RES_1_4_EB 162  // 1/4 eb
#define STR_RES_1_4_E 163  // 1/4 e
#define STR_RES_1_4_EA 164  // 1/4 ea
#define STR_RES_KANAKA 165  // kanaka
#define STR_RES_RATNAN 166  // ratnan
#define STR_RES_GANAMU 167  // ganamu
#define STR_RES_VANASP 168  // vanasp
#define STR_RES_MANAVA 169  // manava
#define STR_RES_TANARU 170  // tanaru
#define STR_RES_SENAVA 171  // senava
#define STR_RES_HANUMA 172  // hanuma
#define STR_RES_DHENUK 173  // dhenuk
#define STR_RES_NATAKA 174  // nataka
#define STR_RES_KOKILA 175  // kokila
#define STR_RES_RUPAVA 176  // rupava
#define STR_RES_GAYAKA 177  // gayaka
#define STR_RES_VAKULA 178  // vakula
#define STR_RES_MAYAMA 179  // mayama
#define STR_RES_CHAKRA 180  // chakra
#define STR_RES_SURYAK 181  // suryak
#define STR_RES_HATAKA 182  // hataka
#define STR_RES_JHANKA 183  // jhanka
#define STR_RES_NATABH 184  // natabh
#define STR_RES_KEERAV 185  // keerav
#define STR_RES_KHARAH 186  // kharah
#define STR_RES_GOURIM 187  // gourim
#define STR_RES_VARUNA 188  // varuna
#define STR_RES_MARARA 189  // marara
#define STR_RES_CHARUK 190  // charuk
#define STR_RES_SARASA 191  // sarasa
#define STR_RES_HARIKA 192  // harika
#define STR_RES_DHEERA 193  // dheera
#define STR_RES_NAGANA 194  // nagana
#define STR_RES_YAGAPR 195  // yagapr
#define STR_RES_RAGAVA 196  // ragava
#define STR_RES_GANGEY 197  // gangey
#define STR_RES_VAGADH 198  // vagadh
#define STR_RES_SHULIN 199  // shulin
#define STR_RES_CHALAN 200  // chalan
#define STR_RES_SALAGA 201  // salaga
#define STR_RES_JALARN 202  // jalarn
#define STR_RES_JHALAV 203  // jhalav
#define STR_RES_NAVANE 204  // navane
#define STR_RES_PAVANI 205  // pavani
#define STR_RES_RAGHUP 206  // raghup
#define STR_RES_GAVAMB 207  // gavamb
#define STR_RES_BHAVAP 208  // bhavap
#define STR_RES_SHUBHA 209  // shubha
#define STR_RES_SHADVI 210  // shadvi
#define STR_RES_SUVARN 211  // suvarn
#define STR_RES_DIVYAM 212  // divyam
#define STR_RES_DHAVAL 213  // dhaval
#define STR_RES_NAMANA 214  // namana
#define STR_RES_KAMAVA 215  // kamava
#define STR_RES_RAMAPR 216  // ramapr
#define STR_RES_GAMANA 217  // gamana
#define STR_RES_VISHWA 218  // vishwa
#define STR_RES_SHAMAL 219  // shamal
#define STR_RES_SHANMU 220  // shanmu
#define STR_RES_SIMHEN 221  // simhen
#define STR_RES_HEMAVA 222  // hemava
#define STR_RES_DHARMA 223  // dharma
#define STR_RES_NEETIM 224  // neetim
#define STR_RES_KANTAM 225  // kantam
#define STR_RES_RISHAB 226  // rishab
#define STR_RES_LATANG 227  // latang
#define STR_RES_VACHAS 228  // vachas
#define STR_RES_MECHAK 229  // mechak
#define STR_RES_CHITRA 230  // chitra
#define STR_RES_SUCHAR 231  // suchar
#define STR_RES_JYOTIS 232  // jyotis
#define STR_RES_DHATUV 233  // dhatuv
#define STR_RES_NASIKA 234  // nasika
#define STR_RES_KOSALA 235  // kosala
#define STR_RES_RASIKA 236  // rasika
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
#define WAV_RES_SINE 0
#define WAV_RES_SINE_SIZE 257
#define WAV_RES_FORMANT_SINE 1
#define WAV_RES_FORMANT_SINE_SIZE 256
#define WAV_RES_FORMANT_SQUARE 2
#define WAV_RES_FORMANT_SQUARE_SIZE 256
#define WAV_RES_BANDLIMITED_PULSE_1 3
#define WAV_RES_BANDLIMITED_PULSE_1_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_2 4
#define WAV_RES_BANDLIMITED_PULSE_2_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_3 5
#define WAV_RES_BANDLIMITED_PULSE_3_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_4 6
#define WAV_RES_BANDLIMITED_PULSE_4_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_5 7
#define WAV_RES_BANDLIMITED_PULSE_5_SIZE 257
#define WAV_RES_BANDLIMITED_PULSE_6 8
#define WAV_RES_BANDLIMITED_PULSE_6_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_0 9
#define WAV_RES_BANDLIMITED_SQUARE_0_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_1 10
#define WAV_RES_BANDLIMITED_SQUARE_1_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_2 11
#define WAV_RES_BANDLIMITED_SQUARE_2_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_3 12
#define WAV_RES_BANDLIMITED_SQUARE_3_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_4 13
#define WAV_RES_BANDLIMITED_SQUARE_4_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_5 14
#define WAV_RES_BANDLIMITED_SQUARE_5_SIZE 257
#define WAV_RES_BANDLIMITED_SQUARE_6 15
#define WAV_RES_BANDLIMITED_SQUARE_6_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_0 16
#define WAV_RES_BANDLIMITED_SAW_0_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_1 17
#define WAV_RES_BANDLIMITED_SAW_1_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_2 18
#define WAV_RES_BANDLIMITED_SAW_2_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_3 19
#define WAV_RES_BANDLIMITED_SAW_3_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_4 20
#define WAV_RES_BANDLIMITED_SAW_4_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_5 21
#define WAV_RES_BANDLIMITED_SAW_5_SIZE 257
#define WAV_RES_BANDLIMITED_SAW_6 22
#define WAV_RES_BANDLIMITED_SAW_6_SIZE 257
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
