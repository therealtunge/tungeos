#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

extern char* itoah(uint8_t i) {
	static char output[3];
	char* p = &output[2];
	*p-- = 0;
	for(int shift = 0; shift < 2; shift++) {
		uint8_t nibble = i & 0xF;
		*p-- = (nibble < 10) ? ('0' + nibble) : ('A' + nibble - 10);
		i >>= 4;
	}
	return ++p;
}

static char *uitoa(unsigned int value) {
	static char buf[11]; // max for 32-bit unsigned: 4294967295 + '\0'
	char *p = buf;
	char *start = p;

	do {
		*p++ = (value % 10) + '0';
		value /= 10;
	} while (value);

	*p = '\0';

	// reverse string
	for (char *i = start, *j = p - 1; i < j; i++, j--) {
		char tmp = *i;
		*i = *j;
		*j = tmp;
	}

	return buf;
}

char *itoa(int value) {
	static char buf[12]; // enough for -2147483648 + '\0'
	char *p = buf;
	int sign = value;

	if (value < 0)
		value = -value;

	char *start = p;
	do {
		*p++ = (value % 10) + '0';
		value /= 10;
	} while (value);

	if (sign < 0)
		*p++ = '-';

	*p = '\0';

	// reverse string
	for (char *i = start, *j = p - 1; i < j; i++, j--) {
		char tmp = *i;
		*i = *j;
		*j = tmp;
	}

	return buf;
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
				case ('c'): {
					write_chr(va_arg(ap, int)); // ok whoever decided that char promotes to int is screwed
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
					write_chr(format[i]);
					break;
				}
			}
		} else {
			write_chr(format[i]);
		}
	}
	va_end(ap);
}

void serial_printf(char *format, ...) {
	va_list ap;
	va_start(ap, format);
	for (int i = 0; i < strlen(format); i++) {
		if (format[i] == 37) { // why is char comparison so janky man, just let me have my format[i] == '%' in peace :(
			switch(format[i + 1]) {
				case ('x'): {
					puts_serial(itoah(va_arg(ap, int)));
					i++;
					break;
				}
				case ('d'): {
					puts_serial(itoa(va_arg(ap, int)));
					i++;
					break;
				}
				case ('u'): {
					puts_serial(uitoa(va_arg(ap, int)));
					i++;
					break;
				}
				case ('c'): {
					write_serial(va_arg(ap, int)); // ok whoever decided that char promotes to int is screwed
					i++;
					break;
				}
				case ('s'): {
					puts_serial(va_arg(ap, int*));
					i++;
					break;
				}
				default: {
					// for the people who dont know that printf needs specifiers
					write_serial(format[i]);
					break;
				}
			}
		} else {
			write_serial(format[i]);
		}
	}
	va_end(ap);
}