#ifndef QUICK_START
#define QUICK_START

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define read_input(str) char* file_name = argc > 1 ? argv[1] : str;\
Lines* file = read_lines(file_name);\
if (!file) { \
    printf("Can't open file"); \
    return 1; \
}

typedef struct {
    FILE* file;
    char* line;
    size_t len;
    ssize_t read;
} Lines;

int read_ulong_array (char* str, unsigned long* buf);
int read_int_array (char* str, int* buf);
size_t get_array_len(char* str);
Lines* read_lines(const char* file_path);
void close_lines(Lines* lines);
char* next_line(Lines* lines);
Lines* rewind_lines(Lines* lines);

#endif
