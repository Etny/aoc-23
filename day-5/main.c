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

ulong map_num_through_range(ulong num, Vector *map) {
    map_range range;

    for (int i = 0; i < map->count; i++) {
        range = *(map_range*)vec_at(map, i);

        if (range.src_start <= num && (range.src_start + range.len) > num) 
            return range.dest_start + (num - range.src_start); 
    }

    return num;
}

ulong seed_to_loc(ulong seed, Vector *maps) {
    Vector *next_map;
    for (int i = 0; i < maps->count; i++) {
        next_map = *(Vector**)vec_at(maps, i);
        seed = map_num_through_range(seed, next_map);
    }

    return seed;
}

void part_one(seed_map seed_map) {
    int i, j;
    ulong lowest_loc = ULONG_MAX, seed;
    for (i = 0; i < seed_map.seed_count; i++) {
        seed = seed_to_loc(seed_map.seeds[i], seed_map.maps); 

        if (seed < lowest_loc)
            lowest_loc = seed;
    }

    printf("Part one: %ld\n", lowest_loc);
}

typedef struct {
    ulong start;
    ulong len;
} num_range;

void part_two(seed_map seed_map) {
    int i, j, k;
    // num_range *ranges = malloc(sizeof(num_range) * (seed_map.seed_count / 2));
    //
    // for (i = 0; i < seed_map.seed_count / 2; i++) 
    //     ranges[i] = (num_range) { seed_map.seeds[i * 2], seed_map.seeds[(i * 2) + 1] };
    //
    Vector *ranges = vec_init(sizeof(num_range), seed_map.seed_count);
    Vector *next_ranges;
    num_range cur, next;

    for (i = 0; i < seed_map.seed_count / 2; i++) {
        cur = (num_range) { seed_map.seeds[i * 2], seed_map.seeds[(i * 2) + 1]};
        vec_insert(ranges, &cur);
    }

    Vector *map;
    map_range cur_mr;
    ulong head_dif;

    for (i = 0; i < seed_map.maps->count; i++) {
        map = *(Vector**)vec_at(seed_map.maps, i);
        next_ranges = vec_init(sizeof(num_range), ranges->count);
        
        for (j = 0; j < ranges->count; j++) {
            cur = *(num_range*)vec_at(ranges, j);
            
            for (k = 0; k < map->count; k++) {
                cur_mr = *(map_range*)vec_at(map, k);

                // seed range head is inside map range
                if (cur_mr.src_start <= cur.start && (cur_mr.src_start + cur_mr.len) >= cur.start) {
                    head_dif = cur.start - cur_mr.src_start;
                    // seed range is not completely contained, split at map range tail 
                    if ((cur.start + cur.len) > (cur_mr.src_start + cur_mr.len)) {
                        next = (num_range) { cur_mr.src_start + cur_mr.len + 1, (cur.start + cur.len) - (cur_mr.src_start + cur_mr.len) };
                        vec_insert(ranges, &next);

                        next = (num_range) { cur_mr.dest_start + head_dif, cur_mr.len - head_dif};
                    }
                    // seed range is completely contained
                    else 
                        next = (num_range){ cur_mr.dest_start + head_dif, cur.len };
                    
                } 
                // seed range tail is inside map range
                else if ((cur.start + cur.len) >= cur_mr.src_start && (cur.start + cur.len) <= (cur_mr.src_start + cur_mr.len) ) {
                    // seed range is not fully contained, split at map range head
                    next = (num_range) { cur.start, cur.len - 1 - (cur.start + cur.len - cur_mr.src_start)};
                    vec_insert(ranges, &next);
                    next = (num_range) { cur_mr.dest_start, (cur.start + cur.len - cur_mr.src_start )};
                } else {
                    next = cur;
                }

                vec_insert(next_ranges, &next);
                            
            }
        }

        printf("After map %d:\n", i);
        for (k = 0; k < ranges->count; k++) {
            cur = *(num_range*)vec_at(ranges, k);
            printf("\t start %ld, len %ld\n", cur.start, cur.len);
        }

        vec_free(ranges);
        ranges = next_ranges;
    }

    ulong lowest_start = ULONG_MAX;

    for (i = 0; i < next_ranges->count; i++) {
        cur = *(num_range*)vec_at( next_ranges, i);
        if (cur.start < lowest_start)
            lowest_start = cur.start;
    }

    printf("Part two: %ld\n", lowest_start);
}

int main(int argc, char** argv) {
    read_input("in-test.txt");

    seed_map map = ingest_map(file);
    part_one(map);          
    part_two(map);
}
