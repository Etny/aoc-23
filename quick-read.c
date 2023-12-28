#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "quick-read.h"

int read_ulong_array (char* str, unsigned long* buf) {
    int i = 0;
    char* token = strtok(str, " ");

    while (token) {
        buf[i++] = atol(token);
        token = strtok(NULL, " ");
    }

    return i;
}

int read_int_array (char* str, int* buf) {
    int i = 0;
    char* token = strtok(str, " ");

    while (token) {
        buf[i++] = atoi(token);
        token = strtok(NULL, " ");
    }

    return i;
}

size_t get_array_len(char* str) {
    size_t i = 0;
    char* cpy = strdup(str);
    char* token = strtok(cpy, " ");

    while (token != NULL) {
        i++;
        token = strtok(NULL, " ");
    }

    return i;
}

Lines* read_lines(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    Lines* lines = (Lines*)malloc(sizeof(Lines));
    
    if (!file || !lines)  {
        printf("Couldn't open file %s", file_path);
        return NULL;
    }
    
    Lines lines_temp = {file, NULL, 0, 0};
    *lines = lines_temp;

    return lines;
}

void close_lines(Lines* lines) {
    fclose(lines->file);
    free(lines->line);
    free(lines);
}
    
char* next_line(Lines* lines) {
    lines->read = getline(&lines->line, &lines->len, lines->file);
    return lines-> read > 0 ? lines->line : NULL;
}

Lines* rewind_lines(Lines* lines) {
    rewind(lines->file);
    return lines;
}
