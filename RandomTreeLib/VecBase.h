#ifndef VECTOR_H_INCLUDE_GUARD
#define VECTOR_H_INCLUDE_GUARD

#define VECTOR_INITIAL_CAPACITY 8
#include "utils.h"
#include <stdbool.h>
typedef char** pointer_ptr;
typedef int (*compare_func)(const void* a, const void* b);
typedef const void* gen_value_cptr;
typedef const void*const gen_param_cptr;
typedef const void*const gen_vector_cptr;
typedef gen_value_cptr (*getter_func)(const void* vectorPtr, uint index);

typedef struct VecBase{
    size_t Size;
    size_t Capacity;
    size_t TypeSize;
    pointer_ptr Array;
} VecBase;

typedef const VecBase* const VectorConstPtr;

VecBase VecInit(pointer_ptr ptr, const size_t typeSize);
VecBase VecInit_C(pointer_ptr ptr, const size_t typeSize, size_t capacity);
void VecAppend(VecBase* vector, gen_param_cptr value);
bool VecContains(VectorConstPtr vecBase, gen_param_cptr value, const compare_func eqFuncPtr,
                 const getter_func getterFunc, gen_vector_cptr vector, uint* foundId);
void VecAppendRange(VecBase* vector, gen_param_cptr value, const size_t len);
void VecRepOrInsRangeAt(VecBase* vector, const uint index, gen_param_cptr value, const size_t len);
void VecResize(VecBase *vector);
void VecResizeRange(VecBase *vector, const size_t len);

#endif