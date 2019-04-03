#ifndef FOREST_H_GUARD
#define FOREST_H_GUARD
#include "Node.h"
#include "utils.h"
#include "ReadLearnData.h"
#include "ConfMatrix.h"

typedef struct
{
	Tree** Trees;
	uint TreesCount;
} Forest;

ConfMatrix* FrstCalculateOnTestData(const Forest* const forest, const LearnData* const table);
Forest* FrstGenerateForest(const LearnData* const table);
void FrstFree(Forest**const forest);

#endif
