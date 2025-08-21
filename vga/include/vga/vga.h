#pragma once
#include <vga/modechange.h>

#define g_wd 720
#define g_ht 480

extern void init_vga();
extern void set_screen(unsigned int c);
extern void clear_screen();