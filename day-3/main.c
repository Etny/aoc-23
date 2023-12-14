#include "../vec.c"
#include "../btree.c"
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

void pos_to_key(int x, int y, char* buf) {
    sprintf(buf, "%d,%d", x, y);
}

void part_one(engine_map map) {
    int i = 0, x, y, total_p1 = 0, total_p2 = 0;
    char c, **c_line, pos_buf[10];
    num* cur;
    bool hits_symbol;

    btree star_tree = btree_init();
    Vector *star_vec;

    while ( (cur = vec_at(map.num_vec, i++))) {
        hits_symbol = false;
        for (x = cur->collumn-1; x <= cur->collumn+cur->length; x++) {
            if (x < 0) continue;
            for (y = cur->line-1; y <= cur->line+1; y++) {
                if (y < 0) continue;
                // if (y == cur->line && x >= cur->collumn && x < cur->collumn + cur->length) continue;

                c_line = vec_at(map.lines_vec, y);
                if (!c_line) continue;

                c = (*c_line)[x];
                if (c && c != '.' && !isdigit(c)) {
                    hits_symbol = true;
                    
                    if (c == '*') {
                        pos_to_key(x, y, pos_buf);
                        star_vec = btree_get(&star_tree, pos_buf);

                        if (!star_vec) {
                            star_vec = vec_init(sizeof(num), 4);
                            btree_insert(&star_tree, pos_buf, star_vec);
                        }

                        if (!vec_contains_val(star_vec, cur))
                            vec_insert(star_vec, cur);
                    }
                }
            }
        }
        
        if (hits_symbol)
            total_p1 += cur->num_val;
    } 

    btree_iter iter = btree_iterate(&star_tree);
    btree_node *n;

    while ((n = next_btree_node(&iter))) {
        star_vec = (Vector*)n->data;
        if (star_vec->count == 2) {
            num *n1 = vec_at(star_vec, 0);
            num *n2 = vec_at(star_vec, 1);
            total_p2 += n1->num_val * n2->num_val;
        }

    }


    printf("Part 1: %d\n", total_p1);
    printf("Part 2: %d\n", total_p2);
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
