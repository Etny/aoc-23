
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void* data;
    size_t elem_size;
    int count;
    int capacity;
} Vector;

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

unsigned int vec_resize(Vector* vec) {
    vec->capacity *= 2;
    vec->data = realloc(vec->data, vec->capacity * vec->elem_size);
    return vec->data ? 1 : 0;
}

void vec_insert(Vector *vec, void *val) {
    char *val_bytes = (char*)val, *data_bytes = (char*)vec->data;
    int i;
    if (vec->count == vec->capacity)
        vec_resize(vec);

    int offset = vec->count++ * vec->elem_size;

    // Yes, I know copying byte-by-byte is slow and shit, but I ain't messing around with weird 
    // generics-in-C shit right now
    for(i = 0; i < vec->elem_size; i++) {
        data_bytes[offset + i] = val_bytes[i];
    }
}

void* vec_at(Vector *vec, unsigned int pos) {
    return vec->data + (pos * vec->elem_size);
}

void vec_free(Vector *vec) {
    free(vec->data);
    free(vec);
}


