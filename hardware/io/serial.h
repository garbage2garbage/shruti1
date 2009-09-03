// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Fast serial (for the onboard UART), using compile time optimizations.
//
// Can be used in buffered mode, or in polled mode (to save space or avoid
// interruptions if there is an even higher priority task than the serial I/O).
//
// Usage:
//
// Set-up:
// typedef Serial<SerialPort0, 31250, BUFFERED, POLLED> Serial;
// then, in the setup() hook: Serial::Init()
//
// Write:
// Serial::Write(40)  // Will block until data is written.
// write_has_succeeded = Serial::NonBlockingWrite(40)  // Will not block.
//
// Buffer manipulation (for buffered I/O):
// Serial::available()  // Number of bytes ready to be read. For polled read too
// my_value = Serial::Read()  // Will wait until data arrives.
// my_value = Serial::NonBlockingRead()  // Will return -1 if no data is there.
//
// Flushing a buffer:
// Serial::InputBuffer::Flush()
//
// TODO(pichenettes): Buffered writes not supported for now (should look up
// the right interrupt handler).

#ifndef HARDWARE_IO_SERIAL_H_
#define HARDWARE_IO_SERIAL_H_

#include "hardware/io/io.h"
#include "hardware/io/pin.h"

namespace hardware_io {

const uint8_t kSerialOutputBufferSize = 32;
const uint8_t kSerialInputBufferSize = 2;

// Low-level interface to the low-level I/O registers. Several specializations
// may be declared for each serial port. This class could theoretically be used
// for non-blocking write or polling reads.
template<typename TxEnableBit, typename TxReadyBit,
         typename RxEnableBit, typename RxReadyBit,
         typename RxInterruptBit,
         typename PrescalerRegisterH, typename PrescalerRegisterL,
         typename DataRegister,
         uint8_t input_buffer_size_,
         uint8_t output_buffer_size_>
struct SerialPort {
  typedef TxEnableBit Tx;
  typedef RxEnableBit Rx;
  typedef RxInterruptBit RxInterrupt;
  enum {
    input_buffer_size = input_buffer_size_,
    output_buffer_size = output_buffer_size_
  };
  static inline void set_prescaler(uint16_t value) {
    *PrescalerRegisterH::ptr() = value >> 8;
    *PrescalerRegisterL::ptr() = value;
  }
  static inline uint8_t tx_ready() { return TxReadyBit::value(); }
  static inline uint8_t rx_ready() { return RxReadyBit::value(); }
  static inline uint8_t data() { return *DataRegister::ptr(); }
  static inline void set_data(uint8_t value) { *DataRegister::ptr() = value; }
};

template<typename SerialPort>
struct SerialInput : public Input {
  enum {
    buffer_size = SerialPort::input_buffer_size,
    data_size = 8
  };
  typedef uint8_t Value;
  
  // Blocking!
  static inline Value Read() { while (!readable()); return ImmediateRead(); }  
  
  // Number of bytes available for read.
  static inline uint8_t readable() { return SerialPort::rx_ready(); }
  
  // A byte, or -1 if reading failed.
  static inline int16_t NonBlockingRead() { return readable() ? Read() : -1; }
  
  // No check for ready state.
  static inline Value ImmediateRead() { return SerialPort::data(); }
  
  // Called in data reception interrupt.
  static inline void Received() {
    // This will discard data if the buffer is full.
    Buffer<SerialInput<SerialPort> >::NonBlockingWrite(ImmediateRead());
  }
};

template<typename SerialPort>
struct SerialOutput : public Output {
  enum {
    buffer_size = SerialPort::output_buffer_size,
    data_size = 8
  };
  typedef uint8_t Value;
  
  // Blocking!
  static inline void Write(Value v) { while (!writable()); Overwrite(v); }
  
  // Number of bytes that can be fed.
  static inline uint8_t writable() { return SerialPort::tx_ready(); }
  
  // 1 if success.
  static inline uint8_t NonBlockingWrite(Value v) {
    if (!writable()) {
      return 0;
    }
    Overwrite(v);
    return 1;
  } 
  
  // No check for ready state.
  static inline void Overwrite(Value v) { SerialPort::set_data(v); }
  
  // Called in data emission interrupt.
  static inline Value Requested() {
    Value v = Buffer<SerialOutput<SerialPort> >::NonBlockingRead();
    if (v >= 0) {
      Overwrite(v);
    }
  }
};

template<typename SerialPort, PortMode input = POLLED, PortMode output = POLLED>
struct SerialImplementation { };

template<typename SerialPort>
struct SerialImplementation<SerialPort, DISABLED, DISABLED> {
  typedef InputOutput<DisabledInput, DisabledOutput> IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, DISABLED, POLLED> {
  typedef InputOutput<DisabledInput, SerialOutput<SerialPort> > IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, DISABLED, BUFFERED> {
  typedef Buffer<SerialOutput<SerialPort> > OutputBuffer;
  typedef InputOutput<DisabledInput, OutputBuffer > IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, POLLED, DISABLED> {
  typedef InputOutput<SerialInput<SerialPort>, DisabledOutput> IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, POLLED, POLLED> {
  typedef InputOutput<SerialInput<SerialPort>, SerialOutput<SerialPort> > IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, POLLED, BUFFERED> {
  typedef Buffer<SerialOutput<SerialPort> > OutputBuffer;
  typedef InputOutput<SerialInput<SerialPort>, OutputBuffer> IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, BUFFERED, DISABLED> {
  typedef Buffer<SerialInput<SerialPort> > InputBuffer;
  typedef InputOutput<InputBuffer, DisabledOutput> IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, BUFFERED, POLLED> {
  typedef Buffer<SerialInput<SerialPort> > InputBuffer;
  typedef InputOutput<InputBuffer, SerialOutput<SerialPort> > IO;
};
template<typename SerialPort>
struct SerialImplementation<SerialPort, BUFFERED, BUFFERED> {
  typedef Buffer<SerialInput<SerialPort> > InputBuffer;
  typedef Buffer<SerialOutput<SerialPort> > OutputBuffer;
  typedef InputOutput<InputBuffer, OutputBuffer> IO;
};

template<typename SerialPort, uint16_t baud_rate, PortMode input = POLLED,
         PortMode output = POLLED>
struct Serial {
  typedef SerialImplementation<SerialPort, input, output> Impl;
  typedef uint8_t Value;
  typedef typename Impl::IO::Input Input;
  typedef typename Impl::IO::Output Output;
  static inline void Init() {
    uint16_t prescaler = (F_CPU / 16 + baud_rate / 2) / baud_rate - 1;
    SerialPort::set_prescaler(prescaler);
    if (output != DISABLED) {
      SerialPort::Tx::set();
    }
    if (input != DISABLED) {
      SerialPort::Rx::set();
    }
    if (input == BUFFERED) {
      SerialPort::RxInterrupt::set();
    }
  }
  static inline void Write(Value v) { Impl::IO::Write(v); }
  static inline uint8_t writable() { return Impl::IO::writable(); }
  static inline uint8_t NonBlockingWrite(Value v ) {
    return Impl::IO::NonBlockingWrite(v);
  }
  static inline void Overwrite(Value v) { Impl::IO::Overwrite(v); }
  static inline Value Read() { return Impl::IO::Read(); }
  static inline uint8_t readable() { return Impl::IO::readable(); }
  static inline int16_t NonBlockingRead() {
    return Impl::IO::NonBlockingRead();
  }
  static inline Value ImmediateRead() { return Impl::IO::ImmediateRead(); }
};

IORegister(UBRR0H);
IORegister(UBRR0L);
IORegister(UCSR0A);
IORegister(UCSR0B);
IORegister(UDR0);

typedef SerialPort<
    BitInRegister<UCSR0BRegister, TXEN0>,
    BitInRegister<UCSR0ARegister, UDRE0>,
    BitInRegister<UCSR0BRegister, RXEN0>,
    BitInRegister<UCSR0ARegister, RXC0>,
    BitInRegister<UCSR0BRegister, RXCIE0>,
    UBRR0HRegister,
    UBRR0LRegister,
    UDR0Register,
    kSerialOutputBufferSize,
    kSerialInputBufferSize> SerialPort0;

}  // namespace hardware_io

#endif HARDWARE_IO_SERIAL_H_
