#ifndef CharsTable_H_INCLUDE_GUARD
#define CharsTable_H_INCLUDE_GUARD

#define VECTOR_INITIAL_CAPACITY 16
#include "CharRow.h"
#include <stdbool.h>

typedef struct {
    int Size;
    int Capacity;
    CharRow** Data;
} CharsTable;

void TInit(CharsTable *vector);
void TAppend(CharsTable *vector, CharRow* value);
void TResize(CharsTable *vector);
void TFreeMemory(CharsTable *vector, bool removeRows);

#endif