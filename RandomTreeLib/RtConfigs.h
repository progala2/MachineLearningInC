#ifndef CONFIGS_H_INCLUDE_GUARD
#define CONFIGS_H_INCLUDE_GUARD
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>

#define CFG_FLD_TRAINING_FILE_NAME TrainingFileName 
#define CFG_FLD_TEST_FILE_NAME TestFileName 
#define CFG_FLD_OUTPUT_FOLDER OutputFolder 
#define CFG_FLD_TREE_COUNT TreeCount 
#define CFG_FLD_MAX_FEATURES_PER_NODE MaxFeaturesPerNode 
#define CFG_FLD_CROSS_VALIDATION_COUNT CrossValidationCount 
#define CFG_FLD_CV_TYPE CvType 

#define CFG_OUTPUT_FOLDER ""
#define CFG_TREE_COUNT 30
#define CFG_MAX_FEATURE_PER_NODE 3
#define CFG_CROSS_VALIDATION_COUNT 0
#define CFG_CV_TYPE Cv_None
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
	char* CFG_FLD_OUTPUT_FOLDER;
	uint CFG_FLD_TREE_COUNT;
	uint CFG_FLD_MAX_FEATURES_PER_NODE;
	uint CFG_FLD_CROSS_VALIDATION_COUNT;
	CrossValType CFG_FLD_CV_TYPE;
};

typedef struct
{
	const char* ConfigName;
	bool (*ConfigReader)(RtConfigs * config, const char* confStr);
} RtReadFunctions;


#define RT_FIELD_READER_P(param) RT_FLD_RDR_F(param)
#define RT_FLD_RDR_F(param) bool RT_FLD_RDR_NAME(param)(RtConfigs * config, const char* confStr)
#define RT_FLD_RDR_NAME(param) Rt ## param ## Reader

RT_FIELD_READER_P(CFG_FLD_TRAINING_FILE_NAME);
RT_FIELD_READER_P(CFG_FLD_TEST_FILE_NAME);
RT_FIELD_READER_P(CFG_FLD_TREE_COUNT);
RT_FIELD_READER_P(CFG_FLD_MAX_FEATURES_PER_NODE);
RT_FIELD_READER_P(CFG_FLD_CV_TYPE);
RT_FIELD_READER_P(CFG_FLD_OUTPUT_FOLDER);
RT_FIELD_READER_P(CFG_FLD_CROSS_VALIDATION_COUNT);

RtConfigs* RtReadConfig(FILE* fp);

void RtFreeMemory(RtConfigs**const input);

#endif