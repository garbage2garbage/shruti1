// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Template class for converting a power of 2 to its logarithm in base 2.

#ifndef HARDWARE_IO_LOG2_
#define HARDWARE_IO_LOG2_

namespace hardware_io {

template<uint8_t x>
struct Log2 {
  enum {
    value = 0
  };
};

template<> struct Log2<1> { enum { value = 0 }; };
template<> struct Log2<2> { enum { value = 1 }; };
template<> struct Log2<4> { enum { value = 2 }; };
template<> struct Log2<8> { enum { value = 3 }; };
template<> struct Log2<16> { enum { value = 4 }; };
template<> struct Log2<32> { enum { value = 5 }; };
template<> struct Log2<64> { enum { value = 6 }; };
template<> struct Log2<128> { enum { value = 7 }; };

}  // namespace hardware_io

#endif   // HARDWARE_IO_LOG2_
