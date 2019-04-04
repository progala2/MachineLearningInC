#include "DoubleVector.h"

DoubleVector* DblVecInit()
{
	DoubleVector* _malloc(sizeof(DoubleVector), vector)
	vector->VecBase = VecInit(&vector->Data, sizeof(double));
	return vector;
}

void DblVecAppend(DoubleVector* vector, const double value)
{
	VecAppend(&vector->VecBase, &value);
}

void DblVecAppendRange(DoubleVector* vector, const double* value, const size_t len)
{
	VecAppendRange(&vector->VecBase, value, len);
}

void DblVecFreeMemory(DoubleVector** vector)
{
	free((*vector)->Data);
	_FreeN(vector);
}
