CC := i686-elf-gcc
CFLAGS := -Wall -O0 -I../libc/include -I./include -nostdlib -fno-builtin
SRCS := $(wildcard *.c) $(wildcard */*.c) idt_load.s
OBJS := $(SRCS:.c=.o) idt_load.o

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f kernel.o

.PHONY: all clean
