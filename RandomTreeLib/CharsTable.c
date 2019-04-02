#include "CharsTable.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "VecBase.h"

void TInit(CharsTable *vector)
{
	vector->VecBase = VecInit((pointer_ptr)&vector->Table, sizeof(CharRow*));
}

void TAppend(CharsTable *vector, CharRow* value)
{
	VecAppend(&vector->VecBase, &value);
}

void TResize(CharsTable *vector)
{
	VecResize(&vector->VecBase);
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

void TFreeMemory(CharsTable** vector, const bool removeRows)
{
	if (*vector == NULL)
		return;

	if (removeRows)
	{
		for (uint i = 0; i < (*vector)->VecBase.Size; ++i)
		{
			CrFreeMemory((*vector)->Table[i]);
			free((*vector)->Table[i]);
		}
	}
	free((*vector)->Table);
	free(*vector);
	*vector = NULL;
}