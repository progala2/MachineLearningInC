#include "StringVector.h"
#include <stdlib.h>

StringVector* SvInit()
{
	StringVector* _malloc(sizeof(StringVector), vec);
	vec->VecBase = VecInit((pointer_ptr)&vec->Table, sizeof(char*));
	return vec;
}

static char* SvGet(const StringVector* const vector, const uint index)
{
	return vector->Table[index];
}

bool SvContains(const StringVector* const vector, const char* const value, uint* foundId)
{
	return VecContains(&vector->VecBase, value, (compare_func)strcmp, SvGet, vector, foundId);
}

void SvFree(StringVector** vector)
{
	for (size_t i = 0; i < (*vector)->VecBase.Size; i++)
	{
		free((*vector)->Table[i]);
	}
	free((*vector)->Table);
	free(*vector);
	*vector = NULL;
}
