#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../quick-read.h"

void part_one(Lines* lines);
void part_two(Lines* lines);

int main() {
    Lines* lines = read_lines("in.txt");

    // part_one(lines);
    part_two(lines);

    close_lines(lines);
}

void part_two(Lines* lines) {
    char* nums[10] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}; 
    size_t num_index = 0;
    int match_map = 511;
    
    int i, n_i, first_digit, last_digit, total = 0;
    char cur, num[3];
    while (next_line(lines)) {
        first_digit = 0;
        for (i = 0; (cur = lines->line[i]); i++) {
            if (isdigit(cur)) {
                if(!first_digit) 
                    first_digit = cur - 48;
                else
                    last_digit = cur - 48;
            }

            for (n_i = 0; nums[n_i]; n_i++) {
                if (!(match_map & (1 << n_i)))
                    continue;

                if (cur != nums[n_i][num_index]) {
                    match_map ^= 1 << n_i;
                } else if(nums[n_i][num_index + 1] == '\0') {
                    if(!first_digit) 
                        first_digit = n_i + 1;
                    else
                        last_digit = n_i + 1;
                    
                    // The last character of a digit might actually be
                    // the first of the next (eg 'eightwo')
                    i--;
                }
            }
            
            if (match_map == 0) {
                match_map = 511;

                // If we fail to match any digit, this character might still
                // be the first of the next actual match, so retry it
                if(num_index > 0)
                    i--;

                num_index = 0;
            } else
                num_index++;
        }
        // printf("%d\n", (first_digit * 10) + last_digit);

        total += (first_digit * 10) + last_digit;
    }

    printf("Part 2: %d\n", total);
}

void part_one(Lines* lines) {
    char first_digit = 0, last_digit;

    char cur;
    size_t i = 0;
    char num[3];

    int total = 0;

    while (next_line(lines)) {
        for (i = 0; (cur = lines->line[i]); i++) {
            if (isdigit(cur)) {
                last_digit = cur;
                if (!first_digit) first_digit = cur;
            }
        } 

        sprintf(num, "%c%c", first_digit, last_digit);
        total += atoi(num);

        first_digit = 0;
    }

    printf("Part 1: %d\n", total);
}

