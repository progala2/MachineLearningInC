#include "CharRow.h"
#include <stdlib.h>
#include "CharsTable.h"

void CrInit(CharRow *vector)
{
	vector->VecBase = VecInit(&vector->Data, sizeof(char));
	VecAppend(&vector->VecBase, "");
}

void CrAppend(CharRow* vector, const char* value, const uint valueLen)
{
	VecRepOrInsRangeAt(&vector->VecBase, vector->VecBase.Size - 1, value, valueLen);
	VecAppend(&vector->VecBase, "");
}

char* CrCopyData(const CharRow* row)
{
	return MemCopyAlloc(row->Data, sizeof(char)*(row->VecBase.Size));
}

void CrFreeMemory(CharRow *vector)
{
	free(vector->Data);
	vector->Data = NULL;
}