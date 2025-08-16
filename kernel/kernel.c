
#include <stdio.h>
#include <kernel.h>
#include <keyboard.h>
#include <ata.h>
#include <stdint.h>

#define ATA_PRIMARY_CMD 0x1F0
#define ATA_REG_COMMAND (ATA_PRIMARY_CMD+7)
#define ATA_DR_DATA (ATA_PRIMARY_CMD+0)
extern char* itoa(int i);
void terminal_scroll(int up_down);

void kernel_main(void)
{
	terminal_initialize();
	keyboard_init();
	printf("keyboard initialized\n");
	printf("welcome to tungeos v0.1\n");
	init_serial();
	uint8_t buffer[512] = {1, 2, 3, 4};
	uint16_t bufferidentify[256] = {0};
	uint8_t *buffer8bitidentify = bufferidentify;
	printf("%d\n", ata_identify(bufferidentify, 0));
//	write_cdrom(0x1F0, 0, 0x1, 1, buffer);
//	printf("%d\n", read_cdrom(0x1F0, 0, 0x0, 1, buffer));
//	for (int i = 0; i < 512; i++) {
//		printf("%x ", buffer8bitidentify[i]);
//	}
	printf("\ndata:\n");
	ata_read_sector_lba28(0x0, buffer, 0);
	
	while (1) {
		unsigned char key = get_current_key();
		if (key != 0) {
			if (key == 0x8F) {
				terminal_scroll(1);
			} else if (key == 0x9D) {
				terminal_scroll(-1);
			} else if (key == ']') {
				for (int i = 0; i < 512; i++) {
					printf("%x ", buffer[i]);
				}
			} else {
				putc(key);
			}
		}
	}
}