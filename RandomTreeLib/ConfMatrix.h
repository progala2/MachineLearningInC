#ifndef CONF_MATRIX_H_GUARD
#define CONF_MATRIX_H_GUARD
#include <stddef.h>
#include <stdio.h>

typedef struct
{
	const char** ClassNames;
	int** TableData;
	size_t N;
} ConfMatrix;

ConfMatrix* CmCreate(const int actual[], const int predicted[], const char*const* classNames, const size_t headSize, const size_t preSize);
void CmPrint(const ConfMatrix* matrix);
void CmPrint_F(FILE* file, const ConfMatrix* matrix);
double CmCalculateAccuracy(const ConfMatrix* const matrix);
void CmFree(ConfMatrix**const matrix);
#endif