#include "VecBase.h"
#include <stdlib.h>
#include <stdbool.h>

VecBase VecInit(pointer_ptr ptr, const size_t typeSize)
{
	return VecInit_C(ptr, typeSize, 0);
}

VecBase VecInit_C(pointer_ptr ptr, const size_t typeSize, size_t capacity)
{
	if (capacity <= 0)
		capacity = VECTOR_INITIAL_CAPACITY;
	VecBase vec;
	vec.Size = 0;
	vec.Capacity = capacity;
	vec.TypeSize = typeSize;
	vec.Array = ptr;
	*vec.Array = malloc(typeSize * vec.Capacity);
	return vec;
}

void VecResize(VecBase* vector)
{
	VecResizeRange(vector, 1);
}

void VecResizeRange(VecBase* vector, const size_t len)
{
	if (vector->Size + len >= vector->Capacity)
	{
		do {
			vector->Capacity *= 2;
		} while (vector->Size + len >= vector->Capacity);

		void* ptrTemp = NULL;
		while ((ptrTemp = realloc(*vector->Array, vector->TypeSize * vector->Capacity)) == NULL);

		*vector->Array = ptrTemp;
	}
	
}

void VecAppend(VecBase * vector, gen_param_cptr value)
{
	VecAppendRange(vector, value, 1);
}

bool VecContains(VectorConstPtr vecBase, gen_param_cptr value, const compare_func eqFuncPtr,
                 const getter_func getterFunc, gen_vector_cptr vector, uint * foundId)
{
	for (uint i = 0; i < vecBase->Size; ++i)
	{
		if (0 == eqFuncPtr(getterFunc(vector, i), value))
		{
			if (foundId != NULL) *foundId = i;
			return true;
		}
	}
	if (foundId != NULL) *foundId = vecBase->Size;
	return false;
}

void VecAppendRange(VecBase* vector, gen_param_cptr value, const size_t len)
{
	VecRepOrInsRangeAt(vector, vector->Size, value, len);
}

void VecRepOrInsRangeAt(VecBase* vector, const uint index, gen_param_cptr value, const size_t len)
{
	const size_t newSize = len - vector->Size + index;
	VecResizeRange(vector, len - vector->Size + index);

	void* ptr = *vector->Array + vector->TypeSize * index;
	memcpy(ptr, value, vector->TypeSize * len);
	vector->Size += newSize;
}
