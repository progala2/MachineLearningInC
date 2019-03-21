#include "utils.h"
#include <stdlib.h>
#include <string.h>

void* MemCopyAlloc(const void* source, const size_t size)
{
	void* dest = malloc(size);
	memcpy(dest, source, size);
	return dest;
}

void FreeTabVoid(char** tab, const size_t count, const size_t movingSize)
{
	for (size_t i = 0; i < count; ++i)
	{
		free(tab[0] + (movingSize * i));
	}
	free(tab);
}

void FreeTab(char** tab, const size_t count)
{
	FreeTabVoid(tab, count, sizeof(char));
}
