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
	input->TrainData.ClassesColumn = NULL;
	input->TrainData.Parameters = NULL;
	input->TestData.ClassesColumn = NULL;
	input->TestData.Parameters = NULL;
	input->Headers = NULL;
	input->ParametersCount = 0;
	input->ColumnsMinMaxes = NULL;
	input->TrainData.RowsCount = 0;
	input->Normalized = false;
	return input;
}

void InitAndGetParameters(double*** parameterColumn, double*** testDataParameters, const size_t parLen, const size_t trainingSize, const size_t testSize)
{
	if (trainingSize < 1 || parLen < 1)
		exit(-1);

	_malloc(sizeof(double*) * parLen, *parameterColumn);
	if (testSize > 0)
	{
		_malloc(sizeof(double*) * parLen, *testDataParameters);
	}
	else
	{
		*testDataParameters = NULL;
	}
	for (uint j = 0; j < parLen; ++j)
	{
		if (testSize > 0)
		{
			_malloc(sizeof(double) * testSize, testDataParameters[0][j]);
		}
		_malloc(sizeof(double) * trainingSize, parameterColumn[0][j]);
	}
}

void InitMinMaxes(LearnData* table, const size_t parLen)
{
	MinMaxParameters* _malloc(sizeof(MinMaxParameters) * parLen, parameterColumn);
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

	InitAndGetParameters(&table->TrainData.Parameters, &table->TestData.Parameters, parLen, table->TrainData.RowsCount, table->TestData.RowsCount);
	InitMinMaxes(table, parLen);
}

void LrnSetParameterColumn(LearnData* table, const uint i, const uint j, const double value)
{
	table->TrainData.Parameters[j][i] = value;
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
		for (uint i = 0; i < table->TrainData.RowsCount; ++i)
		{
			table->TrainData.Parameters[j][i] = (double)((table->TrainData.Parameters[j][i] - table->ColumnsMinMaxes[j].MinValue) / range);
			if (i < table->TestData.RowsCount)
				table->TestData.Parameters[j][i] = (double)((table->TestData.Parameters[j][i] - table->ColumnsMinMaxes[j].MinValue) / range);
		}
	}
	table->Normalized = true;
}

void DisparageDataBetweenTestAndTraining(const Data* const data, const size_t parLen,
	IntVector * const trainingClassesColumn, IntVector * const testClassesColumn, double* const* const trainingParameters,
	double* const* const testParameters, const uint i)
{
	const uint capacityTest = testClassesColumn->VecBase.Capacity - 1;
	const uint capacityTraining = trainingClassesColumn->VecBase.Capacity - 1;
	if (((uint)rand() % HUNDRED_PERCENT < _glConfigs->CFG_FLD_TEST_EXTRACT_PERCENTAGE || capacityTraining == trainingClassesColumn->VecBase.Size) 
		&& capacityTest > testClassesColumn->VecBase.Size)
	{
		for (uint j = 0; j < parLen; ++j)
		{
			testParameters[j][testClassesColumn->VecBase.Size] = data->Parameters[j][i];
		}

		IntVecAppend(testClassesColumn, data->ClassesColumn->Data[i]);
	}
	else
	{
		for (uint j = 0; j < parLen; ++j)
		{
			trainingParameters[j][trainingClassesColumn->VecBase.Size] = data->Parameters[j][i];
		}
		IntVecAppend(trainingClassesColumn, data->ClassesColumn->Data[i]);
	}
}

void LrnSetNewData(Data* data, double** parameters, const size_t rowsCount, IntVector*const classesColumn)
{
	data->RowsCount = rowsCount;
	data->ClassesColumn = classesColumn;
	data->Parameters = parameters;
}

void LrnExtractTestData(LearnData * const learnData)
{
	if (learnData->TestData.RowsCount != 0 && !_glConfigs->CFG_FLD_FORCE_TEST_EXTRACT)
		return;

	const uint maxRowsCount = learnData->TrainData.RowsCount + learnData->TestData.RowsCount;
	IntVector * testClassesColumn = IntVecInit_C(_glConfigs->CFG_FLD_TEST_EXTRACT_PERCENTAGE * maxRowsCount / HUNDRED_PERCENT + 1);
	IntVector * trainingClassesColumn = IntVecInit_C(maxRowsCount - testClassesColumn->VecBase.Capacity + 2);

	double** testParameters;
	double** trainingParameters;
	InitAndGetParameters(&trainingParameters, &testParameters, learnData->ParametersCount,
		trainingClassesColumn->VecBase.Capacity - 1, testClassesColumn->VecBase.Capacity - 1);

	for (uint i = 0; i < learnData->TrainData.RowsCount; ++i)
	{
		DisparageDataBetweenTestAndTraining(&learnData->TrainData, learnData->ParametersCount,
			trainingClassesColumn, testClassesColumn, trainingParameters, testParameters, i);
	}

	for (uint i = 0; i < learnData->TestData.RowsCount; ++i)
	{
		DisparageDataBetweenTestAndTraining(&learnData->TestData, learnData->ParametersCount,
			trainingClassesColumn, testClassesColumn, trainingParameters, testParameters, i);
	}

	FreeTab((void_tab_ptr)learnData->TrainData.Parameters, learnData->ParametersCount);
	IntVecFreeMemory(&learnData->TrainData.ClassesColumn);
	if (learnData->TestData.RowsCount != 0)
	{
		FreeTab((void_tab_ptr)learnData->TestData.Parameters, learnData->ParametersCount);
		IntVecFreeMemory(&learnData->TestData.ClassesColumn);
	}

	LrnSetNewData(&learnData->TrainData, trainingParameters, trainingClassesColumn->VecBase.Size, trainingClassesColumn);
	LrnSetNewData(&learnData->TestData, testParameters, testClassesColumn->VecBase.Size, testClassesColumn);
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

void LrnPrintHeader(FILE* const stream, const LearnData* const table)
{
	fprintf(stream, "%s, ", table->ClassName);
	for (uint j = 0; j < table->ParametersCount; j++)
	{
		fprintf(stream, "%s, ", table->Headers[j]);
	}
	fprintf(stream, "\n");
}

void LrnPrintData_F(FILE* const stream, const Data*const data, const size_t parametersCount, 
	const StringVector*const classes)
{
	const uint parametersCountMinOne = parametersCount - 1;
	for (uint i = 0; i < data->RowsCount; i++)
	{
		fprintf(stream, "%s, ", classes->Table[data->ClassesColumn->Data[i]]);
		for (uint j = 0; j < parametersCountMinOne; j++)
		{
			fprintf(stream, "%.12g, ", data->Parameters[j][i]);
		}
		fprintf(stream, "%.12g\n", data->Parameters[parametersCountMinOne][i]);
	}
}

void LrnPrintTestData_F(FILE* const stream, const LearnData* const table)
{
	LrnPrintHeader(stream, table);
	LrnPrintData_F(stream, &table->TestData, table->ParametersCount, table->Classes);
}

void LrnPrintTrainingData_F(FILE* const stream, const LearnData* const table)
{
	LrnPrintHeader(stream, table);
	LrnPrintData_F(stream, &table->TrainData, table->ParametersCount, table->Classes);
}

void LrnPrintTestAndTrainingData(const LearnData* const table)
{
	LrnPrintTrainingData_F(stdout, table);
	fprintf(stdout, "TestData: \n");
	LrnPrintTestData_F(stdout, table);
}

void LrnFreeMemory(LearnData** const tbl)
{
	LearnData* table = *tbl;
	if (table == NULL)
		return;

	FreeTab((void_tab_ptr)table->TestData.Parameters, table->ParametersCount);
	FreeTab((void_tab_ptr)table->TrainData.Parameters, table->ParametersCount);
	FreeTab((void_tab_ptr)table->Headers, table->ParametersCount);
	free(table->ColumnsMinMaxes);
	IntVecFreeMemory(&table->TrainData.ClassesColumn);
	IntVecFreeMemory(&table->TestData.ClassesColumn);
	free(table->ClassName);
	SvFree(&table->Classes);
	free(table);
	*tbl = NULL;
}
