#pragma once
#include <stdint.h>

int ata_identify(uint16_t *buffer, int slave);
int ata_read_sector_lba28(uint32_t lba, uint8_t *buffer, int slave);
int ata_write_sector_lba28(uint32_t lba, const uint8_t *buffer, int slave);