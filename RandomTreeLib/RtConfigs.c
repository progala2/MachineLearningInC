#include "RtConfigs.h"
#include "CsvReader.h"
#include <stdlib.h>
#include <string.h>

RtConfigs* RtReadConfig(FILE* fp)
{
	CharsTable* table = TReadFile(fp, 1024);
	if (table->Size < 1)
		return NULL;

	char buffer[255];
	if (scanf_s("FILENAME=%254s", buffer, (uint)255) < 1)
		return NULL;

	RtConfigs* configs = malloc(sizeof(RtConfigs));
	configs->CrossValidationCount = 0;
	configs->MaxFeaturesPerNode = 3;
	configs->OutputFolder = "";
	configs->TreeCount = 30;
	configs->CvType = Cv_None;
	configs->FileName = MemCopyAlloc(buffer, MemSizeChars(buffer));
	for (uint i = 1; i < table->Size; ++i)
	{
		
		
	}

	return configs;
}
