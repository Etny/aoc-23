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

int cmp_num_range(const void *elem1, const void *elem2) {
    num_range r1 = *(num_range*)elem1;
    num_range r2 = *(num_range*)elem2;

    if (r1.start > r2.start) return 1;
    if (r2.start > r1.start) return -1;
    return 0;
}

// this sucks
void part_two(seed_map seed_map) {
    int i;
    Vector *ranges = vec_init(sizeof(num_range), seed_map.seed_count);
    num_range src, next;

    for (i = 0; i < seed_map.seed_count / 2; i++) {
        src = (num_range) { seed_map.seeds[i * 2], seed_map.seeds[(i * 2) + 1]};
        vec_insert(ranges, &src);
    }

    Vector *next_ranges = vec_init(sizeof(num_range), 10);
    Vector *matched_ranges = vec_init(sizeof(num_range), 10);

    for (i = 0; i < seed_map.maps->count; i++) {
        Vector *map = *(Vector**)vec_at(seed_map.maps, i);
        vec_clear(next_ranges);
        
        for (int j = 0; j < ranges->count; j++) {
            num_range src = *(num_range*)vec_at(ranges, j);
            ulong src_tail = src.start + src.len - 1;

            for (int k = 0; k < map->count; k++) {
                map_range mr = *(map_range*)vec_at(map, k);
                ulong mr_tail = mr.src_start + mr.len - 1;

                if (mr.src_start > src_tail || mr_tail < src.start)
                    continue;

                ulong overlap_head = mr.src_start > src.start ? mr.src_start : src.start;   // max of heads
                ulong overlap_tail = mr_tail < src_tail ? mr_tail : src_tail;               // min of tails
                ulong overlap_len = overlap_tail - overlap_head + 1;
                
                next = (num_range) { overlap_head, overlap_len };
                vec_insert(matched_ranges, &next);

                next = (num_range) { mr.dest_start + (overlap_head - mr.src_start), overlap_len };
                vec_insert(next_ranges, &next);
            }
            
            if (matched_ranges->count > 0) {
                vec_sort(matched_ranges, cmp_num_range);
                ulong cmp_head = src_tail;
                for (int h = matched_ranges->count-1; h >= 0; h--) {
                    num_range cmp = *(num_range*)vec_at(matched_ranges, h);
                    ulong cmp_tail = cmp.start + cmp.len - 1;

                    if (cmp_tail < cmp_head) {
                        next = (num_range) { cmp_tail + 1, cmp_head - cmp_tail };
                        vec_insert(next_ranges, &next);
                    }

                    cmp_head = cmp.start;
                }

                if (cmp_head > src.start) {
                    next = (num_range) { src.start, src.start - cmp_head };
                    vec_insert(next_ranges, &next);
                }
                vec_clear(matched_ranges);
            } else {
                vec_insert(next_ranges, &src);
            }
        }
                            
        Vector *tmp = ranges;
        ranges = next_ranges;
        next_ranges = tmp;
    }

    ulong lowest_start = ULONG_MAX;

    for (i = 0; i < ranges->count; i++) {
        src = *(num_range*)vec_at( ranges, i);
        if (src.start < lowest_start)
            lowest_start = src.start;
    }

    printf("Part two: %ld\n", lowest_start);
}

int main(int argc, char** argv) {
    read_input("in-test.txt");

    seed_map map = ingest_map(file);
    part_one(map);   
    part_two(map);
}
