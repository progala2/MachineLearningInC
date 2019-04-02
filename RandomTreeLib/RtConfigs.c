#include "RtConfigs.h"
#include <stdlib.h>
#include "CharsTable.h"

#define BUFFER_LEN 255u
#define S_SCAN_F_EQ_1(fieldName) if (sscanf_s(table->Table[i]->Data, XSTRIFY(fieldName)"=%254s", buffer, (uint)BUFFER_LEN) == 1)
#define RT_RDR_TUPLE(param)  {XSTRIFY(CFG_FLD_TRAINING_FILE_NAME), RT_FLD_RDR_NAME(param)}

static const RtReadFunctions configs_readers[] = {
	RT_RDR_TUPLE(CFG_FLD_TRAINING_FILE_NAME),
	RT_RDR_TUPLE(CFG_FLD_TEST_FILE_NAME),
	RT_RDR_TUPLE(CFG_FLD_MAX_FEATURES_PER_NODE),
	RT_RDR_TUPLE(CFG_FLD_CV_TYPE),
	RT_RDR_TUPLE(CFG_FLD_OUTPUT_FOLDER),
	RT_RDR_TUPLE(CFG_FLD_CROSS_VALIDATION_COUNT),
	RT_RDR_TUPLE(CFG_FLD_TREE_COUNT),
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

bool RtTreeCountReader(RtConfigs* config, const char* confStr)
{
	config->CFG_FLD_TREE_COUNT = strtol(confStr, NULL, 10);
	return true;
}

bool RtMaxFeaturesPerNodeReader(RtConfigs* config, const char* confStr)
{
	config->CFG_FLD_MAX_FEATURES_PER_NODE = strtol(confStr, NULL, 10);
	return true;
}

bool RtCvTypeReader(RtConfigs* config, const char* confStr)
{
	const long value = strtol(confStr, NULL, 10);
	if (value >= Cv_Min && value <= CV_Max)
	{
		config->CFG_FLD_CV_TYPE = value;
		return true;
	}
	return false;
}

bool RtOutputFolderReader(RtConfigs* config, const char* confStr)
{
	config->CFG_FLD_OUTPUT_FOLDER = MemCopyChars(confStr);
	return true;
}

bool RtCrossValidationCountReader(RtConfigs* config, const char* confStr)
{
	const long value = strtol(confStr, NULL, 10);
	if (value >= 2 && value <= 10)
	{
		config->CFG_FLD_CROSS_VALIDATION_COUNT = value;
		return true;
	}
	return false;
}

RtConfigs* RtReadConfig(FILE* fp)
{
	CharsTable* table = TReadFile(fp, 1024);
	if (table->VecBase.Size < 1)
		return NULL;

	char buffer[BUFFER_LEN] = { 0 };
	if (sscanf_s(table->Table[0]->Data, XSTRIFY(CFG_FLD_TRAINING_FILE_NAME)"=%254s", buffer, BUFFER_LEN) < 1)
		return NULL;

	RtConfigs* configs = malloc(sizeof(RtConfigs));
	configs->CFG_FLD_CROSS_VALIDATION_COUNT = CFG_CROSS_VALIDATION_COUNT;
	configs->CFG_FLD_MAX_FEATURES_PER_NODE = CFG_MAX_FEATURE_PER_NODE;
	configs->CFG_FLD_OUTPUT_FOLDER = MemCopyChars(CFG_OUTPUT_FOLDER);
	configs->CFG_FLD_TREE_COUNT = CFG_TREE_COUNT;
	configs->CFG_FLD_CV_TYPE = CFG_CV_TYPE;
	configs->CFG_FLD_TRAINING_FILE_NAME = MemCopyChars(buffer);
	configs->CFG_FLD_TEST_FILE_NAME = CFG_TEST_FILE_NAME;

	const uint configTableSize = sizeof configs_readers/sizeof configs_readers[0];
	for (uint i = 1; i < table->VecBase.Size; ++i)
	{
		for(uint j = 0; j < configTableSize; ++j)
		{
			S_SCAN_F_EQ_1(configs_readers[j].ConfigName)
			{
				if (configs_readers[j].ConfigReader(configs, buffer) == false)
					DBG_PRINT(("Something Wrong with %s. Not Loaded.", configs_readers[j].ConfigName));
				break;
			}
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
