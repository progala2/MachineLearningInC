#include "CharRow.h"
#include <stdlib.h>
#include <string.h>
#include "CharsTable.h"

void CrInit(CharRow *vector)
{
	vector->Size = 0;
	vector->Data = malloc(sizeof(char));
	vector->Data[0] = '\0';
}

int CrAppend(CharRow *vector, const char* value, const unsigned int valueLen)
{
	if (vector->Data == NULL)
		return -1;
	const unsigned int newSize = vector->Size + valueLen; 
	
	char* reallocPtr = realloc(vector->Data, sizeof(char)* (newSize+1));
	if (reallocPtr == NULL)
		return -2;

	vector->Data =  reallocPtr;
	memcpy(vector->Data + vector->Size, value, valueLen);
	vector->Size = newSize;
	vector->Data[newSize] = '\0';

	return newSize;
}

char* CrCopyData(const CharRow* row)
{
	return MemCopyAlloc(row->Data, sizeof(char)*(row->Size + 1));
}

void CrFreeMemory(CharRow *vector)
{
	free(vector->Data);
	vector->Data = NULL;
	vector->Size = -1;
}