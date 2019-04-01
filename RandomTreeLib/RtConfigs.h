#ifndef CONFIGS_H_INCLUDE_GUARD
#define CONFIGS_H_INCLUDE_GUARD
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

#define CFG_FLD_TRAINING_FILE_NAME TrainingFileName 
#define CFG_FLD_TEST_FILE_NAME TestFileName 

#define CFG_OUTPUT_FOLDER ""
#define CFG_TREE_COUNT 30
#define CFG_MAX_FEATURE_PER_NODE 3
#define CFG_CROSSVALIDATION_COUNT 0
#define CFG_CROSSVALIDATION_TYPE Cv_None
#define CFG_TEST_FILE_NAME NULL

typedef struct RtConfigs RtConfigs;

typedef enum
{
	Cv_Min = 0,
	Cv_None = 0,
	Cv_Normal = 1,
	Cv_Loo = 2,
	CV_Max = 2
} CrossValType;

struct RtConfigs
{
	char* CFG_FLD_TRAINING_FILE_NAME;
	char* CFG_FLD_TEST_FILE_NAME;
	char* OutputFolder;
	uint TreeCount;
	uint MaxFeaturesPerNode;
	uint CrossValidationCount;
	CrossValType CvType;
};

typedef struct
{
	const char* ConfigName;
	bool (*ConfigReader)(RtConfigs * config, const char* confStr);
} RtReadFunctions;


#define _RtFieldReader_P(param) RT_FLD_RDR_F(param)
#define RT_FLD_RDR_F(param) bool RT_FLD_RDR_NAME(param)(RtConfigs * config, const char* confStr)
#define RT_FLD_RDR_NAME(param) Rt ## param ## Reader

_RtFieldReader_P(CFG_FLD_TRAINING_FILE_NAME);
_RtFieldReader_P(CFG_FLD_TEST_FILE_NAME);

RtConfigs* RtReadConfig(FILE* fp);

void RtFreeMemory(RtConfigs**const input);

#endif