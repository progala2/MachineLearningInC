#ifndef CharsTable_H_INCLUDE_GUARD
#define CharsTable_H_INCLUDE_GUARD

#include "CharRow.h"
#include <stdbool.h>
#include <stdio.h>
#include "VecBase.h"

typedef struct {
    CharRow** Table;
    VecBase VecBase;
} CharsTable;

void TInit(CharsTable *vector);
void TAppend(CharsTable *vector, CharRow* value);
void TResize(CharsTable *vector);
CharsTable* TReadFile(FILE* input, const unsigned int bufferLen);
void TFreeMemory(CharsTable** vector, const bool removeRows);

#endif