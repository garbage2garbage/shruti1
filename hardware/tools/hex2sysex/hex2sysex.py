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
# Hex2SysEx utility

"""Hex2SysEx utility.

usage:
  python hex2sysex.py \
    [--page_size 64] \
    [--delay 200] \
    [--output_file path_to/firmware.mid] \
    path_to/firmware.hex
"""

import logging
import optparse
import os
import sys

# Allows the code to be run from the project root directory
sys.path.append('.')

from music.midi import midifile


def LoadHexFile(path):
  """Loads a Hex file."""

  data = []
  for line_number, line in enumerate(file(path)):
    line = line.strip()
    if len(line) < 9:
      logging.info('Line %(line_number)d: line too short' % locals())
      return None

    if not all(x in '0123456789abcdefABCDEF' for x in line[1:]):
      logging.info('Line %(line_number)d: unknown character' % locals())
      return None

    bytes = [int(line[i:i+2], 16) for i in xrange(1, len(line), 2)]
    if bytes[0] != len(bytes) - 5:
      logging.info('Line %(line_number)d: invalid byte count' % locals())
      return None

    if sum(bytes) % 256 != 0:
      logging.info('Line %(line_number)d: corrupted line' % locals())
      return None

    if bytes[3] == 1:
      if bytes[0] != 0 or bytes[1] != 0 or bytes[2] != 0:
        logging.info('Line %(line_number)d: invalid end of file' % locals())
        return None
      else:
        break
    elif bytes[3] == 0:
      address = bytes[1] << 8 | bytes[2]
      padding_size = address + bytes[0] - len(data)
      if padding_size > 0:
        data += ['\x00'] * padding_size
      data[address:address + bytes[0]] = bytes[4:-1]
  return data


def CreateMidifile(
    input_file_name,
    data,
    output_file,
    options):
  size = len(data)
  page_size = options.page_size
  delay = options.delay
  _, input_file_name = os.path.split(input_file_name)
  comments = [
      'Warning: contains OS data!',
      'Created from %(input_file_name)s' % locals(),
      'Size: %(size)d' % locals(),
      'Page size: %(page_size)d' % locals(),
      'Delay: %(delay)d ms' % locals()]
  m = midifile.MidiFile()
  m.AddTrack().AddEvent(0, midifile.TempoEvent(120.0))
  for comment in comments:
    m.AddTrack().AddEvent(0, midifile.TextEvent(comment))
  t = m.AddTrack()
  page_size *= 2  # Converts from words to bytes
  time = 0
  for i in xrange(0, size, page_size):
    block = ''.join(map(chr, data[i:i+page_size]))
    padding = page_size - len(block)
    block += '\x00' * padding
    t.AddEvent(time, midifile.SysExEvent(
        options.manufacturer_id,
        options.device_id,
        options.update_command + midifile.Nibblize(block)))
    # ms -> s -> beats -> ticks
    time += int(delay / 1000.0 / 0.5 * 96)
  t.AddEvent(time, midifile.SysExEvent(
      options.manufacturer_id,
      options.device_id,
      options.reset_command))
  f = file(output_file, 'w')
  m.Write(f, format=1)
  f.close()


if __name__ == '__main__':
  parser = optparse.OptionParser()
  parser.add_option(
      '-p',
      '--page_size',
      dest='page_size',
      type='int',
      default=64,
      help='Flash page size in words')
  parser.add_option(
      '-d',
      '--delay',
      dest='delay',
      type='int',
      default=200,
      help='Delay between pages in milliseconds')
  parser.add_option(
      '-o',
      '--output_file',
      dest='output_file',
      default=None,
      help='Write output file to FILE',
      metavar='FILE')
  parser.add_option(
      '-m',
      '--manufacturer_id',
      dest='manufacturer_id',
      default='\x00\x20\x77',
      help='Manufacturer ID to use in SysEx message')
  parser.add_option(
      '-v',
      '--device_id',
      dest='device_id',
      default='\x00\x01',
      help='Device ID to use in SysEx message')
  parser.add_option(
      '-u',
      '--update_command',
      dest='update_command',
      default='\x7e\x00',
      help='OS update SysEx command')
  parser.add_option(
      '-r',
      '--reset_command',
      dest='reset_command',
      default='\x7f\x00',
      help='Post-OS update reset SysEx command')

  options, args = parser.parse_args()
  if len(args) != 1:
    logging.fatal('Specify one, and only one firmware .hex file!')
    sys.exit(1)

  data = LoadHexFile(args[0])
  if not data:
    logging.fatal('Error while loading .hex file')
    sys.exit(2)

  output_file = options.output_file
  if not output_file:
    if '.hex' in args[0]:
      output_file = args[0].replace('.hex', '.mid')
    else:
      output_file = args[0] + '.mid'

  CreateMidifile(
      args[0],
      data,
      output_file,
      options)
