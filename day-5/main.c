#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include "../quick-read.h"
#include "../quick-start.h"
#include "../vec.c"
#include <string.h>

typedef struct {
    ulong dest_start;
    ulong src_start;
    ulong len;
} map_range;

// A map is a vector of map ranges;
typedef struct {
    ulong* seeds;
    ulong seed_count;
    Vector *maps; 
} seed_map;

typedef enum {
    MAP_RANGE,
    MAP_HEADER,
    DELIMINATOR
} line_type;

line_type get_line_type(char* str) {
    if (strcmp(str, "\n") == 0)
        return DELIMINATOR;

    for (int i = 0; str[i]; i++)
        if (isalpha(str[i]))
            return MAP_HEADER;

    return MAP_RANGE;
}

ulong read_nums(char* str, ulong* buf) {
    int i = 0;
    char* token = strtok(str, " ");

    while (token) {
        buf[i++] = atol(token);
        token = strtok(NULL, " ");
    }

    return i;
}

map_range read_map_range(char* str) {
    map_range range;
    sscanf(str, "%ld %ld %ld", &range.dest_start, &range.src_start, &range.len);
    return range;
}

seed_map ingest_map(Lines* lines) {
    seed_map map;
    map.maps = vec_init(sizeof(Vector*), 10);

    next_line(lines);

    //read seeds
    char seed_nums[1000];
    ulong seeds[1000];
    sscanf(lines->line, "seeds: %[0-9 ]", seed_nums);
    map.seed_count = read_nums(seed_nums, seeds);
    map.seeds = malloc(sizeof(ulong) * map.seed_count);
    memcpy(map.seeds, &seeds, sizeof(ulong) * map.seed_count);

    Vector *next_map;
    
    while (next_line(lines)) {
        if (get_line_type(lines->line) == MAP_RANGE) {
            next_map = vec_init(sizeof(map_range), 5);
            do {
                if (get_line_type(lines->line) != MAP_RANGE)
                    break;
                map_range new_range = read_map_range(lines->line);
                vec_insert(next_map, &new_range);
            } while (next_line(lines));
            vec_insert(map.maps, &next_map);
        }
    }

    return map;
}

ulong map_num(ulong num, Vector *map) {
    map_range range;

    for (int i = 0; i < map->count; i++) {
        range = *(map_range*)vec_at(map, i);

        if (range.src_start <= num && (range.src_start + range.len) > num) 
            return range.dest_start + (num - range.src_start); 
    }

    return num;
}

void part_one(seed_map seed_map) {
    int i, j;
    ulong lowest_loc = ULONG_MAX;
    for (i = 0; i < seed_map.seed_count; i++) {
        ulong seed = seed_map.seeds[i];

        for (j = 0; j < seed_map.maps->count; j++) {
            Vector *map = *(Vector**)vec_at(seed_map.maps, j);
            seed = map_num(seed, map);
        }

        if(seed < lowest_loc)
            lowest_loc = seed;
    }

    printf("Part one: %ld\n", lowest_loc);
}

int main(int argc, char** argv) {
    read_input("in-test.txt");

    seed_map map = ingest_map(file);
    part_one(map);          
}
