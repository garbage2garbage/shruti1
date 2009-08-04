#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# Saves as a .wav file

import struct
import sys
import numpy

# Constant used when converting between unsigned char and float. A different
# value is used in both directions to avoid clipping.
_UNSIGNED_CHAR_TO_FLOAT_SCALE = 128.0
_FLOAT_TO_UNSIGNED_CHAR_SCALE = 127.0

_DATA_CHUNK_HEADER_SIZE = 8
_FMT_CHUNK_DATA_SIZE = 16
_FMT_CHUNK_HEADER_SIZE = 8
_RIFF_FORMAT_DESCRIPTOR_SIZE = 4


def Quantize(signal, bitdepth, normalize=True):
  """Convert an array of float to an array of integers.

  Args:
    signal: numpy array. source signal.
    bitdepth: int. size of the integer in bits.
    normalize: boolean. whether samples should be scaled to use all the
        available dynamic range.

  Returns:
    array of integers.
  """
  norm = numpy.abs(signal).max()

  # Normalization or clipping.
  if normalize and norm > 0:
    scaled_signal = signal / norm
  else:
    scaled_signal = copy.copy(signal)
    if norm > 1.0:
      scaled_signal[scaled_signal < -1] = -1
      scaled_signal[scaled_signal > 1] = 1

  # Dithering.
  # TODO(oliviergillet): implement noise shapping, especially for
  # 8 bits writing.
  if bitdepth == 8:
    scaled_signal = (scaled_signal + 1.0) * _FLOAT_TO_UNSIGNED_CHAR_SCALE
    scaled_signal = numpy.array(scaled_signal, dtype=numpy.uint8)
  else:
    scale = (1 << (bitdepth - 1)) - 1
    # pylint: disable-msg=C6407
    scaled_signal = scaled_signal * scale
    scaled_signal = numpy.array(scaled_signal, dtype='i%d' % (bitdepth / 8))

  return scaled_signal


def WriteWavFile(signal, sample_rate, file_name, bitdepth=16, normalize=True):
  """Write a .wav file from a numpy array.

  Note: does not support GFile, as the tofile method of numpy arrays can only
        use native python file objects.

  Args:
    signal: 2-dimensional numpy array, of size (num_samples, num_channels).
    sample_rate: int. sample rate of the signal in Hz.
    file_name: string. name of the destination file.
    bitdepth: int. bitdepth in bits (default 16).
    normalize: boolean. if set to True, scale the data to the [-1, 1] range
        before writing.
  """
  if signal.dtype == numpy.uint8 or signal.dtype == numpy.int16:
    bitdepth = signal.dtype.itemsize * 8
    scaled_signal = signal
  else:
    scaled_signal = Quantize(signal, bitdepth, normalize=normalize)

  if scaled_signal.ndim == 1:
    num_channels = 1
  else:
    num_channels = scaled_signal.shape[1]

  # Compute the total size of the output .wav file, minus the size of the
  # first two fields of the RIFF header.

  # RIFF Format.
  total_size = _RIFF_FORMAT_DESCRIPTOR_SIZE
  # 'fmt ' chunk.
  total_size += _FMT_CHUNK_HEADER_SIZE + _FMT_CHUNK_DATA_SIZE
  # 'data' chunk.
  total_size += _DATA_CHUNK_HEADER_SIZE + scaled_signal.nbytes

  f = file(file_name, 'w')
  try:
    f.write('RIFF')
    f.write(struct.pack('<L', total_size))
    f.write('WAVEfmt ')
    bitrate = sample_rate * num_channels * (bitdepth / 8)
    bits_per_sample = num_channels * (bitdepth / 8)
    f.write(struct.pack('<LHHLLHH', 16, 1, num_channels, sample_rate, bitrate,
                        bits_per_sample, bitdepth))
    f.write('data')
    f.write(struct.pack('<L', scaled_signal.nbytes))
    scaled_signal.tofile(f)
  finally:
    f.close()

values = numpy.array([int(line) for line in sys.stdin])
values = (values - 128.0) / 128.0
WriteWavFile(values, 31250, sys.argv[1])
