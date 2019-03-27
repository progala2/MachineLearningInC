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

		DbgPrint(("p: %f\n", p));
		entropy -= p == 0 ? 0 : p * log2(p);
	}
	return entropy;
}

unsigned* CountByClass(const CsvClassTuple* classesColumn, const size_t size, const size_t classesCount)
{
	uint* countByClass = calloc(classesCount, sizeof(uint));

	for (size_t i = 0; i < size; ++i)
	{
		countByClass[classesColumn[i].Value]++;
	}
	return countByClass;
}

extern Root* NdGenerateTree(const RtConfigs* const configs, const int parameterIndex, const double values[], const CsvClassTuple* classesColumn, const size_t size, const unsigned countByClass[], const size_t classCount)
{
	const double entropy = CalculateEntropy(countByClass, classCount, size);

	double* v = malloc(sizeof(double)*configs->MaxFeaturesPerNode);
	for (uint i = 0; i < configs->MaxFeaturesPerNode; ++i)
	{
		v[i] = values[rand() % size];
		DbgPrint(("0: %f \n", v[i]));
	}

	DbgPrint(("entropy: %f\n", entropy));
	double newEntropy = entropy + 1;
	size_t bestI = 0;

	double* probabilityB1 = calloc(classCount, sizeof(double));
	double* probabilityB2 = calloc(classCount, sizeof(double));

	uint* countByClass1 = calloc(classCount, sizeof(uint));
	uint* countByClass2 = calloc(classCount, sizeof(uint));
	double entropyB1 = 0;
	double entropyB2 = 0;
	for (uint i = 0; i < configs->MaxFeaturesPerNode; ++i)
	{
		size_t size1 = 0;
		size_t size2 = 0;
		memset(countByClass1, 0, sizeof(uint)*classCount);
		memset(countByClass2, 0, sizeof(uint)*classCount);
		for (size_t j = 0; j < size; ++j)
		{
			if (values[j] <= v[i])
			{
				size1++;
				countByClass1[classesColumn[j].Value]++;
			}
			else
			{
				size2++;
				countByClass2[classesColumn[j].Value]++;
			}
		}
		if (size1 == 0 || size2 == 0)
			continue;

		const double entropy1 = CalculateEntropy(countByClass1, classCount, size1);
		const double entropy2 = CalculateEntropy(countByClass2, classCount, size2);
		const double tmpEntropy = (size1*entropy1 + size2 * entropy2) / size;
		DbgPrint(("new entropy: %f, e1: %f e2:%f\n", tmpEntropy, entropy1, entropy2));
		if (tmpEntropy < newEntropy)
		{
			entropyB1 = entropy1;
			entropyB2 = entropy2;
			newEntropy = tmpEntropy;
			bestI = i;
			for (uint j = 0; j < classCount; ++j)
			{
				probabilityB1[j] = (double)countByClass1[j] / countByClass[j];
				probabilityB2[j] = (double)countByClass2[j] / countByClass[j];

				DbgPrint(("Probability %d: B1: %f B2:%f\n", j, probabilityB1[j], probabilityB2[j]));
			}
		}
	}

	Root* node = TreeCreateRoot(parameterIndex, v[bestI], TreeCreateLeaf(probabilityB1, classCount, entropyB1), TreeCreateLeaf(probabilityB2, classCount, entropyB2));
	free(countByClass1);
	free(countByClass2);
	free(probabilityB1);
	free(probabilityB2);
	free(v);
	return node;
}

extern Root** NdGenerateForest(const RtConfigs* const configs, const CsvTable* const table)
{
	Root** forest = malloc(sizeof(Root*) * configs->TreeCount);
	uint* countByClass = CountByClass(table->ClassesColumn, table->RowsCount, table->Classes->VecBase.Size);
	for (uint i = 0; i < configs->TreeCount; ++i)
	{
		const uint paramIndex = rand() % table->ParametersCount;
		forest[i] = NdGenerateTree(configs, paramIndex, table->Parameters[paramIndex].Column, table->ClassesColumn, table->RowsCount, countByClass, table->Classes->VecBase.Size);
	}
	free(countByClass);
	return forest;
}

