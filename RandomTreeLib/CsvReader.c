#include "CsvReader.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>
#include "utils.h"

static double* ParseNextRow(const CharRow* row, const uint colLen);
char** ParseFirstRow(const CharRow* row, uint* colLen);

CsvTable* CsvReadTable(const CharsTable* input)
{
	if (input->Size < 2)
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
	table->RowsCount = input->Size - 1;
	table->ClassColumn = malloc(sizeof(double)*table->RowsCount);
	CsvInitParameters(table, parLen);
	for (uint i = 1; i < input->Size; ++i)
	{
		double* parsedDoubles = ParseNextRow(input->Table[i], colLen);
		table->ClassColumn[i - 1] = parsedDoubles[0];
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

double* ParseNextRow(const CharRow* row, const uint colLen)
{
	const char s[2] = ",";
	char *endPrt, *context;
	double* ret = malloc(sizeof(double)*colLen);
	char* tempStr = CrCopyData(row);
	char* token = strtok_s(tempStr, s, &context);

	uint i = 0;
	while (token != 0 && i < colLen)
	{
		ret[i++] = strtold(token, &endPrt);
		token = strtok_s(NULL, s, &context);
	}
	free(tempStr);
	return ret;
}