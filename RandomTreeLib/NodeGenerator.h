#ifndef NODE_GENERATOR_H_INCLUDE_GUARD
#define NODE_GENERATOR_H_INCLUDE_GUARD
#include "Node.h"
#include "RtConfigs.h"
#include "LearnData.h"
#include "ConfMatrix.h"

Root* NdGenerateTree(const RtConfigs* const configs, const int parameterIndex, const double values[], const LrnClassTuple* classesColumn, const size_t size, const unsigned countByClass[], const size_t classCount);

Root** NdGenerateForest(const RtConfigs* const configs, const LearnData* const table);

ConfMatrix* NdCalculateOnTestData(const Root*const* const forest, const LearnData* const table, const size_t treeCount);
#endif
