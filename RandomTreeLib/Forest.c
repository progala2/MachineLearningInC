#include "Forest.h"
#include "NodeGenerator.h"


unsigned* CountByClass(const LrnClassTuple* classesColumn, const size_t size, const size_t classesCount)
{
	uint* countByClass = calloc(classesCount, sizeof(uint));

	for (size_t i = 0; i < size; ++i)
	{
		countByClass[classesColumn[i].Value]++;
	}
	return countByClass;
}

void CalculateTreeDecision(double**const predictionSumPerClass, const double itemValue, const double valueSeparator, const Node* const left, const Node* const right, const uint classesCount)
{
	const Node* nd = itemValue <= valueSeparator ? left : right;

	if (TreeIsLeaf(nd))
	{
		for (uint k = 0; k < classesCount; ++k)
		{
			predictionSumPerClass[0][k] += nd->ClassesProbability[k];
		}
	}
	else
	{
		CalculateTreeDecision(predictionSumPerClass, itemValue, nd->ParameterValueSeparator, nd->Left, nd->Right, classesCount);
	}
}

ConfMatrix* FrstCalculateOnTestData(const Forest* const forest, const LearnData* const table)
{
	const uint rowsCount = table->TestData.RowsCount;
	const uint classesCount = table->Classes->VecBase.Size;
	int* actual = malloc(sizeof(int)*rowsCount);
	int* predicted = malloc(sizeof(int)*rowsCount);
	double* predictionSumPerClass = malloc(sizeof(double)*classesCount);
	for (uint i = 0; i < rowsCount; ++i)
	{
		actual[i] = table->TestData.ClassesColumn[i].Value;
		memset(predictionSumPerClass, 0, classesCount * sizeof(double));
		for (uint j = 0; j < forest->TreesCount; ++j)
		{
			CalculateTreeDecision(&predictionSumPerClass, table->TestData.Parameters[forest->Trees[j]->ParameterIndex][i], forest->Trees[j]->ParameterValueSeparator, forest->Trees[j]->Left, forest->Trees[j]->Right, classesCount);
		}
		uint maxK = 0;
		for (uint k = 0; k < classesCount; ++k)
		{
			if (predictionSumPerClass[k] > predictionSumPerClass[maxK])
				maxK = k;
		}
		predicted[i] = maxK;
	}
	free(predictionSumPerClass);

	ConfMatrix* matrix = CmCreate(actual, predicted, (const char**)table->Classes->Table, table->Classes->VecBase.Size, rowsCount);
	free(actual);
	return matrix;
}

Forest* FrstGenerateForest(const RtConfigs* const configs, const LearnData* const table)
{
	Tree** trees = malloc(sizeof(Tree*) * configs->TreeCount);
	uint* countByClass = CountByClass(table->ClassesColumn, table->RowsCount, table->Classes->VecBase.Size);
	for (uint i = 0; i < configs->TreeCount; ++i)
	{
		trees[i] = NdGenerateTree(configs, table->ParametersCount, table->Parameters, table->ClassesColumn, table->RowsCount, countByClass, table->Classes->VecBase.Size);
	}
	free(countByClass);
	Forest* forest = malloc(sizeof(Forest));
	forest->TreesCount = configs->TreeCount;
	forest->Trees = trees;
	return forest;
}

void FrstFree(Forest**const forest)
{
	if (*forest == NULL)
		return;
	for (uint i = 0; i < (*forest)->TreesCount; ++i)
	{
		TrFree(&(*forest)->Trees[i]);
	}
	_FreeN(forest);
}
