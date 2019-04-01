#ifndef UTILS_H_INCLUDE_GUARD
#define UTILS_H_INCLUDE_GUARD
#include <string.h>
#include <stdarg.h>

typedef unsigned int uint;
typedef char**const void_tab_ptr;
typedef char**const void_ptr_ref;
typedef void(* deleter_func)(void*);

void* MemCopyAlloc(const void* source, const size_t size);

void FreeTab(void_tab_ptr tab, const size_t count);

void FreeTab_Func(void_tab_ptr tab, const size_t count, deleter_func deleter);

void FreeN(void_ptr_ref ptr);  // NOLINT

inline size_t MemSizeChars(const char* source)
{
	return sizeof(char) * (strlen(source) + 1);
}

inline char* MemCopyChars(const char* source)
{
	return (char*)MemCopyAlloc(source, MemSizeChars(source));
}

char* MemCopyCharsNoEnter(const char* source);

#ifdef _DEBUG
#define DbgPrint(x) printf x
#else
#define DbgPrint(x) /*nothing*/
#endif
#endif
