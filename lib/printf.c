#include <stdarg.h>
#include <stdint.h>

void printf(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	for (int i = 0; i < strlen(format); i++) {
		if (format[i] == 37) { // why is char comparison so janky man, just let me have my format[i] == '%' in peace :(
			switch(format[i + 1]) {
				case ('x'): {
					puts(itoah(va_arg(ap, int)));
					i++;
					break;
				}
				case ('d'): {
					puts(itoa(va_arg(ap, int)));
					i++;
					break;
				}
				case ('u'): {
					puts(uitoa(va_arg(ap, int)));
					i++;
					break;
				}
				case ('c'): {
					putc(va_arg(ap, int)); // ok whoever decided that char promotes to int is screwed
					i++;
					break;
				}
				case ('s'): {
					puts(va_arg(ap, int*));
					i++;
					break;
				}
				default: {
					// for the people who dont know that printf needs specifiers
					putc(format[i]);
					break;
				}
			}
		} else {
			putc(format[i]);
		}
	}
	va_end(ap);
}