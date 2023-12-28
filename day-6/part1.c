#include "day6.h"
#include <stdio.h>

static Vector *read_races(Lines *input) {
    Vector *races = vec_init(sizeof(race), 4);

    //read times
    next_line(input);
    char *line = malloc(sizeof(char) * strlen(input->line));
    sscanf(input->line, "Time: %[0-9 ]", line);
    
    char *token = strtok(line, " ");
    while (token) {
        *(race*)vec_insert_ptr(races) = (race) {atol(token), 0};
        token = strtok(NULL, " ");
    }

    //read goals
    next_line(input);
    line = malloc(sizeof(char) * strlen(input->line));
    sscanf(input->line, "Distance: %[0-9 ]", line);

    int i = 0;
    token = strtok(line, " ");
    while(token) {
        ((race*)vec_at(races, i++))->goal = atol(token);
        token = strtok(NULL, " ");
    }

    return races;
}

void part_one(Lines *lines) {
    Vector *races = read_races(lines);
    int result = 1;
    long left, right;

    for (int i = 0; i < races->count; i++) {
        solve_race((race*)vec_at(races, i), &left, &right);
        result *= (right - left) + 1;
    }

    printf("Part one: %d\n", result);
}
