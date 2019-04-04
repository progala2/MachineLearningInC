#include "NodeGenerator.h"
#include "Node.h"
#include <stdlib.h>
#include <math.h>
#include "RtConfigs.h"
#include "DoubleVector.h"

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
	return (double)selected / total;
}

Tree* NdGenerateTree(const uint parametersCount, const ParameterColumn* values, const LrnClassTuple* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount)
{
	const uint parameterIndex = rand() % parametersCount;
	const double entropy = CalculateEntropy(countByClass, classCount, rowsCount);

	const double v = CalculateSeparationValue(values[parameterIndex].Column, rowsCount);

	DBG_PRINT("entropy: %f\n", entropy);

	DoubleVector* _malloc(sizeof(DoubleVector) * parametersCount, columnsLeft);
	DoubleVector* _malloc(sizeof(DoubleVector) * parametersCount, columnsRight);
	//TODO INT VECTOR
	int* _malloc(sizeof(int) * rowsCount, classesLeft);
	int* _malloc(sizeof(int) * rowsCount, classesRight);
	double* _calloc(classCount, sizeof(double), probabilityLeft);
	double* _calloc(classCount, sizeof(double), probabilityRight);
	uint* _calloc(classCount, sizeof(uint), countByClassLeft);
	uint* _calloc(classCount, sizeof(uint), countByClassRight);

	memset(countByClassLeft, 0, sizeof(uint)*classCount);
	memset(countByClassRight, 0, sizeof(uint)*classCount);
	for (size_t j = 0; j < rowsCount; ++j)
	{
		if (values[parameterIndex].Column[j] <= v)
		{
			for (size_t i = 0; i < parametersCount; i++)
			{
				DblVecAppend(&columnsLeft[i], values[i].Column[j]);
			}
			countByClassLeft[classesColumn[j].Value]++;
		}
		else
		{
			for (size_t i = 0; i < parametersCount; i++)
			{
				DblVecAppend(&columnsRight[i], values[i].Column[j]);
			}
			countByClassRight[classesColumn[j].Value]++;
		}
	}

	const double entropy1 = CalculateEntropy(countByClassLeft, classCount, columnsLeft[0].VecBase.Size);
	const double entropy2 = CalculateEntropy(countByClassRight, classCount, columnsRight[0].VecBase.Size);
	DBG_PRINT("e1: %f e2:%f\n", entropy1, entropy2);

	for (uint j = 0; j < classCount; ++j)
	{
		probabilityLeft[j] = CalculateProbability(countByClassLeft[j], countByClass[j]);
		probabilityRight[j] = CalculateProbability(countByClassRight[j], countByClass[j]);

		DBG_PRINT("Probability %d: B1: %f B2:%f\n", j, probabilityLeft[j], probabilityRight[j]);
	}

	Tree* node = TreeCreateRoot(parameterIndex, v, TreeCreateLeaf(probabilityLeft, classCount, entropy1), TreeCreateLeaf(probabilityRight, classCount, entropy2));
	NdSplitNode(node->Left, parametersCount, columnsLeft, NULL, columnsLeft[0].VecBase.Size, LrnCountByClass(classesColumn, rowsCount, classCount), classCount, 1);
	//if (++deepness < configs->MaxDeepness &&)

	free(countByClassLeft);
	free(countByClassRight);
	free(probabilityLeft);
	free(probabilityRight);
	return node;
}

void NdSplitNode(Node* node, const uint parametersCount, const DoubleVector*const values, const int*const classesColumn, const size_t currentRowsCount, const unsigned countByClass[], const size_t classCount, const uint deepness)
{
	if (node-> ElementsCount < 2 || deepness >= _glConfigs->MaxDeepness || node->Entropy == 0)
		return;

	const uint parameterIndex = rand() % parametersCount;
	const double separatorValue = CalculateSeparationValue(values[parameterIndex].Data, currentRowsCount);

	double* _calloc(classCount, sizeof(double), probabilityLeft);
	double* _calloc(classCount, sizeof(double), probabilityRight);
	uint* _calloc(classCount, sizeof(uint), countByClassLeft);
	uint* _calloc(classCount, sizeof(uint), countByClassRight);

	size_t sizeLeft = 0;
	size_t sizeRight = 0;
	memset(countByClassLeft, 0, sizeof(uint)*classCount);
	memset(countByClassRight, 0, sizeof(uint)*classCount);
	for (size_t j = 0; j < currentRowsCount; ++j)
	{
		if (values[parameterIndex].Data[j] <= separatorValue)
		{
			sizeLeft++;
			countByClassLeft[classesColumn[j]]++;
		}
		else
		{
			sizeRight++;
			countByClassRight[classesColumn[j]]++;
		}
	}
	const double entropyLeft = CalculateEntropy(countByClassLeft, classCount, sizeLeft);
	const double entropyRight = CalculateEntropy(countByClassRight, classCount, sizeRight);
	for (uint j = 0; j < classCount; ++j)
	{
		probabilityLeft[j] = CalculateProbability(countByClassLeft[j], countByClass[j]);
		probabilityRight[j] = CalculateProbability(countByClassRight[j], countByClass[j]);
	}

	node->ParameterIndex = parameterIndex;
	_FreeN(&node->ClassesProbability);
	node->ParameterValueSeparator = separatorValue;
	node->Left = TreeCreateLeaf(probabilityLeft, classCount, entropyLeft);
	node->Right = TreeCreateLeaf(probabilityRight, classCount, entropyRight);
	free(countByClassLeft);
	free(countByClassRight);
	free(probabilityLeft);
	free(probabilityRight);
}

