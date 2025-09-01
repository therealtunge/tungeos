struct context {
	unsigned int eax, ebx, ecx, edx;
	unsigned int esi, edi, ebp, esp;
	unsigned int eflags;
	unsigned short ds, es, fs, gs;
};

void dump_context(struct context *ctx);