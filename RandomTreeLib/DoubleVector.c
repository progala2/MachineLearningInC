#include "DoubleVector.h"
#include <stdlib.h>

DoubleVector* DblVecInit()
{
	DoubleVector* _malloc(sizeof(DoubleVector), vector);
	vector->VecBase = VecInit((pointer_ptr)&vector->Data, sizeof(double));
	return vector;
}
DoubleVector* DblVecInit_C(const size_t capacity)
{
	DoubleVector* _malloc(sizeof(DoubleVector), vector);
	vector->VecBase = VecInit_C((pointer_ptr)&vector->Data, sizeof(double), capacity);
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
	if (*vector == NULL)
		return;
	free((*vector)->Data);
	_FreeN(vector);
}

IntVector* IntVecInit()
{
	IntVector* _malloc(sizeof(IntVector), vector);
	vector->VecBase = VecInit((pointer_ptr)&vector->Data, sizeof(int));
	return vector;
}

IntVector* IntVecInit_C(const size_t capacity)
{
	IntVector* _malloc(sizeof(IntVector), vector);
	vector->VecBase = VecInit_C((pointer_ptr)&vector->Data, sizeof(int), capacity);
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

static int IntComp(const int* a, const int* b)
{
	return *a == *b ? 0 : 1;
}
static int* IntGet(const IntVector* const vector, const uint index)
{
	return &vector->Data[index];
}

bool IntVecContains(const IntVector* const vector, const int value, uint* index)
{
	return VecContains(&vector->VecBase, &value, IntComp, IntGet, vector, index);
}

void IntVecFreeMemory(IntVector** vector)
{
	if (*vector == NULL)
		return;
	free((*vector)->Data);
	_FreeN(vector);
}
