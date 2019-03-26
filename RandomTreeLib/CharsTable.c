#include "CharsTable.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void TInit(CharsTable *vector)
{
	vector->Size = 0;
	vector->Capacity = VECTOR_INITIAL_CAPACITY;
	vector->Table = malloc(sizeof(CharRow*) * vector->Capacity);
}

void TAppend(CharsTable *vector, CharRow* value)
{
	TResize(vector);

	vector->Table[vector->Size++] = value;
}

void TResize(CharsTable *vector)
{
	if (vector->Size >= vector->Capacity) {
		vector->Capacity *= 2;
		vector->Table = realloc(vector->Table, sizeof(CharRow*) * vector->Capacity);
	}
}

CharsTable* TReadFile(FILE* input, const unsigned int bufferLen)
{
	char* buffer = malloc(sizeof(char)*bufferLen);
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
		const size_t lineLen = strcspn(buffer, "\n");
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

void TFreeMemory(CharsTable *vector, const bool removeRows)
{
	if (removeRows)
	{
		for (uint i = 0; i < vector->Size; ++i)
		{
			CrFreeMemory(vector->Table[i]);
			free(vector->Table[i]);
		}
	}
	free(vector->Table);
	vector->Table = NULL;
	vector->Size = -1;
	vector->Capacity = -1;
}