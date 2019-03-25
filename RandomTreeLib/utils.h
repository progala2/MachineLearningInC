#ifndef UTILS_H_INCLUDE_GUARD
#define UTILS_H_INCLUDE_GUARD
#include <string.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef char**const void_tab_ptr;

void* MemCopyAlloc(const void* source, const size_t size);

void FreeTab(char** tab, const size_t count);

void FreeTabVoid(char** tab, const size_t count, const size_t movingSize);

void FreeN(void_tab_ptr ptr);  // NOLINT

inline size_t MemSizeChars(const char* source)
{
	return sizeof(char) * (strlen(source) + 1);
}

inline char* MemCopyChars(const char* source)
{
	return (char*)MemCopyAlloc(source, MemSizeChars(source));
}

char* MemCopyCharsNoEnter(const char* source);
#endif
