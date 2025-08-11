/*// Handy register number defines
#define DATA 0
#define ERROR_R 1
#define SECTOR_COUNT 2
#define LBA_LOW 3
#define LBA_MID 4
#define LBA_HIGH 5
#define DRIVE_SELECT 6
#define COMMAND_REGISTER 7

// Control register defines
#define CONTROL 0x206

#define ALTERNATE_STATUS 0

#include <stdint.h>


static __inline void insw(uint16_t __port, void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; insw"
			: "+D"(__buf), "+c"(__n)
			: "d"(__port));
}

static __inline__ void outsw(uint16_t __port, const void *__buf, unsigned long __n) {
	__asm__ __volatile__("cld; rep; outsw"
			: "+S"(__buf), "+c"(__n)
			: "d"(__port));
}

// This code is to wait 400 ns
static void ata_io_wait(const uint8_t p) {
	inb(p + CONTROL + ALTERNATE_STATUS);
}

void ata_device_write_sector_actual(struct ata_device * dev, uint32_t lba) {
	uint16_t bus = dev->io_base;
	uint8_t slave = dev->slave;

	ata_wait(dev, 0);

	while (1) {
		uint8_t status = inb(dev->io_base + ATA_REG_STATUS);
		if (!(status & ATA_SR_BSY)) break;
	}

	outportb(bus + ATA_REG_CONTROL, 0x02);
	outportb(bus + ATA_REG_HDDEVSEL, 0xe0 | slave << 4);
	ata_wait(dev, 0);
	outportb(bus + ATA_REG_FEATURES, 0x00);

	outportb(bus + ATA_REG_SECCOUNT0, 0);
	outportb(bus + ATA_REG_LBA0, (lba & 0xff000000) >> 24);
	outportb(bus + ATA_REG_LBA1, (lba & 0xff00000000) >> 32);
	outportb(bus + ATA_REG_LBA2, (lba & 0xff0000000000) >> 40);

	outportb(bus + ATA_REG_SECCOUNT0, SECTORS_PER_CACHE_BLOCK);
	outportb(bus + ATA_REG_LBA0, (lba & 0x000000ff) >>  0);
	outportb(bus + ATA_REG_LBA1, (lba & 0x0000ff00) >>  8);
	outportb(bus + ATA_REG_LBA2, (lba & 0x00ff0000) >> 16);

	while (1) {
		uint8_t status = inportb(dev->io_base + ATA_REG_STATUS);
		if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRDY)) break;
	}
	spin_lock(atapi_cmd_lock);
	outportb(bus + ATA_REG_COMMAND, ATA_CMD_WRITE_DMA_EXT);

	ata_io_wait(dev);

	outportb(dev->bar4, 0x00 | 0x01);
	sleep_on_unlocking(atapi_waiter, &atapi_cmd_lock);

	#if 0
	ata_wait(dev, 0);
	int size = ATA_SECTOR_SIZE / 2;
	outportsm(bus,buf,size);
	#endif

	outportb(dev->bar4 + 0x2, inportb(dev->bar4 + 0x02) | 0x04 | 0x02);

#if 0
	outportb(bus + 0x07, ATA_CMD_CACHE_FLUSH);
	ata_wait(dev, 0);
#endif
}

// Reads sectors starting from lba to buffer
int read_cdrom(uint16_t port, int slave, uint32_t lba, uint32_t sectors, uint16_t *buffer) {
	volatile uint8_t read_cmd[12] = {0xA8, 0,
		(lba >> 0x18) & 0xFF, (lba >> 0x10) & 0xFF, (lba >> 0x08) & 0xFF,
		(lba >> 0x00) & 0xFF,
		(sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF, (sectors >> 0x08) & 0xFF,
		(sectors >> 0x00) & 0xFF, 0, 0};
	outb(port + DRIVE_SELECT, slave ? 0xB0 : 0xA0); // Drive select
	ata_io_wait(port);
	outb(port + ERROR_R, 0x00); 
	outb(port + LBA_MID, 2048 & 0xFF);
	outb(port + LBA_HIGH, 2048 >> 8);
	outb(port + COMMAND_REGISTER, slave ? 0xA0 : 0x00); // Packet command
	ata_io_wait(port); // I think we might need this delay, not sure, so keep this
	// Wait for status
	while (1) {
		uint8_t error = inb(port + ERROR_R);
		uint8_t status = inb(port + COMMAND_REGISTER);
		if ((status & 0x01) == 1) {
			return 1; // where it fails
		}
		if (!(status & 0x80) && (status & 0x08))
			break;
		ata_io_wait(port);
	}
	insw(port + DATA, buffer, 256);
	outsw(port + DATA, (uint16_t *) read_cmd, 6);
	for (uint8_t i = 0; i < sectors; i++) {
		ata_io_wait(port);
		// Wait until ready
		while (1) {
			uint8_t status = inb(port + COMMAND_REGISTER);
			if (status & 0x01)
				return 1;
			if (!(status & 0x80) && (status & 0x08))
				break;
			ata_io_wait(port);
		}
		int size = inb(port + LBA_HIGH) << 8 | inb(port + LBA_MID); // Get the size of transfer
		insw(port + DATA, (uint16_t *) ((uint8_t *) buffer + i * 0x800), size / 2); // Read it
	}
	return 0;
}
*/
#include <stdint.h>

#define ATA_PRIMARY_CMD_BASE  0x1F0
#define ATA_PRIMARY_CTRL_BASE 0x3F6

#define ATA_REG_DATA     0
#define ATA_REG_ERROR    1
#define ATA_REG_FEATURES 1
#define ATA_REG_SECCOUNT 2
#define ATA_REG_LBA_LOW  3
#define ATA_REG_LBA_MID  4
#define ATA_REG_LBA_HIGH 5
#define ATA_REG_HDDEVSEL 6
#define ATA_REG_STATUS   7
#define ATA_REG_COMMAND  7

#define ATA_REG_CONTROL  0

#define ATA_CMD_READ_PIO 0x20
#define ATA_CMD_IDENTIFY 0xEC

#define ATA_SR_BSY  0x80
#define ATA_SR_DRDY 0x40
#define ATA_SR_DF   0x20
#define ATA_SR_DRQ  0x08
#define ATA_SR_ERR  0x01

#define ATA_DEV_MASTER 0x00
#define ATA_DEV_SLAVE  0x10

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t val);
static inline uint16_t inw(uint16_t port);
static inline void outw(uint16_t port, uint16_t val);

static void ata_io_delay(void) {
	for (int i = 0; i < 4; i++) {
		(void)inb(ATA_PRIMARY_CTRL_BASE);
	}
}

static int ata_wait_irq(void) {
	uint8_t status;
	do {
		status = inb(ATA_PRIMARY_CMD_BASE + ATA_REG_STATUS);
	} while (status & ATA_SR_BSY);
	if (status & ATA_SR_ERR) return -1;
	if (!(status & ATA_SR_DRQ)) return -2;
	return 0;
}

int ata_identify(uint16_t *buffer) {
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_HDDEVSEL, 0xA0 | ATA_DEV_MASTER);
	ata_io_delay();
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
	ata_io_delay();
	uint8_t status = inb(ATA_PRIMARY_CMD_BASE + ATA_REG_STATUS);
	if (status == 0) return -1;
	if (ata_wait_irq() != 0) return -2;
	for (int i = 0; i < 256; i++) {
		buffer[i] = inw(ATA_PRIMARY_CMD_BASE + ATA_REG_DATA);
	}
	return 0;
}

int ata_read_sector_lba28(uint32_t lba, uint8_t *buffer) {
	if (lba > 0x0FFFFFFF) return -1;
	outb(ATA_PRIMARY_CTRL_BASE, 0); // clear nIEN
	while (inb(ATA_PRIMARY_CMD_BASE + ATA_REG_STATUS) & ATA_SR_BSY);
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_HDDEVSEL, 0xE0 | ATA_DEV_MASTER | ((lba >> 24) & 0x0F));
	ata_io_delay();
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_SECCOUNT, 1);
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_LBA_LOW, (uint8_t)(lba & 0xFF));
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_LBA_MID, (uint8_t)((lba >> 8) & 0xFF));
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_LBA_HIGH, (uint8_t)((lba >> 16) & 0xFF));
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
	if (ata_wait_irq() != 0) return -2;
	uint8_t status = inb(ATA_PRIMARY_CMD_BASE + ATA_REG_STATUS);
	if (status & (ATA_SR_ERR | ATA_SR_DF)) return -3;
	for (int i = 0; i < 256; i++) {
		uint16_t val = inw(ATA_PRIMARY_CMD_BASE + ATA_REG_DATA);
		buffer[i * 2] = val & 0xFF;
		buffer[i * 2 + 1] = val >> 8;
	}
	return 0;
}
int ata_write_sector_lba28(uint32_t lba, const uint8_t *buffer) {
	if (lba > 0x0FFFFFFF) return -1;
	outb(ATA_PRIMARY_CTRL_BASE, 0); // clear nIEN
	while (inb(ATA_PRIMARY_CMD_BASE + ATA_REG_STATUS) & ATA_SR_BSY);
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_HDDEVSEL, 0xE0 | ATA_DEV_MASTER | ((lba >> 24) & 0x0F));
	ata_io_delay();
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_SECCOUNT, 1);
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_LBA_LOW, (uint8_t)(lba & 0xFF));
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_LBA_MID, (uint8_t)((lba >> 8) & 0xFF));
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_LBA_HIGH, (uint8_t)((lba >> 16) & 0xFF));
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_COMMAND, 0x30); // ATA_CMD_WRITE_PIO (0x30)
	if (ata_wait_irq() != 0) return -2;
	for (int i = 0; i < 256; i++) {
		uint16_t val = buffer[i * 2] | (buffer[i * 2 + 1] << 8);
		outw(ATA_PRIMARY_CMD_BASE + ATA_REG_DATA, val);
	}
	outb(ATA_PRIMARY_CMD_BASE + ATA_REG_COMMAND, 0xE7);
	if (ata_wait_irq() != 0) return -3;
	return 0;
}

static inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	__asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
	__asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}