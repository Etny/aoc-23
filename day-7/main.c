#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../vec.c"
#include "../quick-read.c"

#define HAND_SIZE 5

typedef enum {
    HIGH_CARD = 0,
    ONE_PAIR = 1,
    TWO_PAIR = 2,
    THREE_OF_A_KIND = 3,
    FULL_HOUSE = 4,
    FOUR_OF_A_KIND = 5,
    FIVE_OF_A_KIND = 6
}  hand_type;

typedef struct {
    int bet;
    hand_type type;
    int card_values[HAND_SIZE];
} hand;

int char_to_val(char c, bool count_J) {
    if (isdigit(c)) 
        return c - '2' + (count_J ? 1 : 0); 
    
    switch (c) {
        case 'A': return 12;
        case 'K': return 11;
        case 'Q': return 10;
        case 'J': return count_J ? 0 : 9;
        default: return count_J ? 9 : 8; // For 'T'
    }
}

hand_type values_to_hand_type(int card_values[HAND_SIZE], bool count_J) {
    int count[13] = { 0 };

    for (int i = 0; i < HAND_SIZE; i++)
        count[card_values[i]]++;

    int max = 0, second_max = 0;
    int i = count_J ? 1 : 0; //We do not care about J in part 2

    for (; i < 13; i++) {
        if (count[i] >= max) {
            second_max = max;
            max = count[i];
        } else if(count[i] > second_max) {
            second_max = count[i];
        }
    }

    int wildcards = count_J ? count[0] : 0;

    switch (max + wildcards) {
        case 1: return HIGH_CARD;
        case 2: return second_max == 2 ? TWO_PAIR : ONE_PAIR;
        case 3: return second_max == 2 ? FULL_HOUSE : THREE_OF_A_KIND;
        case 4: return FOUR_OF_A_KIND;
        default: return FIVE_OF_A_KIND;
    }
}




Vector *get_hands(Lines *lines, bool count_J) {
    Vector *hands = vec_init(sizeof(hand), 10);
    char *cards_buf = malloc(HAND_SIZE + 1);

    while (next_line(lines)) {
        hand hand;
        sscanf(lines->line, "%s %d", cards_buf, &hand.bet);

        for (int i = 0; i < HAND_SIZE; i++) 
            hand.card_values[i] = char_to_val(*(cards_buf + i), count_J);
        
        hand.type = values_to_hand_type(hand.card_values, count_J);

        vec_insert(hands, &hand);
    }

    return hands;
}

int cmp_hands(const void* hl, const void* hr) {
    hand *left = (hand*)hl;
    hand *right = (hand*)hr;

    int ret = left->type - right->type;
    int i = 0;
    
    while (ret == 0 && i < HAND_SIZE) {
        ret = left->card_values[i] - right->card_values[i];
        i++;
    }

    return ret;
}

void print_scores(Lines *lines, bool part_two) {
    Vector *hands = get_hands(lines, part_two);
    vec_sort(hands, cmp_hands);
    
    long result = 0;
    for (int i = 0; i < hands->count; i++) {
        hand *h = vec_at(hands, i);
        result += (i + 1) * h->bet;
    }

    printf("Part %s: %ld\n", part_two ? "two" : "one", result);
    vec_free(hands);
}

int main(int argc, char** argv) {
    read_input("in-test.txt");

    print_scores(file, false);
    rewind_lines(file);
    print_scores(file, true);
}
