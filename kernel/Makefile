CC := i686-elf-gcc
CFLAGS := -Wall -O0 -I../vga/include -I../libc/include -I./include -nostdlib -fno-builtin
SRCS := $(wildcard *.c) $(wildcard */*.c) idt_load.s
OBJS := $(SRCS:.c=.o) idt_load_s.o

all: $(OBJS)

idt_load_s.o: idt_load.s
	i686-elf-as idt_load.s -o idt_load_s.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

.PHONY: all clean
