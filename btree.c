#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"

btree* btree_init() {
    btree* b = malloc(sizeof(btree));
    *b = (btree) { NULL };
    return b;
}

static btree_node* insert_node(btree_node* root, btree_node* insert) {
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
    char* key_cpy = strdup(key);
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

static void btree_node_free(btree_node *node) {
    if (node->left)
        btree_node_free(node->left);
    if (node->right)
        btree_node_free(node->right);

    free(node->data);
    free(node->key);
    free(node);
}

void btree_free(btree *tree) {
    btree_node_free(tree->root);
    free(tree);
}

btree_iter btree_iterate(btree* tree) {
    return (btree_iter) { tree, 0, 0, false };
}

btree_node* next_btree_node(btree_iter* iter) {
    int i = 0;
    btree_node* cur = iter->tree->root;
recur: 
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
    
    if (!cur) {
        // return next_btree_node(iter);
        i = 0;
        cur = iter->tree->root;
        goto recur; // Apparently tail recurssion isn't properly pickup up at lower -O levels, doing this instead
    }
    
    iter->hit_something = true;
    return cur;
}
