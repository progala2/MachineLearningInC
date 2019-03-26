#include "Vector.h"
#include <stdlib.h>

Vector VecInit(pointer_ptr ptr, const size_t typeSize)
{
	Vector vec; 
	vec.Size = 0;
	vec.Capacity = VECTOR_INITIAL_CAPACITY;
	vec.TypeSize = typeSize;
	vec.Array = ptr;
	*vec.Array = malloc(typeSize * vec.Capacity);
	return vec;
}

void VecResize(Vector* vector)
{
	if (vector->Size >= vector->Capacity) {
		vector->Capacity *= 2;
		*vector->Array = realloc(*vector->Array, vector->TypeSize * vector->Capacity);
	}
}

void VecAppend(Vector * vector, const void* const value)
{
	VecResize(vector);

	void* ptr = *vector->Array + vector->TypeSize * vector->Size;
	memcpy(ptr, value, vector->TypeSize);
	++vector->Size;
}
