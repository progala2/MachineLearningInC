#include "RtConfigs.h"
#include <stdlib.h>
#include "CharsTable.h"

const RtConfigs* _glConfigs = NULL;

#define BUFFER_LEN 255u
#define CONFIGS_READER_LEN 11u
#define S_SCAN_F_EQ_1(format, fieldName) if (strcat_s(format, BUFFER_LEN, fieldName) == 0 && strcat_s(format, BUFFER_LEN, "=%254s") == 0 && sscanf_s(table->Table[i]->Data, format, buffer, (uint)BUFFER_LEN) == 1)
#define RT_RDR_TUPLE(param, editable)  RT_RDR_TUPLE_DESC(param, editable, NULL)
#define RT_RDR_TUPLE_DESC(param, editable, desc)  {XSTRIFY(param), RT_FLD_RDR_NAME(param), RT_FLD_GET_NAME(param), editable, desc}

static const RtConfigsSetUp configs_readers[CONFIGS_READER_LEN] = {
	RT_RDR_TUPLE_DESC(CFG_FLD_TRAINING_FILE_NAME, false, "Path to the file with the training data.(obligatory)"),
	RT_RDR_TUPLE_DESC(CFG_FLD_TEST_FILE_NAME, false, "Path to the file with the test data.(Not obligatory)"),
	RT_RDR_TUPLE_DESC(CFG_FLD_MAX_FEATURES_PER_NODE, true, "How many features can be taken for the node spliting."),
	RT_RDR_TUPLE_DESC(CFG_FLD_OUTPUT_FOLDER, true, "Where to save the files."),
	RT_RDR_TUPLE_DESC(CFG_FLD_TREE_COUNT, true, "Number of trees in a forest."),
	RT_RDR_TUPLE_DESC(CFG_FLD_MIN_SPLIT_COUNT, true, "Minimum elements in a node accepted to be splitted"),
	RT_RDR_TUPLE_DESC(CFG_FLD_MIN_ELEMS_IN_LEAF, true, "Minimum elements that can be stored in a leaf."),
	RT_RDR_TUPLE_DESC(CFG_FLD_TEST_EXTRACT_PERCENTAGE, true, "When there is no test data provided, what percetage should be taken for testing."),
	RT_RDR_TUPLE_DESC(CFG_FLD_FORCE_TEST_EXTRACT, true, "Merge training and test data and force extracting data samples from it."),
	RT_RDR_TUPLE_DESC(CFG_FLD_VOTING_TYPE, true, "0: Sum up probabilities from leaves; 1: category with the highest probability in a tree takes 1 point from it."),
	RT_RDR_TUPLE_DESC(CFG_FLD_MAX_DEEPNESS, true, "Maximum deepness of a tree.")
};

static void RtSetUpPropertyFromString_ReadOnly(RtConfigs* const configs, const char* str, const bool editReadOnly);

RtConfigs* RtReadConfigFromFile(FILE* fp)
{
	CharsTable* table = TReadFile(fp, BUFFER_LEN * 4);
	if (table->VecBase.Size < 1)
		return NULL;

	char buffer[BUFFER_LEN] = { 0 };
	if (sscanf_s(table->Table[0]->Data, XSTRIFY(CFG_FLD_TRAINING_FILE_NAME)"=%254s", buffer, BUFFER_LEN) < 1)
		return NULL;

	RtConfigs* _malloc(sizeof(RtConfigs), configs);
	configs->CFG_FLD_MAX_FEATURES_PER_NODE = CFG_MAX_FEATURE_PER_NODE;
	configs->CFG_FLD_MAX_DEEPNESS = CFG_MAX_DEEPNESS;
	configs->CFG_FLD_MIN_ELEMS_IN_LEAF = CFG_MIN_ELEMS_IN_LEAF;
	configs->CFG_FLD_MIN_SPLIT_COUNT = CFG_MIN_SPLIT_COUNT;
	configs->CFG_FLD_OUTPUT_FOLDER = MemCopyChars(CFG_OUTPUT_FOLDER);
	configs->CFG_FLD_TREE_COUNT = CFG_TREE_COUNT;
	configs->CFG_FLD_TRAINING_FILE_NAME = MemCopyChars(buffer);
	configs->CFG_FLD_TEST_FILE_NAME = CFG_TEST_FILE_NAME;
	configs->CFG_FLD_TEST_EXTRACT_PERCENTAGE = CFG_TEST_EXTRACT_PERCENTAGE;
	configs->CFG_FLD_FORCE_TEST_EXTRACT = CFG_FORCE_TEST_EXTRACT;
	configs->CFG_FLD_VOTING_TYPE = CFG_VOTING_TYPE;

	for (uint i = 1; i < table->VecBase.Size; ++i)
	{
		RtSetUpPropertyFromString_ReadOnly(configs, table->Table[i]->Data, true);
	}

	TFreeMemory(&table, true);
	return configs;
}

void RtPrintAllSettings(const RtConfigs* configs)
{
	char buffer[BUFFER_LEN];
	for (uint i = 0; i < CONFIGS_READER_LEN; ++i)
	{
		printf("- %s: %s%s - %s\n", configs_readers[i].ConfigName, configs_readers[i].ConfigGetter(configs, buffer, BUFFER_LEN), configs_readers[i].Editable ? "" : " (ReadOnly)", configs_readers[i].Description != NULL ? configs_readers[i].Description : "No Description");
	}
}

static void RtSetUpPropertyFromString_ReadOnly(RtConfigs * const configs, const char* str, const bool editReadOnly)
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

void RtSetUpPropertyFromString(RtConfigs * const configs, const char* str)
{
	RtSetUpPropertyFromString_ReadOnly(configs, str, false);
}

void RtFreeMemory(RtConfigs * *const input)
{
	if (*input == NULL)
		return;
	FreeN(&(*input)->TrainingFileName);
	FreeN(&(*input)->TestFileName);
	FreeN(&(*input)->OutputFolder);
	_FreeN(input);
}
