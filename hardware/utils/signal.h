// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Signal processing functions for the synthesis engine.

#ifndef HARDWARE_UTILS_SIGNAL_H_
#define HARDWARE_UTILS_SIGNAL_H_

#include "hardware/base/base.h"

namespace hardware_utils {
  
struct Signal {
  static inline int16_t Clip(int16_t value, int16_t min, int16_t max) {
    return value < min ? min : (value > max ? max : value);
  }
  
  static inline uint8_t AddClip(uint8_t value, uint8_t increment, uint8_t max) {
    value += increment;
    if (value > max) {
      value = max;
    }
    return value;
  }
  
  
#ifdef FAST_SIGNAL_PROCESSING
  static inline uint8_t Clip8(int16_t value) {
    uint8_t result;
    asm(
      "mov %0, %A1"   "\n\t"  // by default, copy the value.
      "or %B1, %B1"   "\n\t"  // load H to set flags.
      "brpl .+4"      "\n\t"  // if positive, skip
      "ldi %0, 0"     "\n\t"  // set to 0.
      "rjmp .+4"      "\n\t"  // and jump.
      "breq .+2"      "\n\t"  // if null, skip
      "ldi %0, 255"   "\n\t"  // set to 255
      : "=r" (result)
      : "a" (value)
    );
    return result;  
  }

  static inline int8_t SignedClip8(int16_t value) {
    return Clip8(value + 128) + 128;  
  }

  static inline uint8_t Mix(uint8_t a, uint8_t b, uint8_t balance) {
    union {
      uint16_t sum;
      uint8_t bytes[0];
    } sum;
    asm(
      "mul %3, %2"      "\n\t"  // b * balance
      "movw %A0, r0"    "\n\t"  // to sum
      "com %2"          "\n\t"  // 255 - balance
      "mul %1, %2"      "\n\t"  // a * (255 - balance)
      "com %2"          "\n\t"  // reset balance to its previous value
      "add %A0, r0"     "\n\t"  // add to sum L
      "adc %B0, r1"     "\n\t"  // add to sum H
      "eor r1, r1"      "\n\t"  // reset r1 after multiplication
      : "&=r" (sum)
      : "a" (a), "a" (balance), "a" (b)
      );
    return sum.bytes[1];
  }
  
  
  static inline uint8_t Mix4(uint8_t a, uint8_t b, uint8_t balance) {
    uint16_t sum;
    asm(
      "mul %2, %1"      "\n\t"  // b * balance
      "movw %A3, r0"    "\n\t"  // to sum
      "com %1"          "\n\t"  // 255 - balance
      "subi %1, 240"    "\n\t"  // 15 - balance
      "mul %0, %1"      "\n\t"  // a * (15 - balance)
      "subi %1, 16"     "\n\t"
      "com %1"          "\n\t"  // reset balance to its previous value
      "add %A3, r0"     "\n\t"  // add to sum L
      "adc %B3, r1"     "\n\t"  // add to sum H
      "eor r1, r1"      "\n\t"  // reset r1 after multiplication
      "andi %B3, 15"    "\n\t"  // keep 4 lowest bits of H
      "andi %A3, 240"   "\n\t"  // keep 4 highest bits of L 
      "or %B3, %A3"     "\n\t"  // copy 4 high bits of L to H -> LLLLHHHH
      "swap %B3"        "\n\t"  // swap to get HHHHLLLL
      "mov %0, %B3"     "\n\t"  // move to output
      : "=r" (a)
      : "a" (balance), "a" (b), "a" (sum)
      );
    return a;
  }

  static inline uint16_t UnscaledMix4(uint8_t a, uint8_t b, uint8_t balance) {
    uint16_t sum;
    asm(
      "mul %3, %2"      "\n\t"  // b * balance
      "movw %A0, r0"    "\n\t"  // to sum
      "com %2"          "\n\t"  // 255 - balance
      "subi %2, 240"    "\n\t"  // 15 - balance
      "mul %1, %2"      "\n\t"  // a * (15 - balance)
      "subi %2, 16"     "\n\t"
      "com %2"          "\n\t"  // reset balance to its previous value
      "add %A0, r0"     "\n\t"  // add to sum L
      "adc %B0, r1"     "\n\t"  // add to sum H
      "eor r1, r1"      "\n\t"  // reset r1 after multiplication
      : "+r" (sum)
      : "a" (a), "a" (balance), "a" (b)
      );
    return sum;
  }

  static inline uint8_t ShiftLeft4(uint8_t a) {
    uint8_t result;
    asm(
      "mov %0, %1"      "\n\t"
      "swap %0"         "\n\t"
      "andi %0, 240"     "\n\t"
      : "=r" (result)
      : "a" (a)
      );
    return result;
  }

  static inline uint8_t Swap4(uint8_t a) {
    uint8_t result;
    asm(
      "mov %0, %1"      "\n\t"
      "swap %0"         "\n\t"
      : "=r" (result)
      : "a" (a)
      );
    return result;
  }

  static inline uint8_t ShiftRight4(uint8_t a) {
    uint8_t result;
    asm(
      "mov %0, %1"      "\n\t"
      "swap %0"         "\n\t"
      "andi %0, 15"     "\n\t"
      : "=r" (result)
      : "a" (a)
      );
    return result;
  }
  
  static inline uint8_t MulScale8(uint8_t a, uint8_t b) {
    uint8_t result;
    asm(
      "mul %1, %2"      "\n\t"
      "mov %0, r1"      "\n\t"
      "eor r1, r1"      "\n\t"
      : "=r" (result)
      : "a" (a), "a" (b)
    );
    return result;
  }
  
  static inline uint16_t MulScale1(uint8_t a, uint8_t b) {
    uint16_t result;
    asm(
      "mul %1, %2"      "\n\t"
      "asr r1"          "\n\t"
      "ror r0"          "\n\t"
      "movw %A0, r0"    "\n\t"
      "eor r1, r1"      "\n\t"
      : "=r" (result)
      : "a" (a), "a" (b)
    );
    return result;
  }
  
  static inline int8_t SignedMulScale8(int8_t a, uint8_t b) {
    uint8_t result;
    asm(
      "mulsu %1, %2"    "\n\t"
      "mov %0, r1"      "\n\t"
      "eor r1, r1"      "\n\t"
      : "=r" (result)
      : "a" (a), "a" (b)
    );
    return result;
  }
  
  static inline int8_t SignedSignedMulScale8(int8_t a, int8_t b) {
    uint8_t result;
    asm(
      "muls %1, %2"    "\n\t"
      "mov %0, r1"      "\n\t"
      "eor r1, r1"      "\n\t"
      : "=r" (result)
      : "a" (a), "a" (b)
    );
    return result;
  }
  
  static inline int16_t SignedMulScale4(int8_t a, uint8_t b) {
    int16_t result;
    asm(
      "mulsu %1, %2"    "\n\t"
      "asr r1"          "\n\t"
      "ror r0"          "\n\t"
      "asr r1"          "\n\t"
      "ror r0"          "\n\t"
      "asr r1"          "\n\t"
      "ror r0"          "\n\t"
      "asr r1"          "\n\t"
      "ror r0"          "\n\t"
      "movw %A0, r0"    "\n\t"
      "eor r1, r1"      "\n\t"
      : "=r" (result)
      : "a" (a), "a" (b)
    );
    return result;
  }

#else

  static inline uint8_t Clip8(int16_t value) {
    return value < 0 ? 0 : (value > 255 ? 255 : value);
  }

  static inline int8_t SignedClip8(int16_t value) {
    return value < -128 ? -128 : (value > 127 ? 127 : value);
  }

  static inline uint8_t Mix(uint8_t a, uint8_t b, uint8_t balance) {
    return a * (255 - balance) + b * balance >> 8;
  }
  
  static inline uint8_t Mix4(uint8_t a, uint8_t b, uint8_t balance) {
    return a * (15 - balance) + b * balance >> 4;
  }
  
  static inline uint8_t UnscaledMix4(uint8_t a, uint8_t b, uint8_t balance) {
    return a * (15 - balance) + b * balance;
  }

  static inline uint8_t ShiftRight4(uint8_t a) {
    return a >> 4;
  }

  static inline uint8_t ShiftLeft4(uint8_t a) {
    return a << 4;
  }

  static inline uint8_t Swap4(uint8_t a) {
    return (a << 4) | (a >> 4);
  }
  
  static inline uint8_t MulScale8(uint8_t a, uint8_t b) {
    return a * b >> 8;
  }
  static inline uint16_t MulScale1(uint8_t a, uint8_t b) {
    return a * b >> 1;
  }

  static inline int8_t SignedMulScale8(int8_t a, uint8_t b) {
    return int8_t(a) * b >> 8;
  }
  
  static inline int8_t SignedSignedMulScale8(int8_t a, int8_t b) {
    return int8_t(a) * int8_t(b) >> 8;
  }
  static inline int16_t SignedMulScale4(int8_t a, uint8_t b) {
    return int16_t(int8_t(a) * uint8_t(b)) >> 4;
  }
  
#endif  // FAST_SIGNAL_PROCESSING
};

}  // namespace hardware_utils

#endif  // HARDWARE_UTILS_SIGNAL_H_