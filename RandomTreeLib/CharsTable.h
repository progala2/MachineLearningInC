#ifndef CharsTable_H_INCLUDE_GUARD
#define CharsTable_H_INCLUDE_GUARD

#define VECTOR_INITIAL_CAPACITY 16
#include "CharRow.h"
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"

typedef struct {
    uint Size;
    uint Capacity;
    CharRow** Table;
} CharsTable;

void TInit(CharsTable *vector);
void TAppend(CharsTable *vector, CharRow* value);
void TResize(CharsTable *vector);
CharsTable* TReadFile(FILE* input, const unsigned int bufferLen);
void TFreeMemory(CharsTable *vector, bool removeRows);

#endif