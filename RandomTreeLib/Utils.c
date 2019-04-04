#include "utils.h"
#include <stdlib.h>
#include <string.h>

void* MemCopyAlloc(const void* source, const size_t size)
{
	void* _malloc(size, dest);
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

void* CreateTab(const size_t first, const size_t second, const size_t typeSize)
{
	if (first == 0 || second == 0)
		return NULL;

	void_tab_ptr pointer = malloc(sizeof(void*) * first);
	if (pointer == NULL)
		return NULL;

	for (size_t i = 0; i < first; i++)
	{
		pointer[i] = malloc(typeSize * second);
	}
	return pointer;
}

void FreeTab_Func(void_tab_ptr tab, const size_t count, deleter_func deleter)
{
	for (size_t i = 0; i < count; ++i)
	{
		deleter(tab[i]);
	}
	free(tab);
}