#include "NodeGenerator.h"
#include "Node.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>


double CalculateEntropy(const size_t countByClass[], const size_t classCount, const size_t elemCount)
{
	double entropy = 0;
	for (int i = 0; i < classCount; ++i)
	{
		const double p = (double)countByClass[i] / elemCount;
		printf("p: %f\n", p);
		entropy -= p == 0 ? 0 : p * log2(p);
	}
	return entropy;
}

//int* CountByClass()

extern Node* NdGenerateTree()
{
	const int parameterIndex = 0;
	double values[10] = { 1, 2, 3, 1, 2, 3, 1, 2, 3, 1 };
	const size_t size = 10;
	int classes[10] = { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1 };
	size_t countByClass[2] = { 0, 0 };

	for (int i = 0; i < size; ++i)
	{
		countByClass[classes[i]]++;
	}

	time_t t;
	srand((unsigned)time(&t));
	double entropy = CalculateEntropy(countByClass, 2, 10);

	double v[3];
	v[0] = values[rand() % 10];
	v[1] = values[rand() % 10];
	v[2] = values[rand() % 10];

	printf("0: %f, 1: %f 2: %f entropy: %f\n", v[0], v[1], v[2], entropy);
	double newEntropy = 2;
	size_t besti = 0;
	for (int i = 0; i < 3; ++i)
	{
		size_t size1 = 0;
		size_t size2 = 0;
		size_t countByClass1[2] = { 0, 0 };
		size_t countByClass2[2] = { 0, 0 };
		for (int j = 0; j < size; ++j)
		{
			if (values[j] <= v[i])
			{
				size1++;
				countByClass1[classes[j]]++;
			}
			else
			{
				size2++;
				countByClass2[classes[j]]++;
			}
		}
		if (size1 == 0 || size2 == 0)
			continue;

		const double entropy1 = CalculateEntropy(countByClass1, 2, size1);
		const double entropy2 = CalculateEntropy(countByClass2, 2, size2);
		double tmpEntropy = (size1*entropy1 + size2*entropy2)/size;
		printf("new entropy: %f, e1: %f e2:%f\n", tmpEntropy, entropy1, entropy2);
		if (tmpEntropy < newEntropy)
		{
			newEntropy = tmpEntropy;
			besti = i;
		}
	}

	Node* node = NdCreateParent(1, v[besti], countByClass, 2, NdCreateLeaf(countByClass, 2), NdCreateLeaf(countByClass, 2));
	return node;
}

