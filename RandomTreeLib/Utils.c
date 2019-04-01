#include "utils.h"
#include <stdlib.h>
#include <string.h>

void* MemCopyAlloc(const void* source, const size_t size)
{
	void* dest = malloc(size);
	memcpy(dest, source, size);
	return dest;
}

void FreeTabVoid(void_tab_ptr tab, const size_t count, const size_t movingSize)
{
	for (size_t i = 0; i < count; ++i)
	{
		free(tab[0] + (movingSize * i));
	}
	free(tab);
}

void FreeN(void_tab_ptr ptr)  // NOLINT
{
	if (*ptr == NULL)
		return;
	free(*ptr);
	(*ptr) = NULL;
}

char* MemCopyCharsNoEnter(const char* source)
{
	const size_t memLen = MemSizeChars(source);
	const size_t len = memLen/sizeof(char);
	char* ptr = MemCopyAlloc(source, memLen);
	if (ptr[len - 2] =='\n')
		ptr[len - 2] = '\0';
	return ptr;
}

void FreeTab(char** tab, const size_t count)
{
	FreeTabVoid(tab, count, sizeof(char));
}
