#!/usr/bin/python2.5
#
# Copyright 2009 Olivier Gillet (ol.gillet@gmail.com). All rights reserved
#
# Custom characters definitions.

def StringImageToBytes(string_image):
  """Converts an "ASCII art" image into character definition data.
  
  The output is a list of bytes, and the image is made of spaces or . and X.
  """
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
.....
.....
.....
.....
.....
.....
.....
.....
""",
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
..X..
..X..
.XXX.
X...X
XXXXX
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
.X... 
X.X..
.X..X 
X.X.X 
...XX 
..X.X 
X...X 
.XXX. 
""",
"""
.XXX.
X...X
XXXXX
X...X
X.XXX
X.X.X
X.X.X
.....
"""]

bytes = sum([StringImageToBytes(image) for image in special_characters], [])
characters = [
    ('special_characters', bytes)
]
