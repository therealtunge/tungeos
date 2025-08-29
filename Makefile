TARGET = i686-elf
EMU_TARGET = x86_64

AS = as
CC = gcc
LD = gcc
CFLAGS = -c -std=gnu99 -ffreestanding -O2 -Wall -Ilibc/include -nostdlib -fno-builtin -g
FOLDERS = kernel libc boot vga fs
.PHONY: clean
all: bin iso emu

ifeq (, $(shell which $(TARGET)-$(CC)))
$(error "get $(TARGET)-$(CC)")
endif

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
	$(MAKE) -C libc CFLAGS_EXTRA=-D_LIBK OUT=../libk.o
	@for dir in $(FOLDERS); do \
		$(MAKE) -C $$dir; \
	done
	$(TARGET)-$(LD) -r -o myos.o -ffreestanding -O2 -nostdlib boot/boot.o vga.o kernel/kernel.o libk.o fs.o
	$(TARGET)-$(LD) -T linker.ld -ffreestanding -nostdlib -o myos.bin -ffreestanding -O2 myos.o
iso:
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
emu:
	qemu-system-$(EMU_TARGET) -m 4G -boot d -drive id=cdrom,media=cdrom,file=myos.iso,if=ide,bus=0,unit=1 -drive format=raw,id=disk,file=fat32.img,if=ide,bus=0,unit=0 -serial stdio
