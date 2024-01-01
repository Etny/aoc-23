#ifndef BTREE
#define BTREE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _btree_node {
    char* key;
    void* data;
    struct _btree_node *left;
    struct _btree_node *right;
} btree_node;

typedef struct {
    btree_node *root;
} btree;

typedef struct {
    btree *tree;
    int depth;
    int lr;
    bool hit_something;
} btree_iter;

btree* btree_init();
void btree_insert(btree* tree, char* key, void* data);
void* btree_get(btree* tree, char* key);
btree_iter btree_iterate(btree* tree);
btree_node* next_btree_node(btree_iter* iter);
void btree_free(btree *tree);

#endif
