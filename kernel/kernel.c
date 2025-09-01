
#include <stdio.h>
#include <kernel.h>
#include <keyboard.h>
#include <vga/vga.h>
#include <context.h>

char *to_uppercase_copy(const char *src, char *dst) {
	char *p = dst;
	while (*src) {
		if (*src >= 'a' && *src <= 'z')
			*p = *src - 'a' + 'A';
		else
			*p = *src;
		p++;
		src++;
	}
	*p = '\0';
	return dst;
}

void kernel_main(void)
{
	keyboard_init();
	init_serial();
	init_vga();
	set_screen(1);
	idt_init();
	puts("keyboard initialized\n");
	puts("welcome to tungeos v0.1\n");
	pf_mount();
	serial_printf("%d ", load_executable("/OUT"));
	
//	testfs();
	while (1) {
//		write_chr(getch());
	}
}