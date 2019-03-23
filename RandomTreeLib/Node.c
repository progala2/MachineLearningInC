#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"

Node* CreateNodeLeaf(const int classNumbers[2])
{
	if (classNumbers == NULL)
		return NULL;
	Node* nd = malloc(sizeof(Node));
	nd->ClassNumbers[0] = classNumbers[0];
	nd->ClassNumbers[1] = classNumbers[1];
	nd->Left = NULL;
	nd->Right = NULL;
	return nd;
}

Node* CreateNodeParent(const int parameterIndex, const int parameterValueSeparator, const bool moreThanSeparator,
	const int classNumbers[2], Node* left, Node* right)
{
	if (left == right)
		return NULL;
	
	Node* nd = CreateNodeLeaf(classNumbers);
	nd->Left = left;
	nd->Right = right;
	nd->ParameterIndex = parameterIndex;
	nd->ParameterValueSeparator = parameterValueSeparator;
	nd->MoreThanSeparator = moreThanSeparator;
	return  nd;
}

bool IsLeaf(const Node* nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

void DestroyNode(Node* nd)
{
	if (nd != NULL)
	{
		DestroyNode(nd->Left);
		DestroyNode(nd->Right);
		free(nd);
	}
}