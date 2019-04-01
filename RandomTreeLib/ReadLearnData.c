#include "ReadLearnData.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>
#include "utils.h"

static double* ParseNextRow(const CharRow* row, const uint colLen, void_ptr_ref retClassName);  // NOLINT(misc-misplaced-const)
char** ParseFirstRow(const CharRow* row, uint* colLen);

LearnData* LrnTryInitWithHeaders(const CharsTable* trainingTable, const CharsTable* testTable, uint* colLen)
{
	if (trainingTable->VecBase.Size < 2 || testTable->VecBase.Size < 2)
		return NULL;

	uint colLenTest = 0;
	char** headers = ParseFirstRow(trainingTable->Table[0], colLen);
	char** testHeaders = ParseFirstRow(testTable->Table[0], &colLenTest);
	if (*colLen < 2 || *colLen != colLenTest)
	{
		FreeTab(headers, *colLen);
		FreeTab(testHeaders, colLenTest);
		return NULL;
	}
	FreeTab(testHeaders, colLenTest);

	LearnData* lrnData = LrnInit();

	const uint parLen = *colLen - 1;
	lrnData->ClassName = MemCopyChars(headers[0]);
	lrnData->Headers = MemCopyAlloc(headers + 1, sizeof(char*)*parLen);
	free(headers);

	lrnData->ParametersCount = parLen;
	lrnData->RowsCount = trainingTable->VecBase.Size - 1;
	lrnData->TestData.RowsCount = testTable->VecBase.Size - 1;
	lrnData->ClassesColumn = malloc(sizeof(LrnClassTuple)*lrnData->RowsCount);
	lrnData->TestData.ClassesColumn = malloc(sizeof(LrnClassTuple)*lrnData->TestData.RowsCount);
	LrnInitParameters(lrnData, parLen);
	return lrnData;
}

double* ParseNextRowAndSetUpName(LrnClassTuple* classNameValue, StringVector* classVector, const CharRow* row, const uint colLen)
{
	char* className = NULL;
	uint foundId = 0;
	double* parsedDoublesTest = ParseNextRow(row, colLen, &className);
	if (SvContains(classVector, className, &foundId))
	{
		classNameValue->Name = classVector->Table[foundId];
		classNameValue->Value = foundId;
		free(className);
	}
	else
	{
		SvAppend(classVector, className);
		classNameValue->Name = className;
		classNameValue->Value = classVector->VecBase.Size - 1;
	}
	return parsedDoublesTest;
}

LearnData* LrnReadData(const CharsTable* trainingTable, const CharsTable* testTable)
{
	uint colLen = 0;
	LearnData* lrnData;
	if ((lrnData = LrnTryInitWithHeaders(trainingTable, testTable, &colLen)) == NULL)
		return NULL;

	const uint parLen = colLen - 1;

	lrnData->Classes = SvInit();
	for (uint i = 1; i < trainingTable->VecBase.Size; ++i)
	{
		if (i < testTable->VecBase.Size)
		{
			double* parsedDoublesTest = ParseNextRowAndSetUpName(&lrnData->TestData.ClassesColumn[i - 1], lrnData->Classes, testTable->Table[i], colLen);

			for (uint j = 0; j < parLen; ++j)
			{
				LrnSetTestParameterColumn(lrnData, i - 1, j, parsedDoublesTest[j]);
			}
			free(parsedDoublesTest);
		}
		double* parsedDoubles = ParseNextRowAndSetUpName(&lrnData->ClassesColumn[i - 1], lrnData->Classes, trainingTable->Table[i], colLen);

		for (uint j = 0; j < parLen; ++j)
		{
			LrnSetParameterColumn(lrnData, i - 1, j, parsedDoubles[j]);
		}
		free(parsedDoubles);
	}

	return lrnData;
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

double* ParseNextRow(const CharRow* row, const uint colLen, void_ptr_ref retClassName)  // NOLINT(misc-misplaced-const)
{
	const char s[2] = ",";
	char *endPrt, *context;
	double* ret = malloc(sizeof(double)*colLen);
	char* tempStr = CrCopyData(row);
	char* token = strtok_s(tempStr, s, &context);
	*retClassName = MemCopyChars(token);

	token = strtok_s(NULL, s, &context);
	uint i = 0;
	while (token != 0 && i < (colLen - 1))
	{
		ret[i++] = strtold(token, &endPrt);
		token = strtok_s(NULL, s, &context);
	}

	free(tempStr);
	return ret;
}