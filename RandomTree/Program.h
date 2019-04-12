#ifndef PROGRAM_H_GUARD
#define  PROGRAM_H_GUARD
#include "../RandomTreeLib/RtConfigs.h"
#include "../RandomTreeLib/ReadLearnData.h"
#include "../RandomTreeLib/ConfMatrix.h"
#include "../RandomTreeLib/Forest.h"

#define PRG_HELP_CMD help
#define PRG_RUN_CMD run
#define PRG_SHOW_DATA_CMD showdata
#define PRG_CONF_CMD conf
#define PRG_SAVE_CMD save
#define PRG_EXIT_CMD exit

typedef struct
{
	LearnData* LearnData;
	RtConfigs* Configs;
	ConfMatrix* LastTestMatrix;
	ConfMatrix* LastTrainMatrix;
	Forest* LastForest;
	ConfMatrix*** CrossValidationResults;
	uint CvType;
}Program;

typedef struct
{
	const char* Name;
	bool (*Handler)(Program * program);
	const char* Description;
} PrgCommandHandler;

Program* PrgLoadData();
bool PrgMenuLoop(Program* program);
void PrgFree(Program** program);

#define PRG_FIELD_READER_P(param) PRG_FLD_RDR_F(param)
#define PRG_FLD_RDR_F(param) bool PRG_FLD_RDR_NAME(param)(Program * program)
#define PRG_FLD_RDR_NAME(param) Prg ## param ## Handler

PRG_FLD_RDR_F(PRG_HELP_CMD);
PRG_FLD_RDR_F(PRG_RUN_CMD);
PRG_FLD_RDR_F(PRG_CONF_CMD);
PRG_FLD_RDR_F(PRG_EXIT_CMD);
PRG_FLD_RDR_F(PRG_SHOW_DATA_CMD);
PRG_FLD_RDR_F(PRG_SAVE_CMD);
#endif