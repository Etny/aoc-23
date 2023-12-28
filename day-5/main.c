#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include "../quick-read.h"
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
    size_t seed_count;
    Vector *maps; 
} seed_map;

bool is_map_range(char* str) {
    if (strcmp(str, "\n") == 0)
        return false;

    for (int i = 0; str[i]; i++)
        if (isalpha(str[i]))
            return false;

    return true;
}

seed_map ingest_map(Lines* lines) {
    seed_map map;
    map.maps = vec_init(sizeof(Vector*), 10);

    next_line(lines);

    //read seeds
    char seed_nums[1000];
    sscanf(lines->line, "seeds: %[0-9 ]", seed_nums);
    map.seed_count = get_array_len(seed_nums);
    map.seeds = malloc(sizeof(ulong) * map.seed_count);
    read_ulong_array(seed_nums, map.seeds);

    while (next_line(lines)) {
        if (is_map_range(lines->line)) {
            Vector *next_map = vec_init(sizeof(map_range), 5);
            do {
                map_range range;
                sscanf(lines->line, "%ld %ld %ld", &range.dest_start, &range.src_start, &range.len);
                vec_insert(next_map, &range);
            } while (next_line(lines) && is_map_range(lines->line));
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

// this sucks (but a little less than before)
void part_two(seed_map seed_map) {
    int i;
    Vector *ranges = vec_init(sizeof(num_range), seed_map.seed_count);
    num_range src, next;

    for (i = 0; i < seed_map.seed_count / 2; i++) {
        src = (num_range) { seed_map.seeds[i * 2], seed_map.seeds[(i * 2) + 1]};
        vec_insert(ranges, &src);
    }

    Vector *next_ranges = vec_init(sizeof(num_range), 10);

    for (i = 0; i < seed_map.maps->count; i++) {
        Vector *map = *(Vector**)vec_at(seed_map.maps, i);
        vec_clear(next_ranges);
        
        for (int j = 0; j < ranges->count; j++) {
            num_range src = *(num_range*)vec_at(ranges, j);
            ulong src_tail = src.start + src.len - 1;
            next = src;

            for (int k = 0; k < map->count; k++) {
                map_range mr = *(map_range*)vec_at(map, k);
                ulong mr_tail = mr.src_start + mr.len - 1;

                if (mr.src_start > src_tail || mr_tail < src.start)
                    continue;

                ulong overlap_head = mr.src_start > src.start ? mr.src_start : src.start;   // max of heads
                ulong overlap_tail = mr_tail < src_tail ? mr_tail : src_tail;               // min of tails
                ulong overlap_len = overlap_tail - overlap_head + 1;
                
                if (overlap_head > src.start) {
                    next = (num_range) { src.start, overlap_head - src.start };
                    vec_insert(ranges, &next);
                }

                if (overlap_tail > src_tail) {
                    next = (num_range) { overlap_tail + 1, src_tail - overlap_tail};
                    vec_insert(ranges, &next);
                }
                

                next = (num_range) { mr.dest_start + (overlap_head - mr.src_start), overlap_len };
                break;
            }

            vec_insert(next_ranges, &next);
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
