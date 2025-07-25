#if defined(EEZ_FOR_LVGL)
#include <eez/core/vars.h>
#endif

#include "ui.h"
#include "screens.h"
#include "images.h"
#include "actions.h"
#include "vars.h"

// ASSETS DEFINITION
const uint8_t assets[1873] = {
    0x7E, 0x65, 0x65, 0x7A, 0x03, 0x00, 0x06, 0x00, 0x60, 0x15, 0x00, 0x00, 0x6E, 0x24, 0x00, 0x00,
    0x00, 0x24, 0x00, 0x01, 0x00, 0x17, 0x20, 0x0C, 0x00, 0x53, 0xE0, 0x01, 0x40, 0x01, 0x01, 0x28,
    0x00, 0x13, 0x02, 0x1C, 0x00, 0x00, 0x08, 0x00, 0xA6, 0x1C, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00,
    0x00, 0xF4, 0x12, 0x2C, 0x00, 0x11, 0x10, 0x06, 0x00, 0xB7, 0xFF, 0xFF, 0x14, 0x00, 0x00, 0x00,
    0x40, 0x00, 0x00, 0x00, 0x6C, 0x48, 0x00, 0x57, 0x33, 0x00, 0x00, 0x00, 0x64, 0x10, 0x00, 0x10,
    0x0D, 0x44, 0x00, 0x2F, 0x13, 0x00, 0x01, 0x00, 0x06, 0x10, 0x07, 0x05, 0x00, 0x16, 0x01, 0x10,
    0x00, 0x00, 0x0B, 0x00, 0x2F, 0xFC, 0x12, 0x30, 0x00, 0x07, 0xF2, 0x1F, 0x44, 0x65, 0x66, 0x61,
    0x75, 0x6C, 0x74, 0x00, 0xE8, 0x00, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00,
    0x3C, 0x01, 0x00, 0x00, 0x58, 0x01, 0x00, 0x00, 0x74, 0x01, 0x00, 0x00, 0x90, 0x01, 0x00, 0x00,
    0xAC, 0x01, 0x00, 0x00, 0xC8, 0x01, 0x00, 0x00, 0xE4, 0x01, 0xC5, 0x00, 0xF6, 0xAF, 0x1C, 0x02,
    0x00, 0x00, 0x38, 0x02, 0x00, 0x00, 0x54, 0x02, 0x00, 0x00, 0x70, 0x02, 0x00, 0x00, 0x8C, 0x02,
    0x00, 0x00, 0xA8, 0x02, 0x00, 0x00, 0xC4, 0x02, 0x00, 0x00, 0xE0, 0x02, 0x00, 0x00, 0xFC, 0x02,
    0x00, 0x00, 0x18, 0x03, 0x00, 0x00, 0x34, 0x03, 0x00, 0x00, 0x50, 0x03, 0x00, 0x00, 0x6C, 0x03,
    0x00, 0x00, 0x88, 0x03, 0x00, 0x00, 0xA4, 0x03, 0x00, 0x00, 0xC0, 0x03, 0x00, 0x00, 0xDC, 0x03,
    0x00, 0x00, 0xF8, 0x03, 0x00, 0x00, 0x14, 0x04, 0x00, 0x00, 0x30, 0x04, 0x00, 0x00, 0x4C, 0x04,
    0x00, 0x00, 0x68, 0x04, 0x00, 0x00, 0x84, 0x04, 0x00, 0x00, 0xA0, 0x04, 0x00, 0x00, 0xBC, 0x04,
    0x00, 0x00, 0xD8, 0x04, 0x00, 0x00, 0xF4, 0x04, 0x00, 0x00, 0x10, 0x05, 0x00, 0x00, 0x2C, 0x05,
    0x00, 0x00, 0x50, 0x05, 0x00, 0x00, 0x74, 0x05, 0x00, 0x00, 0x98, 0x05, 0x00, 0x00, 0xBC, 0x05,
    0x00, 0x00, 0xD8, 0x05, 0x00, 0x00, 0xF4, 0x05, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0x3C, 0x06,
    0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x84, 0x06, 0x00, 0x00, 0xA8, 0x06, 0x00, 0x00, 0xCC, 0x06,
    0x00, 0x00, 0xE8, 0x06, 0x00, 0x00, 0x04, 0x07, 0x00, 0x00, 0x20, 0x07, 0x00, 0x00, 0x3C, 0x07,
    0x00, 0x00, 0x58, 0x07, 0x00, 0x00, 0x74, 0x07, 0x00, 0x00, 0x30, 0x75, 0xFC, 0x00, 0x62, 0x03,
    0x00, 0x00, 0x00, 0x88, 0x07, 0x24, 0x01, 0x20, 0x8C, 0x07, 0x82, 0x01, 0x3B, 0x00, 0x00, 0x31,
    0x20, 0x00, 0x26, 0x78, 0x07, 0x10, 0x00, 0x00, 0x20, 0x00, 0x1B, 0x32, 0x20, 0x00, 0x1B, 0x64,
    0x20, 0x00, 0x1B, 0x33, 0x20, 0x00, 0x1B, 0x50, 0x20, 0x00, 0x0C, 0x80, 0x00, 0x13, 0x3C, 0x80,
    0x00, 0x2E, 0x40, 0x07, 0x60, 0x00, 0x5F, 0x04, 0x00, 0x00, 0x00, 0x2C, 0x40, 0x00, 0x0C, 0x13,
    0x1C, 0x40, 0x00, 0x1F, 0x20, 0x40, 0x00, 0x04, 0x1B, 0x0C, 0x40, 0x00, 0x1B, 0x34, 0x20, 0x00,
    0x22, 0xFC, 0x06, 0x98, 0x02, 0x2F, 0x04, 0x07, 0xC0, 0x00, 0x03, 0x2F, 0xF4, 0x06, 0x00, 0x01,
    0x0B, 0x2F, 0xE0, 0x06, 0x00, 0x01, 0x0B, 0x1F, 0xCC, 0x40, 0x00, 0x0C, 0x1F, 0xB8, 0x40, 0x00,
    0x0C, 0x1E, 0xA4, 0x40, 0x00, 0x09, 0xC0, 0x00, 0x1F, 0x90, 0x60, 0x00, 0x0C, 0x1F, 0x80, 0x60,
    0x00, 0x0C, 0x1F, 0x6C, 0x40, 0x00, 0x0C, 0x1F, 0x58, 0x40, 0x00, 0x0C, 0x1F, 0x44, 0xA0, 0x00,
    0x0C, 0x1F, 0x30, 0x60, 0x00, 0x0C, 0x1F, 0x20, 0x60, 0x00, 0x0C, 0x1F, 0x0C, 0x40, 0x00, 0x0C,
    0x2F, 0xF8, 0x05, 0x40, 0x00, 0x0B, 0x2F, 0xE4, 0x05, 0xA0, 0x00, 0x0B, 0x2F, 0xD0, 0x05, 0x60,
    0x00, 0x0B, 0x1B, 0xC0, 0x20, 0x00, 0x1B, 0x35, 0x40, 0x00, 0x1F, 0xAC, 0x40, 0x00, 0x0C, 0x1F,
    0x9C, 0x80, 0x00, 0x0C, 0x1F, 0x88, 0x40, 0x00, 0x0C, 0x2F, 0x78, 0x05, 0x00, 0x03, 0x0B, 0x22,
    0x64, 0x05, 0x00, 0x03, 0x2F, 0x68, 0x05, 0x40, 0x00, 0x03, 0x1F, 0x54, 0x80, 0x00, 0x0C, 0x1F,
    0x40, 0x60, 0x00, 0x0C, 0x13, 0x30, 0x60, 0x00, 0x2F, 0x34, 0x05, 0xC0, 0x01, 0x04, 0x0F, 0x60,
    0x00, 0x0C, 0x1B, 0x0C, 0x20, 0x00, 0x1C, 0x36, 0x80, 0x03, 0x1F, 0x04, 0x40, 0x00, 0x0B, 0x1B,
    0xEC, 0x20, 0x00, 0x1B, 0x37, 0x80, 0x00, 0x22, 0xDC, 0x04, 0xC0, 0x03, 0x22, 0xE0, 0x04, 0x20,
    0x00, 0x22, 0x14, 0x04, 0xB0, 0x00, 0x2A, 0xD8, 0x04, 0x04, 0x06, 0x13, 0xCC, 0x20, 0x00, 0x00,
    0x0C, 0x00, 0x26, 0xC4, 0x04, 0x28, 0x00, 0x1B, 0xBC, 0x28, 0x00, 0x13, 0xB0, 0x28, 0x00, 0x00,
    0x54, 0x00, 0x17, 0xA8, 0x28, 0x00, 0x1B, 0xA4, 0x28, 0x00, 0x17, 0x98, 0x28, 0x00, 0x63, 0x90,
    0x04, 0x00, 0x00, 0xEF, 0x03, 0x48, 0x05, 0x0B, 0x28, 0x00, 0x17, 0x80, 0x78, 0x00, 0x53, 0x78,
    0x04, 0x00, 0x00, 0xF6, 0x28, 0x00, 0x13, 0x70, 0x58, 0x00, 0x23, 0x6C, 0x04, 0xC0, 0x01, 0x03,
    0x28, 0x00, 0x22, 0xFA, 0x03, 0x54, 0x00, 0x22, 0x5C, 0x04, 0x00, 0x01, 0x13, 0x58, 0xE0, 0x00,
    0x13, 0x60, 0x20, 0x00, 0x04, 0x68, 0x00, 0x1B, 0x58, 0x68, 0x00, 0x17, 0x4C, 0x90, 0x00, 0x17,
    0x44, 0x90, 0x00, 0x1B, 0x40, 0x28, 0x00, 0x17, 0x34, 0x90, 0x00, 0x17, 0x2C, 0x28, 0x00, 0x1B,
    0x24, 0x28, 0x00, 0x17, 0x18, 0x28, 0x00, 0x17, 0x10, 0x08, 0x01, 0x2B, 0x08, 0x04, 0x5C, 0x07,
    0x12, 0x03, 0x28, 0x00, 0x10, 0x04, 0x2C, 0x05, 0x16, 0x03, 0x28, 0x00, 0x2A, 0xF8, 0x03, 0x28,
    0x00, 0x17, 0xEC, 0x28, 0x00, 0x26, 0xE4, 0x03, 0x78, 0x00, 0x1B, 0xE8, 0x28, 0x00, 0x26, 0xDC,
    0x03, 0x78, 0x00, 0x5B, 0xD4, 0x03, 0x00, 0x00, 0x38, 0xF0, 0x01, 0x2A, 0xC4, 0x03, 0x10, 0x02,
    0x0C, 0xB0, 0x06, 0x1B, 0xB0, 0x20, 0x00, 0x1C, 0x39, 0x90, 0x03, 0x1F, 0x03, 0x70, 0x03, 0x0B,
    0x2F, 0x88, 0x03, 0x10, 0x03, 0x0B, 0x23, 0x74, 0x03, 0xCC, 0x01, 0x1F, 0x03, 0x10, 0x07, 0x04,
    0x0B, 0x40, 0x00, 0x04, 0x10, 0x01, 0x2B, 0x58, 0x03, 0xB0, 0x01, 0x07, 0xE8, 0x00, 0x2E, 0x44,
    0x03, 0x04, 0x00, 0x1F, 0x4C, 0x04, 0x00, 0x20, 0x1F, 0x54, 0x04, 0x00, 0x0C, 0x1F, 0x5C, 0x04,
    0x00, 0x10, 0x13, 0x64, 0xB0, 0x08, 0x0F, 0x04, 0x00, 0x35, 0x1F, 0x74, 0x04, 0x00, 0x2C, 0x1F,
    0x7C, 0x04, 0x00, 0x2C, 0x1F, 0x84, 0x04, 0x00, 0x24, 0x1F, 0xB0, 0x04, 0x00, 0x08, 0x1F, 0xB8,
    0x04, 0x00, 0x18, 0x1F, 0xC0, 0x04, 0x00, 0x18, 0x1B, 0xC4, 0x04, 0x00, 0x1B, 0xCC, 0x04, 0x00,
    0x13, 0xD4, 0x44, 0x02, 0x62, 0xD8, 0x03, 0x00, 0x00, 0xE0, 0x03, 0xEC, 0x0A, 0x22, 0xE4, 0x03,
    0x5C, 0x03, 0x00, 0x7C, 0x03, 0x14, 0x04, 0x78, 0x03, 0x12, 0x04, 0xA8, 0x03, 0x01, 0x40, 0x07,
    0x03, 0xBC, 0x03, 0x22, 0x08, 0x00, 0x08, 0x00, 0x00, 0x04, 0x00, 0x31, 0x0A, 0x00, 0x09, 0xE0,
    0x03, 0x22, 0x1C, 0x04, 0xF8, 0x03, 0x00, 0x04, 0x00, 0x13, 0x28, 0x8C, 0x0A, 0x13, 0x0B, 0x24,
    0x04, 0x22, 0x3C, 0x04, 0x3C, 0x04, 0x04, 0x44, 0x04, 0x13, 0x48, 0x88, 0x04, 0x00, 0x08, 0x00,
    0x62, 0x50, 0x04, 0x00, 0x00, 0x0C, 0x00, 0x08, 0x00, 0x00, 0x88, 0x04, 0x13, 0x60, 0xC0, 0x0A,
    0x00, 0xD4, 0x04, 0x10, 0x05, 0x24, 0x03, 0x43, 0x04, 0x00, 0x00, 0x7C, 0xD0, 0x0A, 0x00, 0x10,
    0x05, 0x53, 0x94, 0x04, 0x00, 0x00, 0x06, 0x34, 0x05, 0x00, 0xE0, 0x0A, 0x1F, 0xA4, 0x04, 0x00,
    0x2C, 0x17, 0xAC, 0x04, 0x00, 0x13, 0x00, 0xA8, 0x05, 0x20, 0xB0, 0x04, 0x05, 0x06, 0x06, 0x04,
    0x00, 0x13, 0x01, 0x18, 0x00, 0x02, 0x8D, 0x0C, 0x0F, 0x04, 0x00, 0x1B, 0x04, 0xA0, 0x05, 0x0C,
    0x3C, 0x00, 0x00, 0xDF, 0x0B, 0x0C, 0x28, 0x00, 0x2E, 0x4C, 0x04, 0x28, 0x00, 0x1B, 0x03, 0x28,
    0x00, 0x48, 0x03, 0x60, 0x00, 0xE0, 0x64, 0x05, 0x10, 0x01, 0xB8, 0x0A, 0x0E, 0x9C, 0x00, 0x0F,
    0x04, 0x00, 0x2A, 0x00, 0x87, 0x01, 0x4F, 0x05, 0x00, 0x2C, 0xC0, 0x48, 0x00, 0x2F, 0x1F, 0x01,
    0x48, 0x00, 0x34, 0x1F, 0x02, 0x48, 0x00, 0x0E, 0x2A, 0x04, 0x60, 0x28, 0x01, 0x08, 0x04, 0x00,
    0xC0, 0x06, 0x60, 0x06, 0x00, 0x03, 0xC0, 0x2A, 0xC0, 0x07, 0x00, 0x00, 0xC0, 0x0C, 0x00, 0x31,
    0x04, 0xC0, 0x08, 0x10, 0x00, 0x31, 0x00, 0xC0, 0x09, 0x12, 0x00, 0x31, 0x08, 0x00, 0x04, 0x0E,
    0x00, 0x4F, 0x0A, 0x00, 0x00, 0xC0, 0xCC, 0x01, 0x0D, 0x2F, 0xB4, 0x02, 0x6C, 0x01, 0x0B, 0x2E,
    0x0B, 0x00, 0xD8, 0x01, 0x1F, 0x84, 0x34, 0x00, 0x0C, 0x1B, 0x0C, 0x34, 0x00, 0x40, 0x05, 0x60,
    0x00, 0xF0, 0x3C, 0x03, 0x0A, 0x14, 0x00, 0x52, 0x05, 0x00, 0x03, 0xC0, 0x0D, 0xA2, 0x00, 0x0B,
    0x60, 0x00, 0x26, 0x28, 0x02, 0x0C, 0x02, 0x2A, 0x20, 0x02, 0x94, 0x0E, 0x04, 0xBF, 0x0E, 0x10,
    0x05, 0x70, 0x07, 0x1A, 0x02, 0x18, 0x00, 0x23, 0x0E, 0x00, 0xB0, 0x0C, 0x43, 0x02, 0x00, 0x00,
    0x14, 0x0C, 0x00, 0x1F, 0x00, 0x24, 0x00, 0x04, 0x53, 0xF0, 0x01, 0x00, 0x00, 0x15, 0x0C, 0x00,
    0x23, 0xEC, 0x01, 0x88, 0x07, 0x13, 0x01, 0x78, 0x07, 0x12, 0x01, 0x0C, 0x00, 0x22, 0x13, 0x00,
    0x9C, 0x02, 0x13, 0xE4, 0x18, 0x00, 0x64, 0x06, 0x60, 0x00, 0xF0, 0x07, 0x00, 0xBC, 0x00, 0x40,
    0x06, 0x00, 0x02, 0xC0, 0x5B, 0x09, 0x00, 0x04, 0x00, 0x24, 0x13, 0xC0, 0xBC, 0x00, 0x13, 0xBC,
    0x2C, 0x00, 0x01, 0x5C, 0x01, 0x1E, 0x01, 0xBC, 0x00, 0x53, 0xA4, 0x01, 0x00, 0x00, 0xA8, 0x04,
    0x00, 0x1B, 0xAC, 0x8C, 0x0F, 0x22, 0xA0, 0x01, 0x20, 0x00, 0x08, 0x14, 0x00, 0x22, 0x9C, 0x01,
    0x18, 0x00, 0x26, 0x01, 0x00, 0x08, 0x00, 0x13, 0x0F, 0xB8, 0x00, 0x22, 0x94, 0x01, 0x20, 0x10,
    0x01, 0x68, 0x09, 0x07, 0x0C, 0x00, 0x22, 0x8C, 0x01, 0x84, 0x04, 0x10, 0x02, 0xB4, 0x07, 0x03,
    0x34, 0x00, 0x1B, 0x88, 0x3C, 0x00, 0x17, 0x7C, 0x30, 0x00, 0x26, 0x78, 0x01, 0x18, 0x00, 0x00,
    0xD8, 0x0F, 0x13, 0x0D, 0x0C, 0x00, 0x13, 0x70, 0x34, 0x00, 0x04, 0x08, 0x00, 0x22, 0x6C, 0x01,
    0x0C, 0x00, 0x0F, 0xFC, 0x02, 0x29, 0x10, 0x01, 0x38, 0x0B, 0x1E, 0x01, 0x40, 0x02, 0x0F, 0xE0,
    0x01, 0x01, 0x5F, 0x0C, 0x01, 0x00, 0x00, 0x1C, 0x04, 0x00, 0x48, 0x12, 0x03, 0x98, 0x02, 0x00,
    0x08, 0x01, 0x20, 0xE0, 0x10, 0x10, 0x01, 0x03, 0x04, 0x00, 0x04, 0xE8, 0x01, 0x00, 0x18, 0x00,
    0x17, 0x07, 0x0C, 0x00, 0x04, 0x30, 0x00, 0x6A, 0x03, 0x60, 0x14, 0x00, 0x01, 0xC0, 0x10, 0x00,
    0x00, 0xC8, 0x02, 0x10, 0xD8, 0x80, 0x06, 0x0F, 0x04, 0x00, 0x34, 0x04, 0x78, 0x00, 0x00, 0x60,
    0x02, 0x10, 0xCC, 0xA4, 0x0B, 0x1E, 0x00, 0x04, 0x00, 0xF0, 0x0C, 0x27, 0x00, 0x00, 0x00, 0x2E,
    0x00, 0x01, 0x00, 0x2F, 0x00, 0x02, 0x00, 0x2A, 0x00, 0x03, 0x00, 0x29, 0x00, 0x04, 0x00, 0x31,
    0x00, 0x05, 0x00, 0x32, 0x00, 0x06, 0x0C, 0x00, 0x00, 0x04, 0x00, 0x00, 0x48, 0x01, 0x90, 0xE0,
    0x0E, 0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0xE0, 0xC4, 0x00, 0xF3, 0x02, 0x11, 0x00, 0x00, 0xE0,
    0x15, 0x00, 0x00, 0xE0, 0x12, 0x00, 0x00, 0xE0, 0x14, 0x00, 0x00, 0xE0, 0x05, 0x10, 0x00, 0x17,
    0x11, 0x10, 0x00, 0xB1, 0x28, 0x00, 0x07, 0x00, 0x2C, 0x00, 0x09, 0x00, 0x2B, 0x00, 0x08, 0x50,
    0x00, 0x62, 0x2C, 0x00, 0x0A, 0x00, 0x16, 0x00, 0xEC, 0x02, 0x13, 0x17, 0x08, 0x00, 0x13, 0x18,
    0x2C, 0x00, 0x13, 0x18, 0x58, 0x00, 0x44, 0x2D, 0x00, 0x0B, 0x00, 0x1C, 0x00, 0x1F, 0x16, 0x24,
    0x00, 0x04, 0x53, 0x30, 0x00, 0x0C, 0x00, 0x06, 0xD4, 0x00, 0x0C, 0x94, 0x00, 0x10, 0x78, 0x58,
    0x04, 0x00, 0xC8, 0x02, 0x00, 0xF4, 0x02, 0x00, 0xA0, 0x06, 0x00, 0x40, 0x03, 0x00, 0x64, 0x0E,
    0x03, 0xEC, 0x00, 0x10, 0xD8, 0x88, 0x03, 0x00, 0xB8, 0x03, 0x00, 0x58, 0x0B, 0x00, 0xFC, 0x03,
    0x43, 0x01, 0x00, 0x00, 0x14, 0x94, 0x12, 0xF3, 0x06, 0x2C, 0x01, 0x00, 0x00, 0x38, 0x01, 0x00,
    0x00, 0x44, 0x01, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00, 0x5C, 0x01, 0x00, 0x00, 0x68, 0xA4, 0x12,
    0x22, 0x80, 0x01, 0x10, 0x03, 0x10, 0x98, 0xF0, 0x01, 0x03, 0x01, 0x00, 0x53, 0x01, 0x01, 0x01,
    0x03, 0x03, 0x94, 0x0A, 0x0C, 0x01, 0x00, 0x1B, 0x01, 0x10, 0x00, 0x13, 0x0E, 0x08, 0x00, 0x2A,
    0x68, 0x01, 0x10, 0x00, 0x27, 0x5C, 0x01, 0x54, 0x13, 0x07, 0x38, 0x00, 0x04, 0x10, 0x00, 0x1B,
    0x02, 0x10, 0x00, 0x2A, 0x10, 0x0E, 0x40, 0x00, 0x1B, 0x20, 0x40, 0x00, 0x1B, 0x3C, 0x70, 0x00,
    0x1B, 0x04, 0x70, 0x00, 0x1B, 0xF8, 0x20, 0x00, 0x1B, 0xEC, 0x10, 0x00, 0x1B, 0xE0, 0x10, 0x00,
    0x1B, 0xD4, 0x80, 0x00, 0x1B, 0x09, 0x10, 0x00, 0x1B, 0xC8, 0x10, 0x00, 0x1B, 0x00, 0xB8, 0x00,
    0x1B, 0x01, 0x10, 0x00, 0x09, 0x44, 0x14, 0x07, 0x9C, 0x0C, 0x17, 0x07, 0xB0, 0x03, 0x08, 0x10,
    0x00, 0x1B, 0x08, 0x10, 0x00, 0x0C, 0x08, 0x00, 0x1B, 0x06, 0x10, 0x00, 0x13, 0x1A, 0x08, 0x00,
    0xF0, 0x03, 0xEF, 0x81, 0x93, 0x00, 0xEF, 0x81, 0x94, 0x00, 0xD1, 0x87, 0x3A, 0x00, 0xD0, 0xBC,
    0x3A, 0x00, 0xD1, 0x81, 0x14, 0x00, 0xA0, 0x8D, 0x00, 0xEF, 0x81, 0x8B, 0x00, 0xD1, 0x87, 0x00,
    0x00
};

native_var_t native_vars[] = {
    { NATIVE_VAR_TYPE_NONE, 0, 0 },
    { NATIVE_VAR_TYPE_FLOAT, get_var_temperature, set_var_temperature }, 
    { NATIVE_VAR_TYPE_FLOAT, get_var_relative_humidity, set_var_relative_humidity }, 
    { NATIVE_VAR_TYPE_FLOAT, get_var_absolute_humidity, set_var_absolute_humidity }, 
    { NATIVE_VAR_TYPE_FLOAT, get_var_temperature_setpoint, set_var_temperature_setpoint }, 
    { NATIVE_VAR_TYPE_INTEGER, get_var_solid_relay_st, set_var_solid_relay_st }, 
    { NATIVE_VAR_TYPE_FLOAT, get_var_time_setpoint, set_var_time_setpoint }, 
    { NATIVE_VAR_TYPE_INTEGER, get_var_timer_seconds, set_var_timer_seconds }, 
    { NATIVE_VAR_TYPE_INTEGER, get_var_drier_state, set_var_drier_state }, 
};


ActionExecFunc actions[] = {
    0
};


#if defined(EEZ_FOR_LVGL)

void ui_init() {
    eez_flow_init(assets, sizeof(assets), (lv_obj_t **)&objects, sizeof(objects), images, sizeof(images), actions);
}

void ui_tick() {
    eez_flow_tick();
    tick_screen(g_currentScreen);
}

#else

static int16_t currentScreen = -1;

static lv_obj_t *getLvglObjectFromIndex(int32_t index) {
    if (index == -1) {
        return 0;
    }
    return ((lv_obj_t **)&objects)[index];
}

void loadScreen(enum ScreensEnum screenId) {
    currentScreen = screenId - 1;
    lv_obj_t *screen = getLvglObjectFromIndex(currentScreen);
    lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 200, 0, false);
}

void ui_init() {
    create_screens();
    loadScreen(SCREEN_ID_MAIN);
}

void ui_tick() {
    tick_screen(currentScreen);
}

#endif
