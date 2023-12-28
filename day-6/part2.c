#include "day6.h"

static void remove_spaces(char* in) {
    char *out = in;
    do {
        while (*out == ' ')
            out++;
    } while ((*in++ = *out++));
}

static race read_race(Lines *input) {
    race r;

    //read time
    next_line(input);
    char *line = malloc(sizeof(char) * strlen(input->line));
    sscanf(input->line, "Time: %[0-9 ]", line);
    remove_spaces(line); 
    r.time = atol(line);

    //read goal
    next_line(input);
    line = malloc(sizeof(char) * strlen(input->line));
    sscanf(input->line, "Distance: %[0-9 ]", line);
    remove_spaces(line);
    r.goal = atol(line);

    return r;
}

void part_two(Lines *lines) {
    race r = read_race(lines);
    long left, right;

    solve_race(&r, &left, &right);

    printf("Part two: %ld\n", (right - left) + 1);
}
