#include "CharsTable.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "utils.h"
#include <string.h>

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

CharsTable* TReadFile(FILE* input, const unsigned int bufferLen)
{
	char* buffer = malloc(sizeof(char)*bufferLen);
	size_t lineLen = 0;
	CharRow* row = NULL;
	CharsTable* table = malloc(sizeof(CharsTable));
	TInit(table);
	while (fgets(buffer, bufferLen, input) != NULL)
	{
		if (row == NULL)
		{
			row = malloc(sizeof(CharRow));
			CrInit(row);
			TAppend(table, row);
		}
		lineLen = strcspn(buffer, "\n");
		if (buffer[lineLen] == '\n')
		{
			buffer[lineLen] = 0;

			CrAppend(row, buffer, lineLen);
			row = NULL;
		}
		else
		{
			CrAppend(row, buffer, lineLen);
		}

	}
	free(buffer);
	return table;
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