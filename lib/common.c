#include <stdint.h>

void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
	uint8_t *temp = (uint8_t *)dest;
	for ( ; len != 0; len--) *temp++ = val;
}
uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
void io_wait() {
	inb(0x80);
}