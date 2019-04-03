#include "NodeGenerator.h"
#include "Node.h"
#include <stdlib.h>
#include <math.h>
#include "RtConfigs.h"

double CalculateEntropy(const unsigned countByClass[], const size_t classCount, const size_t elemCount)
{
	double entropy = 0;
	for (size_t i = 0; i < classCount; ++i)
	{
		const double p = (double)countByClass[i] / elemCount;

		DBG_PRINT(("p: %f\n", p))
		entropy -= p == 0 ? 0 : p * log2(p);
	}
	return entropy;
}

Tree* NdGenerateTree(const RtConfigs* const configs, const uint parametersCount, const ParameterColumn* values, const LrnClassTuple* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount)
{
	const uint parameterIndex = rand() % parametersCount;
	const double entropy = CalculateEntropy(countByClass, classCount, rowsCount);

	double* v = malloc(sizeof(double)*configs->MaxFeaturesPerNode);
	for (uint i = 0; i < configs->MaxFeaturesPerNode; ++i)
	{
		v[i] = values[parameterIndex].Column[rand() % rowsCount];
		DBG_PRINT(("0: %f \n", v[i]));
	}

	DBG_PRINT(("entropy: %f\n", entropy));
	double newEntropy = entropy + 1;
	size_t bestI = 0;

	double* probabilityB1 = calloc(classCount, sizeof(double));
	double* probabilityB2 = calloc(classCount, sizeof(double));

	uint* countByClass1 = calloc(classCount, sizeof(uint));
	uint* countByClassRight = calloc(classCount, sizeof(uint));
	double entropyB1 = 0;
	double entropyB2 = 0;
	int deepness = 1;
	for (uint i = 0; i < configs->MaxFeaturesPerNode; ++i)
	{
		size_t size1 = 0;
		size_t size2 = 0;
		memset(countByClass1, 0, sizeof(uint)*classCount);
		memset(countByClassRight, 0, sizeof(uint)*classCount);
		for (size_t j = 0; j < rowsCount; ++j)
		{
			if (values[parameterIndex].Column[j] <= v[i])
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
		DBG_PRINT(("new entropy: %f, e1: %f e2:%f\n", tmpEntropy, entropy1, entropy2));
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

				DBG_PRINT(("Probability %d: B1: %f B2:%f\n", j, probabilityB1[j], probabilityB2[j]));
			}
		}
	}

	Tree* node = TreeCreateRoot(parameterIndex, v[bestI], TreeCreateLeaf(probabilityB1, classCount, entropyB1), TreeCreateLeaf(probabilityB2, classCount, entropyB2));
	free(countByClass1);
	free(countByClassRight);
	free(probabilityB1);
	free(probabilityB2);
	free(v);
	return node;
}
void NdSplitNode(Node* node, const RtConfigs* const configs, const uint parametersCount, const double*const* values, const LrnClassTuple* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount)
{
	const uint parameterIndex = rand() % parametersCount;
	const double entropy = node->Entropy;

	double* v = malloc(sizeof(double)*configs->MaxFeaturesPerNode);
	for (uint i = 0; i < configs->MaxFeaturesPerNode; ++i)
	{
		v[i] = values[parameterIndex][rand() % rowsCount];
		DBG_PRINT(("0: %f \n", v[i]));
	}

	DBG_PRINT(("entropy: %f\n", entropy));
	double newEntropy = entropy + 1;
	size_t bestI = 0;

	double* probabilityB1 = calloc(classCount, sizeof(double));
	double* probabilityB2 = calloc(classCount, sizeof(double));

	uint* countByClass1 = calloc(classCount, sizeof(uint));
	uint* countByClassRight = calloc(classCount, sizeof(uint));
	double entropyB1 = 0;
	double entropyB2 = 0;
	int deepness = 1;
	for (uint i = 0; i < configs->MaxFeaturesPerNode; ++i)
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
		DBG_PRINT(("new entropy: %f, e1: %f e2:%f\n", tmpEntropy, entropy1, entropy2));
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

				DBG_PRINT(("Probability %d: B1: %f B2:%f\n", j, probabilityB1[j], probabilityB2[j]));
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

