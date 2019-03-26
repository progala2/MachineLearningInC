#include "CharsVector.h"
#include <stdlib.h>

StringVector* SvInit()
{
	StringVector* vec = malloc(sizeof(StringVector));
	vec->VecBase = VecInit((pointer_ptr)&vec->Table, sizeof(char*));
	return vec;
}
