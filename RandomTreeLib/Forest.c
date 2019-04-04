#include "Forest.h"
#include "NodeGenerator.h"


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
	if (rowsCount == 0 || classesCount < 2)
		return NULL;

	int* _malloc(sizeof(int)*rowsCount, actual);
	int* _malloc(sizeof(int)*rowsCount, predicted);
	double* _malloc(sizeof(double)*classesCount, predictionSumPerClass);
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

Forest* FrstGenerateForest(const LearnData* const table)
{
	Tree** _malloc(sizeof(Tree*) * _glConfigs->TreeCount, trees);
	uint* countByClass = LrnCountByClass(table->ClassesColumn, table->RowsCount, table->Classes->VecBase.Size);
	for (uint i = 0; i < _glConfigs->TreeCount; ++i)
	{
		trees[i] = NdGenerateTree(table->ParametersCount, table->Parameters, table->ClassesColumn, table->RowsCount, countByClass, table->Classes->VecBase.Size);
	}
	free(countByClass);
	Forest* forest = malloc(sizeof(Forest));
	forest->TreesCount = _glConfigs->TreeCount;
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
