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

sample_rate = 31250.0

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
    ('formant_sine', sine_samples),
    ('formant_square', square_samples)
])


"""----------------------------------------------------------------------------
Band-limited waveforms
----------------------------------------------------------------------------"""

def Scale(array, min=0, max=255, use_min=0):
  if use_min != 0:
    m = use_min
  else:
    m = array.min()
  array = (array - m) / (array.max() - m)
  array = array * (max - min) + min
  return numpy.round(array).astype(int)

# Sine wave.
numpy.random.seed(21)
sine = -numpy.sin(numpy.arange(257.0) / 256.0 * 2 * numpy.pi) * 127.5 + 127.5
sine += numpy.random.rand(257) - 0.5

# Band limited waveforms.
num_zones = (107 - 24) / 16 + 2
bl_pulse_tables = []
bl_square_tables = []
bl_saw_tables = []
bl_tri_tables = []

# The Juno-6 / Juno-60 waveforms have a brighter harmonic content, which can be
# recreated by adding to the signal a 1-pole high-pass filtered version of
# itself.
juniness = 1.0

wrap = numpy.fmod(numpy.arange(257) + 128, 256)
quadrature = numpy.fmod(numpy.arange(257) + 64, 256)

for zone in range(num_zones):
  f0 = 440.0 * 2.0 ** ((24 + 16 * zone - 69) / 12.0)
  period = sample_rate / f0
  m = 2 * numpy.floor(period / 2) + 1.0
  i = wrap / 256.0
  pulse = numpy.sin(numpy.pi * i * m) / (m * numpy.sin(numpy.pi * i) + 1e-9)
  pulse[128] = 1.0
  bl_pulse_tables.append(('bandlimited_pulse_%d' % zone,
                          Scale(pulse[quadrature], 0, 255, -1.0)))

  square = numpy.cumsum(pulse - pulse[wrap])
  triangle = -numpy.cumsum(square[::-1] - square.mean()) / 256

  square -= juniness * triangle
  if zone == num_zones - 1:
    square = sine
  bl_square_tables.append(('bandlimited_square_%d' % zone,
                          Scale(square[quadrature], 0, 255)))
  
  triangle = triangle[quadrature]
  if zone == num_zones - 1:
    triangle = sine
  bl_tri_tables.append(('bandlimited_triangle_%d' % zone,
                        Scale(triangle[quadrature], 0, 255)))

  saw = -numpy.cumsum(pulse[wrap] - pulse.mean())
  saw -= juniness * numpy.cumsum(saw - saw.mean()) / 256
  if zone == num_zones - 1:
    saw = sine
  bl_saw_tables.append(('bandlimited_saw_%d' % zone,
                       Scale(saw[quadrature], 0, 255)))

# Blit are never generated at SR, always at SR/2.
del bl_pulse_tables[0]

waveforms.extend(bl_pulse_tables)
waveforms.extend(bl_square_tables)
waveforms.extend(bl_saw_tables)
waveforms.extend(bl_tri_tables)
waveforms.append((
    'wavetable',
    numpy.loadtxt('hardware/shruti/data/wavetable/wavetable_2048.txt').ravel()
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

waveforms.append(('vowel_data', vowel_data))
