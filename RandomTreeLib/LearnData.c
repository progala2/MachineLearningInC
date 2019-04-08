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
	input->ColumnsMinMaxes = NULL;
	input->RowsCount = 0;
	input->Normalized = false;
	return input;
}

void InitAndGetParameters(double*** parameterColumn, double*** testDataParameters, const size_t parLen, const size_t trainingSize, const size_t testSize)
{
	_malloc_v(sizeof(double*) * parLen, *parameterColumn);
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
		_malloc_v(sizeof(double) * trainingSize, parameterColumn[0][j]);
	}
}

void InitMinMaxes(LearnData* table, const size_t parLen)
{
	MinMaxParameters* _malloc_v(sizeof(MinMaxParameters) * parLen, parameterColumn);
	for (uint j = 0; j < parLen; ++j)
	{
		parameterColumn[j].MaxValue = DBL_MIN;
		parameterColumn[j].MinValue = DBL_MAX;
	}
	table->ColumnsMinMaxes = parameterColumn;
}

void LrnInitParameters(LearnData* table, const uint parLen)
{
	if (parLen < 1)
		return;

	InitAndGetParameters(&table->Parameters, &table->TestData.Parameters, parLen, table->RowsCount, table->TestData.RowsCount);
	InitMinMaxes(table, parLen);
}

void LrnSetParameterColumn(LearnData* table, const uint i, const uint j, const double value)
{
	table->Parameters[j][i] = value;
	table->ColumnsMinMaxes[j].MaxValue = fmax(value, table->ColumnsMinMaxes[j].MaxValue);
	table->ColumnsMinMaxes[j].MinValue = fmin(value, table->ColumnsMinMaxes[j].MinValue);
}

void LrnSetTestParameterColumn(LearnData* table, const uint i, const uint j, const double value)
{
	table->TestData.Parameters[j][i] = value;
	table->ColumnsMinMaxes[j].MaxValue = fmax(value, table->ColumnsMinMaxes[j].MaxValue);
	table->ColumnsMinMaxes[j].MinValue = fmin(value, table->ColumnsMinMaxes[j].MinValue);

}

void LrnNormalize(LearnData* table)
{
	for (uint j = 0; j < table->ParametersCount; ++j)
	{
		const long double range = (long double)table->ColumnsMinMaxes[j].MaxValue - table->ColumnsMinMaxes[j].MinValue;
		for (uint i = 0; i < table->RowsCount; ++i)
		{
			table->Parameters[j][i] = (double)((table->Parameters[j][i] - table->ColumnsMinMaxes[j].MinValue) / range);
			if (i < table->TestData.RowsCount)
				table->TestData.Parameters[j][i] = (double)((table->TestData.Parameters[j][i] - table->ColumnsMinMaxes[j].MinValue) / range);
		}
	}
	table->Normalized = true;
}

void DisparageDataBetweenTestAndTraining(IntVector * originalClassVector, const double* const* const parameters, const size_t parLen,
	IntVector * const trainingClassesColumn, IntVector * const testClassesColumn, double* const* const trainingParameters,
	double* const* const testParameters, const uint i)
{
	const uint capacityTest = testClassesColumn->VecBase.Capacity - 1;
	const uint capacityTraining = trainingClassesColumn->VecBase.Capacity - 1;
	if ((rand() % HUNDRED_PERCENT < _glConfigs->CFG_FLD_TEST_EXTRACT_PERCENTAGE || capacityTraining == trainingClassesColumn->VecBase.Size) && capacityTest > testClassesColumn->VecBase.Size)
	{
		for (uint j = 0; j < parLen; ++j)
		{
			testParameters[j][testClassesColumn->VecBase.Size] = parameters[j][i];
		}

		IntVecAppend(testClassesColumn, originalClassVector->Data[i]);
	}
	else
	{
		for (uint j = 0; j < parLen; ++j)
		{
			trainingParameters[j][trainingClassesColumn->VecBase.Size] = parameters[j][i];
		}
		IntVecAppend(trainingClassesColumn, originalClassVector->Data[i]);
	}
}

void LrnExtractTestData(LearnData * const learnData)
{
	if (learnData->TestData.RowsCount != 0 && !_glConfigs->CFG_FLD_FORCE_TEST_EXTRACT)
		return;

	const uint maxRowsCount = learnData->RowsCount + learnData->TestData.RowsCount;
	IntVector * testClassesColumn = IntVecInit_C(_glConfigs->CFG_FLD_TEST_EXTRACT_PERCENTAGE * maxRowsCount / HUNDRED_PERCENT + 1);
	IntVector * trainingClassesColumn = IntVecInit_C(maxRowsCount - testClassesColumn->VecBase.Capacity + 2);

	double** testParameters;
	double** trainingParameters;
	InitAndGetParameters(&trainingParameters, &testParameters, learnData->ParametersCount,
		trainingClassesColumn->VecBase.Capacity - 1, testClassesColumn->VecBase.Capacity - 1);
	
	for (uint i = 0; i < learnData->RowsCount; ++i)
	{
		DisparageDataBetweenTestAndTraining(learnData->ClassesColumn, learnData->Parameters, learnData->ParametersCount,
			trainingClassesColumn, testClassesColumn, trainingParameters, testParameters, i);
	}
	
	for (uint i = 0; i < learnData->TestData.RowsCount; ++i)
	{
		DisparageDataBetweenTestAndTraining(learnData->TestData.ClassesColumn, learnData->TestData.Parameters, learnData->ParametersCount,
			trainingClassesColumn, testClassesColumn, trainingParameters, testParameters, i);
	}

	FreeTab((void_tab_ptr)learnData->Parameters, learnData->ParametersCount);
	IntVecFreeMemory(&learnData->ClassesColumn);
	if (learnData->TestData.RowsCount != 0)
	{
		FreeTab((void_tab_ptr)learnData->TestData.Parameters, learnData->ParametersCount);
		IntVecFreeMemory(&learnData->TestData.ClassesColumn);
	}

	learnData->Parameters = trainingParameters;
	learnData->ClassesColumn = trainingClassesColumn;
	learnData->RowsCount = trainingClassesColumn->VecBase.Size;


	learnData->TestData.RowsCount = testClassesColumn->VecBase.Size;
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

	FreeTab((void_tab_ptr)table->TestData.Parameters, table->ParametersCount);
	FreeTab((void_tab_ptr)table->Parameters, table->ParametersCount);
	FreeTab((void_tab_ptr)table->Headers, table->ParametersCount);
	free(table->ColumnsMinMaxes);
	IntVecFreeMemory(&table->ClassesColumn);
	IntVecFreeMemory(&table->TestData.ClassesColumn);
	free(table->ClassName);
	SvFree(&table->Classes);
	free(table);
	*tbl = NULL;
}
