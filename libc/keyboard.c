#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_RELEASED_MASK 0x80
#define is_numlock(a)  (a == 0x48 || a == 0x50)


volatile uint8_t numlock = 0;

char keymap[128] = {
	0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
	'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   '-', 0,   0,   0,
	'+', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0};
volatile uint8_t buffer_index = 0;
extern inline uint8_t inb(uint16_t port) {
	uint8_t value;
	asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

extern inline void outb(uint16_t port, uint8_t value) {
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

extern volatile int printf(const char *format, ...) __attribute__((used));

/*
__attribute__((noinline, optimize("O0")))
char numlock_handler_helper(uint8_t scancode) {
	if (numlock) {
		return (scancode == 0x48 ? 0x81 : (scancode == 0x50 ? 0x8D : 0x0));
	} else {
		return (scancode == 0x48 ? '8' : (scancode == 0x50 ? '2' : 0x0));
	}
}*/

char numlock_helper(uint8_t ret) {
	if (ret == 0x81) {
		scroll_up((uint16_t *) 0xB8000);
		return 0;
	} else if (ret == 0x8D) {
		scroll_down((uint16_t *) 0xB8000);
		return 0;
	} else {
		return ret;
	}
}

char numlock_handler(uint8_t scancode) {
	volatile char ret = 0;
	if (numlock) {
		ret = (scancode == 0x48 ? 0x81 : (scancode == 0x50 ? 0x8D : 0x0));
	} else {
		return (scancode == 0x48 ? '8' : (scancode == 0x50 ? '2' : 0x0));
	}
	return numlock_helper(ret);
}

char get_current_key() {
	uint8_t status = inb(KEYBOARD_STATUS_PORT);
	if (status & 1) {
		uint8_t scancode = inb(KEYBOARD_DATA_PORT);
		if (!(scancode & KEY_RELEASED_MASK)) {
			if (scancode == 0x45) {
				numlock = !numlock;
			} else if (is_numlock(scancode)) {
				return numlock_handler(scancode);
			} else {
				return keymap[scancode];
			}
		}
	}
	return 0;
}

void keyboard_init() {
	outb(0x21, inb(0x21) & ~0x02);	
}
