#include "ReadLearnData.h"
#include <stdlib.h>
#include "CharsTable.h"
#include <string.h>
#include "utils.h"

static double* ParseNextRow(const CharRow* row, const uint parLen, void_ptr_ref retClassName);  // NOLINT(misc-misplaced-const)
static char** ParseFirstRow(const CharRow* row, uint* colLen);

static LearnData* LrnTryInitWithHeaders(const CharsTable* trainingTable, const CharsTable* testTable, uint* colLen)
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
	lrnData->ClassName = headers[0];
	lrnData->Headers = MemCopyAlloc(headers + 1, sizeof(char*)*parLen);
	free(headers);

	lrnData->ParametersCount = parLen;
	lrnData->TrainData.RowsCount = trainingTable->VecBase.Size - 1;
	lrnData->TestData.RowsCount = testTable->VecBase.Size - 1;
	lrnData->TrainData.ClassesColumn = IntVecInit_C(lrnData->TrainData.RowsCount + 1);
	lrnData->TestData.ClassesColumn = IntVecInit_C(lrnData->TestData.RowsCount + 1);
	LrnInitParameters(lrnData, parLen);
	return lrnData;
}

static LearnData* LrnTryInitWithHeaders_NoTest(const CharsTable* trainingTable, uint* colLen)
{
	if (trainingTable->VecBase.Size < 2)
		return NULL;

	char** headers = ParseFirstRow(trainingTable->Table[0], colLen);
	if (*colLen < 2)
	{
		FreeTab(headers, *colLen);
		return NULL;
	}
	LearnData* lrnData = LrnInit();

	const uint parLen = *colLen - 1;
	lrnData->ClassName = headers[0];
	lrnData->Headers = MemCopyAlloc(headers + 1, sizeof(char*)*parLen);
	free(headers);

	lrnData->ParametersCount = parLen;
	lrnData->TrainData.RowsCount = trainingTable->VecBase.Size - 1;
	lrnData->TrainData.ClassesColumn = IntVecInit_C(lrnData->TrainData.RowsCount + 1);
	lrnData->TestData.ClassesColumn = NULL;
	lrnData->TestData.RowsCount = 0;
	LrnInitParameters(lrnData, parLen);
	return lrnData;
}

static double* ParseNextRowAndSetUpName(int* classNameValue, StringVector* classVector, const CharRow* row, const uint colLen)
{
	char* className = NULL;
	uint foundId = 0;
	double* parsedDoublesTest = ParseNextRow(row, colLen - 1, &className);
	if (SvContains(classVector, className, &foundId))
	{
		*classNameValue = foundId;
		free(className);
	}
	else
	{
		SvAppend(classVector, className);
		*classNameValue = (int)classVector->VecBase.Size - 1;
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
	int classValueTemp = 0;
	for (uint i = 1; i < trainingTable->VecBase.Size; ++i)
	{
		if (i < testTable->VecBase.Size)
		{
			double* parsedDoublesTest = ParseNextRowAndSetUpName(&classValueTemp, lrnData->Classes, testTable->Table[i], colLen);
			IntVecAppend(lrnData->TestData.ClassesColumn, classValueTemp);

			for (uint j = 0; j < parLen; ++j)
			{
				LrnSetTestParameterColumn(lrnData, i - 1, j, parsedDoublesTest[j]);
			}
			free(parsedDoublesTest);
		}
		double* parsedDoubles = ParseNextRowAndSetUpName(&classValueTemp, lrnData->Classes, trainingTable->Table[i], colLen);
		IntVecAppend(lrnData->TrainData.ClassesColumn, classValueTemp);

		for (uint j = 0; j < parLen; ++j)
		{
			LrnSetParameterColumn(lrnData, i - 1, j, parsedDoubles[j]);
		}
		free(parsedDoubles);
	}

	return lrnData;
}

LearnData* LrnReadData_NoTest(const CharsTable* trainingTable)
{
	uint colLen = 0;
	LearnData* lrnData;
	if ((lrnData = LrnTryInitWithHeaders_NoTest(trainingTable, &colLen)) == NULL)
		return NULL;

	const uint parLen = colLen - 1;

	lrnData->Classes = SvInit();
	int classValueTemp = 0;
	for (uint i = 1; i < trainingTable->VecBase.Size; ++i)
	{
		double* parsedDoubles = ParseNextRowAndSetUpName(&classValueTemp, lrnData->Classes, trainingTable->Table[i], colLen);
		IntVecAppend(lrnData->TrainData.ClassesColumn, classValueTemp);

		for (uint j = 0; j < parLen; ++j)
		{
			LrnSetParameterColumn(lrnData, i - 1, j, parsedDoubles[j]);
		}
		free(parsedDoubles);
	}

	return lrnData;
}

static char** ParseFirstRow(const CharRow* row, uint* colLen)
{
	if (row->VecBase.Size < 1)
		return NULL;

	const char s[2] = ",";
	char*context = NULL;
	*colLen = 0;
	char** _malloc(sizeof(char*)*row->VecBase.Size, ret);
	char* tempStr = CrCopyData(row);

	char* token = strtok_s(tempStr, s, &context);

	while (token != 0 && *colLen < row->VecBase.Size)
	{
		ret[*colLen] = MemCopyChars(token);
		++(*colLen);
		token = strtok_s(NULL, s, &context);
	}
	free(tempStr);
	return ret;
}

static double* ParseNextRow(const CharRow* row, const uint parLen, void_ptr_ref retClassName)  // NOLINT(misc-misplaced-const)
{
	if (parLen < 1)
		return NULL;

	const char s[2] = ",";
	char *endPrt, *context = NULL;
	double* _malloc(sizeof(double)* parLen, ret);
	char* tempStr = CrCopyData(row);
	char* token = strtok_s(tempStr, s, &context);
	*retClassName = MemCopyChars(token);

	token = strtok_s(NULL, s, &context);
	uint i = 0;
	while (i < parLen && token != 0)
	{
		ret[i++] = strtod(token, &endPrt);
		token = strtok_s(NULL, s, &context);
	}

	free(tempStr);
	return ret;
}