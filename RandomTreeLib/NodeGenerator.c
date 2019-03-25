#include "NodeGenerator.h"
#include "Node.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <processthreadsapi.h>
#include "RtConfigs.h"

double CalculateEntropy(const unsigned countByClass[], const size_t classCount, const size_t elemCount)
{
	double entropy = 0;
	for (size_t i = 0; i < classCount; ++i)
	{
		const double p = (double)countByClass[i] / elemCount;
#ifdef DEBUG
		printf("p: %f\n", p);
#endif
		entropy -= p == 0 ? 0 : p * log2(p);
	}
	return entropy;
}

unsigned* CountByClass(const size_t classesColumn[], const size_t size, const size_t classesCount)
{
	unsigned* countByClass = calloc(classesCount,sizeof(unsigned));

	for (size_t i = 0; i < size; ++i)
	{
		countByClass[classesColumn[i]]++;
	}
	return countByClass;
}

extern Root* NdGenerateTree(const RtConfigs* const configs, const int parameterIndex, const double values[], const size_t classesColumn[], const size_t size, const int classes[], const unsigned countByClass)
{
	srand(GetCurrentProcessId() + parameterIndex);
	const double entropy = CalculateEntropy(countByClass, 2, 10);

	double* v = malloc(sizeof(double)*configs->MaxFeaturesPerNode);
	for (int i = 0; i < configs->MaxFeaturesPerNode; ++i)
	{
		v[i] = values[rand() % size]; 
		printf("0: %f \n", v[i]);
	}

	printf("entropy: %f\n", entropy);
	double newEntropy = 2;
	size_t bestI = 0;

	double countByClassB1[2] = { 0, 0 };
	double countByClassB2[2] = { 0, 0 };
	for (int i = 0; i < 3; ++i)
	{
		size_t size1 = 0;
		size_t size2 = 0;
		size_t countByClass1[2] = { 0, 0 };
		size_t countByClass2[2] = { 0, 0 };
		for (size_t j = 0; j < size; ++j)
		{
			if (values[j] <= v[i])
			{
				size1++;
				countByClass1[classesColumn[j]]++;
			}
			else
			{
				size2++;
				countByClass2[classesColumn[j]]++;
			}
		}
		if (size1 == 0 || size2 == 0)
			continue;

		const double entropy1 = CalculateEntropy(countByClass1, 2, size1);
		const double entropy2 = CalculateEntropy(countByClass2, 2, size2);
		const double tmpEntropy = (size1*entropy1 + size2 * entropy2) / size;
		printf("new entropy: %f, e1: %f e2:%f\n", tmpEntropy, entropy1, entropy2);
		if (tmpEntropy < newEntropy)
		{
			newEntropy = tmpEntropy;
			bestI = i;
			countByClassB1[0] = (double)countByClass1[0]/countByClass[0];
			countByClassB1[1] = (double)countByClass1[1]/countByClass[1];
			countByClassB2[0] = (double)countByClass2[0]/countByClass[0];
			countByClassB2[1] = (double)countByClass2[1]/countByClass[1];
			
		printf("B10: %f, B11: %f B20:%f\n", countByClassB1[0], countByClassB1[1], countByClassB2[0]);
		}
	}

	Root* node = TreeCreateRoot(1, v[bestI], TreeCreateLeaf(countByClassB1, 2), TreeCreateLeaf(countByClassB2, 2));
	free(v);
	return node;
}

