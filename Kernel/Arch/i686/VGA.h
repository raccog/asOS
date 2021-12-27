/*
 *  Interface for writing text to VGA memory.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

enum class VgaColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGrey = 7,
    DarkGrey = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    LightBrown = 14,
    White = 15
};

static inline uint8_t VgaEntryColor(VgaColor fg, VgaColor bg) {
    return static_cast<uint8_t>(fg) | static_cast<uint8_t>(bg) << 4;
}

static inline uint16_t VgaEntry(unsigned char uc, uint8_t color) {
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;

}

class Terminal {
    size_t row, col;
    uint8_t color;
    volatile uint16_t *buffer;

    void init();

public:
    Terminal();

    void setColor(uint8_t color);
    void putEntryAt(char c, uint8_t color, size_t x, size_t y);
    void putChar(char c);
    void write(const uint8_t *data, size_t size);
    void writeString(const char *data);
};
