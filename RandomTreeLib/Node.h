#ifndef Node_H_INCLUDE_GUARD
#define Node_H_INCLUDE_GUARD

#include <stdbool.h>
#include <stdlib.h>

typedef struct Node Node;
typedef struct Node
{
	Node* Left;
	Node* Right;
	int ParameterIndex;
	double ParameterValueSeparator;
	double Entropy;
	double* ClassesProbability;
} Node;

typedef struct
{
	Node* Left;
	Node* Right;
	int ParameterIndex;
	double ParameterValueSeparator;
} Root;

Node* TreeCreateLeaf(const double* classesProbability, const size_t size);

Root* TreeCreateRoot(const int parameterIndex, const double parameterValueSeparator, Node* left,
                   Node* right);

bool TreeIsLeaf(const Node* nd);

void TreeFreeNd(Node**const nd);

void TreeFree(Root**const nd);

#endif