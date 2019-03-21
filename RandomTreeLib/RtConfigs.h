#ifndef CONFIGS_H_INCLUDE_GUARD
#define CONFIGS_H_INCLUDE_GUARD
#include "utils.h"
#include <stdio.h>

typedef enum
{
	Cv_None = 0,
	Cv_Normal = 1,
	Cv_Loo = 2
} CrossValType;

typedef struct
{
	char* FileName;
	char* OutputFolder;
	uint TreeCount;
	uint MaxFeaturesPerNode;
	uint CrossValidationCount;
	CrossValType CvType;
} RtConfigs;

RtConfigs* RtReadConfig(FILE* fp);

#endif