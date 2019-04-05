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

double CalculateSeparationValue(const double* const column, const size_t rowsCount)
{
	const uint halfCount = rowsCount / 2;
	return (column[rand() % halfCount] + column[halfCount + rand() % halfCount])*0.5;
}

double CalculateProbability(const uint selected, const uint total)
{

	return total == 0 ? 0 : (double)selected / total;
}

Tree* NdGenerateTree(const uint parametersCount, const ParameterColumn* values, const IntVector* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount)
{
	if (parametersCount < 1)
		return NULL;
	Node tempNode = { 0 };
	tempNode.Entropy = CalculateEntropy(countByClass, classCount, rowsCount);
	tempNode.ElementsCount = rowsCount;
	DoubleVector** _malloc(sizeof(DoubleVector*) * parametersCount, valuesVector);
	for (size_t i = 0; i < parametersCount; i++)
	{
		valuesVector[i] = DblVecInit();
		DblVecAppendRange(valuesVector[i], values[i].Column, rowsCount);
	}

	NdSplitNode(&tempNode, parametersCount, valuesVector, classesColumn, countByClass, classCount, 0);
	
	FreeDblVecTab(valuesVector, parametersCount);
	
	Tree* node = TreeCreateRoot(tempNode.ParameterIndex, tempNode.ParameterValueSeparator, tempNode.Left, tempNode.Right);

	return node;
}

void NdSplitNode(Node* node, const uint parametersCount, const DoubleVector*const*const values, const IntVector*const classesColumn, const unsigned countByClass[], const size_t classCount, const uint deepness)
{
	if (node-> ElementsCount < 2 || deepness >= _glConfigs->MaxDeepness || node->Entropy == 0 || parametersCount < 1)
		return;
	const size_t rowsCount = classesColumn->VecBase.Size;
	const uint parameterIndex = rand() % parametersCount;
	const double separatorValue = CalculateSeparationValue(values[parameterIndex]->Data, rowsCount);

	DoubleVector** _malloc_v(sizeof(DoubleVector*) * parametersCount, columnsLeft);
	DoubleVector** _malloc_v(sizeof(DoubleVector*) * parametersCount, columnsRight);
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
		if (values[parameterIndex]->Data[j] <= separatorValue)
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

	const double entropyLeft = CalculateEntropy(countByClassLeft, classCount, columnsLeft[0]->VecBase.Size);
	const double entropyRight = CalculateEntropy(countByClassRight, classCount, columnsRight[0]->VecBase.Size);
	DBG_PRINT("e1: %f e2:%f\n", entropyLeft, entropyRight);

	for (uint j = 0; j < classCount; ++j)
	{
		probabilityLeft[j] = CalculateProbability(countByClassLeft[j], countByClass[j]);
		probabilityRight[j] = CalculateProbability(countByClassRight[j], countByClass[j]);

		DBG_PRINT("Probability %d: B1: %f B2:%f\n", j, probabilityLeft[j], probabilityRight[j]);
	}

	node->ParameterIndex = parameterIndex;
	_FreeN(&node->ClassesProbability);
	node->ParameterValueSeparator = separatorValue;
	
	node->Left = TreeCreateLeaf(probabilityLeft, classCount, entropyLeft, classesLeft->VecBase.Size);
	NdSplitNode(node->Left, parametersCount, columnsLeft, classesLeft, countByClassLeft, classCount, deepness + 1);
	FreeDblVecTab(columnsLeft, parametersCount);
	IntVecFreeMemory(&classesLeft);

	node->Right = TreeCreateLeaf(probabilityRight, classCount, entropyRight, classesRight->VecBase.Size);
	NdSplitNode(node->Right, parametersCount, columnsRight, classesRight, countByClassRight, classCount, deepness + 1);
	FreeDblVecTab(columnsRight, parametersCount);
	IntVecFreeMemory(&classesRight);
	
	free(countByClassLeft);
	free(countByClassRight);
	free(probabilityLeft);
	free(probabilityRight);
}

