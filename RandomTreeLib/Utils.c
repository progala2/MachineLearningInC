#include "utils.h"
#include <stdlib.h>
#include <string.h>

void* MemCopyAlloc(const void* source, const size_t size)
{
	void* dest = malloc(size);
	memcpy(dest, source, size);
	return dest;
}

void FreeN(void_ptr_ref ptr)  // NOLINT
{
	if (*ptr == NULL)
		return;
	free(*ptr);
	*ptr = NULL;
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

void FreeTab(void_tab_ptr tab, const size_t count)
{
	FreeTab_Func(tab, count, free);
}

void FreeTab_Func(void_tab_ptr tab, const size_t count, deleter_func deleter)
{
	for (size_t i = 0; i < count; ++i)
	{
		deleter(tab[i]);
	}
	free(tab);
}