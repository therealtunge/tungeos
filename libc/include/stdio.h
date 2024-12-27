
#ifndef STDIO_H
#define STDIO_H
void puts(const char *string);
void putc(const char string);
void printf(char *format, ...);
int getcursor_x();
int getcursor_y();
void terminal_initialize();
void update_cursor(int _x, int _y);
int strlen(const char* str);
#endif