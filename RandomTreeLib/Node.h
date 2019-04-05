#ifndef Node_H_INCLUDE_GUARD
#define Node_H_INCLUDE_GUARD

#include <stdbool.h>
#include "utils.h"

typedef struct Node
{
	struct Node* Left;
	struct Node* Right;
	int ParameterIndex;
	double ParameterValueSeparator;
	double Entropy;
	double* ClassesProbability;
	uint ElementsCount;
} Node;

typedef struct
{
	Node* Left;
	Node* Right;
	int ParameterIndex;
	double ParameterValueSeparator;
} Tree;

Node* TreeCreateLeaf(const double* classesProbability, const size_t size, const double entropy, const uint elemCount);

Tree* TreeCreateRoot(const int parameterIndex, const double parameterValueSeparator, Node* left, Node* right);

bool TreeIsLeaf(const Node*const nd);

bool TreeIsFullLeaf(const Node*const nd);

void TreeFreeNd(Node**const nd);

void TrFree(Tree**const nd);

#endif