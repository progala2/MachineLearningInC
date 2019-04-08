#include "NodeGenerator.h"
#include "Node.h"
#include <stdlib.h>
#include <math.h>
#include "RtConfigs.h"
#include "DoubleVector.h"

void FreeDblVecTab(DoubleVector** tab, const size_t parametersCount)
{
	for (size_t i = 0; i < parametersCount; i++)
	{
		DblVecFreeMemory(&tab[i]);
	}
	free(tab);
}

double CalculateEntropy(const unsigned countByClass[], const size_t classCount, const size_t total)
{
	double entropy = 0;
	for (size_t i = 0; i < classCount; ++i)
	{
		const double p = (double)countByClass[i] / total;
		entropy -= p == 0 ? 0 : p * log2(p);
	}
	return entropy;
}

DoubleVector* CalculateSeparationValue(const DoubleVector* const* const table, const IntVector* const parameterIndexes, const size_t rowsCount)
{
	DoubleVector* retVector = DblVecInit_C(parameterIndexes->VecBase.Size + 1);
	const uint halfCount = rowsCount / 2;
	for (size_t i = 0; i < parameterIndexes->VecBase.Size; i++)
	{
		DblVecAppend(retVector, (table[parameterIndexes->Data[i]]->Data[rand() % halfCount] + table[parameterIndexes->Data[i]]->Data[halfCount + rand() % halfCount]) * 0.5);
	}
	return retVector;
}

double CalculateProbability(const uint selected, const uint total)
{
	return total == 0 ? 0 : (double)selected / total;
}

extern Node* NdGenerateTree(const uint parametersCount, const double** values, const IntVector * classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount)
{
	if (parametersCount < 1)
		return NULL;
	Node* _malloc(sizeof(Node), tempNode);
	tempNode->ClassesProbability = NULL;
	tempNode->Entropy = CalculateEntropy(countByClass, classCount, rowsCount);
	tempNode->ElementsCount = rowsCount;
	tempNode->Left = NULL;
	tempNode->Right = NULL;
	DoubleVector** _malloc(sizeof(DoubleVector*) * parametersCount, valuesVector);
	for (size_t i = 0; i < parametersCount; i++)
	{
		valuesVector[i] = DblVecInit();
		DblVecAppendRange(valuesVector[i], values[i], rowsCount);
	}

	do
	{
		NdSplitNode(tempNode, parametersCount, (const DoubleVector* const*)valuesVector, classesColumn, countByClass, classCount, 0); 	
	} while (TrIsLeaf(tempNode) == true);


	FreeDblVecTab(valuesVector, parametersCount);

	return tempNode;
}

static bool CheckIfItIsLeftForOneElem(const double elem, const double sep, const int type)
{
	switch (type)
	{
	default:
	case PARAMETER_SEPARATION_LESS_OR_EQ_TYPE:
		if (elem > sep)
			return false;
		break;
	case PARAMETER_SEPARATION_MORE_TYPE:
		if (elem <= sep)
			return false;
		break;
	}
	return true;
}

bool CheckIfItIsLeftElem(const DoubleVector * const* const table, const uint rowIndex, const IntVector * const parameterIndexes, const DoubleVector * const separationValue, const
	IntVector * const separationType)
{
	for (size_t i = 0; i < parameterIndexes->VecBase.Size; i++)
	{
		if (CheckIfItIsLeftForOneElem(table[parameterIndexes->Data[i]]->Data[rowIndex], separationValue->Data[i], separationType->Data[i]) == false)
			return false;
	}
	return true;
}

extern bool CheckIfItIsLeftElem_T(const double* const* const table, const uint rowIndex, const IntVector * const parameterIndexes, const DoubleVector * const separationValue, const
	IntVector * const separationType)
{
	for (size_t i = 0; i < parameterIndexes->VecBase.Size; i++)
	{
		if (CheckIfItIsLeftForOneElem(table[parameterIndexes->Data[i]][rowIndex], separationValue->Data[i], separationType->Data[i]) == false)
			return false;
	}
	return true;
}

IntVector* CalculateParameterIndexes(const uint parametersCount)
{
	IntVector* parameterIndexes = IntVecInit_C(_glConfigs->MaxFeaturesPerNode + 1);
	for (size_t i = 0; i < _glConfigs->MaxFeaturesPerNode; i++)
	{
		const uint val = rand() % parametersCount;
		if (!IntVecContains(parameterIndexes, val, NULL))
			IntVecAppend(parameterIndexes, val);
	}
	return parameterIndexes;
}

IntVector* CalculateParameterSeparatorTypes()
{
	IntVector* parameterSeparatorTypes = IntVecInit_C(_glConfigs->MaxFeaturesPerNode + 1);
	for (size_t i = 0; i < _glConfigs->MaxFeaturesPerNode; i++)
	{
		IntVecAppend(parameterSeparatorTypes, rand() % PARAMETER_SEPARATION_TYPES_NUMBER);
	}
	return parameterSeparatorTypes;
}

void NdSplitNode(Node * node, const uint parametersCount, const DoubleVector * const* const values, const IntVector * const classesColumn, const unsigned countByClass[], const size_t classCount, const uint deepness)
{
	if (node->ElementsCount < _glConfigs->MinSplitCount || deepness >= _glConfigs->MaxDeepness || node->Entropy == 0 || parametersCount < 1)
		return;

	const size_t rowsCount = classesColumn->VecBase.Size;
	IntVector * parameterIndexes = CalculateParameterIndexes(parametersCount);
	IntVector * parameterSeparatorTypes = CalculateParameterIndexes(parametersCount);
	DoubleVector * separatorValues = CalculateSeparationValue(values, parameterIndexes, rowsCount);

	DoubleVector * *_malloc_v(sizeof(DoubleVector*) * parametersCount, columnsLeft);
	DoubleVector * *_malloc_v(sizeof(DoubleVector*) * parametersCount, columnsRight);
	for (size_t i = 0; i < parametersCount; i++)
	{
		columnsLeft[i] = DblVecInit();
		columnsRight[i] = DblVecInit();
	}
	IntVector* NULLCHECK_V(IntVecInit(), classesLeft);
	IntVector* NULLCHECK_V(IntVecInit(), classesRight);
	double* _calloc_v(classCount, sizeof(double), probabilityLeft);
	double* _calloc_v(classCount, sizeof(double), probabilityRight);
	uint* _calloc_v(classCount, sizeof(uint), countByClassLeft);
	uint* _calloc_v(classCount, sizeof(uint), countByClassRight);

	memset(countByClassLeft, 0, sizeof(uint) * classCount);
	memset(countByClassRight, 0, sizeof(uint) * classCount);
	for (size_t j = 0; j < rowsCount; ++j)
	{
		if (CheckIfItIsLeftElem(values, j, parameterIndexes, separatorValues, parameterSeparatorTypes))
		{
			for (size_t i = 0; i < parametersCount; i++)
			{
				DblVecAppend(columnsLeft[i], values[i]->Data[j]);
			}
			IntVecAppend(classesLeft, classesColumn->Data[j]);
			countByClassLeft[classesColumn->Data[j]]++;
		}
		else
		{
			for (size_t i = 0; i < parametersCount; i++)
			{
				DblVecAppend(columnsRight[i], values[i]->Data[j]);
			}
			IntVecAppend(classesRight, classesColumn->Data[j]);
			countByClassRight[classesColumn->Data[j]]++;
		}
	}
	const uint leftCount = columnsLeft[0]->VecBase.Size, rightCount = columnsRight[0]->VecBase.Size;
	if (leftCount >= _glConfigs->MinElemsInLeaf && rightCount >= _glConfigs->MinElemsInLeaf)
	{
		const double entropyLeft = CalculateEntropy(countByClassLeft, classCount, leftCount);
		DBG_PRINT("e1: %f\n", entropyLeft);

		for (uint j = 0; j < classCount; ++j)
		{
			probabilityLeft[j] = CalculateProbability(countByClassLeft[j], countByClass[j]);
			probabilityRight[j] = CalculateProbability(countByClassRight[j], countByClass[j]);

			DBG_PRINT("Probability %d: B1: %f B2:%f\n", j, probabilityLeft[j], probabilityRight[j]);
		}

		node->ParameterIndexes = parameterIndexes;
		_FreeN(&node->ClassesProbability);
		node->ParameterValueSeparators = separatorValues;
		node->ParameterSeparatorTypes = parameterSeparatorTypes;

		node->Left = TrCreateLeaf(probabilityLeft, classCount, entropyLeft, leftCount);
		NdSplitNode(node->Left, parametersCount, (const DoubleVector*const*)columnsLeft, classesLeft, countByClassLeft, classCount, deepness + 1);
	}

	FreeDblVecTab(columnsLeft, parametersCount);
	IntVecFreeMemory(&classesLeft);

	if (leftCount >= _glConfigs->MinElemsInLeaf && rightCount >= _glConfigs->MinElemsInLeaf)
	{
		const double entropyRight = CalculateEntropy(countByClassRight, classCount, rightCount);
		DBG_PRINT("e2: %f\n", entropyRight);
		node->Right = TrCreateLeaf(probabilityRight, classCount, entropyRight, rightCount);
		NdSplitNode(node->Right, parametersCount, (const DoubleVector*const*)columnsRight, classesRight, countByClassRight, classCount, deepness + 1);
	}
	else
	{
		IntVecFreeMemory(&parameterIndexes);
		DblVecFreeMemory(&separatorValues);
	}

	FreeDblVecTab(columnsRight, parametersCount);
	IntVecFreeMemory(&classesRight);

	free(countByClassLeft);
	free(countByClassRight);
	free(probabilityLeft);
	free(probabilityRight);
}

