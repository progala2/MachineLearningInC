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
	VecResizeRange(vector, 1);
}

void VecResizeRange(Vector* vector, const size_t len)
{
	if (vector->Size + len >= vector->Capacity)
	{
		do {
			vector->Capacity *= 2;
		} while (vector->Size + len >= vector->Capacity);

		*vector->Array = realloc(*vector->Array, vector->TypeSize * vector->Capacity);
	}
	
}

void VecAppend(Vector * vector, const void* const value)
{
	VecAppendRange(vector, value, 1);
}

void VecAppendRange(Vector* vector, const void* const value, const size_t len)
{
	VecRepOrInsRangeAt(vector, vector->Size, value, len);
}

void VecRepOrInsRangeAt(Vector* vector, const uint index, const void* const value, const size_t len)
{
	const size_t newSize = len - vector->Size + index;
	VecResizeRange(vector, len - vector->Size + index);

	void* ptr = *vector->Array + vector->TypeSize * index;
	memcpy(ptr, value, vector->TypeSize * len);
	vector->Size += newSize;
}