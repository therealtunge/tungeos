#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

static char* itoa(int i) {
	if(i == 0) {
		return "0";
	}
	static char output[24];  // 64-bit MAX_INT is 20 digits
	char* p = &output[23];
	for(*p--=0;i;i/=10) *p--=i%10+0x30; 
	return ++p;    
}


bool is_format_identifier(char c) {
	return c == 'c' || c == 'd' || c == 's';
}
/*
void printf(const char *restrict format, ...) {
	va_list ap;
	va_start(ap, format);
	for (int i = 0; i < strlen(format); i++) {
		switch (format[i]) {
			case ('%'): {
				if (is_format_identifier(format[i + 1])) {
					switch (format[i + 1]) {
						case ('d'): {
							char *a = itoa(va_arg(ap, int));
							puts((const char *)a);
							i+=2;
							break;
						}
						case ('s'): {
							puts(va_arg(ap, char*));
							i+=2;
							break;
						}
						case ('c'): {
							putc(va_arg(ap, int));
							i+=2;
							break;
						}
						default: {
							putc(format[i]);
							i++;
							break;
						}
					}
				}
			}
			default: {
				if (format[i] != 0) {
					putc(format[i]);
				}
				break;
			}
		}
	}
}*/
void printf(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	for (int i = 0; i < strlen(format); i++) {
		if (format[i] == 37) { // why is char comparison so janky man, just let me have my format[i] == '%' in peace :(
			switch(format[i + 1]) {
				case ('d'): {
					puts(itoa(va_arg(ap, int)));
					i++;
					break;
				}
				case ('c'): {
					putc(va_arg(ap, int)); // ok whoever decided that char promotes to int is screwed
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