#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"
#include "RtConfigs.h"

Node* TreeCreateLeaf(const double* classesProbability, const size_t size, const double entropy, const uint elemCount)
{
	if (classesProbability == NULL || size < 2 || entropy < 0)
		return NULL;

	Node* _malloc(sizeof(Node), nd);
	nd->ClassesProbability = MemCopyAlloc(classesProbability, sizeof(double) * size);
	nd->Left = NULL;
	nd->Right = NULL;
	nd->Entropy = entropy;
	nd->ElementsCount = elemCount;
	nd->ParameterIndexes = NULL;
	nd->ParameterValueSeparators = NULL;
	return nd;
}

Tree* TreeCreateRoot(IntVector* parameterIndex, DoubleVector* parameterValueSeparator,
                   Node* left, Node* right)
{
	if (left == right || left == NULL || right == NULL)
		return NULL;
	Tree* _malloc(sizeof(Tree), nd);	
	nd->Left = left;
	nd->Right = right;
	nd->ParameterIndexes = parameterIndex;
	nd->ParameterValueSeparators = parameterValueSeparator;
	return  nd;
}

/**
 * \brief Check whether it has no children nodes.
 * \param nd Tree's Node.
 * \return True if has not children.
 */
bool TreeIsLeaf(const Node*const nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

bool TreeIsFullLeaf(const Node*const nd)
{
	return nd->Entropy == 1;
}

void TreeFreeNd(Node**const nd)
{
	if (*nd == NULL)
		return;
	Node* hp = *nd;
	TreeFreeNd(&hp->Left);
	TreeFreeNd(&hp->Right);
	free(hp->ClassesProbability);
	IntVecFreeMemory(&hp->ParameterIndexes);
	DblVecFreeMemory(&hp->ParameterValueSeparators);
	_FreeN(nd);
}

void TrFree(Tree** const nd)
{
	if (*nd == NULL)
		return;
	Tree* hp = *nd;
	TreeFreeNd(&hp->Left);
	TreeFreeNd(&hp->Right);
	IntVecFreeMemory(&hp->ParameterIndexes);
	DblVecFreeMemory(&hp->ParameterValueSeparators);
	_FreeN(nd);
}
