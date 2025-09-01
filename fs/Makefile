TARGET = i686-elf
CC = gcc
LD = ld
CFLAGS := -Wall -Iinclude -I../libc/include -ffreestanding -nostdlib -fno-builtin -O0 -fPIE

SRCS := $(wildcard *.c)

OBJS := $(SRCS:.c=.o)

OUT := ../fs.o

.PHONY: all clean

all: $(OBJS) $(OUT)

%.o: %.c
	$(TARGET)-$(CC) $(CFLAGS) -c -o $@ $<

$(OUT): $(OBJS)
	$(TARGET)-$(LD) -o $(OUT) $(OBJS) --oformat elf32-i386 -r 

clean:
	rm -f $(OBJS)
