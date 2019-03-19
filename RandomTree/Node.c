#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"

struct Node* CreateNodeLeaf(const int classNumbers[2])
{
	if (classNumbers == NULL)
		return NULL;
	struct Node* nd = malloc(sizeof(struct Node));
	nd->ClassNumbers[0] = classNumbers[0];
	nd->ClassNumbers[1] = classNumbers[1];
	nd->Left = NULL;
	nd->Right = NULL;
	return nd;
}

struct Node* CreateNodeParent(const int parameterIndex, const int parameterValueSeparator, const bool moreThanSeparator,
	const int classNumbers[2], struct Node* left, struct Node* right)
{
	if (left == right)
		return NULL;
	
	struct Node* nd = CreateNodeLeaf(classNumbers);
	nd->Left = left;
	nd->Right = right;
	nd->ParameterIndex = parameterIndex;
	nd->ParameterValueSeparator = parameterValueSeparator;
	nd->MoreThanSeparator = moreThanSeparator;
	return  nd;
}

bool IsLeaf(struct Node* nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

void DestroyNode(struct Node* nd)
{
	if (nd != NULL)
	{
		DestroyNode(nd->Left);
		DestroyNode(nd->Right);
		free(nd);
	}
}