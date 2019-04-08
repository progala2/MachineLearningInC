#ifndef CONF_MATRIX_H_GUARD
#define CONF_MATRIX_H_GUARD
#include <stddef.h>

typedef struct
{
	const char** ClassNames;
	int** TableData;
	size_t N;
} ConfMatrix;

ConfMatrix* CmCreate(const int actual[], const int predicted[], const char*const* classNames, const size_t headSize, const size_t predSize);
void CmPrint(const ConfMatrix* matrix);
double CmCalculateError(const ConfMatrix* const matrix);
double CmCalculatePrecision(const ConfMatrix* const matrix);
double CmCalculateRecall(const ConfMatrix* const matrix);
void CmFree(ConfMatrix**const matrix);
#endif