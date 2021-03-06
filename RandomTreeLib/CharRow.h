#ifndef CharRow_H_INCLUDE_GUARD
#define CharRow_H_INCLUDE_GUARD
#include "VecBase.h"

typedef struct {
    char* Data;
	VecBase VecBase;
} CharRow;

void CrInit(CharRow *vector);
void CrAppend(CharRow* vector, const char* value, const uint valueLen);
void CrFreeMemory(CharRow *vector);
char* CrCopyData(const CharRow* row);

#endif