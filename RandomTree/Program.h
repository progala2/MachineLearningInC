#ifndef PROGRAM_H_GUARD
#define  PROGRAM_H_GUARD
#include "../RandomTreeLib/RtConfigs.h"
#include "../RandomTreeLib/ReadLearnData.h"

typedef struct
{
	LearnData* LearnData;
	RtConfigs* Configs;
}Program;

Program* PrgLoadData();
bool PrgMenuLoop(Program* program);
void PrgFree(Program** program);
#endif