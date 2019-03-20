#include "CsvTable.h"
#include <stdlib.h>
#include <float.h>
#include "utils.h"
#include <math.h>
#include <stdbool.h>

void CsvInit(CsvTable* input)
{
	input->ClassName = NULL;
	input->ClassColumn = NULL;
	input->Headers = NULL;
	input->ParametersCount = 0;
	input->Parameters = NULL;
	input->RowsCount = 0;
	input->Normalized = false;
}

void CsvInitParameters(CsvTable* table, const uint parLen)
{
	table->Parameters = malloc(sizeof(ParameterColumn)*parLen);
	for (uint j = 0; j < parLen; ++j)
	{
		table->Parameters[j].Column = malloc(sizeof(double) * table->RowsCount);
		table->Parameters->MaxValue = DBL_MAX;
		table->Parameters->MinValue = DBL_MIN;
	}
}

void CsvSetParameterColumn(CsvTable* table, const uint i, const uint j, const double value)
{
	table->Parameters[j].Column[i] = value;
	table->Parameters[j].MaxValue = fmax(value, table->Parameters[j].MaxValue);
	table->Parameters[j].MinValue = fmin(value, table->Parameters[j].MinValue);
}

void CsvNormalize(CsvTable * table)
{
	for (uint j = 0; j < table->ParametersCount; ++j)
	{
		const long double range = (long double)table->Parameters[j].MaxValue - table->Parameters[j].MinValue;
		for (uint i = 0; i < table->RowsCount; ++i)
		{
			table->Parameters[j].Column[i] = (table->Parameters[j].Column[i] - table->Parameters[j].MinValue)/range;
		}
	}
	table->Normalized = true;
}

void CsvFreeMemory(CsvTable* table)
{
	for (int i = 0; i < table->ParametersCount; ++i)
	{
		free(table->Headers[i]);
		free(table->Parameters[i].Column);
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
