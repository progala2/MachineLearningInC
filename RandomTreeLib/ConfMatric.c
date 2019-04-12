#include "ConfMatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include "CharsTable.h"

ConfMatrix* CmCreate(const int actual[], const int predicted[], const char* const* classNames, const size_t headSize, const size_t preSize)
{
	if (headSize < 2)
		return NULL;

	ConfMatrix* _malloc(sizeof(ConfMatrix), matrix);

	_malloc(sizeof(char*) * headSize, matrix->ClassNames);
	_malloc(sizeof(int*) * headSize, matrix->TableData);
	for (size_t i = 0; i < headSize; ++i)
	{
		matrix->ClassNames[i] = classNames[i];
		_calloc(headSize, sizeof(int), matrix->TableData[i]);
	}
	for (size_t i = 0; i < preSize; ++i)
	{
		matrix->TableData[actual[i]][predicted[i]]++;
	}
	matrix->N = headSize;
	return matrix;
}

void CmPrint(const ConfMatrix* matrix)
{
	CmPrint_F(stdout, matrix);
}

void CmPrint_F(FILE* file, const ConfMatrix* matrix)
{
	CmPrint_FC(file, matrix, " ");
}

void CmPrint_FC(FILE* file, const ConfMatrix* matrix, const char*const cornerText)
{
	fprintf(file,"%10s, ", cornerText);
	const uint n1 = matrix->N - 1;
	for (size_t i = 0; i < n1; ++i)
	{
		fprintf(file, "%10s, ", matrix->ClassNames[i]);
	}
	fprintf(file, "%10s\n", matrix->ClassNames[n1]);

	for (size_t i = 0; i < matrix->N; ++i)
	{
		fprintf(file, "%10s, ", matrix->ClassNames[i]);
		for (size_t j = 0; j < n1; ++j)
		{
			fprintf(file, "%10d, ", matrix->TableData[i][j]);
		}
		fprintf(file, "%10d\n", matrix->TableData[i][n1]);
	}
}

double CmCalculateAccuracy(const ConfMatrix* const matrix)
{
	double divider = 0;
	double numerator = 0;
	for (uint i = 0; i < matrix->N; ++i)
	{
		for (uint j = 0; j < matrix->N; ++j)
		{
			divider += matrix->TableData[i][j];
			if (i != j)
				numerator += matrix->TableData[i][j];
		}
	}
	return 1 - numerator / divider;
}

void CmFree(ConfMatrix * *const matrix)
{
	if (*matrix == NULL)
		return;
	FreeTab((void_tab_ptr)(*matrix)->TableData, (*matrix)->N);
	free((*matrix)->ClassNames);
	free(*matrix);
	*matrix = NULL;
}
