#pragma once
#include <vga/modechange.h>

#define g_wd 320
#define g_ht 200

#define write_pixel(x, y, c) write_pixel8(x, y, c)

extern void init_vga();
extern void set_screen(unsigned int c);
extern void clear_screen();