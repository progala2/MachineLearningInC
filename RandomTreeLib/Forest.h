#ifndef FOREST_H_GUARD
#define FOREST_H_GUARD
#include "Node.h"
#include "ReadLearnData.h"
#include "ConfMatrix.h"

typedef struct
{
	Node** Trees;
	uint TreesCount;
} Forest;

ConfMatrix* FrstCalculateOnData(const Forest* const forest, const LearnData* const table, const Data* const data);
Forest* FrstGenerateForest(const Data* const table, const size_t parametersCount, const size_t classesCount);
void FrstFree(Forest**const forest);

#endif
