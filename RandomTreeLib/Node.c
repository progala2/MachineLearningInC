#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"
#include "RtConfigs.h"

Node* TreeCreateLeaf(const double* classesProbability, const size_t size)
{
	if (classesProbability == NULL)
		return NULL;
	if (size < 2)
		return NULL;
	Node* nd = malloc(sizeof(Node));
	nd->ClassesProbability = MemCopyAlloc(classesProbability, sizeof(double) * size);
	nd->Left = NULL;
	nd->Right = NULL;
	return nd;
}

Root* TreeCreateRoot(const int parameterIndex, const double parameterValueSeparator,
                   Node* left, Node* right)
{
	if (left == right || left == NULL || right == NULL)
		return NULL;
	Root* nd = malloc(sizeof(Root));	
	nd->Left = left;
	nd->Right = right;
	nd->ParameterIndex = parameterIndex;
	nd->ParameterValueSeparator = parameterValueSeparator;
	return  nd;
}

bool TreeIsLeaf(const Node* nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

void TreeFreeNd(Node**const nd)
{
	TreeFreeNd(&(*nd)->Left);
	TreeFreeNd(&(*nd)->Right);
	free((*nd)->ClassesProbability);
	free(*nd);
	*nd = NULL;
}

void TreeFree(Root** const nd)
{
	TreeFreeNd(&(*nd)->Left);
	TreeFreeNd(&(*nd)->Right);
	free(*nd);
	*nd = NULL;
}
