#include <stddef.h>
#include <stdint.h>

typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15
} vga_color;

static inline uint8_t vga_color_entry(vga_color fg, vga_color bg) {
	return fg | bg << 4;
}

static inline uint8_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_col;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_init(void) {
	terminal_row = 0;
	terminal_col = 0;
	terminal_color = vga_color_entry(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t *) 0xb8000;
}

void putc(char c) {
	const size_t index = terminal_row * VGA_WIDTH + terminal_col;
	terminal_buffer[index] = vga_entry(c, terminal_color);
	if (++terminal_col == VGA_WIDTH) {
		terminal_col = 0;
		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = 0;
		}
	}
}

extern "C" int bootloader_main(void) {
	terminal_init();

	putc('!');

	return 0;
}
