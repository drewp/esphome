#pragma once
#include "esphome/core/helpers.h"

namespace esphome {
namespace ili9486_8bit {

// clang-format off
static const uint8_t PROGMEM INITCMD_TFT35[] = {
  0x11, 0x80,
  0x3A, 1, 0x55,
  0xC2, 1, 0x44,
  0xC5, 4, 0x00, 0x00, 0x00, 0x00,
  0xE0, 15, 0x0f,0x1f,0x1c,0x0c,0x0f,0x08,0x48,0x98,0x37,0x0a,0x13,0x04,0x11,0x0d,0x00,
  0xE1, 15, 0x0f,0x32,0x2e,0x0b,0x0d,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,
  ILI9486_INVOFF, 0x80,
  0x36, 1, 0x48,
  ILI9486_DISPON, 0x80,

  // ILI9486_MADCTL, 1, MADCTL_BGR | MADCTL_MV, //hardware rotation
  0x00                                   // End of list
};

// clang-format on
}  // namespace ili9486_8bit
}  // namespace esphome
