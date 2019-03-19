#include "CsvReader.h"
#include <stdlib.h>

struct CsvTable* ReadCsv(const char* input)
{
	return malloc(sizeof(struct CsvTable));
}

void DestroyCsvTable(struct CsvTable** table)
{
	struct CsvTable* locTable = (*table);
	for(int i = 0; i < (*table)->HeadersLen; ++i)
	{
		free(locTable->Headers[i]);
	}
	free(locTable->Headers);
	
	free(*table);
	(*table) = NULL;
}
