#ifndef CONFIGS_H_INCLUDE_GUARD
#define CONFIGS_H_INCLUDE_GUARD
#include "utils.h"
#include <stdio.h>

#define CONFIG_DEFAULT_OUTPUT_FOLDER ""
#define CONFIG_DEFAULT_TREE_COUNT 30
#define CONFIG_DEFAULT_MAX_FEATURE_PER_NODE 3
#define CONFIG_DEFAULT_CROSSVALIDATION_COUNT 0
#define CONFIG_DEFAULT_CROSSVALIDATION_TYPE Cv_None
#define CONFIG_DEFAULT_TEST_FILE_NAME NULL

typedef enum
{
	Cv_Min = 0,
	Cv_None = 0,
	Cv_Normal = 1,
	Cv_Loo = 2,
	CV_Max = 2
} CrossValType;

typedef struct
{
	char* TrainingFileName;
	char* TestFileName;
	char* OutputFolder;
	uint TreeCount;
	uint MaxFeaturesPerNode;
	uint CrossValidationCount;
	CrossValType CvType;
} RtConfigs;

RtConfigs* RtReadConfig(FILE* fp);

void RtFreeMemory(RtConfigs**const input);
#endif