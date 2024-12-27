#include <kernel.h>
#include <stdio.h>

void kprint(const char *msg) {
	puts(msg); // TODO: implement kprint 
}

void panic() {
	// TODO: implement panic
	puts("!! kernel panic !!");
	while (1) {
	}
}