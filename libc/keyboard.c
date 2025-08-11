#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_RELEASED_MASK 0x80

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

char get_current_key() {
    uint8_t status = inb(KEYBOARD_STATUS_PORT);
    if (status & 1) {
        uint8_t scancode = inb(KEYBOARD_DATA_PORT);
        if (!(scancode & KEY_RELEASED_MASK)) {
            return keymap[scancode];
        }
    }
    return 0;
}

void keyboard_init() {
	outb(0x21, inb(0x21) & ~0x02);	
}
