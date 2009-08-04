#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# Simple matlib plotter.

import sys

symbol = None
start = 0
symbols = []
for line in file(sys.argv[1]):
  parts = line.split()
  address = int(parts[0], 16)
  name = parts[2]
  if symbol and symbol[:2] != '__':
    size = address - start
    if size > 28 and size < 32768:
      symbols.append((size, symbol))
  symbol = name
  start = address

for size, symbol in sorted(symbols, reverse=True):
  print symbol, size
