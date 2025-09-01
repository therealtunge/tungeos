TARGET := i686-elf
AS := as
CC := gcc
SRCS := boot.s
OBJS := $(SRCS:.s=.o)

all: $(OBJS)

%.o: %.s
	$(TARGET)-$(AS) -c $< -o $@

clean:
	rm -f $(OBJS)

.PHONY: all clean
