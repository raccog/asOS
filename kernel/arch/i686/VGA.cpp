#include "kernel/arch/i686/VGA.h"

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}

Terminal::Terminal() {
    init();
}

void Terminal::init() {
    row = 0;
    col = 0;
    color = VgaEntryColor(VgaColor::LightGrey, VgaColor::Black);
    buffer = reinterpret_cast<uint16_t *>(0xb8000);
    for (size_t y = 0; y < VGA_HEIGHT; ++y) {
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            const size_t index = y * VGA_WIDTH + x;
            buffer[index] = VgaEntry(' ', color);
        }
    }
}

void Terminal::setColor(uint8_t color) {
    this->color = color;
}

void Terminal::putEntryAt(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    buffer[index] = VgaEntry(c, color);
}

void Terminal::putChar(char c) {
    putEntryAt(c, color, col, row);
    if (++col == VGA_WIDTH) {
        col = 0;
        if (++row == VGA_HEIGHT) {
            row = 0;
        }
    }
}

void Terminal::write(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        putChar(data[i]);
    }
}

void Terminal::writeString(const char *str) {
    write(reinterpret_cast<const uint8_t *>(str), strlen(str));
}
