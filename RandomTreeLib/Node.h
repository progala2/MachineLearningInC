#ifndef Node_H_INCLUDE_GUARD
#define Node_H_INCLUDE_GUARD

#include <stdbool.h>
#include "utils.h"
#include "DoubleVector.h"

typedef struct Node
{
	struct Node* Left;
	struct Node* Right;
	IntVector* ParameterIndexes;
	DoubleVector* ParameterValueSeparators;
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
} Tree;

Node* TreeCreateLeaf(const double* classesProbability, const size_t size, const double entropy, const uint elemCount);

Tree* TreeCreateRoot(IntVector* parameterIndex, DoubleVector* parameterValueSeparator, Node* left, Node* right);

bool TreeIsLeaf(const Node*const nd);

bool TreeIsFullLeaf(const Node*const nd);

void TreeFreeNd(Node**const nd);

void TrFree(Tree**const nd);

#endif