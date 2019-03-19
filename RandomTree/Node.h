#ifndef Node_H_INCLUDE_GUARD
#define Node_H_INCLUDE_GUARD

#include <stdbool.h>
struct Node
{
	struct Node* Left;
	struct Node* Right;
	int ParameterIndex;
	int ParameterValueSeparator;
	bool MoreThanSeparator;
	int ClassNumbers[2];
};

struct Node* CreateNodeLeaf(const int classNumbers[2]);

struct Node* CreateNodeParent(const int parameterIndex, const int parameterValueSeparator, const bool moreThanSeparator,
	const int classNumbers[2], struct Node* left, struct Node* right);

bool IsLeaf(struct Node* nd);

void DestroyNode(struct Node* nd);

#endif