#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_RELEASED_MASK 0x80

bool numlock = false;
char keymap[128] = {
	0,   27, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\n', 0, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 
	'b', 'n', 'm', ',', '.', '/', 0, '*',
	0, ' ', 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, [0x45] = 0x8D, [0x4D] = '2'};
volatile uint8_t buffer_index = 0;
extern inline uint8_t inb(uint16_t port) {
	uint8_t value;
	asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

extern inline void outb(uint16_t port, uint8_t value) {
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static char get_numlock(uint8_t scancode) {
	if (numlock) {
		switch (scancode) {
			case (0x48): {
				return 0x8F;
			}
			case (0x4B): {
				return 0x8D;
			}
			case (0x4D): {
				return 0x90;
			}
			case (0x50): {
				return 0x9D;
			}
		}
	} else {
		switch (scancode) {
			case (0x48): {
				return '8';
			}
			case (0x4B): {
				return '4';
			}
			case (0x4D): {
				return '6';
			}
			case (0x50): {
				return '2';
			}
		}
	}
}

char get_current_key() {
	uint8_t status = inb(KEYBOARD_STATUS_PORT);
	if (status & 1) {
		uint8_t scancode = inb(KEYBOARD_DATA_PORT);
		if (!(scancode & KEY_RELEASED_MASK)) {
			if (scancode == 0x45) {
				numlock = !numlock;
			} else if (scancode == 0x48 || scancode == 0x4B || scancode == 0x4D || scancode == 0x50) {
				return get_numlock(scancode);
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
