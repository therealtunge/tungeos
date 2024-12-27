
#include <stdio.h>
#include <kernel.h>
#include <keyboard.h>

void kernel_main(void)
{
	terminal_initialize();
	keyboard_init();
	while (1) {
		char key = get_current_key();
		if (key != 0) {
			putc(key);
		} else {

		}
	}
	
}
