#ifndef VECTOR_H_INCLUDE_GUARD
#define VECTOR_H_INCLUDE_GUARD

#define VECTOR_INITIAL_CAPACITY 16
#include "utils.h"
#include <stdbool.h>
typedef char** pointer_ptr;
typedef int (*compare_func)(const void* a, const void* b);
typedef struct {
    size_t Size;
    size_t Capacity;
    size_t TypeSize;
    pointer_ptr Array;
} Vector;

Vector VecInit(pointer_ptr ptr, const size_t typeSize);
void VecAppend(Vector* vector, const void* const value);
bool VecContains(const Vector*const vecBase, const void* const value, compare_func, void*(*getter)(const void*const, const uint), const
                 void* const vector, uint* foundId);
void VecAppendRange(Vector* vector, const void* const value, const size_t len);
void VecRepOrInsRangeAt(Vector* vector, const uint index, const void* const value, const size_t len);
void VecResize(Vector *vector);
void VecResizeRange(Vector *vector, const size_t len);

#endif