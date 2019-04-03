#include "NodeGenerator.h"
#include "Node.h"
#include <stdlib.h>
#include <math.h>
#include "RtConfigs.h"

double CalculateEntropy(const unsigned countByClass[], const size_t classCount, const size_t total)
{
	double entropy = 0;
	for (size_t i = 0; i < classCount; ++i)
	{
		const double p = (double)countByClass[i] / total;

		DBG_PRINT("p: %f\n", p)
			entropy -= p == 0 ? 0 : p * log2(p);
	}
	return entropy;
}

double CalculateSeparationValue(const double* const column, const size_t rowsCount)
{
	const uint halfCount = rowsCount/2;
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

	double* probabilityBLeft = calloc(classCount, sizeof(double));
	double* probabilityBRight = calloc(classCount, sizeof(double));

	uint* countByClassLeft = calloc(classCount, sizeof(uint));
	uint* countByClassRight = calloc(classCount, sizeof(uint));
	int deepness = 0;
	size_t size1 = 0;
	size_t size2 = 0;
	memset(countByClassLeft, 0, sizeof(uint)*classCount);
	memset(countByClassRight, 0, sizeof(uint)*classCount);
	for (size_t j = 0; j < rowsCount; ++j)
	{
		if (values[parameterIndex].Column[j] <= v)
		{
			size1++;
			countByClassLeft[classesColumn[j].Value]++;
		}
		else
		{
			size2++;
			countByClassRight[classesColumn[j].Value]++;
		}
	}

	const double entropy1 = CalculateEntropy(countByClassLeft, classCount, size1);
	const double entropy2 = CalculateEntropy(countByClassRight, classCount, size2);
	DBG_PRINT("e1: %f e2:%f\n", entropy1, entropy2);

	for (uint j = 0; j < classCount; ++j)
	{
		probabilityBLeft[j] = CalculateProbability(countByClassLeft[j], countByClass[j]);
		probabilityBRight[j] = CalculateProbability(countByClassRight[j], countByClass[j]);

		DBG_PRINT("Probability %d: B1: %f B2:%f\n", j, probabilityBLeft[j], probabilityBRight[j]);
	}
	
	Tree* node = TreeCreateRoot(parameterIndex, v, TreeCreateLeaf(probabilityBLeft, classCount, entropy1), TreeCreateLeaf(probabilityBRight, classCount, entropy2));
	//if (++deepness < configs->MaxDeepness &&)
	
	free(countByClassLeft);
	free(countByClassRight);
	free(probabilityBLeft);
	free(probabilityBRight);
	return node;
}

void NdSplitNode(Node* node, const uint parametersCount, const double*const* values, const LrnClassTuple* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount, const uint deepness)
{
	const uint parameterIndex = rand() % parametersCount;
	const double entropy = node->Entropy;

	double* v = malloc(sizeof(double)*_glConfigs->MaxFeaturesPerNode);
	for (uint i = 0; i < _glConfigs->MaxFeaturesPerNode; ++i)
	{
		v[i] = values[parameterIndex][rand() % rowsCount];

	}

	double newEntropy = entropy + 1;
	size_t bestI = 0;

	double* probabilityB1 = calloc(classCount, sizeof(double));
	double* probabilityB2 = calloc(classCount, sizeof(double));

	uint* countByClass1 = calloc(classCount, sizeof(uint));
	uint* countByClassRight = calloc(classCount, sizeof(uint));
	double entropyB1 = 0;
	double entropyB2 = 0;
	for (uint i = 0; i < _glConfigs->MaxFeaturesPerNode; ++i)
	{
		size_t size1 = 0;
		size_t size2 = 0;
		memset(countByClass1, 0, sizeof(uint)*classCount);
		memset(countByClassRight, 0, sizeof(uint)*classCount);
		for (size_t j = 0; j < rowsCount; ++j)
		{
			if (values[parameterIndex][j] <= v[i])
			{
				size1++;
				countByClass1[classesColumn[j].Value]++;
			}
			else
			{
				size2++;
				countByClassRight[classesColumn[j].Value]++;
			}
		}
		if (size1 == 0 || size2 == 0)
			continue;

		const double entropy1 = CalculateEntropy(countByClass1, classCount, size1);
		const double entropy2 = CalculateEntropy(countByClassRight, classCount, size2);
		const double tmpEntropy = (size1*entropy1 + size2 * entropy2) / rowsCount;
		if (tmpEntropy < newEntropy)
		{
			entropyB1 = entropy1;
			entropyB2 = entropy2;
			newEntropy = tmpEntropy;
			bestI = i;
			for (uint j = 0; j < classCount; ++j)
			{
				probabilityB1[j] = (double)countByClass1[j] / countByClass[j];
				probabilityB2[j] = (double)countByClassRight[j] / countByClass[j];
			}
		}
	}

	node->ParameterIndex = parameterIndex;
	_FreeN(&node->ClassesProbability);
	node->ParameterValueSeparator = v[bestI];
	node->Left = TreeCreateLeaf(probabilityB1, classCount, entropyB1);
	node->Right = TreeCreateLeaf(probabilityB2, classCount, entropyB2);
	free(countByClass1);
	free(countByClassRight);
	free(probabilityB1);
	free(probabilityB2);
	free(v);
}

