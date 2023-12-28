# ifndef VEC_C
# define VEC_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vec.h"

// typedef struct {
//     void* data;
//     size_t elem_size;
//     size_t count;
//     size_t capacity;
// } Vector;

Vector* vec_init(size_t elem_size, unsigned int cap) {
    Vector *v = malloc(sizeof(Vector));
    if (v == NULL)
        return NULL;
    
    v->elem_size = elem_size;
    v->count = 0;
    v->capacity = cap;
    v->data = malloc(v->capacity * v->elem_size);

    if(v->data == NULL) {
        free(v);
        return NULL;
    }

    return v;
}

unsigned int vec_resize(Vector *vec) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->capacity * vec->elem_size);
    return vec->data ? 1 : 0;
}

void vec_clear(Vector *vec) {
    vec->count = 0;
}

void vec_sort(Vector *vec, int(*comb)(const void*, const void*)) {
    qsort(vec->data, vec->count, vec->elem_size, comb);
}

void* vec_insert_ptr(Vector *vec) {
    if (vec->count == vec->capacity)
        vec_resize(vec);

    return vec->data + (vec->count++ * vec->elem_size);
}

void vec_insert(Vector *vec, void *val) {
    void *next = vec_insert_ptr(vec);

    // Yes, I know copying byte-by-byte is slow and shit, but I ain't messing around with weird 
    // generics-in-C shit right now
    memcpy(next, val, vec->elem_size);
}

void* vec_at(Vector *vec, size_t pos) {
    if (pos >= vec->count)
        return NULL;
    return vec->data + (pos * vec->elem_size);
}

bool vec_contains_val(Vector *vec, void* data) {
    for (int i = 0; i < vec->count; i++)
        if (memcmp(vec_at(vec, i), data, vec->elem_size) == 0)
            return true;
    
    return false;
}

void vec_free(Vector *vec) {
    free(vec->data);
    free(vec);
}

#endif
