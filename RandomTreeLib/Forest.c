#include "Forest.h"
#include "NodeGenerator.h"


void CalculateTreeDecision(double*const predictionSumPerClass, const double** itemValues, const uint rowIndex, const IntVector* parameterIndexes, const DoubleVector* valueSeparator, const Node* const left, const Node* const right, const uint classesCount)
{
	const Node* nd = CheckIfItIsLeftElem_T(itemValues, rowIndex, parameterIndexes, valueSeparator) ? left : right;

	if (TreeIsLeaf(nd))
	{
		for (uint k = 0; k < classesCount; ++k)
		{
			predictionSumPerClass[k] += nd->ClassesProbability[k];
		}
	}
	else
	{
		CalculateTreeDecision(predictionSumPerClass, itemValues, rowIndex, nd->ParameterIndexes, nd->ParameterValueSeparators, nd->Left, nd->Right, classesCount);
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
		actual[i] = table->TestData.ClassesColumn->Data[i];
		memset(predictionSumPerClass, 0, classesCount * sizeof(double));
		for (uint j = 0; j < forest->TreesCount; ++j)
		{
			CalculateTreeDecision(predictionSumPerClass, table->TestData.Parameters, i, forest->Trees[j]->ParameterIndexes, forest->Trees[j]->ParameterValueSeparators, forest->Trees[j]->Left, forest->Trees[j]->Right, classesCount);
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
	free(predicted);
	return matrix;
}

Forest* FrstGenerateForest(const LearnData* const table)
{
	if (_glConfigs->TreeCount < 1)
		return NULL;

	Tree** _malloc(sizeof(Tree*) * _glConfigs->TreeCount, trees);
	uint* countByClass = LrnCountByClass(table->ClassesColumn, table->RowsCount, table->Classes->VecBase.Size);
	
	for (uint i = 0; i < _glConfigs->TreeCount; ++i)
	{
		trees[i] = NdGenerateTree(table->ParametersCount, table->Parameters, table->ClassesColumn, table->RowsCount, countByClass, table->Classes->VecBase.Size);
	}
	free(countByClass);
	Forest* _malloc(sizeof(Forest), forest);
	forest->TreesCount = _glConfigs->TreeCount;
	forest->Trees = trees;
	return forest;
}

void FrstFree(Forest**const forest)
{
	if (*forest == NULL)
		return;
	Forest* help = *forest;
	for (uint i = 0; i < help->TreesCount; ++i)
	{
		TrFree(&help->Trees[i]);
	}
	free(help->Trees);
	_FreeN(forest);
}
