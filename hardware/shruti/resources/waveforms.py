#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet.
#
# Author: Olivier Gillet (ol.gillet@gmail.com)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# -----------------------------------------------------------------------------
#
# Waveform definitions.

import numpy

"""----------------------------------------------------------------------------
Waveforms for vowel synthesis
----------------------------------------------------------------------------"""

SAMPLE_RATE = 31250.0

waveforms = []
# Create amplitude modulated sine/square tables for formants.
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
    ('formant_sine', sine_samples, 1),
    ('formant_square', square_samples, 1)
])


"""----------------------------------------------------------------------------
Band-limited waveforms
----------------------------------------------------------------------------"""

WAVETABLE_SIZE = 256

# The Juno-6 / Juno-60 waveforms have a brighter harmonic content, which can be
# recreated by adding to the signal a 1-pole high-pass filtered version of
# itself.
JUNINESS = 1.0


# If you look at the bandlimited waveform produced by the approach described
# in the BLIT paper, you'll find that they are not causal: it is as if the
# band-limiting filter "anticipated" the discontinuity and started the
# oscillations/ringing to smooth the discontinuity *before* it happens.
# Such things do not exist in nature.
# A way of avoiding this is to use a causal version of the band-limiting filter,
# obtained by minimum phase reconstruction of the sinc. It doesn't sound better.
CAUSAL = False


def Scale(array, min=0, max=255, center=True):
  if center:
    array -= array.mean()
  mx = numpy.abs(array).max()
  array = (array + mx) / (2 * mx)
  array = array * (max - min) + min
  return numpy.round(array).astype(int)


def MinimumPhaseReconstruction(signal, fft_size=16384):
  Xf = numpy.fft.fft(signal, fft_size)
  real_cepstrum = numpy.fft.ifft(numpy.log(numpy.abs(Xf))).real
  real_cepstrum[1:fft_size / 2] *= 2
  real_cepstrum[fft_size / 2 + 1:] = 0
  min_phi = numpy.fft.ifft(numpy.exp(numpy.fft.fft(real_cepstrum))).real
  return min_phi


def ApplyFormants(signal, f0, formants, amplitude_decay=0.5,
                  f0_center=261.5, f0_bias=0.5):
  n = len(signal)
  signal = numpy.tile(signal, 16)
  result = 0
  amplitude = 1.0
  for formant in formants:
    formant = formant * (f0 / f0_center) ** f0_bias
    period = float(f0) / formant * n
    response = numpy.sin(numpy.arange(0, n * 4) * 2 * numpy.pi / period)
    response *= numpy.exp(-numpy.arange(0, n * 4) / period / 4.0)
    result += numpy.convolve(signal, response, mode='same') * amplitude
    amplitude *= amplitude_decay
  return result[8 * n : 8 * n + n]


# Sine wave.
numpy.random.seed(21)
sine = -numpy.sin(numpy.arange(WAVETABLE_SIZE + 1) / float(WAVETABLE_SIZE) * 2 * numpy.pi) * 127.5 + 127.5
sine += numpy.random.rand(WAVETABLE_SIZE + 1) - 0.5

# Band limited waveforms.
num_zones = (107 - 24) / 16 + 2
bl_pulse_tables = []
bl_square_tables = []
bl_saw_tables = []
bl_tri_tables = []
bl_reed_1_tables = []
bl_reed_2_tables = []
bl_voice_1_tables = []
bl_voice_2_tables = []

wrap = numpy.fmod(numpy.arange(WAVETABLE_SIZE + 1) + WAVETABLE_SIZE / 2, WAVETABLE_SIZE)
quadrature = numpy.fmod(numpy.arange(WAVETABLE_SIZE + 1) + WAVETABLE_SIZE / 4, WAVETABLE_SIZE)
octature = numpy.fmod(numpy.arange(WAVETABLE_SIZE + 1) + WAVETABLE_SIZE / 8, WAVETABLE_SIZE)
fill = numpy.fmod(numpy.arange(WAVETABLE_SIZE + 1), WAVETABLE_SIZE)

if CAUSAL:
  window = numpy.hanning(WAVETABLE_SIZE)
else:
  window = 1


for zone in range(num_zones):
  f0 = 440.0 * 2.0 ** ((24 + 16 * zone - 69) / 12.0)
  period = SAMPLE_RATE / f0
  m = 2 * numpy.floor(period / 2) + 1.0
  i = numpy.arange(-WAVETABLE_SIZE / 2, WAVETABLE_SIZE / 2) / float(WAVETABLE_SIZE)
  pulse = numpy.sin(numpy.pi * i * m) / (m * numpy.sin(numpy.pi * i) + 1e-9)
  pulse[WAVETABLE_SIZE / 2] = 1.0
  pulse *= window
  if CAUSAL:
    pulse = MinimumPhaseReconstruction(pulse)[:(WAVETABLE_SIZE + 1)]
  else:
    pulse = pulse[fill]
  bl_pulse_tables.append(('bandlimited_pulse_%d' % zone,
                          Scale(pulse, center=False), 1))

  square = numpy.cumsum(pulse - pulse[wrap])
  triangle = -numpy.cumsum(square[::-1] - square.mean()) / WAVETABLE_SIZE

  square -= JUNINESS * triangle
  if zone == num_zones - 1:
    square = sine
  bl_square_tables.append(('bandlimited_square_%d' % zone,
                          Scale(square[quadrature]), None))
  
  triangle = triangle[quadrature]
  if zone == num_zones - 1:
    triangle = sine
  bl_tri_tables.append(('bandlimited_triangle_%d' % zone,
                        Scale(triangle[quadrature]), 1))

  saw = -numpy.cumsum(pulse[wrap] - pulse.mean())
  saw -= JUNINESS * numpy.cumsum(saw - saw.mean()) / WAVETABLE_SIZE
  if zone == num_zones - 1:
    saw = sine
  bl_saw_tables.append(('bandlimited_saw_%d' % zone,
                       Scale(saw[quadrature]), None))
                       
  reed_1 = numpy.cumsum(pulse - pulse[quadrature])
  reed_2 = numpy.cumsum(pulse - pulse[octature])
  voice = reed_1
  if zone == num_zones - 1:
    reed_1 = sine
    reed_2 = sine
  voice_1 = ApplyFormants(pulse, f0, [400.0, 800.0, 3200.0])
  voice_2 = ApplyFormants(pulse, f0, [700.0, 1300.0, 3200.0])
  bl_reed_1_tables.append(('bandlimited_reed_1_%d' % zone,
                           Scale(reed_1[quadrature]), 4))
  bl_reed_2_tables.append(('bandlimited_reed_2_%d' % zone,
                           Scale(reed_2[octature]), 4))
  bl_voice_1_tables.append(('bandlimited_voice_1_%d' % zone,
                            Scale(voice_1), 4))
  bl_voice_2_tables.append(('bandlimited_voice_2_%d' % zone,
                            Scale(voice_2), 4))

# Blit are never generated at SR, always at SR/2.
del bl_pulse_tables[0]

waveforms.extend(bl_pulse_tables)
waveforms.extend(bl_square_tables)
waveforms.extend(bl_saw_tables)
waveforms.extend(bl_tri_tables)
waveforms.extend(bl_reed_1_tables)
waveforms.extend(bl_reed_2_tables)
waveforms.extend(bl_voice_1_tables)
waveforms.extend(bl_voice_2_tables)
waveforms.append((
    'wavetable',
    numpy.loadtxt('hardware/shruti/shruti1/data/wavetable.txt').ravel(),
    1
))


"""----------------------------------------------------------------------------
Vowel data (formant amplitudes)
-----------------------------------------------------------------------------"""

vowel_data = [
27, 40,  89, 253, 16,
18, 51,  62, 220, 96,
15, 69,  93, 236, 112,
10, 84, 110, 218, 128,
23, 44,  87, 252, 16,
13, 29,  80, 216, 0,
 6, 46,  81, 195, 0,
 9, 51,  95, 243, 3,
 6, 73,  99, 122, 233]

waveforms.append(('vowel_data', vowel_data, 1))
