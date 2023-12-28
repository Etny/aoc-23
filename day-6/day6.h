#ifndef DAY6
#define DAY6

#include "../vec.h"
#include "../quick-read.h"

typedef struct {
    long time;
    long goal;
} race;

void part_one(Lines *lines);
void part_two(Lines *lines);
void solve_race(race *r, long *left, long *right);

#endif
