#ifndef DOUBLE_VECTOR_H_INCLUDE_GUARD
#define DOUBLE_VECTOR_H_INCLUDE_GUARD
#include "VecBase.h"

typedef struct {
	double* Data;
	VecBase VecBase;
} DoubleVector;

void DblVecInit(DoubleVector* vector);
void DblVecAppend(DoubleVector* vector, const double value);
void DblVecAppendRange(DoubleVector* vector, const double* value, const size_t len);
void DblVecFreeMemory(DoubleVector** vector);
#endif