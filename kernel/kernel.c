
#include <stdio.h>
#include <kernel.h>
#include <keyboard.h>
#include <vga/vga.h>

void scroll_up(uint16_t *buf);
void scroll_down(uint16_t *buf);


typedef unsigned int uint32_t;
extern int atapi_read_sectors(uint8_t drive, uint32_t lba, uint16_t sectors, void *outbuf);
extern int ata_read_sector_lba28(uint32_t lba, uint8_t *buffer);
extern int ata_write_sector_lba28(uint32_t lba, const uint8_t *buffer);
extern int atapi_wait_drq(void);
extern void ata_select(uint8_t drive);
extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);

extern void demo_graphics(void);

int ata_identify(uint16_t *buffer);

#define ATA_PRIMARY_CMD 0x1F0
#define ATA_REG_COMMAND (ATA_PRIMARY_CMD+7)
#define ATA_DR_DATA (ATA_PRIMARY_CMD+0)
extern char* itoa(int i);


void kernel_main(void)
{
	terminal_initialize();
	keyboard_init();
//	puts("keyboard initialized\n");
//	puts("welcome to tungeos v0.1\n");
	init_serial();
//	write_cdrom(0x1F0, 0, 0x1, 1, buffer);
//	printf("%d\n", read_cdrom(0x1F0, 0, 0x0, 1, buffer));
//	ata_write_sector_lba28(0x0, buffer);
//	printf("\ndata:\n");
	init_vga();
	set_screen(1);
	puts("abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
	puts("0123456789 !@#$\n");
	testfs();
	while (1) {
		char key = get_current_key();
		if (key != 0) {
			write_chr(key);
		}
	}
}