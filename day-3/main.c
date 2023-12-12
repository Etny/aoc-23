#include "../vec.c"
#include "../quick-read.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    int line;
    int collumn;
    int length;
    int num_val;
} num;

typedef struct {
    Vector *num_vec;
    Vector *lines_vec;
} engine_map;

engine_map map_pieces(Lines *lines) {
    engine_map result;
    result.num_vec = vec_init(sizeof(num), 10);
    result.lines_vec = vec_init(sizeof(char*), 10);

    char* line_copy;
    int line = 0, line_len;

    while (next_line(lines)) {
        line_len = strlen(lines->line);
        line_copy = malloc(line_len);
        strncpy(line_copy, lines->line, line_len);
        line_copy[line_len-1] = '\0';   //remove trailing newline
        vec_insert(result.lines_vec, &line_copy);

        int length, num_val, collumn = 0;
    
        while(lines->line[collumn]) {
            if(isdigit(lines->line[collumn])) {
                length = 0;
                num_val = 0;
                while (lines->line[collumn + length] && isdigit(lines->line[collumn + length])) {
                    num_val = (num_val * 10) + (lines->line[collumn + length] - '0');
                    length ++;
                }
                num n = {line, collumn, length, num_val};
                vec_insert(result.num_vec, &n);
                collumn += length;
            } else
                collumn++;
        }
        line++;
    }
    
    return result;
}

void part_one(engine_map map) {
    int i = 0, x, y, total = 0;
    char c, **c_line;
    num* cur;
    while ( (cur = vec_at(map.num_vec, i++))) {
        for (x = cur->collumn-1; x <= cur->collumn+cur->length; x++) {
            if (x < 0) continue;
            for (y = cur->line-1; y <= cur->line+1; y++) {
                if (y < 0) continue;
                // if (y == cur->line && x >= cur->collumn && x < cur->collumn + cur->length) continue;

                c_line = vec_at(map.lines_vec, y);
                if (!c_line) continue;

                c = (*c_line)[x];
                if (c && c != '.' && !isdigit(c)) {
                    total += cur->num_val;
                    // printf("Gottem: num %d at %d,%d hit %c @ %d,%d\n", cur->num_val, cur->line, cur->collumn, c, x, y);
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

    engine_map map = map_pieces(lines);
    part_one(map);
}
