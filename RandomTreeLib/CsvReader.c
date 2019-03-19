#include "CsvReader.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>

void CsvInit(CsvTable* input)
{
	input->ClassName = NULL;
	input->ClassColumn = NULL;
	input->Headers = NULL;
	input->ParametersLen = 0;
	input->Parameters = NULL;
	input->RowsNumber = 0;
}

CsvTable* CsvReadTable(const CharsTable* input)
{
	if (input->Size < 2)
		return (CsvTable*)-1;
	
	CsvTable* table = malloc(sizeof(CsvTable));
	CsvInit(table);


	for (int i = 1; i < input->Size; ++i)
	{
		
	}
	return NULL;
}

char** ParseFirstRow(const CharRow* row)
{
	const char s[2] = ","; 
    char* tok;
  
    // Use of strtok 
    // get first token 
    tok = strtok_s(row->Data, s, ); 
  
    // Checks for delimeter 
    while (tok != 0) { 
        printf("%s, ", tok); 
  
        // Use of strtok 
        // go through other tokens 
        tok = strtok(0, s); 
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

void CsvFreeMemory(CsvTable* table)
{
	for (int i = 0; i < table->ParametersLen; ++i)
	{
		free(table->Headers[i]);
	}
	free(table->Headers);
}
