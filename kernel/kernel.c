#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "multiboot.h"
#include <stdio.h>

uint32_t *fb;
int fb_width, fb_height;

void kernel_main(multiboot_info_t *mboot_ptr) 
{
	fb = mboot_ptr->framebuffer_addr;
	fb_height = mboot_ptr->framebuffer_height;
	fb_width = mboot_ptr->framebuffer_width;
	printf("found fb at: %u\n", fb);
	printf("mboot->fb_height: %d\nmboot->fb_width: %d\n", mboot_ptr->framebuffer_height, mboot_ptr->framebuffer_width);
}
