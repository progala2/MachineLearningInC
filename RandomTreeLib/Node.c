#include <stdlib.h>
#include <stdbool.h>
#include "Node.h"
#include "RtConfigs.h"

Node* TrCreateLeaf(const double* classesProbability, const size_t size, const double entropy, const uint elemCount)
{
	if (classesProbability == NULL || size < 2 || entropy < 0)
		return NULL;

	Node* _malloc(sizeof(Node), nd)
	nd->ClassesProbability = MemCopyAlloc(classesProbability, sizeof(double) * size);
	nd->Left = NULL;
	nd->Right = NULL;
	nd->Entropy = entropy;
	nd->ElementsCount = elemCount;
	nd->ParameterIndexes = NULL;
	nd->ParameterValueSeparators = NULL;
	nd->ParameterSeparatorTypes = NULL;
	return nd;
}

Node* TrCreateRoot(IntVector* parameterIndex, IntVector* separatorTypes,
                   DoubleVector* parameterValueSeparator, Node* left, Node* right)
{
	if (left == right || left == NULL || right == NULL)
		return NULL;
	Node* _malloc(sizeof(Node), nd);	
	nd->Left = left;
	nd->Right = right;
	nd->ParameterIndexes = parameterIndex;
	nd->ParameterValueSeparators = parameterValueSeparator;
	nd->ParameterSeparatorTypes = separatorTypes;
	return  nd;
}

/**
 * \brief Check whether it has no children nodes.
 * \param nd Tree's Node.
 * \return True if has not children.
 */
bool TrIsLeaf(const Node*const nd)
{
	return nd->Left == nd->Right && nd->Right == NULL;
}

bool TrIsFullLeaf(const Node*const nd)
{
	return nd->Entropy == 1;
}

void TrFreeNode(Node**const nd)
{
	if (*nd == NULL)
		return;
	Node* hp = *nd;
	TrFreeNode(&hp->Left);
	TrFreeNode(&hp->Right);
	free(hp->ClassesProbability);
	IntVecFreeMemory(&hp->ParameterIndexes);
	IntVecFreeMemory(&hp->ParameterSeparatorTypes);
	DblVecFreeMemory(&hp->ParameterValueSeparators);
	_FreeN(nd);
}
