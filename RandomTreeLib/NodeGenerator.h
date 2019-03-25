#ifndef NODE_GENERATOR_H_INCLUDE_GUARD
#define NODE_GENERATOR_H_INCLUDE_GUARD
#include "Node.h"
#include "RtConfigs.h"

Root* NdGenerateTree(const RtConfigs* const configs, const int parameterIndex, const double values[], const size_t classesColumn[], const size_t size, const int classes[], const unsigned countByClass);

#endif
