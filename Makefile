TARGET = i686-elf
EMU_TARGET = x86_64

AS = as
CC = gcc
LD = gcc
CFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Ilibc/include -nostdlib -fno-builtin
FOLDERS = $(w)
.PHONY: clean
all: bin iso emu

ifeq (, $(shell which $(TARGET)-$(CC)))
$(error "get $(TARGET)-$(CC)")
endif
EXCLUDED_DIR := isodir
ALL_DIRS := $(shell find . -mindepth 1 -maxdepth 1 -type d ! -name $(EXCLUDED_DIR))

clean:
	cd boot; \
	make clean
	cd libc; \
	make clean
	cd kernel; \
	make clean
	rm -f $(wildcard *.o *.iso *.bin)
	rm -rf isodir
bin:
	@for dir in $(ALL_DIRS); do \
		$(MAKE) -C $$dir; \
	done
	$(TARGET)-$(LD) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot/boot.o kernel/kernel.o libc.o -lgcc
iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
emu:
	qemu-system-$(EMU_TARGET) -cdrom myos.iso
