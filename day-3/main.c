#include "../vec.c"
#include "../quick-read.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int line;
    int collumn;
    int length;
} num;

void map_pieces(Lines *lines) {
    Vector *lines_vec = vec_init(sizeof(char*), 10);
    Vector *num_vec = vec_init(sizeof(num), 10);

    char* line_copy;
    int line = 0;

    while (next_line(lines)) {
        line_copy = malloc(strlen(lines->line) + 1);
        strcpy(line_copy, lines->line);
        vec_insert(lines_vec, &line_copy);

        int length, collumn = 0;
    
        while(lines->line[collumn]) {
            if(isdigit(lines->line[collumn])) {
                length = 0;
                while (lines->line[collumn + length] && isdigit(lines->line[collumn + length]))
                    length ++;
                num n = {line, collumn, length};
                vec_insert(num_vec, &n);
                collumn += length;
            } else
                collumn++;
        }


        line++;
    }

    int i = 0;
    num* a;
    while ( (a = vec_at(num_vec, i++))) {
        printf("line %d, collum %d, len %d\n", a->line, a->collumn, a->length);
    } 
}

int main(int argc, char** argv) {
    char* file_name = argc < 2 ? "in-test.txt" : argv[1];
    Lines* lines = read_lines(file_name);

    if(lines == NULL) {
        printf("Can't open file: %s", file_name);
        return 1;
    }

    map_pieces(lines);
}
