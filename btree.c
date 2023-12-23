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

btree btree_init() {
    return (btree) { NULL };
}

btree_node* insert_node(btree_node* root, btree_node* insert) {
    if (!root)
        return insert;

    int cmp = strcmp(root->key, insert->key);

    if (cmp > 0)
        root->left = insert_node(root->left, insert);
    else if (cmp < 0)
        root->right = insert_node(root->right, insert);
    else
        root->data = insert->data;

    return root;
}

void btree_insert(btree* tree, char* key, void* data) {
    btree_node* to_insert = malloc(sizeof(btree_node));
    char* key_cpy = malloc(strlen(key) + 1);
    strcpy(key_cpy, key);
    *to_insert = (btree_node) { key_cpy, data, NULL, NULL };
    
    tree->root = insert_node(tree->root, to_insert);
}

void* btree_get(btree* tree, char* key) {
    int cmp;
    btree_node* cur = tree->root;
    
    while (cur) {
        cmp = strcmp(cur->key, key);
        if (cmp == 0)
            return cur->data;
        else
            cur = cmp > 0 ? cur->left : cur->right;
    }

    return NULL;
}

btree_iter btree_iterate(btree* tree) {
    return (btree_iter) { tree, 0, 0, false };
}

btree_node* next_btree_node(btree_iter* iter) {
    int i = 0;
    btree_node* cur = iter->tree->root;
    while (i < iter->depth && cur) 
        cur = ((iter->lr >> i++) & 1) ? cur->right : cur->left;

    iter->lr++;
    if (iter->lr >= 1 << iter->depth) {
        if (!iter->hit_something && !cur) 
            return NULL;
        
        iter->depth++;
        iter->lr = 0;
        iter->hit_something = false;
    }
    
    if (!cur) 
        return next_btree_node(iter);
    
    iter->hit_something = true;
    return cur;
}
