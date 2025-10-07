TARGET = i686-elf
EMU_TARGET = i386

AS = as
CC = gcc
LD = gcc
CFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Iinclude -nostdlib -fno-builtin -g -nostdinc
FOLDERS = kernel boot utils lib
.PHONY: clean
all: bin iso emu

ifeq (, $(shell which $(TARGET)-$(CC)))
$(error "get $(TARGET)-$(CC)")
endif

clean:
	cd boot; \
	make clean
	cd utils; \
	make clean
	cd kernel; \
	make clean
	cd lib; \
	make clean
	rm -f $(wildcard *.o *.iso *.bin)
	rm -rf isodir
bin:
	@for dir in $(FOLDERS); do \
		$(MAKE) -C $$dir; \
	done
	$(TARGET)-$(LD) -r -o tungeos.o -ffreestanding -O2 -nostdlib boot/boot.o utils/*.o kernel/*.o lib/*.o
	$(TARGET)-$(LD) -T linker.ld -ffreestanding -nostdlib -o tungeos.bin -ffreestanding -O2 tungeos.o
iso:
	mkdir -p isodir/boot/grub
	cp tungeos.bin isodir/boot/tungeos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o tungeos.iso isodir
emu:
	qemu-system-$(EMU_TARGET) -m 4G -cdrom tungeos.iso