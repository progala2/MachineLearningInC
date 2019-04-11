#ifndef CsvTable_H_INCLUDE_GUARD
#define CsvTable_H_INCLUDE_GUARD
#include "utils.h"
#include "StringVector.h"
#include "DoubleVector.h"
#include <stdio.h>

typedef struct
{
	double MaxValue;
	double MinValue;
} MinMaxParameters;

typedef struct
{
	uint RowsCount;
	double** Parameters;
	IntVector *ClassesColumn;
} Data;

typedef struct
{
	uint ParametersCount;
	char** Headers;
	StringVector* Classes;
	
	MinMaxParameters* ColumnsMinMaxes;
	Data TestData;
	Data TrainData;
	char* ClassName;
	bool Normalized;
} LearnData;

LearnData* LrnInit();
void LrnInitParameters(LearnData* table, const uint parLen);
void LrnSetParameterColumn(LearnData* table, const uint i, const uint j, const double value);
void LrnSetTestParameterColumn(LearnData* table, const uint i, const uint j, const double value);
void LrnNormalize(LearnData* table);
void LrnExtractTestData(LearnData*const learnData);
Data* LrnExtractNewTrainAndTestData(LearnData*const learnData);
unsigned* LrnCountByClass(const IntVector* classesColumn, const size_t classesCount);

void LrnPrintTrainingData_F(FILE* const stream, const LearnData* const table);
void LrnPrintTestData_F(FILE* const stream, const LearnData* const table);

void LrnFreeMemory(LearnData** const tbl);

#endif
