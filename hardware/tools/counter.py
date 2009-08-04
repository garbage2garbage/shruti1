cycles = {
   "add": 1,
   "adc": 1,
   "adiw": 2,
   "sub": 1,
   "subi": 1,
   "sbc": 1,
   "sbci": 1,
   "sbiw": 2,
   "and": 1,
   "andi": 1,
   "or": 1,
   "ori": 1,
   "eor": 1,
   "com": 1,
   "neg": 1,
   "sbr": 1,
   "cbr": 1,
   "inc": 1,
   "dec": 1,
   "tst": 1,
   "clr": 1,
   "ser": 1,
   "mul": 2,
   "muls": 2,
   "mulsu": 2,
   "fmul": 2,
   "fmuls": 2,
   "fmulsu": 2,
   "rjmp": 2,
   "ijmp": 2,
   "jmp": 3,
   "rcall": 3,
   "icall": 3,
   "call": 4,
   "ret": 4,
   "reti": 4,
   "cpse": 2,
   "cp": 1,
   "cpc": 1,
   "cpi": 1,
   "sbrc": 2,
   "sbrs": 2,
   "sbic": 2,
   "sbis": 2,
   "brbs": 2,
   "brbc": 2,
   "breq": 2,
   "brne": 2,
   "brcs": 2,
   "brcc": 2,
   "brsh": 2,
   "brlo": 2,
   "brmi": 2,
   "brpl": 2,
   "brge": 2,
   "brlt": 2,
   "brhs": 2,
   "brhc": 2,
   "brts": 2,
   "brtc": 2,
   "brvs": 2,
   "brvc": 2,
   "brie": 2,
   "brid": 2,
   "sbi": 2,
   "cbi": 2,
   "lsl": 1,
   "lsr": 1,
   "rol": 1,
   "ror": 1,
   "asr": 1,
   "swap": 1,
   "bset": 1,
   "bclr": 1,
   "bst": 1,
   "bld": 1,
   "sec": 1,
   "clc": 1,
   "sen": 1,
   "cln": 1,
   "sez": 1,
   "clz": 1,
   "sei": 1,
   "cli": 1,
   "ses": 1,
   "cls": 1,
   "sev": 1,
   "clv": 1,
   "set": 1,
   "clt": 1,
   "seh": 1,
   "clh": 1,
   "mov": 1,
   "movw": 1,
   "ldi": 2,
   "ld": 1,
   "ldd": 2,
   "lds": 2,
   "st": 2,
   "std": 2,
   "sts": 2,
   "lpm": 3,
   "in": 1,
   "out": 1,
   "push": 2,
   "pop": 2,
   "nop": 1,
   "sleep": 1,
   "wdr": 1
}

code = """
  data_.fm.modulator_phase += data_.fm.modulator_phase_increment;
  561c:	ea ee       	ldi	r30, 0xEA	; 234
  561e:	f4 e0       	ldi	r31, 0x04	; 4
  5620:	80 91 ea 04 	lds	r24, 0x04EA
  5624:	90 91 eb 04 	lds	r25, 0x04EB
  5628:	32 91       	ld	r19, -Z
  562a:	22 91       	ld	r18, -Z
  562c:	82 0f       	add	r24, r18
  562e:	93 1f       	adc	r25, r19
  5630:	91 83       	std	Z+1, r25	; 0x01
  5632:	80 83       	st	Z, r24
  int8_t modulator = ReadSample(waveform_table[WAV_RES_SINE],
                                data_.fm.modulator_phase) - 128;
  5634:	60 91 17 01 	lds	r22, 0x0117
  5638:	70 91 18 01 	lds	r23, 0x0118
  563c:	fb 01       	movw	r30, r22
  563e:	e9 0f       	add	r30, r25
  5640:	f1 1d       	adc	r31, r1
  5642:	e4 91       	lpm	r30, Z+
  int16_t modulation = SignedMulScale4(modulator, parameter_);
  phase_ += phase_increment_ + modulation;
  5644:	20 91 c8 04 	lds	r18, 0x04C8
  5648:	30 91 c9 04 	lds	r19, 0x04C9
  564c:	80 91 c6 04 	lds	r24, 0x04C6
  5650:	90 91 c7 04 	lds	r25, 0x04C7
  5654:	28 0f       	add	r18, r24
  5656:	39 1f       	adc	r19, r25
  5658:	e0 58       	subi	r30, 0x80	; 128
  565a:	4e 2f       	mov	r20, r30
  565c:	55 27       	eor	r21, r21
  565e:	47 fd       	sbrc	r20, 7
  5660:	50 95       	com	r21
  5662:	80 91 dd 04 	lds	r24, 0x04DD
  5666:	90 e0       	ldi	r25, 0x00	; 0
  5668:	fc 01       	movw	r30, r24
  566a:	4e 9f       	mul	r20, r30
  566c:	c0 01       	movw	r24, r0
  566e:	4f 9f       	mul	r20, r31
  5670:	90 0d       	add	r25, r0
  5672:	5e 9f       	mul	r21, r30
  5674:	90 0d       	add	r25, r0
  5676:	11 24       	eor	r1, r1
  5678:	e4 e0       	ldi	r30, 0x04	; 4
  567a:	95 95       	asr	r25
  567c:	87 95       	ror	r24
  567e:	ea 95       	dec	r30
  5680:	e1 f7       	brne	.-8      	; 0x567a <_ZN15hardware_shruti10OscillatorILi1ELb0EE8RenderFmEv+0x5e>
  5682:	28 0f       	add	r18, r24
  5684:	39 1f       	adc	r19, r25
  5686:	30 93 c7 04 	sts	0x04C7, r19
  568a:	20 93 c6 04 	sts	0x04C6, r18
static inline uint8_t InterpolateSample(const prog_uint8_t* table,
                                        uint16_t phase) {
  return Mix(
      ResourcesManager::LookupSample(table, uint8_t(phase >> 8)),
      ResourcesManager::LookupSample(table, uint8_t(1 + (phase >> 8))),
      phase & 0xff);
  568e:	83 2f       	mov	r24, r19
  5690:	fb 01       	movw	r30, r22
  5692:	e3 0f       	add	r30, r19
  5694:	f1 1d       	adc	r31, r1
  5696:	94 91       	lpm	r25, Z+
  5698:	8f 5f       	subi	r24, 0xFF	; 255
  569a:	68 0f       	add	r22, r24
  569c:	71 1d       	adc	r23, r1
  569e:	fb 01       	movw	r30, r22
  56a0:	34 91       	lpm	r19, Z+
  56a2:	82 2f       	mov	r24, r18
  56a4:	80 95       	com	r24
  56a6:	89 9f       	mul	r24, r25
  56a8:	c0 01       	movw	r24, r0
  56aa:	11 24       	eor	r1, r1
  56ac:	23 9f       	mul	r18, r19
  56ae:	90 01       	movw	r18, r0
  56b0:	11 24       	eor	r1, r1
  56b2:	82 0f       	add	r24, r18
  56b4:	93 1f       	adc	r25, r19
  int8_t modulator = ReadSample(waveform_table[WAV_RES_SINE],
                                data_.fm.modulator_phase) - 128;
  int16_t modulation = SignedMulScale4(modulator, parameter_);
  phase_ += phase_increment_ + modulation;
  return InterpolateSample(waveform_table[WAV_RES_SINE], phase_);
}
  56b6:	89 2f       	mov	r24, r25
  56b8:	08 95       	ret
"""
total = 0
for line in code.split('\n'):
  items = [x.strip() for x in line.split('\t')]
  line_count = 0
  if len(items[0]) == 5 and items[0][-1] == ':':
    for x in items:
      if x in cycles:
        line_count = max(line_count, cycles[x])
    print '\t'.join(items) + '  -- ' + str(line_count)
    total += line_count

print total