#include <stdio.h>
#include <stdint.h>

static int term_x, term_y;

extern char font8x8_basic[128][8];
extern uint32_t *fb;
extern int fb_width, fb_height;


static void _putc(char c, int _x, int _y, uint32_t fg, uint32_t bg)
{
	for(int y = 0; y < 8; y++)
	{
		uint8_t tmp;
		uint8_t bits = font8x8_basic[(uint8_t)c][y];
		for (int x = 0; x < 8; x++) {
			tmp = (bits & (1 << (x)));
			fb[(_x + x) + ((_y + y) * fb_width)] = tmp ? fg: bg;
		}
	}
}

void putc(char c) {
	if (c == '\n') {
		term_y += 8;
		term_x = 0;
	} else {
		_putc(c, term_x, term_y, 0xffffff, 0x0);
	}
	return;
}

void puts(const char *str) {
	while (*str) {
		putc(*str);
		term_x += 8;
		str++;
	}
	putc('\n');
	return;
}