#ifndef DOUBLE_VECTOR_H_INCLUDE_GUARD
#define DOUBLE_VECTOR_H_INCLUDE_GUARD
#include "VecBase.h"

typedef struct {
	double* Data;
	VecBase VecBase;
} DoubleVector;

DoubleVector* DblVecInit();
void DblVecAppend(DoubleVector* vector, const double value);
void DblVecAppendRange(DoubleVector* vector, const double* value, const size_t len);
void DblVecFreeMemory(DoubleVector** vector);

typedef struct {
	int* Data;
	VecBase VecBase;
} IntVector;

IntVector* IntVecInit();
IntVector* IntVecInit_C(const size_t capacity);
void IntVecAppend(IntVector* vector, const int value);
void IntVecAppendRange(IntVector* vector, const int* value, const size_t len);
void IntVecFreeMemory(IntVector** vector);
#endif