import numpy

header = """// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Resources definitions.
//
// Automatically generated with:
// python arc.py resources.py
"""

namespace = 'hardware_shruti'
target = '.'
modifier = 'PROGMEM'
types = ['uint8_t', 'uint16_t']

includes = """
#include "hardware/base/base.h"

#ifdef __TEST__
#define prog_char char
#define prog_uint16_t uint16_t
#define prog_uint8_t uint8_t
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif
"""

# ------------------------------------------------------------------------------
# Strings
# ------------------------------------------------------------------------------

strings = """
alg
prm
rng
syn
tun
lfo
atk
dec
sus
rel
wv1
rt1
wv2
rt2
mod
src
dst
amt
oct
rag
prt
chn
bpm
pat
swg

algorithm
parameter
range
sync
detune
env~vcf
lfo2~vcf
osc bal
sub osc.
resonance
envelope
sequencer
attack
decay
sustain
release
lfo1 wave
lfo1 rate
lfo2 wave
lfo2 rate
mod.
source
dest.
amount
octave
raga
porta
midi chan
tempo
pattern
swing

oscillator 1
oscillator 2
mixer
filter
lfos
modulation
keyboard
arpeggio

off
on

tri
sqr
s&h
saw

blit
saw+
square
triang
cz
fm
8bits
voice
table
sweep

lf1
lf2
seq
arp
whl
bnd
as1
as2
env
vel
not
gat
lfo1
lfo2
stpseq
 arp
mwheel
bender
assgn1
assgn2
envlpe
velo
note
gate

cut
vca
pw1
pw2
\x041
\x042
mix
noi
sub
res

cutoff
 vca
pwm1
pwm2
osc1
osc2
 mix
noise
subosc
reso

\x051
\x061
\x071
?1
\x052
\x062
\x072
?2
\x053
\x063
\x073
?3
\x054
\x064
\x074
?4
\x055
\x065
\x075
?5
\x056
\x066
\x076
?6

    mutable
  instruments
load/save patch
step sequencer
load
exit
save
extern
equal
"""

# ------------------------------------------------------------------------------
# Lookup tables for pitch / LFOs / env
# ------------------------------------------------------------------------------

# Create lookup table for LFO increments (logarithmic frequency).
lookup_tables = []

sample_rate = 31250.0 / 2.0
control_rate = sample_rate / 16
min_frequency = 1.0 / 8.0  # Hertz
max_frequency = 20.0  # Hertz

excursion = 65536
num_values = 128
min_increment = excursion * min_frequency / control_rate
max_increment = excursion * max_frequency / control_rate

rates = numpy.linspace(numpy.log(min_increment),
                       numpy.log(max_increment), num_values)

lookup_tables.append(
    ('lfo_increments', numpy.exp(rates).astype(int))
)


# Create lookup table for envelope times (x^0.25).
max_time = 4.0  # seconds
min_time = 1.0 / control_rate  # seconds
excursion = 32767.0
gamma = 0.25
min_increment = excursion / (max_time * control_rate)
max_increment = excursion / (min_time * control_rate)
          
rates = numpy.linspace(numpy.power(max_increment, -gamma),
                       numpy.power(min_increment, -gamma), num_values)

lookup_tables.append(
    ('env_portamento_increments', numpy.power(rates, -1/gamma).astype(int))
)

# Create table for pitch.
a4_midi = 69
a4_pitch = 440.0
excursion = 65536.0
notes = numpy.arange(96 * 128.0, (96 + 12) * 128.0, 2)
pitches = a4_pitch * 2 ** ((notes - a4_midi * 128) / (128 * 12))
increments = excursion / sample_rate * pitches

lookup_tables.append(
    ('oscillator_increments', increments.astype(int))
)

"""List of 22 shrutis with different notation schemes.

The most common scheme is in the 3th column.
"""
shrutis = [
  # Swara ref 1, Swara ref 2, Swara, Swara (carnatic, common), Just, Ratio
  ('S', 'sa', 's', 'C', 1),
  ('r1', 'ra', 'r1', '?', 256.0/243.0),
  ('r2', 'ri', 'r2', 'C#', 16.0/15.0),
  ('R1', 'ru', 'r3', '?', 10.0/9.0),
  ('R2', 're', 'r4', 'D', 9.0/8.0),
  ('g1', 'ga', 'g1', 'pD#', 32.0/27.0),
  ('g2', 'gi', 'g2', 'D#', 6.0/5.0),
  ('G1', 'gu', 'g3', 'E', 5.0/4.0),
  ('G2', 'ge', 'g4', 'pE', 81.0/64.0),
  ('m1', 'ma', 'm1', 'F', 4.0/3.0),
  ('m2', 'mi', 'm2', '?', 27.0/20.0),
  ('M1', 'mu', 'm3', 'F#', 45.0/32.0),
  ('M2', 'me', 'm4', '?', 729.0/512.0),
  ('P', 'pa', 'p', 'G', 3.0/2.0),
  ('d1', 'dha', 'd1', '?', 128.0/81.0),
  ('d2', 'dhi', 'd2', 'G#', 8.0/5.0),
  ('D1', 'dhu', 'd3', 'A', 5.0/3.0),
  ('D2', 'dhe', 'd4', '?', 27.0/16.0),
  ('n1', 'na', 'n1', 'A#', 16.0/9.0),
  ('n2', 'ni', 'n2', '?', 9.0/5.0),
  ('N1', 'nu', 'n3', 'B', 15.0/8.0),
  ('N2', 'ne', 'n4', 'pB', 243.0/128.0),
]

"""A recommended key on the keyboard for each of the swara.

From:
http://commons.wikimedia.org/wiki/Melakarta_ragams_(svg)
"""
recommended_keys = {
  's': 0,
  'r1': 1,
  'r2': 2,
  'r3': 3,
  'g1': 2,
  'g2': 3,
  'g3': 4,
  'm1': 5,
  'm2': 6,
  'p': 7,
  'd1': 8,
  'd2': 9,
  'd3': 10,
  'n1': 9,
  'n2': 10,
  'n3': 11 
}

shruti_dictionary = {}
for entry in shrutis:
  for name in entry[:-1]:
    shruti_dictionary[name] = entry[-1]


def Compute(scale):
  """Translate a list of 12 note/swaras names into pitch corrections."""
  values = [shruti_dictionary.get(x) for x in scale.split(' ')]
  equal = 2 ** (numpy.arange(12.0) / 12.0)
  return (numpy.log2(values / equal) * 12 * 128).astype(int)


def LayoutRaga(raga):
  """Find a good assignments of swaras to keys for a raga."""
  raga = raga.lower()
  scale = numpy.zeros((12,))
  mapping = ['' for i in range(12)]
  for swara in raga.split(' '):
    key = recommended_keys.get(swara)
    mapping[key] = swara

  # Fill unassigned notes
  for i, n in enumerate(mapping):
    if n == '':
      candidates = []
      for swara, key in recommended_keys.items():
        if key == i:
          candidates.append(swara)
      for candidate in candidates:
        if candidate[0] != mapping[i - 1]:
          mapping[i] = candidate
          break
      else:
        mapping[i] = candidates[0]
  
  scale = [shruti_dictionary.get(swara) for swara in mapping]
  return Compute(' '.join(mapping))


altered_e_b = [0, 0, 0, 0, -64, 0, 0, 0, 0, 0, 0, -64]
altered_e = [0, 0, 0, 0, -64, 0, 0, 0, 0, 0, 0, 0]
altered_e_a = [0, 0, 0, 0, -64, 0, 0, 0, 0, -64, 0, 0]

scales = [
    ('just', Compute('C C# D D# E F F# G G# A A# B')),
    ('pythagorean', Compute('C C# D pD# pE F F# G G# A A# pB')),
    ('1/4 eb', numpy.array(altered_e_b, dtype=int)),
    ('1/4 e', numpy.array(altered_e, dtype=int)),
    ('1/4 ea', numpy.array(altered_e_a, dtype=int)),
    ('kanakangi', LayoutRaga('S R1 G1 M1 P D1 N1')),
    ('ratnangi', LayoutRaga('S R1 G1 M1 P D1 N2')),
    ('ganamurti', LayoutRaga('S R1 G1 M1 P D1 N3')),
    ('vanaspati', LayoutRaga('S R1 G1 M1 P D2 N2')),
    ('manavati', LayoutRaga('S R1 G1 M1 P D2 N3')),
    ('tanarupi', LayoutRaga('S R1 G1 M1 P D3 N3')),
    ('senavati', LayoutRaga('S R1 G2 M1 P D1 N1')),
    ('hanumatodi', LayoutRaga('S R1 G2 M1 P D1 N2')),
    ('dhenuka', LayoutRaga('S R1 G2 M1 P D1 N3')),
    ('natakapriya', LayoutRaga('S R1 G2 M1 P D2 N2')),
    ('kokilapriya', LayoutRaga('S R1 G2 M1 P D2 N3')),
    ('rupavati', LayoutRaga('S R1 G2 M1 P D3 N3')),
    ('gayakapriya', LayoutRaga('S R1 G3 M1 P D1 N1')),
    ('vakulabharanam', LayoutRaga('S R1 G3 M1 P D1 N2')),
    ('mayamalavagowla', LayoutRaga('S R1 G3 M1 P D1 N3')),
    ('chakravakam', LayoutRaga('S R1 G3 M1 P D2 N2')),
    ('suryakantam', LayoutRaga('S R1 G3 M1 P D2 N3')),
    ('hatakambari', LayoutRaga('S R1 G3 M1 P D3 N3')),
    ('jhankaradhwani', LayoutRaga('S R2 G2 M1 P D1 N1')),
    ('natabhairavi', LayoutRaga('S R2 G2 M1 P D1 N2')),
    ('keeravani', LayoutRaga('S R2 G2 M1 P D1 N3')),
    ('kharaharapriya', LayoutRaga('S R2 G2 M1 P D2 N2')),
    ('gourimanohari', LayoutRaga('S R2 G2 M1 P D2 N3')),
    ('varunapriya', LayoutRaga('S R2 G2 M1 P D3 N3')),
    ('mararanjani', LayoutRaga('S R2 G3 M1 P D1 N1')),
    ('charukesi', LayoutRaga('S R2 G3 M1 P D1 N2')),
    ('sarasangi', LayoutRaga('S R2 G3 M1 P D1 N3')),
    ('harikambhoji', LayoutRaga('S R2 G3 M1 P D2 N2')),
    ('dheerasankarabharanam', LayoutRaga('S R2 G3 M1 P D2 N3')),
    ('naganandini', LayoutRaga('S R2 G3 M1 P D3 N3')),
    ('yagapriya', LayoutRaga('S R3 G3 M1 P D1 N1')),
    ('ragavardhini', LayoutRaga('S R3 G3 M1 P D1 N2')),
    ('gangeyabhushani', LayoutRaga('S R3 G3 M1 P D1 N3')),
    ('vagadheeswari', LayoutRaga('S R3 G3 M1 P D2 N2')),
    ('shulini', LayoutRaga('S R3 G3 M1 P D2 N3')),
    ('chalanata', LayoutRaga('S R3 G3 M1 P D3 N3')),
    ('salagam', LayoutRaga('S R1 G1 M2 P D1 N1')),
    ('jalarnavam', LayoutRaga('S R1 G1 M2 P D1 N2')),
    ('jhalavarali', LayoutRaga('S R1 G1 M2 P D1 N3')),
    ('navaneetam', LayoutRaga('S R1 G1 M2 P D2 N2')),
    ('pavani', LayoutRaga('S R1 G1 M2 P D2 N3')),
    ('raghupriya', LayoutRaga('S R1 G1 M2 P D3 N3')),
    ('gavambhodi', LayoutRaga('S R1 G2 M2 P D1 N1')),
    ('bhavapriya', LayoutRaga('S R1 G2 M2 P D1 N2')),
    ('shubhapantuvarali', LayoutRaga('S R1 G2 M2 P D1 N3')),
    ('shadvidamargini', LayoutRaga('S R1 G2 M2 P D2 N2')),
    ('suvarnangi', LayoutRaga('S R1 G2 M2 P D2 N3')),
    ('divyamani', LayoutRaga('S R1 G2 M2 P D3 N3')),
    ('dhavalambari', LayoutRaga('S R1 G3 M2 P D1 N1')),
    ('namanarayani', LayoutRaga('S R1 G3 M2 P D1 N2')),
    ('kamavardani', LayoutRaga('S R1 G3 M2 P D1 N3')),
    ('ramapriya', LayoutRaga('S R1 G3 M2 P D2 N2')),
    ('gamanashrama', LayoutRaga('S R1 G3 M2 P D2 N3')),
    ('vishwambari', LayoutRaga('S R1 G3 M2 P D3 N3')),
    ('shamalangi', LayoutRaga('S R2 G2 M2 P D1 N1')),
    ('shanmukhapriya', LayoutRaga('S R2 G2 M2 P D1 N2')),
    ('simhendramadhyamam', LayoutRaga('S R2 G2 M2 P D1 N3')),
    ('hemavati', LayoutRaga('S R2 G2 M2 P D2 N2')),
    ('dharmavati', LayoutRaga('S R2 G2 M2 P D2 N3')),
    ('neetimati', LayoutRaga('S R2 G2 M2 P D3 N3')),
    ('kantamani', LayoutRaga('S R2 G3 M2 P D1 N1')),
    ('rishabhapriya', LayoutRaga('S R2 G3 M2 P D1 N2')),
    ('latangi', LayoutRaga('S R2 G3 M2 P D1 N3')),
    ('vachaspati', LayoutRaga('S R2 G3 M2 P D2 N2')),
    ('mechakalyani', LayoutRaga('S R2 G3 M2 P D2 N3')),
    ('chitrambari', LayoutRaga('S R2 G3 M2 P D3 N3')),
    ('sucharitra', LayoutRaga('S R3 G3 M2 P D1 N1')),
    ('jyotiswarupini', LayoutRaga('S R3 G3 M2 P D1 N2')),
    ('dhatuvardani', LayoutRaga('S R3 G3 M2 P D1 N3')),
    ('nasikabhushani', LayoutRaga('S R3 G3 M2 P D2 N2')),
    ('kosalam', LayoutRaga('S R3 G3 M2 P D2 N3')),
    ('rasikapriya', LayoutRaga('S R3 G3 M2 P D3 N3')),
]

for scale, values in scales:
  strings += '\n' + scale[:6]
  lookup_tables.append(('scale_%s' % scale, values))

# ------------------------------------------------------------------------------
# Waveforms
# ------------------------------------------------------------------------------

waveforms = []
numpy.random.seed(21)
# Create sine wave table.
sine = -numpy.cos(numpy.arange(257.0) / 256.0 * 2 * numpy.pi) * 127.5 + 127.5
sine += numpy.random.rand(257) - 0.5
waveforms.append(
    ('sine', sine.astype(int))
)
# Create amplitude modulated resonance tables.
sine_samples = []
square_samples = []
sine = numpy.sin(numpy.arange(16.0) / 16.0 * 2 * numpy.pi)
for i, x in enumerate(sine):
  gains = numpy.exp(0.184 * numpy.arange(16.0))
  gains[0] = 0
  values = gains * x * 4
  values = numpy.round(values).astype(int)
  amps = numpy.round(gains)
  if (i >= 8):
    amps = -amps
  square_samples.extend(amps.astype(int))
  sine_samples.extend(values)

waveforms.extend([
    ('formant_sine', sine_samples),
    ('formant_square', square_samples)
])

def Scale(array, min=0, max=255, use_min=0):
  if use_min != 0:
    m = use_min
  else:
    m = array.min()
  array = (array - m) / (array.max() - m)
  array = array * (max - min) + min
  return numpy.round(array).astype(int)

# Band limited waveforms.
num_zones = (107 - 24) / 16 + 1
bl_pulse_tables = []
bl_square_tables = []
bl_saw_tables = []
bl_tri_tables = []
for zone in range(num_zones):
  f0 = 440.0 * 2.0 ** ((24 + 16 * (zone + 1) - 69) / 12.0)
  period = sample_rate / f0
  m = 2 * numpy.floor(period / 2) + 1.0
  wrap = numpy.fmod(numpy.arange(257) + 128, 256)
  i = wrap / 256.0
  pulse = numpy.sin(numpy.pi * i * m) / (m * numpy.sin(numpy.pi * i) + 1e-9)
  pulse[128] = 1.0
  bl_pulse_tables.append(('bandlimited_pulse_%d' % zone,
                          Scale(pulse, 0, 255, -1.0)))
  square = numpy.cumsum(pulse - pulse[wrap])
  bl_square_tables.append(('bandlimited_square_%d' % zone,
                          Scale(square, 0, 255)))
  triangle = numpy.cumsum(square - square.mean())
  bl_tri_tables.append(('bandlimited_triangle_%d' % zone,
                        Scale(triangle, 0, 255)))
  saw = numpy.cumsum(pulse[wrap] - pulse.mean())
  bl_saw_tables.append(('bandlimited_saw_%d' % zone, Scale(saw, 0, 255)))

waveforms.extend(bl_pulse_tables)
waveforms.extend(bl_square_tables)
waveforms.extend(bl_saw_tables)
waveforms.extend(bl_tri_tables)
waveforms.append((
    'wavetable',
    numpy.loadtxt('hardware/shruti/data/wavetable/wavetable_2048.txt').ravel()
))


# ------------------------------------------------------------------------------
# Speech data
# ------------------------------------------------------------------------------

speech_data = [
27,40,89,253,16,
18,51,62,220,96,
15,69,93,236,112,
10,84,110,218,128,
23,44,87,252,16,
13,29,80,216,0,
6,46,81,195,0,
9, 51, 95, 243, 3,
6, 73, 99, 122, 233]

waveforms.append(('speech_data', speech_data))

# ------------------------------------------------------------------------------
# Arpeggiator patterns
# ------------------------------------------------------------------------------

def XoxTo16BitInt(pattern):
  uint16 = 0
  i = 0
  for char in pattern:
    if char == 'o':
      uint16 += (2 ** i)
      i += 1
    elif char == '-':
      i += 1
  assert i == 16
  return uint16


def ConvertPatterns(patterns):
  return [XoxTo16BitInt(pattern) for pattern in patterns]


lookup_tables.append(
  ('arpeggiator_patterns', ConvertPatterns([
      'o-o- o-o- o-o- o-o-',
      'ooo- ooo- ooo- ooo-',
      'o-o- oooo o-o- oooo',
      'o-o- oo-o o-o- oo-o',
      'o--o --o- -o-- o-o-',
      'o--o o--- o-o- o-oo'])))

# ------------------------------------------------------------------------------
# Custom characters for LCD
# ------------------------------------------------------------------------------

def StringImageToBytes(string_image):
  rows = [row.strip() for row in string_image.split('\n') if row.strip()]
  assert len(rows) == 8
  bytes = []
  for row in rows:
    int8 = 0
    assert len(row) == 5
    for char in row:
      int8 *= 2
      if char == 'X':
        int8 += 1
    int8 += 0  # 128 + 64 + 32
    bytes.append(int8)
  return bytes

special_characters = [
"""
...X.
...XX
...XX
...X.
...X.
.XXX.
XXXX.
.XX..
""",
"""
.....
.....
.....
.XXX.
X.X.X
X.XXX
X...X
.XXX.
""",
"""
.....
.....
.....
..X..
..X..
.XXX.
X...X
XXXXX
""",
"""
.....
.....
..X..
.XXX.
X.X.X
X...X
X...X
.XXX.
""",
"""
.....
.....
.....
.X...
X.X.X
...X.
.....
.....
""",
"""
..X..
.XXX.
X.X.X
..X..
..X..
..X..
..X..
.....
""",
"""
.....
..X..
..X..
..X..
..X..
X.X.X
.XXX.
..X..
""",
"""
.X...
XXX..
.X...
.X.X.
.X.X.
...X.
..XXX
...X.
"""]

bytes = sum([StringImageToBytes(image) for image in special_characters], [])
characters = [
    ('special_characters', bytes)
]
# ------------------------------------------------------------------------------
# Summary of all resources
# ------------------------------------------------------------------------------

# multi-line string or list of tuples | table name | prefix | type | python type
# store index in RAM
resources = [
  (strings, 'string', 'STR_RES', 'prog_char', str, False),
  (lookup_tables, 'lookup_table', 'LUT_RES', 'prog_uint16_t', int, False),
  (waveforms, 'waveform', 'WAV_RES', 'prog_uint8_t', int, True),
  (characters, 'character', 'CHR_RES', 'prog_uint8_t', int, True),
]

create_specialized_manager = True
