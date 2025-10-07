CC := i686-elf-gcc
CFLAGS := -Wall -O0 -I../include -nostdlib -fno-builtin -nostdinc
SRCS := $(wildcard *.c) $(wildcard */*.c)
OBJS := $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

.PHONY: all clean