#include "RtConfigs.h"
#include "CsvReader.h"
#include <stdlib.h>

RtConfigs* RtReadConfig(FILE* fp)
{
	CharsTable* table = TReadFile(fp, 1024);
	if (table->Size < 1)
		return NULL;

	#define BUFFER_LEN 255
	char buffer[BUFFER_LEN] = { 0 };
	if (sscanf_s(table->Table[0]->Data,"FileName=%254s", buffer, (uint)BUFFER_LEN) < 1)
		return NULL;

	RtConfigs* configs = malloc(sizeof(RtConfigs));
	configs->CrossValidationCount = 0;
	configs->MaxFeaturesPerNode = 3;
	configs->OutputFolder = MemCopyChars("");
	configs->TreeCount = 30;
	configs->CvType = Cv_None;
	configs->FileName = MemCopyChars(buffer);
	for (uint i = 1; i < table->Size; ++i)
	{
		if (sscanf_s(table->Table[i]->Data, "MaxFeaturesPerNode=%254s", buffer, (uint)BUFFER_LEN) == 1)
		{
			configs->MaxFeaturesPerNode = strtol(buffer, NULL, 10);
		}
		else if (sscanf_s(table->Table[i]->Data, "OutputFolder=%254s", buffer, (uint)BUFFER_LEN) == 1)
		{
			configs->OutputFolder = MemCopyChars(buffer);
		}
		else if (sscanf_s(table->Table[i]->Data, "CrossValidationType=%254s", buffer, (uint)BUFFER_LEN) == 1)
		{
			const long value = strtol(buffer, NULL, 10);
			if (value >= Cv_Min && value <= CV_Max)
			{
				configs->CvType = value;
			}
		}
		else if (sscanf_s(table->Table[i]->Data, "CrossValidationCount=%254s", buffer, (uint)BUFFER_LEN) == 1)
		{
			const long value = strtol(buffer, NULL, 10);
			if (value >= 2 && value <= 10)
			{
				configs->CrossValidationCount = value;
			}
		}
		else if (sscanf_s(table->Table[i]->Data, "TreeCount=%254s", buffer, (uint)BUFFER_LEN) == 1)
		{
			configs->TreeCount = strtol(buffer, NULL, 10);
		}
	}

	if (configs->CvType == Cv_Normal && configs->CrossValidationCount < 2)
	{
		RtFreeMemory(&configs);
		return NULL;
	}

	return configs;
}

void RtFreeMemory(RtConfigs**const input)
{
	FreeN(&(*input)->FileName);
	FreeN(&(*input)->OutputFolder);
	FreeN((void_tab_ptr)input);
}
