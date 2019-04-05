#include "DoubleVector.h"
#include <stdlib.h>

DoubleVector* DblVecInit()
{
	DoubleVector* _malloc(sizeof(DoubleVector), vector);
	vector->VecBase = VecInit((pointer_ptr)&vector->Data, sizeof(double));
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

IntVector* IntVecInit()
{
	IntVector* _malloc(sizeof(IntVector), vector);
	vector->VecBase = VecInit(&vector->Data, sizeof(int));
	return vector;
}

IntVector* IntVecInit_C(const size_t capacity)
{
	IntVector* _malloc(sizeof(IntVector), vector);
	vector->VecBase = VecInit_C(&vector->Data, sizeof(int), capacity);
	return vector;
}

void IntVecAppend(IntVector* vector, const int value)
{
	VecAppend(&vector->VecBase, &value);
}

void IntVecAppendRange(IntVector* vector, const int* value, const size_t len)
{
	VecAppendRange(&vector->VecBase, value, len);
}

void IntVecFreeMemory(IntVector** vector)
{
	free((*vector)->Data);
	_FreeN(vector);
}
