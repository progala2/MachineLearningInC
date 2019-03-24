#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"
#include "RtConfigs.h"

Node* NdCreateLeaf(const int* classNumbers, const size_t size)
{
	if (classNumbers == NULL)
		return NULL;
	if (size < 2)
		return NULL;
	Node* nd = malloc(sizeof(Node));
	nd->ClassNumbers = MemCopyAlloc(classNumbers, size);
	nd->Left = NULL;
	nd->Right = NULL;
	return nd;
}

Node* NdCreateParent(const int parameterIndex, const double parameterValueSeparator, const bool moreThanSeparator,
                     const int* classNumbers, const size_t paramSize, Node* left, Node* right)
{
	if (left == right)
		return NULL;

	Node* nd = NdCreateLeaf(classNumbers, paramSize);
	nd->Left = left;
	nd->Right = right;
	nd->ParameterIndex = parameterIndex;
	nd->ParameterValueSeparator = parameterValueSeparator;
	nd->MoreThanSeparator = moreThanSeparator;
	return  nd;
}

bool NdIsLeaf(const Node* nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

void NdFree(Node* nd)
{
	NdFree(nd->Left);
	NdFree(nd->Right);
	free(nd->ClassNumbers);
	free(nd);
}