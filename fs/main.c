#include <stdio.h>
#include <stdint.h>
#include <ata.h>
typedef int bool;
#define false 0
#define true 1
#include "structs.h"


uint32_t BPB_FAT_addr(struct BPB_t *bpb)
{
	return bpb->RsvdSecCnt * bpb->BytsPerSec;
}


uint32_t BPB_Root_addr(struct BPB_t *bpb)
{
	return BPB_FAT_addr(bpb) + bpb->NumFATs * bpb->FATSz16 * bpb->BytsPerSec;
}

uint32_t BPB_Data_addr(struct BPB_t *bpb)
{
	return BPB_Root_addr(bpb) + bpb->RootEntCnt * 32;
								//size of root
}

uint32_t BPB_Data_Sector_count(struct BPB_t *bpb)
{
	return bpb->TotSec32 - BPB_Data_addr(bpb) / bpb->BytsPerSec;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, int size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (int i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

bool read_bytes(unsigned int offset, void *buf, unsigned int length)
{
//	serial_printf("%d %d", offset, length);
	if (length > 512) {
		panic();
	}
	uint8_t fakebuf[512];
	uint32_t lba = offset / 512;
	ata_read_sector_lba28(lba, fakebuf);
	memcpy(buf, fakebuf + (offset % 512), length);
	return true;
}

#define LEN_BOOT 446
#define LEN_PART 16
#define NUM_PARTS 4
#define BEGIN_PART_LBA 8
#define LEN_PART_LBA 4
#define SIG 0xAA55
bool check_sig()
{
	uint16_t sig;
	if(!read_bytes(510, &sig, sizeof(sig)))
		return false;
	if(sig != SIG) {
		serial_printf("sanity check failed:\n");
		serial_printf("%x", sig / 256);
		serial_printf("%x", sig % 256);
	}
	return sig == SIG;
}

uint16_t get_next_cluster(struct BPB_t *bpb, uint16_t cluster)
{
	uint16_t result;
	read_bytes(BPB_FAT_addr(bpb) + cluster*2, &result, sizeof(result));
	return result;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
void print_cluster(struct BPB_t *bpb, struct dir_t *dir)
{
	const uint32_t cluster_size = bpb->BytsPerSec * bpb->SecPerClus;

	for(uint16_t c = dir->FstClusLO; c < 0xFFF8; c = get_next_cluster(bpb, c)) {
		char buf[cluster_size];
		//FAT16 spec says that you have to subtract 2 from the cluster number to get the actual data
		//so weird. but whatevs....
		read_bytes(BPB_Data_addr(bpb) + (c - 2) * cluster_size, buf, sizeof(buf));
		serial_printf("%s", (int)(sizeof(buf)), buf);
	}
}

int testfs()
{
	if(!check_sig())
		return 1;

	struct BPB_t bpb;
	read_bytes(	0x0, &bpb, sizeof(bpb));

	if(bpb.BytsPerSec != 512 || bpb.NumFATs != 2)
		return 1;
	serial_printf( "FAT addr: 0x%x\n"
			"Root addr: 0x%x\n"
			"Data addr: 0x%x\n",
			BPB_FAT_addr(&bpb),
			BPB_Root_addr(&bpb),
			BPB_Data_addr(&bpb)
		  );

	for(int i = 0; i < bpb.RootEntCnt; i++) {
		struct dir_t dir;
		uint32_t offset = BPB_Root_addr(&bpb) + i*32;
		read_bytes(offset, &dir, sizeof(dir));

		if(dir.Name[0] == 0) {
			break;
		} else if(dir.Name[0] == 0xE5) {
			serial_printf("<unused space>\n");
			continue;
		}


		serial_printf("\n");
		for (int i = 0; i < (int)(sizeof(dir.Name)/sizeof(char)); i++){
			serial_printf("%c", dir.Name[i]);
		}
		if(dir.Attr & DIR_ATTR_LFN) {
			serial_printf("<LONG FILENAME>\n");
		} else if(dir.Attr & DIR_ATTR_DIRECTORY) {
			serial_printf("<DIRECTORY>\n");
		} else {
			serial_printf("<FILE>\n");

			if(i == 0xF)
				print_cluster(&bpb, &dir);
			else
				serial_printf("<omitting print>\n");
		}
	}

	return 0;
}

