#include <fat/pff.h>
#include <context.h>

#define USERSPACE_BASE ((unsigned char *)0x10000000)

typedef struct {
	unsigned int entrypoint;
	unsigned int size;
} header;

int load_executable(char *path) {
	struct context ctx, ctx1;
//	serial_printf("a\n");
	int res = pf_open(path);
	if (res) {return res;}
	header hdr;
//	serial_printf("b\n");
	res = pf_read(&hdr, 8);
//	serial_printf("%d, %d, %d\n", res, hdr.entrypoint, hdr.size);
	if (res) {return res;}
//	serial_printf("c\n");
	res = pf_read(((unsigned char *)0x10000000), hdr.size);
//	serial_printf("%d ", res);
	if (res) {return res;}
//	serial_printf("d\n");
	save_context(&ctx);
	dump_context(&ctx);
	__asm__ volatile("call *%0" :: "r"(USERSPACE_BASE + hdr.entrypoint));
	restore_context(&ctx);
	return 0;
}