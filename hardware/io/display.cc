// Copyright 2009 Olivier Gillet. All rights reserved
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Driver for a 2x16 LCD display.

#include "hardware/io/display.h"

#include <string.h>

#include "hardware/resources/resources_manager.h"
#include "hardware/utils/logging.h"

using hardware_resources::SimpleResourcesManager;

namespace hardware_io {

Display::Display() {
  memset(local_, ' ', kLcdBufferSize);
  memset(remote_, '?', kLcdBufferSize);
  scan_position_last_write_ = 255;
  blink_ = 0;
  cursor_position_ = 255;
}

void Display::Init() {
  // Assume that the display is wrongly set to 9600, and send a command to
  // switch it to 2400 bps.
  // TODO(oliviergillet): add commands for other rates.
  if (kLcdBaudRate == 2400) {
    DisplayPanicSerialOutput::Write(124);
    DisplayPanicSerialOutput::Write(11);
  }
  DisplaySerialOutput::Init();
}

void Display::WriteCommand(uint8_t command, uint8_t argument) {
  if (command) {
    DisplaySerialOutput::Write(command);
  }
  DisplaySerialOutput::Write(argument);
}

void Display::SetCustomCharMap(
    const uint8_t* characters,
    uint8_t num_characters) {
  WriteCommand(0xfe, 0x01);
  for (uint8_t i = 0; i < num_characters; ++i) {
    WriteCommand(0xfe, 0x40 + i * 8);
    for (uint8_t j = 0; j < 8; ++j) {
      // The 6th bit is not used, so it is set to prevent character definition
      // data to be misunderstood with special commands.
      WriteCommand(0, 0x20 |  SimpleResourcesManager::Lookup<uint8_t, uint8_t>(
          characters, i * 8 + j));
    }
    WriteCommand(0xfe, 0x01);
  }
}

void Display::Print(uint8_t line, const char* text) {
  if (line == 0) {
    LOG(INFO) << "display\ttext\t+----------------+";
  }
  LOG(INFO) << "display\ttext\t|" << text << "|";
  if (line == 1) {
    LOG(INFO) << "display\ttext\t+----------------+";
  }
  uint8_t row = kLcdWidth;
  uint8_t* destination = local_ + (line << kLcdWidthShift);
  while (*text && row) {
    uint8_t character = *text;
    // Do not write control characters.
    if (character == 124 || character == 254 ||
        (character >= 8 && character < 32)) {
      *destination++ = ' ';
    } else {
      *destination++ = character;
    }
    ++text;
    --row;
  }
}

void Display::SetBrightness(uint8_t brightness) {
  WriteCommand(0x7c, 128 + brightness);
}

void Display::Update() {
  // The following code is likely to write 3 bytes at most. If there are less
  // than 3 bytes available for write in the output buffer, there's no reason
  // to take the risk to continue.
  if (DisplaySerialOutput::writable() < 3) {
    return;
  }
  // It is now safe that all write of 3 bytes to the display buffer will not
  // block.

  blink_clock_ = (blink_clock_ + 1) & kLcdCursorBlinkRate;
  if (blink_clock_ == 0) {
    blink_ = ~blink_;
    status_ = 0;
  }
  
  uint8_t character = 0;
  // Determine which character to show at the current position.
  // If the scan position is the cursor and it is shown (blinking), draw the
  // cursor.
  if (scan_position_ == cursor_position_ && blink_) {
    character = kLcdCursor;
  } else {
    // Otherwise, check if there's a status indicator to display. It is
    // displayed either on the left or right of the first line, depending on
    // the available space.
    if (status_ && (scan_position_ == 0 || scan_position_ == kLcdWidthWrap) &&
        local_[scan_position_] == ' ') {
      character = status_ - 1;
    } else {
      character = local_[scan_position_];
    }
  }
  // TODO(oliviergillet): check if we can get rid of the
  // scan_position_ == cursor_position_ condition (dead code?).
  if (character != remote_[scan_position_] ||
      scan_position_ == cursor_position_) {
    // There is a character to transmit!
    // If the new character to transmit is just after the previous one, and on
    // the same line, we're good, we don't need to reposition the cursor.
    if ((scan_position_ == scan_position_last_write_ + 1) &&
        (scan_position_ & kLcdWidthWrap)) {
      // We use overwrite because we have checked before that there is
      // enough room in the buffer.
      DisplaySerialOutput::Overwrite(character);
    } else {
      // The character to transmit is at a different position, we need to move
      // the cursor, and compute the cursor move command argument.
      uint8_t cursor_position = 0x80;
      cursor_position |= (scan_position_ & ~kLcdWidthWrap) << kLcdLineShift;
      cursor_position |= (scan_position_ & kLcdWidthWrap);
      DisplaySerialOutput::Overwrite(0xfe);
      DisplaySerialOutput::Overwrite(cursor_position);
      DisplaySerialOutput::Overwrite(character);
    }
    // We can now assume that remote display will be updated. This works because
    // the entry to this block of code is protected by a check on the
    // transmission success!
    remote_[scan_position_] = character;
    scan_position_last_write_ = scan_position_;
  }
  scan_position_ = (scan_position_ + 1) & kLcdBufferSizeWrap;
}

}  // namespace hardware_io
