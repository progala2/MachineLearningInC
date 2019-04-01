#ifndef CsvTable_H_INCLUDE_GUARD
#define CsvTable_H_INCLUDE_GUARD
#include "utils.h"
#include <stdbool.h>
#include "StringVector.h"

typedef struct
{
	double* Column;
	double MaxValue;
	double MinValue;
} ParameterColumn;

typedef struct
{
	int Value;
	const char* Name;
} LrnClassTuple;

typedef struct
{
	uint RowsCount;
	double** Parameters;
	LrnClassTuple *ClassesColumn;
} TestData;

typedef struct
{
	uint ParametersCount;
	uint RowsCount;
	char** Headers;
	StringVector* Classes;
	ParameterColumn* Parameters;
	TestData TestData;
	LrnClassTuple *ClassesColumn;
	char* ClassName;
	bool Normalized;
} LearnData;

void LrnInit(LearnData* input);
void LrnInitParameters(LearnData* table, const uint parLen);
void LrnSetParameterColumn(LearnData* table, const uint i, const uint j, const double value);
void LrnNormalize(LearnData* table);
void LrnFreeMemory(LearnData** const tbl);

#endif
