#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../quick-read.h"


char* nums_for[9] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}; 
char* nums_rev[9] = {"eno", "owt", "eerht", "ruof", "evif", "xis", "neves", "thgie", "enin" };

void part_one(Lines* lines);
void part_two(Lines* lines);
int scan_for_digit(char* line, int increment, bool allow_word);

int main() {
    Lines* lines = read_lines("in.txt");

    if (!lines) {
        printf("Failed to open file");
        return -1;
    }

    part_one(lines);
    part_two(rewind_lines(lines));

    close_lines(lines);
}

void find_code(Lines* lines, bool allow_words) {
    int first, last, total = 0;

    while (next_line(lines)) {
        first = scan_for_digit(lines->line, 1, allow_words);
        last = scan_for_digit(lines->line + strlen(lines->line) - 1, -1, allow_words);
        total += (first * 10) + last;
    }

    printf("%d\n", total);
}

void part_one(Lines* lines) {
    printf("Part 1: ");
    find_code(lines, false);
}

void part_two(Lines* lines){
    printf("Part 2: ");
    find_code(lines, true);
}

int scan_for_digit(char* line, int increment, bool allow_word) {
    char** nums = increment > 0 ? nums_for : nums_rev;
    char progress[9] = {0};
    int n_i;

    while (*line) {
        if (isdigit(*line)) 
            return *line - '0';

        if (!allow_word)
            goto end;

        for (n_i = 0; n_i < 9; n_i++) {
            if (*line != nums[n_i][progress[n_i]++])
                progress[n_i] = *line == *nums[n_i] ? 1 : 0;
            else if(nums[n_i][progress[n_i]] == '\0')
                    return n_i + 1;
        }

        end: line += increment;
    } 

    return 0;
}

