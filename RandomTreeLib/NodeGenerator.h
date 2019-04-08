#ifndef NODE_GENERATOR_H_INCLUDE_GUARD
#define NODE_GENERATOR_H_INCLUDE_GUARD
#include "Node.h"
#include "RtConfigs.h"
#include "LearnData.h"
#include "DoubleVector.h"

bool CheckIfItIsLeftElem(const DoubleVector* const* const table, const uint rowIndex, const IntVector* const parameterIndexes, const DoubleVector* const separationValue, const
                         IntVector* const separationType);
bool CheckIfItIsLeftElem_T(const double* const* const table, const uint rowIndex, const IntVector* const parameterIndexes, const DoubleVector* const separationValue, const
                           IntVector* const separationType);
void NdSplitNode(Node* node, const uint parametersCount, const DoubleVector* const* const values, const IntVector* const classesColumn, const unsigned countByClass[], const size_t classCount, const uint deepness);
Tree* NdGenerateTree(const uint parametersCount, const ParameterColumn* values, const IntVector* classesColumn, const size_t rowsCount, const unsigned countByClass[], const size_t classCount);
#endif
