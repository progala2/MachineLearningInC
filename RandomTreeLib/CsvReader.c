#include "CsvReader.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>
#include "utils.h"

static double* ParseNextRow(const CharRow* row, const uint colLen, char** retTuple);
char** ParseFirstRow(const CharRow* row, uint* colLen);

char** LrnTryGetHeader(const CharsTable* trainingTable, const CharsTable* testTable, uint* colLen)
{
	if (trainingTable->VecBase.Size < 2 || testTable->VecBase.Size < 2)
		return NULL;

	uint colLen2 = 0;
	char** headers = ParseFirstRow(trainingTable->Table[0], colLen);
	char** testHeaders = ParseFirstRow(testTable->Table[0], &colLen2);
	if (*colLen < 2 || *colLen != colLen2)
	{
		FreeTab(headers, *colLen);
		FreeTab(testHeaders, *colLen);
		return NULL;
	}
	FreeTab(testHeaders, *colLen);

	return headers;
}

LearnData* LrnReadData(const CharsTable* trainingTable, const CharsTable* testTable)
{
	uint colLen = 0;
	char** headers;
	if ((headers = LrnTryGetHeader(trainingTable, testTable, &colLen)) == NULL)
		return NULL;

	LearnData* table = malloc(sizeof(LearnData));
	LrnInit(table);

	const uint parLen = colLen - 1;
	table->ClassName = MemCopyChars(headers[0]);
	table->Headers = MemCopyAlloc(headers + 1, sizeof(char*)*parLen);
	free(headers);

	table->ParametersCount = parLen;
	table->RowsCount = trainingTable->VecBase.Size - 1;
	table->TestData.RowsCount = testTable->VecBase.Size - 1;
	table->ClassesColumn = malloc(sizeof(LrnClassTuple)*table->RowsCount);
	table->TestData.ClassesColumn = malloc(sizeof(LrnClassTuple)*table->TestData.RowsCount);
	LrnInitParameters(table, parLen);
	StringVector* stringVector = SvInit();
	for (uint i = 1; i < trainingTable->VecBase.Size; ++i)
	{
		char* name = NULL;
		uint foundId = 0;
		double* parsedDoubles = ParseNextRow(trainingTable->Table[i], colLen, &name);
		if (SvContains(stringVector, name, &foundId))
		{
			table->ClassesColumn[i - 1].Name = stringVector->Table[foundId];
			table->ClassesColumn[i - 1].Value = foundId;
			free(name);
		}
		else
		{
			SvAppend(stringVector, name);
			table->ClassesColumn[i - 1].Name = name;
			table->ClassesColumn[i - 1].Value = stringVector->VecBase.Size - 1;
		}
		
		for (uint j = 0; j < parLen; ++j)
		{
			LrnSetParameterColumn(table, i - 1, j, parsedDoubles[j]);
		}
		free(parsedDoubles);
	}
	table->Classes = stringVector;
	
	return table;
}

char** ParseFirstRow(const CharRow* row, uint* colLen)
{
	const char s[2] = ",";
	char*context;
	*colLen = 0;
	char** ret = malloc(sizeof(char*)*row->VecBase.Size);
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
