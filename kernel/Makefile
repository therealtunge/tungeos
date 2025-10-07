CC := i686-elf-gcc
CFLAGS := -Wall -O0 -I../lib/include -I../include -nostdlib -fno-builtin
SRCS := $(wildcard *.c) $(wildcard */*.c)
OBJS := $(SRCS:.c=.o)

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

.PHONY: all clean