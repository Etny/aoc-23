#ifndef VEC_HEADER
#define VEC_HEADER

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void* data;
    size_t elem_size;
    size_t count;
    size_t capacity;
} Vector;

Vector *vec_init(size_t elem_size, unsigned int cap);
void vec_clear(Vector *vec);
void vec_sort(Vector *vec, int(*comb)(const void*, const void*));
void *vec_insert_ptr(Vector *vec);
void vec_insert(Vector *vec, void *val);
void* vec_at(Vector *vec, size_t pos);
bool vec_conains_val(Vector *vec, void* data);
void vec_free(Vector* vec);




#endif
