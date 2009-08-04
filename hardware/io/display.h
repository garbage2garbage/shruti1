// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
// 
// Driver for a 2x16 LCD display, with double buffering. All updates to the
// content of the screen are done in an in-memory "local" text page. A "remote"
// text page mirrors the current state of the LCD display. A timer (the same as
// for the audio rendering) periodically scans the local and remote pages for
// differences, transmit serially the modified character in the local page to
// the LCD, and update the remote buffer to reflect that the character was
// transmitted.

#ifndef HARDWARE_IO_DISPLAY_H_
#define HARDWARE_IO_DISPLAY_H_

#include "hardware/base/base.h"
#include "hardware/io/software_serial.h"

namespace hardware_io {

static const uint8_t kLcdWidth = 16;
static const uint8_t kLcdWidthWrap = kLcdWidth - 1;
static const uint8_t kLcdWidthShift = 4;
static const uint8_t kLcdLines = 2;
static const uint8_t kLcdBufferSize = kLcdWidth * kLcdLines;
static const uint8_t kLcdBufferSizeWrap = kLcdWidth * kLcdLines - 1;
static const uint8_t kLcdLineShift = 2;

static const uint16_t kLcdBaudRate = 2400;
static const uint8_t kLcdNoCursor = 0xff;
static const uint8_t kLcdCursorBlinkRate = 0x7f;
static const uint8_t kLcdCursor = 0xff;

static const uint8_t kPinLcdTx = 2;
static const uint16_t kMainTimerRate = 31250;

class Display {
  // TODO(oliviergillet): make this class static, with all the parameters as
  // template arguments.
  typedef BufferedSoftwareSerialOutput<
      Pin<kPinLcdTx>,
      kMainTimerRate,
      kLcdBaudRate,
      8> DisplaySerialOutput;

  // To ensure that the LCD display is not in a weird state (eg. set to 9600),
  // send a "Set baud rate to 2400" message at 9600 at startup. At worst, it
  // will be misunderstood and will print weird characters for a short moment.
  typedef SoftwareSerialOutput<
      Pin<kPinLcdTx>,
      9600> DisplayPanicSerialOutput;

 public:
  Display();
  void Init();
  void Print(uint8_t line, const char* text);
  void SetBrightness(uint8_t brightness);  // 0 to 29.
  void SetCustomCharMap(const uint8_t* characters, uint8_t num_characters);
  // Use kLcdNoCursor (255) or any other value outside of the screen to hide.
  inline void set_cursor_position(uint8_t cursor) { cursor_position_ = cursor; }
  inline void set_status(uint8_t status) {
    // TODO(oliviergillet): we're using the same clock for blinking the cursor
    // and the status indicator. ewwww...
    blink_clock_ = 0;
    status_ = status + 1;
    // Make sure that the next character to be redrawn will be the status char.
    scan_position_ = local_[0] == ' ' ? 0 : kLcdWidthWrap;
  }

  inline void Tick() { DisplaySerialOutput::Tick(); }
  void Update();

 private:
  // Writes a pair of related bytes (command/argument). The main purpose of
  // this function is simply to avoid the write function to be inlined too
  // often.
  void WriteCommand(uint8_t command, uint8_t argument);
  // Character pages storing what the display currently shows (remote), and
  // what it ought to show (local).
  uint8_t local_[kLcdBufferSize];
  uint8_t remote_[kLcdBufferSize];

  // Position of the last character being transmitted.
  uint8_t scan_position_;
  uint8_t scan_position_last_write_;
  uint8_t blink_;
  uint8_t blink_clock_;
  uint8_t cursor_position_;
  uint8_t status_;

  DISALLOW_COPY_AND_ASSIGN(Display);
};

}  // namespace hardware_io

#endif   // HARDWARE_IO_DISPLAY_H_
