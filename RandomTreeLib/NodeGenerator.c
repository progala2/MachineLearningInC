#include "NodeGenerator.h"
#include "Node.h"
#include <stddef.h>

Node* NdGenerateTree()
{
	const int parameterIndex = 0;
	double* values;
	double* classes;
	double* validationVal;
	double* ValidationClasses;

	Node* node = NdCreateParent(1, valueSeparator, true, classNumber, 2 , NdCreateLeaf(classNumber, 2), NdCreateLeaf(classNumber, 2))
	return node;
}
