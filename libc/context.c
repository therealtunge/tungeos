#include <context.h>
#include <stdint.h>

void save_context(struct context *ctx) {
	__asm__ volatile("movl %%eax, %0" : "=m"(ctx->eax));
	__asm__ volatile("movl %%ebx, %0" : "=m"(ctx->ebx));
	__asm__ volatile("movl %%ecx, %0" : "=m"(ctx->ecx));
	__asm__ volatile("movl %%edx, %0" : "=m"(ctx->edx));
	__asm__ volatile("movl %%esi, %0" : "=m"(ctx->esi));
	__asm__ volatile("movl %%edi, %0" : "=m"(ctx->edi));
	__asm__ volatile("movl %%ebp, %0" : "=m"(ctx->ebp));

	unsigned int eflags;
	__asm__ volatile("pushfl; popl %0" : "=r"(eflags));
	ctx->eflags = eflags;

	unsigned short seg;
	__asm__ volatile("movw %%ds, %0" : "=r"(seg)); ctx->ds = seg;
	__asm__ volatile("movw %%es, %0" : "=r"(seg)); ctx->es = seg;
	__asm__ volatile("movw %%fs, %0" : "=r"(seg)); ctx->fs = seg;
	__asm__ volatile("movw %%gs, %0" : "=r"(seg)); ctx->gs = seg;
}

void restore_context(const struct context *ctx) {
	__asm__ volatile("movw %0, %%ds" :: "r"(ctx->ds));
	__asm__ volatile("movw %0, %%es" :: "r"(ctx->es));
	__asm__ volatile("movw %0, %%fs" :: "r"(ctx->fs));
	__asm__ volatile("movw %0, %%gs" :: "r"(ctx->gs));

	__asm__ volatile("pushl %0; popfl" :: "r"(ctx->eflags));

	__asm__ volatile("movl %0, %%ebp" :: "m"(ctx->ebp));
	__asm__ volatile("movl %0, %%edi" :: "m"(ctx->edi));
	__asm__ volatile("movl %0, %%esi" :: "m"(ctx->esi));
	__asm__ volatile("movl %0, %%edx" :: "m"(ctx->edx));
	__asm__ volatile("movl %0, %%ecx" :: "m"(ctx->ecx));
	__asm__ volatile("movl %0, %%ebx" :: "m"(ctx->ebx));
	__asm__ volatile("movl %0, %%eax" :: "m"(ctx->eax));
}

void dump_context(struct context *ctx) {
	serial_printf("\nDS: %u ES: %u FS: %u GS: %u\n", ctx->ds, ctx->es, ctx->fs, ctx->gs);
	serial_printf("EFLAGS: %u EBP: %u EDI: %u ESI: %u\n", ctx->eflags, ctx->ebp, ctx->edi, ctx->esi);
	serial_printf("EAX: %u EBX: %u ECX: %u EDX: %u\n", ctx->eax, ctx->ebx, ctx->ecx, ctx->edx);
}