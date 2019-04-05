#include "LearnData.h"
#include <stdlib.h>
#include <float.h>
#include "utils.h"
#include <math.h>
#include <stdbool.h>

LearnData* LrnInit()
{
	LearnData* _malloc(sizeof(LearnData), input);
	input->ClassName = NULL;
	input->ClassesColumn = NULL;
	input->TestData.ClassesColumn = NULL;
	input->TestData.Parameters = NULL;
	input->Headers = NULL;
	input->ParametersCount = 0;
	input->Parameters = NULL;
	input->RowsCount = 0;
	input->Normalized = false;
	return input;
}

void LrnInitParameters(LearnData* table, const uint parLen)
{
	if (parLen < 1)
		return;

	_malloc_v(sizeof(ParameterColumn)*parLen, table->Parameters);
	_malloc_v(sizeof(double*)*parLen, table->TestData.Parameters);
	for (uint j = 0; j < parLen; ++j)
	{
		_malloc_v(sizeof(double) * table->TestData.RowsCount, table->TestData.Parameters[j]);
		_malloc_v(sizeof(double) * table->RowsCount, table->Parameters[j].Column);
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

void LrnSetTestParameterColumn(LearnData* table, const uint i, const uint j, const double value)
{
	table->TestData.Parameters[j][i] = value;
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
			if (i < table->TestData.RowsCount)
				table->TestData.Parameters[j][i] = (table->TestData.Parameters[j][i] - table->Parameters[j].MinValue)/range;
		}
	}
	table->Normalized = true;
}

unsigned* LrnCountByClass(const IntVector* classesColumn, const size_t size, const size_t classesCount)
{
	uint* _calloc(classesCount, sizeof(uint), countByClass);

	for (size_t i = 0; i < size; ++i)
	{
		countByClass[classesColumn->Data[i]]++;
	}
	return countByClass;
}

void LrnFreeMemory(LearnData** const tbl)
{
	LearnData* table = *tbl;
	for (uint i = 0; i < table->ParametersCount; ++i)
	{
		free(table->Parameters[i].Column);
	}
	FreeTab((void_tab_ptr)table->TestData.Parameters, table->ParametersCount);
	FreeTab((void_tab_ptr)table->Headers, table->ParametersCount);
	free(table->Parameters);
	IntVecFreeMemory(&table->ClassesColumn);
	IntVecFreeMemory(&table->TestData.ClassesColumn);
	free(table->ClassName);
	SvFree(&table->Classes);
	free(table);
	*tbl = NULL;
}
