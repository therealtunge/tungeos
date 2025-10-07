AS := i686-elf-as
SRCS := boot.s
OBJS := boot.o
all: $(OBJS)

%.o: %.s
	$(AS) -c $< -o $@

clean:
	rm -f *.o

.PHONY: all clean