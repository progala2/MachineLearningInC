#ifndef CsvTable_H_INCLUDE_GUARD
#define CsvTable_H_INCLUDE_GUARD
#include "utils.h"
#include <stdbool.h>

typedef struct
{
	double* Column;
	double MaxValue;
	double MinValue;
} ParameterColumn;

typedef struct
{
	uint ParametersCount;
	uint RowsCount;
	char** Headers;
	ParameterColumn* Parameters;
	double* ClassColumn;
	char* ClassName;
	bool Normalized;
} CsvTable;

void CsvInit(CsvTable* input);
void CsvInitParameters(CsvTable* table, const uint parLen);
void CsvSetParameterColumn(CsvTable* table, const uint i, const uint j, const double value);
void CsvNormalize(CsvTable* table);
void CsvFreeMemory(CsvTable* table);

#endif