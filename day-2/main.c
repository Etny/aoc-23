#include "../quick-read.h"
#include <stdio.h>
#include <string.h>

ssize_t color_to_index(char *color) {
    if (strcmp(color, "red") == 0)
        return 0;
    else if(strcmp(color, "green") == 0)
        return 1;
    else
        return 2;
}


int get_game_max(char* line, int* array) {
    int game_num, games_offset;
    sscanf(line, "Game %d: %n", &game_num, &games_offset);
    char* games = line + games_offset;
    
    // No clue why I have to do this. I guess it just can't be NULL,
    // but it must be reallocated because this is unusable. Weird.
    char **set_r = (char**)malloc(0);
    char *values, *color = (char*)malloc(6);
    char* set = strtok_r(games, ";\n", set_r);

    int num, index;
    while (set) {
        values = strtok(set, ",");
        while (values) {
            sscanf(values, "%d %s", &num, color);

            index = color_to_index(color);
            if (array[index] < num)
                array[index] = num;

            values = strtok(NULL, ",");
        }
        set = strtok_r(NULL, ";", set_r);
    }
    
    return game_num;
}

int main(int argc, char** argv) {
    char* file_name = argc < 2 ? "in-test.txt" : argv[1]; 
    Lines* lines;

    if(!(lines = read_lines(file_name)))
        return -1;

    int total_p1 = 0, total_p2 = 0, game;
    int limits[] = {12, 13, 14};

    while(next_line(lines))  {
        int i, temp[3] = {0};
        game = get_game_max(lines->line, temp);

        for(i = 0; i < 3; i++) 
            if (temp[i] > limits[i])
                game = 0;
    
        total_p1 += game;
        total_p2 += temp[0] * temp[1] * temp[2];
    }
    
    printf("Part 1: %d\n", total_p1);
    printf("Part 2: %d\n", total_p2);
}
