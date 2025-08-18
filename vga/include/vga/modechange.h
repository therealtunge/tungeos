#pragma once
extern unsigned char g_720x480x16[];
extern unsigned char g_320x200x256[];
void write_regs(unsigned char *regs);
extern void write_pixel8(unsigned x, unsigned y, unsigned c);