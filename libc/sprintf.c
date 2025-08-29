#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

extern char* itoah(uint8_t i);
extern char* itoa(int i);
void sprintf(char *target, char *format, ...) {
	va_list ap;
	int len = 0;
	va_start(ap, format);
	for (int i = 0; i < strlen(format); i++) {
		if (format[i] == 37) { // why is char comparison so janky man, just let me have my format[i] == '%' in peace :(
			switch(format[i + 1]) {
				case ('x'): {
					char *a = itoah(va_arg(ap, int));
					memcpy(target, a, strlen(a));
					target += strlen(a);
					i++;
					break;
				}
				case ('d'): {
					char *a = itoa(va_arg(ap, int));
					memcpy(target, a, strlen(a));
					target += strlen(a);
					i++;
					break;
				}
				case ('c'): {
					target[0] = va_arg(ap, int);
					target++;
					i++;
					break;
				}
				case ('s'): {
					char *a = va_arg(ap, int*);
					memcpy(target, a, strlen(a));
					target += strlen(a);
					i++;
					break;
				}
				default: {
					// for the people who dont know that printf needs specifiers
					target[0] = format[i];
					target++;
					break;
				}
			}
		} else {
			target[0] = format[i];
			target++;
		}
	}
	va_end(ap);
}