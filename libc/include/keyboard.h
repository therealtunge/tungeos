#ifndef KEYBOARD_H
#define KEYBOARD_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEY_RELEASED_MASK 0x80

extern volatile uint8_t buffer_index;

char _getch();
void keyboard_init();
char getch();

#endif
