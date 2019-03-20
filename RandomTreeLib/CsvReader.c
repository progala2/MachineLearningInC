#include "CsvReader.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>
#include "utils.h"

static double* ParseNextRow(const CharRow* row, const uint colLen);
char** ParseFirstRow(const CharRow* row, uint* colLen);

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

	uint colLen = 0;
	//TODO universal table deleter
	char** headers = ParseFirstRow(input->Data[0], &colLen);
	if (colLen < 2)
		return (CsvTable*)-1;

	CsvTable* table = malloc(sizeof(CsvTable));
	CsvInit(table);

	uint parLen = colLen - 1;
	table->ClassName = headers[0];
	table->Headers = malloc(sizeof(char*) * parLen);
	memcpy(table->Headers, headers + 1, sizeof(char*)*parLen);
	free(headers);

	table->ParametersLen = parLen;
	table->RowsNumber = input->Size - 1;
	table->ClassColumn = malloc(sizeof(double)*table->RowsNumber);
	table->Parameters = malloc(sizeof(double*)*table->RowsNumber);
	double* parsedDoubles;
	for (int i = 1; i < input->Size; ++i)
	{
		parsedDoubles = ParseNextRow(input->Data[i], colLen);
		table->ClassColumn[i - 1] = parsedDoubles[0];
		table->Parameters[i - 1] = malloc(sizeof(double) * parLen);
		memcpy(table->Parameters[i - 1], parsedDoubles + 1, sizeof(double) * parLen);
		free(parsedDoubles);
	}
	return table;
}

char** ParseFirstRow(const CharRow* row, uint* colLen)
{
	const char s[2] = ",";
	char* token, *context;
	*colLen = 0;
	char** ret = malloc(sizeof(char*)*row->Size);
	char* tempStr = CrCopyData(row);

	token = strtok_s(tempStr, s, &context);

	while (token != 0)
	{
		size_t tokenLen = strlen(token) + 1;
		ret[*colLen] = malloc(sizeof(char)*tokenLen);
		memcpy(ret[*colLen], token, tokenLen);
		++(*colLen);
		token = strtok_s(NULL, s, &context);
	}
	free(tempStr);
	return ret;
}

double* ParseNextRow(const CharRow* row, const uint colLen)
{
	const char s[2] = ",";
	char* token, *endPrt, *context;
	double* ret = malloc(sizeof(double)*colLen);
	char* tempStr = CrCopyData(row);
	token = strtok_s(tempStr, s, &context);

	uint i = 0;
	while (token != 0 && i < colLen)
	{
		ret[i++] = strtold(token, &endPrt);
		token = strtok_s(NULL, s, &context);
	}
	free(tempStr);
	return ret;
}

void CsvFreeMemory(CsvTable* table)
{
	for (int i = 0; i < table->ParametersLen; ++i)
	{
		free(table->Headers[i]);
		free(table->Parameters[i]);
	}
	free(table->Parameters);
	table->Parameters = NULL;
	free(table->ClassColumn);
	table->ClassColumn = NULL;
	free(table->Headers);
	table->Headers = NULL;
	free(table->ClassName);
	table->ClassName = NULL;
}
