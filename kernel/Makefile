CC := i686-elf-gcc
AS := nasm
ASFLAGS := -felf
CFLAGS := -Wall -O0 -Iinclude -I../lib/include -I../include -nostdlib -fno-builtin
SRCS := $(wildcard *.c) $(wildcard */*.c) isr_asm.s gdt.s
OBJS := $(SRCS:.c=.o) isr_asm.o gdt.o

all: $(OBJS)



%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f *.o

.PHONY: all clean