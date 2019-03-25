#ifndef CONF_MATRIX_H_GUARD
#define CONF_MATRIX_H_GUARD
#include <stddef.h>

typedef struct
{
	const char** Headers;
	int** TableData;
	size_t N;
} ConfMatrix;

ConfMatrix* CmCreate(const int actual[], const int predicted[], const char* headers[], const size_t headSize, const size_t predSize);
void CmPrint(const ConfMatrix* matrix);
void CmFree(ConfMatrix**const matrix);
#endif