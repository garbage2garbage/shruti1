// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
// 
// Audio output. Supports PWM and DAC.

#ifndef HARDWARE_IO_AUDIO_OUTPUT_H_
#define HARDWARE_IO_AUDIO_OUTPUT_H_

#include "hardware/base/base.h"
#include "hardware/io/io.h"

namespace hardware_io {

enum UnderrunPolicy {
  EMIT_CLICK = 0,
  HOLD_SAMPLE = 1
};

template<typename OutputPort,
         uint8_t buffer_size_ = 32,
         uint8_t block_size = 16,
         UnderrunPolicy underrun_policy = HOLD_SAMPLE>
class AudioOutput {
 public:
	enum {
		buffer_size = buffer_size_,
		data_size = OutputPort::data_size,
	};
	typedef AudioOutput<OutputPort, buffer_size_, block_size, underrun_policy> Me;
  typedef typename DataTypeForSize<data_size>::Type Value;
  typedef Buffer<Me> OutputBuffer;
  
  static inline void Init() {
    OutputPort::Init();
  }
  
	static inline void Write(Value v) { while (!writable()); Overwrite(v); }
	static inline void Overwrite(Value v) { OutputBuffer::Overwrite(v); }
	
	static inline uint8_t writable() { return OutputBuffer::writable(); }
	static inline uint8_t writable_block() {
    return OutputBuffer::writable() >= block_size;
  }
	static inline uint8_t NonBlockingWrite(Value v) {
		if (!writable()) {
			return 0;
		}
		Overwrite(v);
		return 1;
	}
	
	// Called in data emission interrupt.
	static inline void EmitSample() {
    int16_t v = OutputBuffer::NonBlockingRead();
    if (v >= 0) {
      OutputPort::Write(Value(v));
    } else {
      ++num_glitches_;
      if (underrun_policy == EMIT_CLICK) {
        OutputPort::Write(0);  // Introduces clicks for debugging purposes.
      }
    }
	}
  static uint16_t num_glitches() { return num_glitches_; }

 private:
  static uint16_t num_glitches_;
  DISALLOW_COPY_AND_ASSIGN(AudioOutput);
};

/* static */
template<typename OutputPort, uint8_t buffer_size_, uint8_t block_size,
         UnderrunPolicy underrun_policy>
uint16_t AudioOutput<OutputPort, buffer_size_, block_size,
                     underrun_policy>::num_glitches_ = 0;

}  // namespace hardware_io

#endif  // HARDWARE_IO_AUDIO_OUTPUT_H_
