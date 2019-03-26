#ifndef VECTOR_H_INCLUDE_GUARD
#define VECTOR_H_INCLUDE_GUARD

#define VECTOR_INITIAL_CAPACITY 16
#include "utils.h"
typedef char** pointer_ptr;

typedef struct {
    size_t Size;
    size_t Capacity;
    size_t TypeSize;
    pointer_ptr Array;
} Vector;

Vector VecInit(pointer_ptr ptr, const size_t typeSize);
void VecAppend(Vector* vector, const void* const value);
void VecResize(Vector *vector);
//void VecFreeMemory(Vector *vector, bool removeRows);

#endif