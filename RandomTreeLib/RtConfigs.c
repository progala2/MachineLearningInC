#include "RtConfigs.h"
#include "CsvReader.h"
#include <stdlib.h>

RtConfigs* RtReadConfig(FILE* fp)
{
	CharsTable* table = TReadFile(fp, 1024);
	if (table->VecBase.Size < 1)
		return NULL;

	#define BUFFER_LEN 255
	char buffer[BUFFER_LEN] = { 0 };
	if (sscanf_s(table->Table[0]->Data,"TrainingFileName=%254s", buffer, (uint)BUFFER_LEN) < 1)
		return NULL;

	RtConfigs* configs = malloc(sizeof(RtConfigs));
	configs->CrossValidationCount = CONFIG_DEFAULT_CROSSVALIDATION_COUNT;
	configs->MaxFeaturesPerNode = CONFIG_DEFAULT_MAX_FEATURE_PER_NODE;
	configs->OutputFolder = MemCopyChars(CONFIG_DEFAULT_OUTPUT_FOLDER);
	configs->TreeCount = CONFIG_DEFAULT_TREE_COUNT;
	configs->CvType = CONFIG_DEFAULT_CROSSVALIDATION_TYPE;
	configs->TrainingFileName = MemCopyChars(buffer);
	configs->TestFileName = CONFIG_DEFAULT_TEST_FILE_NAME;

	for (uint i = 1; i < table->VecBase.Size; ++i)
	{
		if (sscanf_s(table->Table[i]->Data, "TestFileName=%254s", buffer, (uint)BUFFER_LEN) == 1)
		{
			configs->TestFileName = MemCopyChars(buffer);
		}
		else if (sscanf_s(table->Table[i]->Data, "MaxFeaturesPerNode=%254s", buffer, (uint)BUFFER_LEN) == 1)
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
	FreeN(&(*input)->TrainingFileName);
	FreeN(&(*input)->TestFileName);
	FreeN(&(*input)->OutputFolder);
	FreeN((void_tab_ptr)input);
}
