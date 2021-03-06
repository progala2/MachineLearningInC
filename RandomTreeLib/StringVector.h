#ifndef CharsVector_H_INCLUDE_GUARD
#define CharsVector_H_INCLUDE_GUARD

#include "VecBase.h"

typedef struct {
    char** Table;
	VecBase VecBase;
} StringVector;

StringVector* SvInit();

inline void SvAppend(StringVector* vector, char* value)
{
	VecAppend(&vector->VecBase, &value);
}

bool SvContains(const StringVector* const vector, const char* const value, uint* foundId);
void SvFree(StringVector** vector);

#endif
