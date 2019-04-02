#include "RtConfigs.h"
#include <stdlib.h>
#include "CharsTable.h"

#define BUFFER_LEN 255
#define SSCANF_EQ_1(fieldName) if (sscanf_s(table->Table[i]->Data, XSTRIFY(fieldName)"=%254s", buffer, (uint)BUFFER_LEN) == 1)
#define RT_RDR_TUPLE(param)  {XSTRIFY(CFG_FLD_TRAINING_FILE_NAME), RT_FLD_RDR_NAME(param)}

static const RtReadFunctions ConfigsReaders[] = {
	RT_RDR_TUPLE(CFG_FLD_TRAINING_FILE_NAME),
	RT_RDR_TUPLE(CFG_FLD_TEST_FILE_NAME)
};

bool RtTrainingFileNameReader(RtConfigs* config, const char* confStr)
{
	config->CFG_FLD_TRAINING_FILE_NAME = MemCopyChars(confStr);
	return true;
}

bool RtTestFileNameReader(RtConfigs* config, const char* confStr)
{	
	config->CFG_FLD_TEST_FILE_NAME = MemCopyChars(confStr);
	return true;
}

RtConfigs* RtReadConfig(FILE* fp)
{
	CharsTable* table = TReadFile(fp, 1024);
	if (table->VecBase.Size < 1)
		return NULL;

	char buffer[BUFFER_LEN] = { 0 };
	if (sscanf_s(table->Table[0]->Data, XSTRIFY(CFG_FLD_TRAINING_FILE_NAME)"=%254s", buffer, (uint)BUFFER_LEN) < 1)
		return NULL;

	RtConfigs* configs = malloc(sizeof(RtConfigs));
	configs->CrossValidationCount = CFG_CROSSVALIDATION_COUNT;
	configs->MaxFeaturesPerNode = CFG_MAX_FEATURE_PER_NODE;
	configs->OutputFolder = MemCopyChars(CFG_OUTPUT_FOLDER);
	configs->TreeCount = CFG_TREE_COUNT;
	configs->CvType = CFG_CROSSVALIDATION_TYPE;
	configs->CFG_FLD_TRAINING_FILE_NAME = MemCopyChars(buffer);
	configs->CFG_FLD_TEST_FILE_NAME = CFG_TEST_FILE_NAME;

	for (uint i = 1; i < table->VecBase.Size; ++i)
	{
		SSCANF_EQ_1(CFG_FLD_TEST_FILE_NAME)
		{
			configs->TestFileName = MemCopyChars(buffer);
		}
		else SSCANF_EQ_1(MaxFeaturesPerNode)
		{
			configs->MaxFeaturesPerNode = strtol(buffer, NULL, 10);
		}
		else SSCANF_EQ_1(OutputFolder)
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
