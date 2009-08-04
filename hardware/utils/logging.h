// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Googley logging/assert functions. Used for the unit tests and for making
// development/debugging on a non-AVR platform (eg OS X) easier.

#ifndef HARDWARE_UTILS_LOGGING_H_
#define HARDWARE_UTILS_LOGGING_H_

// Use this only for the test/desktop target.
#ifdef __TEST__

#include <time.h>
#include <iostream>

namespace hardware_utils {

// Googley logging/assert macros, taken from Google's open-source projects
// eg: http://code.google.com/p/plda/source/browse/trunk/common.h
extern char* kSegmentFaultCauser;

enum LogSeverity { INFO = 0, WARNING, ERROR, FATAL };

#define AS_STRING(name) # name

extern const char* severity_name[];

class Logger {
 public:
  Logger(LogSeverity ls, const std::string& file, int line)
      : ls_(ls), file_(file), line_(line) {}
  std::ostream& stream() const {
    return std::cout << severity_name[ls_] << "\t" << file_
                     << "\t" << line_ << "\t" << clock() << "\t";
  }
  ~Logger() {
    std::cout << std::endl;
    if (ls_ == FATAL) {
      *kSegmentFaultCauser = '\0';
    }
  }
 private:
  LogSeverity ls_;
  std::string file_;
  int line_;
};

#define LOG(ls) hardware_utils::Logger(ls, __FILE__, __LINE__).stream()

#define CHECK(a) if (!(a)) { \
    LOG(hardware_utils::FATAL) << "CHECK failed\n\t" << #a << " = " << (a); \
  }

#define CHECK_EQ(a, b) if ((a) != (b)) { \
    LOG(hardware_utils::FATAL) << "CHECK_EQ failed\n\t" << #a << " = " << (a) \
               << "\n\t" << #b << " = " << (b); \
  }

#define CHECK_NE(a, b) if ((a) == (b)) { \
    LOG(hardware_utils::FATAL) << "CHECK_NE failed\n\t" << #a << " = " << (a) \
               << "\n\t" << #b << " = " << (b); \
  }

#define CHECK_GT(a, b) if ((a) <= (b)) { \
    LOG(hardware_utils::FATAL) << "CHECK_GT failed\n\t" << #a << " = " << (a) \
               << "\n\t" << #b << " = " << (b); \
  }

#define CHECK_LT(a, b) if ((a) >= (b)) { \
    LOG(hardware_utils::FATAL) << "CHECK_LT failed\n\t" << #a << " = " << (a) \
               << "\n\t" << #b << " = " << (b); \
  }

#define CHECK_GE(a, b) if ((a) < (b)) { \
    LOG(hardware_utils::FATAL) << "CHECK_GE failed\n\t" << #a << " = " << (a) \
               << "\n\t" << #b << " = " << (b); \
  }

#define CHECK_LE(a, b) if ((a) > (b)) { \
    LOG(hardware_utils::FATAL) << "CHECK_LE failed\n\t" << #a << " = " << (a) \
               << "\n\t" << #b << " = " << (b); \
  }

// Same as check, but will also be built into the AVR code, in which case this
// aborts the current (void) function.
#define ARGUMENT_CHECK(a) CHECK(a)
#define ARGUMENT_CHECK_EQ(a, b) CHECK_EQ(a, b)
#define ARGUMENT_CHECK_NE(a, b) CHECK_NE(a, b)
#define ARGUMENT_CHECK_GT(a, b) CHECK_GT(a, b)
#define ARGUMENT_CHECK_LT(a, b) CHECK_LT(a, b)
#define ARGUMENT_CHECK_GE(a, b) CHECK_GE(a, b)
#define ARGUMENT_CHECK_LE(a, b) CHECK_LE(a, b)

}  // namespace hardware_utils

#else

struct NullStream {
  template<typename T>
  NullStream& operator<<(T arg) { return *this; }
};

#define CHECK(a)
#define CHECK_EQ(a, b)
#define CHECK_NE(a, b)
#define CHECK_GT(a, b)
#define CHECK_LT(a, b)
#define CHECK_GE(a, b)
#define CHECK_LE(a, b)
#define LOG(ls) NullStream()

// Same as check, but will also be built into the AVR code, in which case this
// aborts the current (void) function.
#define ARGUMENT_CHECK(a) if (!(a)) { return; }
#define ARGUMENT_CHECK_EQ(a, b) if ((a) != (b)) { return; }
#define ARGUMENT_CHECK_NE(a, b) if ((a) == (b)) { return; }
#define ARGUMENT_CHECK_GT(a, b) if ((a) <= (b)) { return; }
#define ARGUMENT_CHECK_LT(a, b) if ((a) >= (b)) { return; }
#define ARGUMENT_CHECK_GE(a, b) if ((a) < (b)) { return; }
#define ARGUMENT_CHECK_LE(a, b) if ((a) > (b)) { return; }

#endif // __TEST__

#define PPBYTE(x) int(x) << " ('" << char(x) << "')";

#endif  // HARDWARE_UTILS_LOGGING_H_
