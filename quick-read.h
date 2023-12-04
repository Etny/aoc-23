#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    FILE* file;
    char* line;
    size_t len;
    ssize_t read;
} Lines;

Lines* read_lines(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    Lines* lines = malloc(sizeof(Lines));
    
    if (!file || !lines) return NULL;
    
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
