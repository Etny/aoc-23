#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "day6.h"
#include "../quick-read.c"
#include "../vec.c"

void solve_race(race *r, long *left, long *right) {
    // find solutions for x * (time - x) > goal for 0 <= x <= time
    // this means find (integer) range of y > 0 for 
    //    y = x * (time - x) - goal
    //    y = -x^2 + (time * x) - goal
    // so find D (b^2 - 4ac) for abc rule
    // a = -1, b = time, c = -goal
    double sqr_D = sqrt(pow(r->time, 2) - (4 * r->goal));

    *left = floor(((-r->time + sqr_D) / -2)) + 1;
    *right = ceil((-r->time - sqr_D) / -2) - 1;
}

int main(int argc, char** argv) {
    read_input("in-test.txt");

    part_one(file);
    rewind_lines(file);
    part_two(file);
}
