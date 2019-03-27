#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"
#include "RtConfigs.h"

Node* TreeCreateLeaf(const double* classesProbability, const size_t size, const double entropy)
{
	if (classesProbability == NULL || size < 2 || entropy < 0)
		return NULL;

	Node* nd = malloc(sizeof(Node));
	nd->ClassesProbability = MemCopyAlloc(classesProbability, sizeof(double) * size);
	nd->Left = NULL;
	nd->Right = NULL;
	nd->Entropy = entropy;
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

bool TreeIsWeakLeaf(const Node*const nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

bool TreeIsLeaf(const Node*const nd)
{
	return nd->Entropy == 1;
}

void TreeFreeNd(Node**const nd)
{
	if (*nd == NULL)
		return;
	TreeFreeNd(&(*nd)->Left);
	TreeFreeNd(&(*nd)->Right);
	free((*nd)->ClassesProbability);
	free(*nd);
	*nd = NULL;
}

void TreeFree(Root** const nd)
{
	if (*nd == NULL)
		return;
	TreeFreeNd(&(*nd)->Left);
	TreeFreeNd(&(*nd)->Right);
	free(*nd);
	*nd = NULL;
}
