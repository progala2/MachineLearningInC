#ifndef CsvTable_H_INCLUDE_GUARD
#define CsvTable_H_INCLUDE_GUARD
#include "utils.h"
#include "StringVector.h"
#include "DoubleVector.h"

typedef struct
{
	double* Column;
	double MaxValue;
	double MinValue;
} ParameterColumn;

typedef struct
{
	uint RowsCount;
	double** Parameters;
	IntVector *ClassesColumn;
} TestData;

typedef struct
{
	uint ParametersCount;
	uint RowsCount;
	char** Headers;
	StringVector* Classes;
	ParameterColumn* Parameters;
	TestData TestData;
	IntVector*ClassesColumn;
	char* ClassName;
	bool Normalized;
} LearnData;

LearnData* LrnInit();
void LrnInitParameters(LearnData* table, const uint parLen);
void LrnSetParameterColumn(LearnData* table, const uint i, const uint j, const double value);
void LrnSetTestParameterColumn(LearnData* table, const uint i, const uint j, const double value);
void LrnNormalize(LearnData* table);

unsigned* LrnCountByClass(const IntVector* classesColumn, const size_t size, const size_t classesCount);
void LrnFreeMemory(LearnData** const tbl);

#endif
