#include <vga/vga.h>

extern void init_vga() {
	write_regs(g_320x200x256);
}

#define VGA_FB ((unsigned char *)0xC03F0000)
extern void set_screen(unsigned int c) {
	for(int i = 0; i < (g_ht * g_wd); i++)
		VGA_FB[i] = c;	
}

extern inline void clear_screen() {
	set_screen(0);
}