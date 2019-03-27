#ifndef CharsVector_H_INCLUDE_GUARD
#define CharsVector_H_INCLUDE_GUARD

#include "Vector.h"

typedef struct {
    char** Table;
	Vector VecBase;
} StringVector;

StringVector* SvInit();

inline void SvAppend(StringVector* vector, char* value)
{
	VecAppend(&vector->VecBase, &value);
}

#endif