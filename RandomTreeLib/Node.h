#ifndef NODE_H_INCLUDE_GUARD
#define NODE_H_INCLUDE_GUARD

#include <stdbool.h>
#include "utils.h"
#include "DoubleVector.h"

#define PARAMETER_SEPARATION_TYPES_NUMBER 2

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

typedef struct
{
	Node* Left;
	Node* Right;
	IntVector* ParameterIndexes;
	DoubleVector* ParameterValueSeparators;
	IntVector* ParameterSeparatorTypes;
} Tree;

Node* TrCreateLeaf(const double* classesProbability, const size_t size, const double entropy, const uint elemCount);

Tree* TrCreateRoot(IntVector* parameterIndex, DoubleVector* parameterValueSeparator, Node* left, Node* right);

bool TrIsLeaf(const Node*const nd);

bool TrIsFullLeaf(const Node*const nd);

void TrFreeNode(Node**const nd);

void TrFree(Tree**const nd);

#endif