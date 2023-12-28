#include <stdio.h>
#include "../quick-read.c"
#include <string.h>
#include "../vec.c"

typedef struct {
    
} scratchcard;

int read_nums(char* str, int* buf) {
    int i = 0;
    char* token = strtok(str, " ");

    while (token) {
        buf[i++] = atoi(token);
        token = strtok(NULL, " ");
    }
    
    return i;
}

Vector* read_cards(Lines* lines) {
    Vector *cards_vec = vec_init(sizeof(int), 100);

    char winning[100], has[100];
    int winning_num[10], winning_num_count, has_num[10], has_num_count;
    int amount_winning;
    while (next_line(lines)) {
        amount_winning = 0;
        sscanf(lines->line, "Card %*d: %[0-9 ] | %[0-9 ]", winning, has);

        winning_num_count = read_nums(winning, winning_num);
        has_num_count = read_nums(has, has_num);

        for (int x = 0; x < winning_num_count; x++) {
            for (int y = 0; y < has_num_count; y++) {
                if (winning_num[x] == has_num[y]) {
                    amount_winning++; 
                    break;
                }
            }
        }
        
        vec_insert(cards_vec, &amount_winning);
    }


    return cards_vec;
}


void part_one(Vector *cards) {
    int total = 0; 
    for (int i = 0; i < cards->count; i++) {
        if (*(int*)vec_at(cards, i))
            total += 1 << (*(int*)vec_at(cards, i) - 1);
    }

    printf("Part one: %d\n", total);
}

void part_two(Vector *cards) {
    int *amount = malloc(sizeof(int) * cards->count);
    int i, winning_at, offset, total = 0;

    for (i = 0; i < cards->count; i++)
        amount[i] = 1;

    for (i = 0; i < cards->count; i++) {
        total += amount[i];
        winning_at = *(int*)vec_at(cards, i);

        for (offset = 1; offset <= winning_at; offset++) 
            amount[i + offset] += amount[i];
    }

    printf("Part two: %d\n", total);
}

int main(int argc, char** argv) {
    char* file_name = argc > 1 ? argv[1] : "in-test.txt";
    Lines* file = read_lines(file_name);

    if (!file) {
        printf("Can't open file");
        return 1;
    }

    Vector *cards = read_cards(file);

    part_one(cards);
    part_two(cards);
}
