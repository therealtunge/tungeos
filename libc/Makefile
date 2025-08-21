TARGET = i686-elf
CC = gcc
LD = ld
CFLAGS := $(CFLAGS_EXTRA) -Wall -Iinclude -ffreestanding -nostdlib -fno-builtin -O0

SRCS := $(wildcard *.c)

OBJS := $(SRCS:.c=.o)

OUT := ../libc.o

.PHONY: all clean

all: $(OBJS) $(OUT)

%.o: %.c
	echo $(CFLAGS)
	$(TARGET)-$(CC) $(CFLAGS) -c -o $@ $<

$(OUT): $(OBJS)
	$(TARGET)-$(LD) -o $(OUT) $(OBJS) --oformat elf32-i386 -r 

clean:
	rm -f $(OBJS)
