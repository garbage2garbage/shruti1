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
#define STR_RES_SYN 3  // syn
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
#define STR_RES_SYNC 28  // sync
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
#define STR_RES_LF1 80  // lf1
#define STR_RES_LF2 81  // lf2
#define STR_RES_SEQ 82  // seq
#define STR_RES_ARP 83  // arp
#define STR_RES_WHL 84  // whl
#define STR_RES_BND 85  // bnd
#define STR_RES_AS1 86  // as1
#define STR_RES_AS2 87  // as2
#define STR_RES_ENV 88  // env
#define STR_RES_VEL 89  // vel
#define STR_RES_NOT 90  // not
#define STR_RES_GAT 91  // gat
#define STR_RES_LFO1 92  // lfo1
#define STR_RES_LFO2 93  // lfo2
#define STR_RES_STPSEQ 94  // stpseq
#define STR_RES__ARP 95  //  arp
#define STR_RES_MWHEEL 96  // mwheel
#define STR_RES_BENDER 97  // bender
#define STR_RES_ASSGN1 98  // assgn1
#define STR_RES_ASSGN2 99  // assgn2
#define STR_RES_ENVLPE 100  // envlpe
#define STR_RES_VELO 101  // velo
#define STR_RES_NOTE 102  // note
#define STR_RES_GATE 103  // gate
#define STR_RES_CUT 104  // cut
#define STR_RES_VCA 105  // vca
#define STR_RES_PW1 106  // pw1
#define STR_RES_PW2 107  // pw2
#define STR_RES_41 108  // 1
#define STR_RES_42 109  // 2
#define STR_RES_MIX 110  // mix
#define STR_RES_NOI 111  // noi
#define STR_RES_SUB 112  // sub
#define STR_RES_RES 113  // res
#define STR_RES_CUTOFF 114  // cutoff
#define STR_RES__VCA 115  //  vca
#define STR_RES_PWM1 116  // pwm1
#define STR_RES_PWM2 117  // pwm2
#define STR_RES_OSC1 118  // osc1
#define STR_RES_OSC2 119  // osc2
#define STR_RES__MIX 120  //  mix
#define STR_RES_NOISE 121  // noise
#define STR_RES_SUBOSC 122  // subosc
#define STR_RES_RESO 123  // reso
#define STR_RES_51 124  // 1
#define STR_RES_61 125  // 1
#define STR_RES_71 126  // 1
#define STR_RES__1 127  // ?1
#define STR_RES_52 128  // 2
#define STR_RES_62 129  // 2
#define STR_RES_72 130  // 2
#define STR_RES__2 131  // ?2
#define STR_RES_53 132  // 3
#define STR_RES_63 133  // 3
#define STR_RES_73 134  // 3
#define STR_RES__3 135  // ?3
#define STR_RES_54 136  // 4
#define STR_RES_64 137  // 4
#define STR_RES_74 138  // 4
#define STR_RES__4 139  // ?4
#define STR_RES_55 140  // 5
#define STR_RES_65 141  // 5
#define STR_RES_75 142  // 5
#define STR_RES__5 143  // ?5
#define STR_RES_56 144  // 6
#define STR_RES_66 145  // 6
#define STR_RES_76 146  // 6
#define STR_RES__6 147  // ?6
#define STR_RES_____MUTABLE 148  //     mutable
#define STR_RES___INSTRUMENTS 149  //   instruments
#define STR_RES_LOAD_SAVE_PATCH 150  // load/save patch
#define STR_RES_STEP_SEQUENCER 151  // step sequencer
#define STR_RES_LOAD 152  // load
#define STR_RES_EXIT 153  // exit
#define STR_RES_SAVE 154  // save
#define STR_RES_EXTERN 155  // extern
#define STR_RES_EQUAL 156  // equal
#define STR_RES_JUST 157  // just
#define STR_RES_PYTHAG 158  // pythag
#define STR_RES_1_4_EB 159  // 1/4 eb
#define STR_RES_1_4_E 160  // 1/4 e
#define STR_RES_1_4_EA 161  // 1/4 ea
#define STR_RES_KANAKA 162  // kanaka
#define STR_RES_RATNAN 163  // ratnan
#define STR_RES_GANAMU 164  // ganamu
#define STR_RES_VANASP 165  // vanasp
#define STR_RES_MANAVA 166  // manava
#define STR_RES_TANARU 167  // tanaru
#define STR_RES_SENAVA 168  // senava
#define STR_RES_HANUMA 169  // hanuma
#define STR_RES_DHENUK 170  // dhenuk
#define STR_RES_NATAKA 171  // nataka
#define STR_RES_KOKILA 172  // kokila
#define STR_RES_RUPAVA 173  // rupava
#define STR_RES_GAYAKA 174  // gayaka
#define STR_RES_VAKULA 175  // vakula
#define STR_RES_MAYAMA 176  // mayama
#define STR_RES_CHAKRA 177  // chakra
#define STR_RES_SURYAK 178  // suryak
#define STR_RES_HATAKA 179  // hataka
#define STR_RES_JHANKA 180  // jhanka
#define STR_RES_NATABH 181  // natabh
#define STR_RES_KEERAV 182  // keerav
#define STR_RES_KHARAH 183  // kharah
#define STR_RES_GOURIM 184  // gourim
#define STR_RES_VARUNA 185  // varuna
#define STR_RES_MARARA 186  // marara
#define STR_RES_CHARUK 187  // charuk
#define STR_RES_SARASA 188  // sarasa
#define STR_RES_HARIKA 189  // harika
#define STR_RES_DHEERA 190  // dheera
#define STR_RES_NAGANA 191  // nagana
#define STR_RES_YAGAPR 192  // yagapr
#define STR_RES_RAGAVA 193  // ragava
#define STR_RES_GANGEY 194  // gangey
#define STR_RES_VAGADH 195  // vagadh
#define STR_RES_SHULIN 196  // shulin
#define STR_RES_CHALAN 197  // chalan
#define STR_RES_SALAGA 198  // salaga
#define STR_RES_JALARN 199  // jalarn
#define STR_RES_JHALAV 200  // jhalav
#define STR_RES_NAVANE 201  // navane
#define STR_RES_PAVANI 202  // pavani
#define STR_RES_RAGHUP 203  // raghup
#define STR_RES_GAVAMB 204  // gavamb
#define STR_RES_BHAVAP 205  // bhavap
#define STR_RES_SHUBHA 206  // shubha
#define STR_RES_SHADVI 207  // shadvi
#define STR_RES_SUVARN 208  // suvarn
#define STR_RES_DIVYAM 209  // divyam
#define STR_RES_DHAVAL 210  // dhaval
#define STR_RES_NAMANA 211  // namana
#define STR_RES_KAMAVA 212  // kamava
#define STR_RES_RAMAPR 213  // ramapr
#define STR_RES_GAMANA 214  // gamana
#define STR_RES_VISHWA 215  // vishwa
#define STR_RES_SHAMAL 216  // shamal
#define STR_RES_SHANMU 217  // shanmu
#define STR_RES_SIMHEN 218  // simhen
#define STR_RES_HEMAVA 219  // hemava
#define STR_RES_DHARMA 220  // dharma
#define STR_RES_NEETIM 221  // neetim
#define STR_RES_KANTAM 222  // kantam
#define STR_RES_RISHAB 223  // rishab
#define STR_RES_LATANG 224  // latang
#define STR_RES_VACHAS 225  // vachas
#define STR_RES_MECHAK 226  // mechak
#define STR_RES_CHITRA 227  // chitra
#define STR_RES_SUCHAR 228  // suchar
#define STR_RES_JYOTIS 229  // jyotis
#define STR_RES_DHATUV 230  // dhatuv
#define STR_RES_NASIKA 231  // nasika
#define STR_RES_KOSALA 232  // kosala
#define STR_RES_RASIKA 233  // rasika
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
