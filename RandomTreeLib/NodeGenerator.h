#ifndef NODE_GENERATOR_H_INCLUDE_GUARD
#define NODE_GENERATOR_H_INCLUDE_GUARD
#include "Node.h"
#include "RtConfigs.h"
#include "CsvTable.h"

Root* NdGenerateTree(const RtConfigs* const configs, const int parameterIndex, const double values[], const CsvClassTuple* classesColumn, const size_t size, const unsigned countByClass[], const size_t classCount);

Root** NdGenerateForest(const RtConfigs* const configs, const CsvTable* const table);

#endif
