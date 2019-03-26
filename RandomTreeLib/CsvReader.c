#include "CsvReader.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>
#include "utils.h"

static double* ParseNextRow(const CharRow* row, const uint colLen, char** retTuple);
char** ParseFirstRow(const CharRow* row, uint* colLen);

CsvTable* CsvReadTable(const CharsTable* input)
{
	if (input->VecBase.Size < 2)
		return (CsvTable*)-1;

	uint colLen = 0;
	char** headers = ParseFirstRow(input->Table[0], &colLen);
	if (colLen < 2)
	{
		FreeTab(headers, colLen);
		return (CsvTable*)-1;
	}
	CsvTable* table = malloc(sizeof(CsvTable));
	CsvInit(table);

	const uint parLen = colLen - 1;
	table->ClassName = headers[0];
	table->Headers = MemCopyAlloc(headers + 1, sizeof(char*)*parLen);
	free(headers);

	table->ParametersCount = parLen;
	table->RowsCount = input->VecBase.Size - 1;
	table->ClassesColumn = malloc(sizeof(CsvClassTuple)*table->RowsCount);
	CsvInitParameters(table, parLen);
	for (uint i = 1; i < input->VecBase.Size; ++i)
	{
		char* name = NULL;
		double* parsedDoubles = ParseNextRow(input->Table[i], colLen, &name);
		table->ClassesColumn[i - 1].Name = name;
		for (uint j = 0; j < parLen; ++j)
		{
			CsvSetParameterColumn(table, i - 1, j, parsedDoubles[j + 1]);
		}
		free(parsedDoubles);
	}
	return table;
}

char** ParseFirstRow(const CharRow* row, uint* colLen)
{
	const char s[2] = ",";
	char*context;
	*colLen = 0;
	char** ret = malloc(sizeof(char*)*row->Size);
	char* tempStr = CrCopyData(row);

	char* token = strtok_s(tempStr, s, &context);

	while (token != 0)
	{
		ret[*colLen] = MemCopyChars(token);
		++(*colLen);
		token = strtok_s(NULL, s, &context);
	}
	free(tempStr);
	return ret;
}

double* ParseNextRow(const CharRow* row, const uint colLen, char** retTuple)
{
	const char s[2] = ",";
	char *endPrt, *context;
	double* ret = malloc(sizeof(double)*colLen);
	char* tempStr = CrCopyData(row);
	char* token = strtok_s(tempStr, s, &context);
	*retTuple = MemCopyChars(token);
	
	token = strtok_s(NULL, s, &context);
	uint i = 0;
	while (token != 0 && i < (colLen-1))
	{
		ret[i++] = strtold(token, &endPrt);
		token = strtok_s(NULL, s, &context);
	}
	free(tempStr);
	return ret;
}
