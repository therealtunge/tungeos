
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		putc(bytes[i]);
	return true;
}

void kernel_main(void)
{
	int i;
	terminal_initialize();
	print("abobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaabobaaboba", 100);
	/*printf(" ___ _,_ _, _  _, __,  _,  _,\n\
  |  | | |\\ | / _ |_  / \\ (_ \n\
  |  | | | \\| \\ / |   \\ / , )\n\
  ~  `~' ~  ~  ~  ~~~  ~   ~   \n\
	");*/
}
