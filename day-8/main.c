#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "../quick-read.c"
#include "../vec.c"
#include "../btree.c"

#define LEFT true
#define RIGHT false

typedef bool LR;

typedef struct {
    bool is_zzz;
    bool is_start;
    bool is_final;
    size_t left;
    size_t right;
} node;

typedef struct {
    char* name;
    char* left;
    char* right;
} node_prototype;

Vector *get_LR(Lines *lines) {
    Vector *lr = vec_init(sizeof(LR), 10);
    
    next_line(lines);
    char *line = lines->line;
    
    do {
        *(LR*)vec_insert_ptr(lr) = (*line == 'L') ? LEFT : RIGHT;
    } while(*(++line) != '\n');

    next_line(lines); // skip empty line
        
    return lr;
}

// I could use a BTree to store nodes, which would remove the need for 'flattening'
// from strings to indices. But assuming the 'walk' is the part that
// requires the most number crunching, this is worth it
Vector *get_nodes(Lines *lines, size_t *start) {
    Vector *nodes = vec_init(sizeof(node), 10);
    Vector *prototypes = vec_init(sizeof(node_prototype), 10);
    btree *indices = btree_init();
    char name[4], left[4], right[4];
    size_t index = 0;

    while(next_line(lines)) {
        sscanf(lines->line, "%[A-Z0-9] = (%[A-Z0-9], %[A-Z0-9])", name, left, right);
        vec_insert_ptr(nodes); // reserve a spot (to be initialized later)
        node_prototype *ptyp = malloc(sizeof(node_prototype));
        ptyp->name = strdup(name);
        ptyp->left = strdup(left);
        ptyp->right= strdup(right);
        vec_insert(prototypes, ptyp);
        
        size_t *index_ptr = malloc(sizeof(size_t));
        *index_ptr = index++;
        btree_insert(indices, name, index_ptr);
    }

    for (int i = 0; i < prototypes->count; i++) {
        node_prototype *ptyp = vec_at(prototypes, i);
        node *n = vec_at(nodes, i);
        size_t *left_ptyp = btree_get(indices, ptyp->left);
        size_t *right_ptyp = btree_get(indices, ptyp->right);

        if (strcmp("AAA", ptyp->name) == 0)
            *start = i;
        n->is_zzz = strcmp("ZZZ", ptyp->name) == 0;
        n->is_start = ptyp->name[2] == 'A';
        n->is_final = ptyp->name[2] == 'Z';
        n->left = *left_ptyp;
        n->right = *right_ptyp;
    }

    vec_free(prototypes);
    btree_free(indices);
    return nodes;
}

unsigned long get_to_final(size_t cur, Vector *nodes, Vector *lr, bool only_zzz) {
    node *n = nodes->data;
    LR *lr_a = lr->data;

    size_t lr_index = 0;
    LR cur_lr;
    unsigned long steps = 0;

    while (only_zzz ? !n[cur].is_zzz : !n[cur].is_final) {
        cur_lr = lr_a[lr_index];
        lr_index = (lr_index + 1) % lr->count;

        cur = cur_lr == LEFT ? n[cur].left : n[cur].right;
        steps++;
    }


    return steps;
}

void part_one(Vector *nodes, size_t start, Vector *lr) {
    printf("Part one: %ld\n", get_to_final(start, nodes, lr, true));
}

unsigned long LCM(unsigned long a, unsigned long b) {
    unsigned long gcd = a < b ? a / 2 : b / 2;
    while (gcd--) {
        if (a % gcd == 0 && b % gcd == 0)
            break;
    }

    return (a * b) / gcd;
}

void part_two(Vector *nodes, Vector *lr) {
    Vector *steps = vec_init(sizeof(unsigned long), 6);

    for (int i = 0; i < nodes->count; i++) {
        node *n = vec_at(nodes, i);
        if (!n->is_start)
            continue;
        
        // I assumed we would also have to worry about the amount of steps
        // it would take for the Z node reached to cycle back to itself,
        // so that each start node would have a cycle that goes: 
        // (initial steps to Z) -> (time to cycle) -> (time to cycle)
        // ad infinitum. However it seems the input data is crafted
        // so that (initial steps to Z) == (time to cycle), so we
        // can just use the first value
        unsigned long to_final = get_to_final(i, nodes, lr, false);
        vec_insert(steps, &to_final);
    }

    // Poor man's reduce
    unsigned long *steps_ar = steps->data;
    unsigned long result = steps_ar[0];

    for (int i = 1; i < steps->count; i++) 
        result = LCM(result, steps_ar[i]);
    

    printf("Part two: %ld\n", result);
}

int main(int argc, char** argv) {
    read_input("in-test.txt");

    Vector *lr = get_LR(file);
    size_t AAA = 0;
    Vector *nodes = get_nodes(file, &AAA);
    part_one(nodes, AAA, lr);
    part_two(nodes, lr);
}
