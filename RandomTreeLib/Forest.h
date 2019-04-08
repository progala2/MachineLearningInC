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

ConfMatrix* FrstCalculateOnTestData(const Forest* const forest, const LearnData* const table);
ConfMatrix* FrstCalculateOnTrainingData(const Forest* const forest, const LearnData* const table);
Forest* FrstGenerateForest(const LearnData* const table);
void FrstFree(Forest**const forest);

#endif
