state = 0
count = 0

source = file('build/shruti.lss')

for i, line in enumerate(source):
  if state == 0 or state == 2:
    if '\tmul\t' in line:
      state += 1
    else:
      state = 0
  elif state == 1 or state == 3:
    if '\tadd\t' in line:
      if state == 1:
        state += 1
      else:
        print i - 3
        count += 1
    else:
      state = 0
print count, 'bad muls'
