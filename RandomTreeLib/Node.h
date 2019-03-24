#ifndef Node_H_INCLUDE_GUARD
#define Node_H_INCLUDE_GUARD

#include <stdbool.h>

typedef struct Node Node;
typedef struct Node
{
	Node* Left;
	Node* Right;
	int ParameterIndex;
	double ParameterValueSeparator;
	bool MoreThanSeparator;
	int* ClassNumbers;
} Node;

Node* NdCreateLeaf(const int* classNumbers, const size_t size);

Node* NdCreateParent(const int parameterIndex, const double parameterValueSeparator, const bool moreThanSeparator,
                     const int* classNumbers, const size_t paramSize, Node* left, Node* right);

bool NdIsLeaf(const Node* nd);

void NdFree(Node* nd);

#endif