#include "../vec.c"
#include "../quick-read.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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
    
    int ii = 0;
    char* cc;
    while ((cc = vec_at(lines_vec, ii++))) {
        printf("%s\n", cc); 
    }

    int i = 0, x, y, total = 0;
    char c;
    num* cur;
    while ( (cur = vec_at(num_vec, i++))) {
        for (x = cur->collumn-1; x <= cur->collumn+cur->length+1; x++) {
            if (x < 0) continue;
            for (y = cur->line-1; y <= cur->line+1; y++) {
                if (y < 0 || y >= lines_vec->count) continue;
                if (y == cur->line && x >= cur->collumn && x < cur->collumn + cur->length) continue;
                c = ((char*)vec_at(lines_vec, y))[x];
                if (c && c != '.' && !isdigit(c)) {
                    printf("Gottem: %d, %d: %c\n", cur->line, cur->collumn, c);
                    goto end;
                } 
            }
        }
        
        end: asm("NOP");
    } 

    printf("Part 1: %d\n", total);
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
