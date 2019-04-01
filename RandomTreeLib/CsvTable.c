#include "CsvTable.h"
#include <stdlib.h>
#include <float.h>
#include "utils.h"
#include <math.h>
#include <stdbool.h>

void LrnInit(LearnData* input)
{
	input->ClassName = NULL;
	input->ClassesColumn = NULL;
	input->TestData.ClassesColumn = NULL;
	input->TestData.Parameters = NULL;
	input->Headers = NULL;
	input->ParametersCount = 0;
	input->Parameters = NULL;
	input->RowsCount = 0;
	input->Normalized = false;
}

void LrnInitParameters(LearnData* table, const uint parLen)
{
	table->Parameters = malloc(sizeof(ParameterColumn)*parLen);
	table->TestData.Parameters = malloc(sizeof(double*)*parLen);
	for (uint j = 0; j < parLen; ++j)
	{
		table->TestData.Parameters[j] = malloc(sizeof(double) * table->TestData.RowsCount);
		table->Parameters[j].Column = malloc(sizeof(double) * table->RowsCount);
		table->Parameters[j].MaxValue = DBL_MIN;
		table->Parameters[j].MinValue = DBL_MAX;
	}
}

void LrnSetParameterColumn(LearnData* table, const uint i, const uint j, const double value)
{
	table->Parameters[j].Column[i] = value;
	table->Parameters[j].MaxValue = fmax(value, table->Parameters[j].MaxValue);
	table->Parameters[j].MinValue = fmin(value, table->Parameters[j].MinValue);
}

void LrnNormalize(LearnData * table)
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

void LrnFreeMemory(LearnData** const tbl)
{
	LearnData* table = (*tbl);
	for (uint i = 0; i < table->ParametersCount; ++i)
	{
		free(table->Headers[i]);
		free(table->Parameters[i].Column);
	}
	FreeTabVoid((void_tab_ptr)table->TestData.Parameters, table->TestData.RowsCount, sizeof(double));
	free(table->Parameters);
	free(table->ClassesColumn);
	free(table->TestData.ClassesColumn);
	free(table->Headers);
	free(table->ClassName);
	free(table);
	*tbl = NULL;
}
