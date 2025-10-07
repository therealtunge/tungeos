#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
static int term_x, term_y = 0;

extern char font8x8_basic[128][8];
extern uint32_t *fb;
extern int fb_width, fb_height;

char* itoah(uint8_t i) {
	static char output[3];
	char* p = &output[2];
	*p-- = 0;
	for(int shift = 0; shift < 2; shift++) {
		uint8_t nibble = i & 0xF;
		*p-- = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
		i >>= 4;
	}
	return ++p;
}

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
		term_x += 8;
	}
	return;
}

void puts(const char *str) {
	while (*str) {
		putc(*str);
		str++;
	}
	putc('\n');
	return;
}
static int strlen(const char* str) 
{
	char *str_ = str;
	while (*str_) {
		str_++;
	}
	return str_ - str;
}
void printf(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	for (int i = 0; i < strlen(format); i++) {
		if (format[i] == 37) { // why is char comparison so janky man, just let me have my format[i] == '%' in peace :(
			switch(format[i + 1]) {
				case ('x'): {
					puts(itoah(va_arg(ap, int)));
					i++;
					break;
				}
				case ('d'): {
					puts(itoa(va_arg(ap, int)));
					i++;
					break;
				}
				case ('c'): {
					putc(va_arg(ap, int)); // ok whoever decided that char promotes to int is screwed
					i++;
					break;
				}
				case ('s'): {
					puts(va_arg(ap, int*));
					i++;
					break;
				}
				default: {
					// for the people who dont know that printf needs specifiers
					putc(format[i]);
					break;
				}
			}
		} else {
			putc(format[i]);
		}
	}
	va_end(ap);
}