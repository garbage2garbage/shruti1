// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Googley logging/assert functions. Used for the unit tests and for making
// development/debugging on a non-AVR platform (eg OS X) easier.

#include "logging.h"

#ifdef __TEST__

namespace google_logging {

/* extern */
char* kSegmentFaultCauser = "Used to cause artificial segmentation fault";

/* extern */
const char* severity_name[] = {
  AS_STRING(INFO), AS_STRING(WARNING), AS_STRING(ERROR), AS_STRING(FATAL)
};

}  // namespace google_logging

#endif __TEST__