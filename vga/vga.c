#include <vga/vga.h>

extern void init_vga() {
	write_regs(g_320x200x256);
}
extern void set_screen(unsigned int c) {
	for(int y = 0; y < g_ht; y++)
		for(int x = 0; x < g_wd; x++)
			write_pixel8(x, y, c);
}

extern inline void clear_screen() {
	set_screen(0);
}