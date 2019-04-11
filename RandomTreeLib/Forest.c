#include "Forest.h"
#include "TreeGenerator.h"
#include <float.h>


void WinnerTakesAllDecision(double* const predictionSumPerClass, const double* const classesProbability, const uint classesCount)
{
	uint bestK = 0;
	double max = classesProbability[0];
	for (uint k = 1; k < classesCount; ++k)
	{
		if (max < classesProbability[k])
		{
			bestK = k;
			max = classesProbability[k];
		}
	}
	predictionSumPerClass[bestK] += 1;
}

void SumAllPredictionsDecision(double* const predictionSumPerClass, const double* const classesProbability, const uint classesCount)
{
	for (uint k = 0; k < classesCount; ++k)
	{
		predictionSumPerClass[k] += classesProbability[k];
	}
}

static void CalculateTreeDecision(double* const predictionSumPerClass, const double* const* const itemValues, const uint rowIndex, const Node* const node, const uint classesCount)
{
	const Node* nd = CheckIfItIsLeftElem_T(itemValues, rowIndex, node->ParameterIndexes, node->ParameterValueSeparators, node->ParameterSeparatorTypes) ? node->Left : node->Right;

	if (TrIsLeaf(nd))
	{
		switch (_glConfigs->CFG_FLD_VOTING_TYPE)
		{
		default:
		case 0:
			SumAllPredictionsDecision(predictionSumPerClass, nd->ClassesProbability, classesCount);
			return;
		case 1:
			WinnerTakesAllDecision(predictionSumPerClass, nd->ClassesProbability, classesCount);
			return;
		}			
	}

	CalculateTreeDecision(predictionSumPerClass, itemValues, rowIndex, nd, classesCount);
}

ConfMatrix* FrstCalculateOnData(const Forest* const forest, const LearnData* const table, const Data* const data)
{
	const uint rowsCount = data->RowsCount;
	const uint classesCount = table->Classes->VecBase.Size;
	if (rowsCount == 0 || classesCount < 2)
		return NULL;

	int* _malloc(sizeof(int) * rowsCount, actual);
	int* _malloc(sizeof(int) * rowsCount, predicted);
	double* _malloc(sizeof(double) * classesCount, predictionSumPerClass);
	for (uint i = 0; i < rowsCount; ++i)
	{
		actual[i] = data->ClassesColumn->Data[i];
		memset(predictionSumPerClass, 0, classesCount * sizeof(double));
		for (uint j = 0; j < forest->TreesCount; ++j)
		{
			CalculateTreeDecision(predictionSumPerClass, (const double**)data->Parameters, i, forest->Trees[j], classesCount);
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

	Node** _malloc(sizeof(Node*) * _glConfigs->TreeCount, trees);
	uint* countByClass = LrnCountByClass(table->TrainData.ClassesColumn, table->Classes->VecBase.Size);

	for (uint i = 0; i < _glConfigs->TreeCount; ++i)
	{
		trees[i] = NdGenerateTree(table->ParametersCount, &table->TrainData, countByClass, table->Classes->VecBase.Size);
	}
	free(countByClass);
	Forest* _malloc(sizeof(Forest), forest);
	forest->TreesCount = _glConfigs->TreeCount;
	forest->Trees = trees;
	return forest;
}

void FrstFree(Forest** const forest)
{
	if (*forest == NULL)
		return;
	Forest * help = *forest;
	for (uint i = 0; i < help->TreesCount; ++i)
	{
		TrFreeNode(&help->Trees[i]);
	}
	free(help->Trees);
	_FreeN(forest);
}
