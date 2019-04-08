#ifndef NODE_H_INCLUDE_GUARD
#define NODE_H_INCLUDE_GUARD

#include <stdbool.h>
#include "utils.h"
#include "DoubleVector.h"

#define PARAMETER_SEPARATION_TYPES_NUMBER 2
#define PARAMETER_SEPARATION_LESS_OR_EQ_TYPE 0
#define PARAMETER_SEPARATION_MORE_TYPE 1

typedef struct Node
{
	struct Node* Left;
	struct Node* Right;
	IntVector* ParameterIndexes;
	DoubleVector* ParameterValueSeparators;
	IntVector* ParameterSeparatorTypes;
	double Entropy;
	double* ClassesProbability;
	uint ElementsCount;
} Node;

Node* TrCreateLeaf(const double* classesProbability, const size_t size, const double entropy, const uint elemCount);

Node* TrCreateRoot(IntVector* parameterIndex, IntVector* separatorTypes, DoubleVector* parameterValueSeparator, Node* left, Node* right);

bool TrIsLeaf(const Node*const nd);

bool TrIsFullLeaf(const Node*const nd);

void TrFreeNode(Node**const nd);

#endif