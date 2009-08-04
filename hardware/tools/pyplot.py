#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# Simple matlib plotter.

import sys
import numpy
import pylab

values = [int(line) for line in sys.stdin]
pylab.plot(values)
pylab.show()
