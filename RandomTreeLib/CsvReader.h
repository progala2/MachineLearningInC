#ifndef CsvReader_H_INCLUDE_GUARD
#define CsvReader_H_INCLUDE_GUARD
#include <stdio.h>
#include "CharsTable.h"

typedef struct
{
	int ParametersLen;
	long RowsNumber;
	char** Headers;
	double** Parameters;
	double* ClassColumn;
	char* ClassName;
} CsvTable;

void CsvInit(CsvTable* input);
CsvTable* CsvReadTable(const CharsTable* input);
CharsTable* TReadFile(FILE* input, const unsigned int bufferLen);
void CsvFreeMemory(CsvTable* table);

#endif