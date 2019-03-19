#include "CharsTable.h"
#include <stdlib.h>
#include <stdbool.h>


void TInit(CharsTable *vector)
{
	vector->Size = 0;
	vector->Capacity = VECTOR_INITIAL_CAPACITY;
	vector->Data = malloc(sizeof(CharRow*) * vector->Capacity);
}

void TAppend(CharsTable *vector, CharRow* value)
{
	TResize(vector);

	vector->Data[vector->Size++] = value;
}

void TResize(CharsTable *vector)
{
	if (vector->Size >= vector->Capacity) {
		vector->Capacity *= 2;
		vector->Data = realloc(vector->Data, sizeof(CharRow*) * vector->Capacity);
	}
}

void TFreeMemory(CharsTable *vector, bool removeRows)
{
	if (removeRows)
	{
		for (int i = 0; i < vector->Size; ++i)
		{
			CrFreeMemory(vector->Data[i]);
			free(vector->Data[i]);
		}

	}
	free(vector->Data);
	vector->Data = NULL;
	vector->Size = -1;
	vector->Capacity = -1;
}