#include "ConfMatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include "CharsTable.h"

ConfMatrix* CmCreate(const int actual[], const int predicted[], const char* headers[], const size_t headSize, const size_t predSize)
{
	if (headSize < 2)
		return NULL;

	ConfMatrix* matrix = malloc(sizeof(ConfMatrix));
	
	matrix->Headers = malloc(sizeof(char*)*headSize);
	matrix->TableData = malloc(sizeof(int*)*headSize);
	for (size_t i = 0; i < headSize; ++i)
	{
		matrix->Headers[i] = headers[i];
		matrix->TableData[i] = calloc(headSize, sizeof(int));
	}
	for (size_t i = 0; i < predSize; ++i)
	{
		matrix->TableData[actual[i]][predicted[i]]++;
	}
	matrix->N = headSize;
	return matrix;
}

void CmPrint(const ConfMatrix* matrix)
{
	printf("%10s ", " ");
	for (size_t i = 0; i < matrix->N; ++i)
	{
		printf("%10s ", matrix->Headers[i]);
	}
	for (size_t i = 0; i < matrix->N; ++i)
	{
		printf("%10s ", matrix->Headers[i]);
		for (size_t j = 0; j < matrix->N; ++j)
		{
			printf("%10d ", matrix->TableData[i][j]);
		}
	}
}

void CmFree(ConfMatrix**const matrix)
{
	FreeTabVoid((void_tab_ptr)(*matrix)->TableData, (*matrix)->N, sizeof(int));
	free((*matrix)->Headers);
	free(*matrix);
	(*matrix) = NULL;
}
