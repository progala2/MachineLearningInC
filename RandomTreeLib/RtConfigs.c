#include "RtConfigs.h"
#include <stdlib.h>
#include "CharsTable.h"

#define BUFFER_LEN 255u
#define CONFIGS_READER_LEN 7u
#define S_SCAN_F_EQ_1(format, fieldName) if (strcat_s(format, BUFFER_LEN, fieldName) == 0 && strcat_s(format, BUFFER_LEN, "=%254s") == 0 && sscanf_s(table->Table[i]->Data, format, buffer, (uint)BUFFER_LEN) == 1)
#define RT_RDR_TUPLE(param, editable)  {XSTRIFY(param), RT_FLD_RDR_NAME(param), RT_FLD_GET_NAME(param), editable}

static const RtReadFunctions configs_readers[CONFIGS_READER_LEN] = {
	RT_RDR_TUPLE(CFG_FLD_TRAINING_FILE_NAME, false),
	RT_RDR_TUPLE(CFG_FLD_TEST_FILE_NAME, false),
	RT_RDR_TUPLE(CFG_FLD_MAX_FEATURES_PER_NODE, true),
	RT_RDR_TUPLE(CFG_FLD_CV_TYPE, true),
	RT_RDR_TUPLE(CFG_FLD_OUTPUT_FOLDER, true),
	RT_RDR_TUPLE(CFG_FLD_CROSS_VALIDATION_COUNT, true),
	RT_RDR_TUPLE(CFG_FLD_TREE_COUNT, true),
};

static void RtSetUpPropertyFromString_ReadOnly(RtConfigs*const configs, const char* str, const bool editReadOnly);

RtConfigs* RtReadConfigFromFile(FILE* fp)
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

	for (uint i = 1; i < table->VecBase.Size; ++i)
	{
		RtSetUpPropertyFromString_ReadOnly(configs, table->Table[i]->Data, true);
	}

	if (configs->CvType == Cv_Normal && configs->CrossValidationCount < 2)
	{
		RtFreeMemory(&configs);
		return NULL;
	}

	return configs;
}

void RtPrintAllSettings(const RtConfigs* configs)
{
	char buffer[BUFFER_LEN];
	for (uint i = 0; i < CONFIGS_READER_LEN; ++i)
	{
		printf("- %s: %s%s\n", configs_readers[i].ConfigName, configs_readers[i].ConfigGetter(configs, buffer, BUFFER_LEN), configs_readers[i].Editable ? "": " (ReadOnly)");	
	}
}

static void RtSetUpPropertyFromString_ReadOnly(RtConfigs*const configs, const char* str, const bool editReadOnly)
{
	char buffer[BUFFER_LEN] = { 0 };
	char formatBuffer[BUFFER_LEN] = { 0 };
	for (uint j = 0; j < CONFIGS_READER_LEN; ++j)
	{
		if (!editReadOnly && !configs_readers[j].Editable)
			continue;
		formatBuffer[0] = 0;
		strcat_s(formatBuffer, BUFFER_LEN, configs_readers[j].ConfigName);
		strcat_s(formatBuffer, BUFFER_LEN, "=%254s");
		if (sscanf_s(str, formatBuffer, buffer, (uint)BUFFER_LEN) == 1)
		{
			if (configs_readers[j].ConfigReader(configs, buffer) == false)
				printf("Something Wrong with %s. Not Loaded.", configs_readers[j].ConfigName);
			break;
		}
	}
}

void RtSetUpPropertyFromString(RtConfigs*const configs, const char* str)
{
	RtSetUpPropertyFromString_ReadOnly(configs, str, false);
}

void RtFreeMemory(RtConfigs**const input)
{
	FreeN(&(*input)->TrainingFileName);
	FreeN(&(*input)->TestFileName);
	FreeN(&(*input)->OutputFolder);
	FreeN((void_tab_ptr)input);
}
