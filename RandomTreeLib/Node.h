#ifndef Node_H_INCLUDE_GUARD
#define Node_H_INCLUDE_GUARD

#include <stdbool.h>

typedef struct Node Node;
typedef struct Node
{
	Node* Left;
	Node* Right;
	int ParameterIndex;
	int ParameterValueSeparator;
	bool MoreThanSeparator;
	int ClassNumbers[2];
} Node;

Node* CreateNodeLeaf(const int classNumbers[2]);

Node* CreateNodeParent(const int parameterIndex, const int parameterValueSeparator, const bool moreThanSeparator,
	const int classNumbers[2], Node* left, Node* right);

bool IsLeaf(const Node* nd);

void DestroyNode(Node* nd);

#endif