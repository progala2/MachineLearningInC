#ifndef NODE_GENERATOR_H_INCLUDE_GUARD
#define NODE_GENERATOR_H_INCLUDE_GUARD
#include "Node.h"
#include "RtConfigs.h"
#include "LearnData.h"

Tree* NdGenerateTree(const uint parametersCount, const ParameterColumn* values, const LrnClassTuple* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount);
#endif
