.section .multiboot
.align 4

.set REQUEST_FLAGS, 7

.set MAGIC, 0x1badb002
.set FLAGS, REQUEST_FLAGS  /* crt mode */
.set CHECKSUM, -(MAGIC + FLAGS)
.set MODE_TYPE, 0
.set WIDTH, 1024  /* requested width */
.set HEIGHT, 720  /* requested height */
.set DEPTH, 32    /* requested bits per pixel BPP */
.set HEADER_ADDR, 0
.set LOAD_ADDR, 0
.set LOAD_END_ADDR, 0
.set BSS_END_ADDR, 0
.set ENTRY_ADDR, 0

.section .multiboot
multiboot:
    .long MAGIC
    .long FLAGS
    .long CHECKSUM
    .long HEADER_ADDR
    .long LOAD_ADDR
    .long LOAD_END_ADDR
    .long BSS_END_ADDR
    .long ENTRY_ADDR
    .long MODE_TYPE
    .long WIDTH
    .long HEIGHT
    .long DEPTH
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*
The linker script specifies _start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
.global _start
.type _start, @function
_start:
	/*
	The bootloader has loaded us into 32-bit protected mode on a x86
	machine. Interrupts are disabled. Paging is disabled. The processor
	state is as defined in the multiboot standard. The kernel has full
	control of the CPU. The kernel can only make use of hardware features
	and any code it provides as part of itself. There's no printf
	function, unless the kernel provides its own <stdio.h> header and a
	printf implementation. There are no security restrictions, no
	safeguards, no debugging mechanisms, only what the kernel provides
	itself. It has absolute and complete power over the
	machine.
	*/

	/*
	To set up a stack, we set the esp register to point to the top of the
	stack (as it grows downwards on x86 systems). This is necessarily done
	in assembly as languages such as C cannot function without a stack.
	*/
	mov $stack_top, %esp

	/*
	This is a good place to initialize crucial processor state before the
	high-level kernel is entered. It's best to minimize the early
	environment where crucial features are offline. Note that the
	processor is not fully initialized yet: Features such as floating
	point instructions and instruction set extensions are not initialized
	yet. The GDT should be loaded here. Paging should be enabled here.
	C++ features such as global constructors and exceptions will require
	runtime support to work as well.
	*/

	/*
	Enter the high-level kernel. The ABI requires the stack is 16-byte
	aligned at the time of the call instruction (which afterwards pushes
	the return pointer of size 4 bytes). The stack was originally 16-byte
	aligned above and we've pushed a multiple of 16 bytes to the
	stack since (pushed 0 bytes so far), so the alignment has thus been
	preserved and the call is well defined.
	*/
	push %ebx
	call kernel_main

	/*
	If the system has nothing more to do, put the computer into an
	infinite loop. To do that:
	1) Disable interrupts with cli (clear interrupt enable in eflags).
	   They are already disabled by the bootloader, so this is not needed.
	   Mind that you might later enable interrupts and return from
	   kernel_main (which is sort of nonsensical to do).
	2) Wait for the next interrupt to arrive with hlt (halt instruction).
	   Since they are disabled, this will lock up the computer.
	3) Jump to the hlt instruction if it ever wakes up due to a
	   non-maskable interrupt occurring or due to system management mode.
	*/
	cli
1:	hlt
	jmp 1b

/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start