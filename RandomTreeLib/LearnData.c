#include "LearnData.h"
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include "RtConfigs.h"

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

void InitCopyAndGetParameters(ParameterColumn** parameterColumn, double*** testDataParameters, const size_t parLen, const size_t trainingSize, const size_t testSize,
	const ParameterColumn* const currentCol)
{
	_malloc_v(sizeof(ParameterColumn) * parLen, *parameterColumn);
	_malloc_v(sizeof(double*) * parLen, *testDataParameters);
	for (uint j = 0; j < parLen; ++j)
	{
		_malloc_v(sizeof(double) * testSize, testDataParameters[0][j]);
		_malloc_v(sizeof(double) * trainingSize, parameterColumn[0][j].Column);
		parameterColumn[0][j].MaxValue = currentCol[j].MaxValue;
		parameterColumn[0][j].MinValue = currentCol[j].MinValue;
	}
}

void InitAndGetParameters(ParameterColumn** parameterColumn, double*** testDataParameters, const size_t parLen, const size_t trainingSize, const size_t testSize)
{
	_malloc_v(sizeof(ParameterColumn) * parLen, *parameterColumn);
	if (testSize > 0)
	{
		_malloc_v(sizeof(double*) * parLen, *testDataParameters);
	}
	else
	{
		*testDataParameters = NULL;
	}
	for (uint j = 0; j < parLen; ++j)
	{
		if (testSize > 0)
		{
			_malloc_v(sizeof(double) * testSize, testDataParameters[0][j]);
		}
		_malloc_v(sizeof(double) * trainingSize, parameterColumn[0][j].Column);
		parameterColumn[0][j].MaxValue = DBL_MIN;
		parameterColumn[0][j].MinValue = DBL_MAX;
	}
}

void LrnInitParameters(LearnData* table, const uint parLen)
{
	if (parLen < 1)
		return;

	InitAndGetParameters(&table->Parameters, &table->TestData.Parameters, parLen, table->RowsCount, table->TestData.RowsCount);
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

void LrnNormalize(LearnData* table)
{
	for (uint j = 0; j < table->ParametersCount; ++j)
	{
		const long double range = (long double)table->Parameters[j].MaxValue - table->Parameters[j].MinValue;
		for (uint i = 0; i < table->RowsCount; ++i)
		{
			table->Parameters[j].Column[i] = (table->Parameters[j].Column[i] - table->Parameters[j].MinValue) / range;
			if (i < table->TestData.RowsCount)
				table->TestData.Parameters[j][i] = (table->TestData.Parameters[j][i] - table->Parameters[j].MinValue) / range;
		}
	}
	table->Normalized = true;
}


void LrnExtractTestData(LearnData * const learnData)
{
	if (learnData->TestData.RowsCount != 0)
		return;

	IntVector * testClassesColumn = IntVecInit_C(_glConfigs->CFG_FLD_TEST_EXTRACT_PERCENTAGE * learnData->RowsCount / HUNDRED_PERCENT + 1);
	IntVector * trainingClassesColumn = IntVecInit_C(learnData->RowsCount - testClassesColumn->VecBase.Capacity + 2);

	const uint capacityTest = testClassesColumn->VecBase.Capacity - 1;
	const uint capacityTraining = trainingClassesColumn->VecBase.Capacity - 1;
	double** testParameters;
	ParameterColumn * trainingParameters;
	InitCopyAndGetParameters(&trainingParameters, &testParameters, learnData->ParametersCount, capacityTraining, capacityTest, learnData->Parameters);
	for (uint i = 0; i < learnData->RowsCount; ++i)
	{
		if ((rand() % HUNDRED_PERCENT < _glConfigs->CFG_FLD_TEST_EXTRACT_PERCENTAGE || capacityTraining == trainingClassesColumn->VecBase.Size) && capacityTest > testClassesColumn->VecBase.Size)
		{
			for (uint j = 0; j < learnData->ParametersCount; ++j)
			{
				testParameters[j][testClassesColumn->VecBase.Size] = learnData->Parameters[j].Column[i];	
			}
			
			IntVecAppend(testClassesColumn, learnData->ClassesColumn->Data[i]);
			
		}
		else
		{
			for (uint j = 0; j < learnData->ParametersCount; ++j)
			{
				trainingParameters[j].Column[testClassesColumn->VecBase.Size] = learnData->Parameters[j].Column[i];
			}
			IntVecAppend(trainingClassesColumn, learnData->ClassesColumn->Data[i]);
		}
	}
	
	for (uint i = 0; i < learnData->ParametersCount; ++i)
	{
		free(learnData->Parameters[i].Column);
	}
	IntVecFreeMemory(&learnData->ClassesColumn);
	
	learnData->Parameters = trainingParameters;
	learnData->ClassesColumn = trainingClassesColumn;
	learnData->RowsCount = capacityTraining;

	
	learnData->TestData.RowsCount = capacityTest;
	learnData->TestData.ClassesColumn = testClassesColumn;
	learnData->TestData.Parameters = testParameters;
}

unsigned* LrnCountByClass(const IntVector * classesColumn, const size_t classesCount)
{
	uint* _calloc(classesCount, sizeof(uint), countByClass);

	for (size_t i = 0; i < classesColumn->VecBase.Size; ++i)
	{
		countByClass[classesColumn->Data[i]]++;
	}
	return countByClass;
}

void LrnFreeMemory(LearnData * *const tbl)
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
