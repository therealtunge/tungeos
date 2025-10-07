#include <stdint.h>
char* itoa(int value)
{
	static char buffer[12];
	char* ptr = buffer + sizeof(buffer) - 1;
	unsigned int uvalue;
	int negative = 0;
	*ptr = '\0';
	if(value < 0)
	{
		negative = 1;
		uvalue = (unsigned int)(-value);
	}
	else uvalue = (unsigned int)value;
	if(uvalue == 0)
	{
		*(--ptr) = '0';
	}
	else
	{
		while(uvalue)
		{
			*(--ptr) = '0' + (uvalue % 10);
			uvalue /= 10;
		}
	}
	if(negative)
		*(--ptr) = '-';
	return ptr;
}

char* itoah(uint8_t i) {
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

char *uitoa(unsigned int value) {
	static char buf[11];
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