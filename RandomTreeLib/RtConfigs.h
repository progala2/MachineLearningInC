#ifndef CONFIGS_H_INCLUDE_GUARD
#define CONFIGS_H_INCLUDE_GUARD
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CFG_FLD_TRAINING_FILE_NAME TrainingFileName 
#define CFG_FLD_TEST_FILE_NAME TestFileName 
#define CFG_FLD_OUTPUT_FOLDER OutputFolder 
#define CFG_FLD_TREE_COUNT TreeCount 
#define CFG_FLD_MIN_SPLIT_COUNT MinSplitCount 
#define CFG_FLD_MIN_ELEMS_IN_LEAF MinElemsInLeaf 
#define CFG_FLD_MAX_FEATURES_PER_NODE MaxFeaturesPerNode 
#define CFG_FLD_MAX_DEEPNESS MaxDeepness 
#define CFG_FLD_TEST_EXTRACT_PERCENTAGE TestExtractPercentage 
#define CFG_FLD_FORCE_TEST_EXTRACT ForceTestExtract 

#define CFG_OUTPUT_FOLDER ""
#define CFG_TREE_COUNT 30
#define CFG_MAX_FEATURE_PER_NODE 3
#define CFG_MAX_DEEPNESS 1
#define CFG_MIN_SPLIT_COUNT 2
#define CFG_MIN_ELEMS_IN_LEAF 1
#define CFG_TEST_FILE_NAME NULL
#define CFG_TEST_EXTRACT_PERCENTAGE 10
#define CFG_FORCE_TEST_EXTRACT 0

typedef struct RtConfigs RtConfigs;

struct RtConfigs
{
	char* CFG_FLD_TRAINING_FILE_NAME;
	char* CFG_FLD_TEST_FILE_NAME;
	char* CFG_FLD_OUTPUT_FOLDER;
	uint CFG_FLD_TREE_COUNT;
	uint CFG_FLD_MAX_FEATURES_PER_NODE;
	uint CFG_FLD_MAX_DEEPNESS;
	uint CFG_FLD_MIN_SPLIT_COUNT;
	uint CFG_FLD_MIN_ELEMS_IN_LEAF;
	uint CFG_FLD_TEST_EXTRACT_PERCENTAGE;
	uint CFG_FLD_FORCE_TEST_EXTRACT;
};

typedef struct
{
	const char* ConfigName;
	bool (*ConfigReader)(RtConfigs * config, const char* confStr);
	const char* (*ConfigGetter)(const RtConfigs * config, char* buffer, uint bufferLen);
	bool Editable;
	const char* Description;
} RtConfigsSetUp;


RtConfigs* RtReadConfigFromFile(FILE* fp);
void RtPrintAllSettings(const RtConfigs* configs);
void RtSetUpPropertyFromString(RtConfigs*const configs, const char* str);

void RtFreeMemory(RtConfigs**const input);

#define RT_FLD_RDR_P(param) RT_FLD_RDR_F(param)
#define RT_FLD_RDR_F(param) bool RT_FLD_RDR_NAME(param)(RtConfigs * config, const char* confStr)

#define RT_FLD_RDR_F_CHAR(param) inline bool RT_FLD_RDR_NAME(param)(RtConfigs * config, const char* confStr)\
	{\
		config->param = MemCopyChars(confStr);\
		return true;\
	}

#define RT_FLD_RDR_F_INT_MAX_MIN(param, mx, mn, type) inline bool RT_FLD_RDR_NAME(param)(RtConfigs * config, const char* confStr)\
	{\
		const long value = strtol(confStr, NULL, 10);\
		if (value >= (mn) && value <= (mx))\
		{\
			config->param = (type)value;\
			return true;\
		}\
		return false;\
	}
#define RT_FLD_RDR_NAME(param) Rt ## param ## Reader

#define RT_FLD_GET_P_CHAR(param) RT_FLD_GET_F_CHAR(param)
#define RT_FLD_GET_F_CHAR(param) inline const char* RT_FLD_GET_NAME(param)(const RtConfigs *const config, char*const buffer, const uint bufferLen)\
	{\
		return config->param;\
	}
#define RT_FLD_GET_F_INT(param) inline const char* RT_FLD_GET_NAME(param)(const RtConfigs *const config, char*const buffer, const uint bufferLen)\
	{\
		sprintf_s(buffer, bufferLen,"%d", config->param);\
		return buffer;\
	}
#define RT_FLD_GET_NAME(param) Rt ## param ## Getter

#define RT_FLD_READER_GETTER_P_CHAR_CHAR(param) \
	RT_FLD_RDR_F_CHAR(param)\
	RT_FLD_GET_F_CHAR(param)
#define RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(param, mx, mn, type) \
	RT_FLD_RDR_F_INT_MAX_MIN(param, mx, mn, type)\
	RT_FLD_GET_F_INT(param)

RT_FLD_READER_GETTER_P_CHAR_CHAR(CFG_FLD_TRAINING_FILE_NAME)
RT_FLD_READER_GETTER_P_CHAR_CHAR(CFG_FLD_TEST_FILE_NAME)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_TREE_COUNT, INT_MAX, 1, uint)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_MAX_FEATURES_PER_NODE, INT_MAX, 1, uint)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_MAX_DEEPNESS, INT_MAX, 1, uint)
RT_FLD_READER_GETTER_P_CHAR_CHAR(CFG_FLD_OUTPUT_FOLDER)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_MIN_SPLIT_COUNT, INT_MAX, 2, uint)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_MIN_ELEMS_IN_LEAF, INT_MAX, 1, uint)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_TEST_EXTRACT_PERCENTAGE, 50, 1, uint)
RT_FLD_READER_GETTER_P_INT_INT_MAX_MIN(CFG_FLD_FORCE_TEST_EXTRACT, 1, 0, uint)

extern const RtConfigs* _glConfigs;
#endif