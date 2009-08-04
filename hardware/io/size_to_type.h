// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Template class for converting an integer template argument to the
// corresponding size type.

#ifndef HARDWARE_IO_SIZE_TO_TYPE_H_
#define HARDWARE_IO_SIZE_TO_TYPE_H_

namespace hardware_io {

template<uint8_t size>
struct DataTypeForSize {
  typedef int16_t Type;
};

template<> struct DataTypeForSize<1> { typedef uint8_t Type; };
template<> struct DataTypeForSize<2> { typedef uint8_t Type; };
template<> struct DataTypeForSize<3> { typedef uint8_t Type; };
template<> struct DataTypeForSize<4> { typedef uint8_t Type; };
template<> struct DataTypeForSize<5> { typedef uint8_t Type; };
template<> struct DataTypeForSize<6> { typedef uint8_t Type; };
template<> struct DataTypeForSize<7> { typedef uint8_t Type; };
template<> struct DataTypeForSize<8> { typedef uint8_t Type; };
  
}  // namespace hardware_io

#endif   // HARDWARE_IO_SIZE_TO_TYPE_H_
