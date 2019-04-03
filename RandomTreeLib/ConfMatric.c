#include "ConfMatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include "CharsTable.h"

ConfMatrix* CmCreate(const int actual[], const int predicted[], const char*const* classNames, const size_t headSize, const size_t predSize)
{
	if (headSize < 2)
		return NULL;

	ConfMatrix* matrix = malloc(sizeof(ConfMatrix));

	matrix->ClassNames = malloc(sizeof(char*)*headSize);
	matrix->TableData = malloc(sizeof(int*)*headSize);
	for (size_t i = 0; i < headSize; ++i)
	{
		matrix->ClassNames[i] = classNames[i];
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
		printf("%10s ", matrix->ClassNames[i]);
	}

	printf("\n");
	for (size_t i = 0; i < matrix->N; ++i)
	{
		printf("%10s ", matrix->ClassNames[i]);
		for (size_t j = 0; j < matrix->N; ++j)
		{
			printf("%10d ", matrix->TableData[i][j]);
		}

		printf("\n");
	}
}

void CmFree(ConfMatrix**const matrix)
{
	FreeTab((void_tab_ptr)(*matrix)->TableData, (*matrix)->N);
	free((*matrix)->ClassNames);
	free(*matrix);
	*matrix = NULL;
}
