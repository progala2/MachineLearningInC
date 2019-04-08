#include "Program.h"
#include <stdlib.h>
#include "../RandomTreeLib/Node.h"
#include "../RandomTreeLib/NodeGenerator.h"
#include "../RandomTreeLib/Forest.h"

#define BUFFER_LEN 255u
#define COMMANDS_LEN 5u
#define CONFIG_DEFAULT_CONFIG_FILE "config.cfg"
#define PGR_CH_TUPLE(param, description)  {XSTRIFY(param), PRG_FLD_RDR_NAME(param), description}

static PrgCommandHandler _commands[COMMANDS_LEN] = {
	PGR_CH_TUPLE(PRG_HELP_CMD, "List commands."),
	PGR_CH_TUPLE(PRG_RUN_CMD, "Train a model."),
	PGR_CH_TUPLE(PRG_CONF_CMD, "Show current settings and provide new one if necessary."),
	PGR_CH_TUPLE(PRG_EXIT_CMD, "Exit the program."),
	PGR_CH_TUPLE(PRG_SHOW_DATA_CMD, "Show given test and training data."),
};

Program* PrgLoadData()
{
	RtConfigs* configs = NULL;
	LearnData* lrnData;
	char buffer[BUFFER_LEN];
	while (1)
	{
		printf("Give me your config file! (Max path size: 255; Enter 'd' for default name - " CONFIG_DEFAULT_CONFIG_FILE  "): ");
		if (scanf_s("%254s", buffer, BUFFER_LEN) < 1)
		{
			printf("Something wrong with your filename...\n");
			continue;
		}
		if (buffer[0] == 'd')
			strcpy_s(buffer, strlen(CONFIG_DEFAULT_CONFIG_FILE) + 1, CONFIG_DEFAULT_CONFIG_FILE);

		FILE * fp = NULL;
		if (fopen_s(&fp, buffer, "r") < 0 || fp == NULL)
		{
			printf("Something wrong with your config file...\n");
			fclose(fp);
			continue;
		}
		configs = RtReadConfigFromFile(fp);
		if (configs == NULL)
		{
			printf("Something wrong with your config file...\nEnsure that you don't put spaces next to '='\n");
			fclose(fp);
			continue;
		}
		fclose(fp);

		if (fopen_s(&fp, configs->TrainingFileName, "r") < 0 || fp == NULL)
		{
			printf("Something wrong with your training csv file...\n");
			RtFreeMemory(&configs);
			continue;
		}
		CharsTable* charsTable = NULL;
		if ((charsTable = TReadFile(fp, 1024)) == NULL)
		{
			printf("Something wrong with your training file...\nEnsure that commas are used as separators and the numbers of columns in each row\n");
			printf("You have to provide at least two columns: first is class columns and the rest are used as parameters\n");
			fclose(fp);
			RtFreeMemory(&configs);
			continue;
		}
		fclose(fp);

		if (configs->TestFileName == CFG_TEST_FILE_NAME)
		{
			if ((lrnData = LrnReadData_NoTest(charsTable)) == NULL)
			{
				printf("You have to provide at least two columns: first is class columns and the rest are used as parameters\n");
				RtFreeMemory(&configs);
				TFreeMemory(&charsTable, true);
				continue;
			}
		}
		else
		{
			if (fopen_s(&fp, configs->TestFileName, "r") < 0 || fp == NULL)
			{
				printf("Something wrong with your test csv file...\n");
				RtFreeMemory(&configs);
				continue;
			}
			CharsTable* testCharsTable = NULL;
			if ((testCharsTable = TReadFile(fp, 1024)) == NULL ||
				(lrnData = LrnReadData(charsTable, testCharsTable)) == NULL)
			{
				printf("Something wrong with your test file...\nEnsure that commas are used as separators and the numbers of columns in each row\n");
				printf("You have to provide at least two columns: first is class columns and the rest are used as parameters\n");
				fclose(fp);
				RtFreeMemory(&configs);
				TFreeMemory(&charsTable, true);
				TFreeMemory(&testCharsTable, true);
				continue;
			}
			TFreeMemory(&testCharsTable, true);
			fclose(fp);
		}

		TFreeMemory(&charsTable, true);

		break;
	}
	while (1)
	{
		printf("Do you want to normalize the data?(y/n)\n");
		if (scanf_s("%1s", buffer, (size_t)255) < 1 || buffer[0] != 'y' && buffer[0] != 'n')
		{
			printf("YES OR NO?!\n");
			continue;
		}
		if (buffer[0] != 'n')
		{
			LrnNormalize(lrnData);
		}
		break;
	}
	Program* _malloc(sizeof(Program), program);
	program->Configs = configs;
	_glConfigs = configs;
	program->LearnData = lrnData;
	return program;
}

bool PrgMenuLoop(Program* program)
{
	char buffer[BUFFER_LEN];
	while (1)
	{
		printf("What to do now?\n");
		printf("Write help for commands list.\n");

		if (scanf_s("%254s", buffer, (size_t)BUFFER_LEN) == 0)
			continue;

		buffer[254] = 0;
		for (uint i = 0; i < COMMANDS_LEN; ++i)
		{
			if (strcmp(buffer, _commands[i].Name) == 0)
			{
				if (!_commands[i].Handler(program))
					return true;
				break;
			}
		}
	}
}

void PrgFree(Program** const program)
{
	if (*program == NULL)
		return;
	RtFreeMemory(&(*program)->Configs);
	LrnFreeMemory(&(*program)->LearnData);
	free(*program);
	*program = NULL;
}

PRG_FLD_RDR_F(PRG_HELP_CMD)
{
	for (uint i = 0; i < COMMANDS_LEN; ++i)
	{
		printf("%s: %s\n", _commands[i].Name, _commands[i].Description);
	}
	return true;
}

PRG_FLD_RDR_F(PRG_RUN_CMD)
{
	if (program->LearnData->TestData.RowsCount == 0 || _glConfigs->CFG_FLD_FORCE_TEST_EXTRACT)
		LrnExtractTestData(program->LearnData);

	Forest* forest = FrstGenerateForest(program->LearnData);
	ConfMatrix * matrix = FrstCalculateOnTestData(forest, program->LearnData);
	printf("Test data: \n");
	CmPrint(matrix);
	printf("Error rate: %lf\n", CmCalculateError(matrix));
	CmFree(&matrix);
	matrix = FrstCalculateOnTrainingData(forest, program->LearnData);
	printf("Training data: \n");	
	CmPrint(matrix);
	printf("Error rate: %lf\n", CmCalculateError(matrix));
	FrstFree(&forest);
	CmFree(&matrix);
	return true;
}

PRG_FLD_RDR_F(PRG_CONF_CMD)
{
	char buffer[BUFFER_LEN];
	RtPrintAllSettings(program->Configs);
	printf("Provide new settings.(ex. 'TreeCount=60')\n To stop type: Ctrl+z, Enter, Enter.\n");
	while (scanf_s("%254s", buffer, BUFFER_LEN) > 0)
	{
		RtSetUpPropertyFromString(program->Configs, buffer);
		RtPrintAllSettings(program->Configs);
	}
	return true;
}

PRG_FLD_RDR_F(PRG_SHOW_DATA_CMD)
{
	LrnPrintTestAndTrainingData(program->LearnData);
	return true;
}

PRG_FLD_RDR_F(PRG_EXIT_CMD)
{
	return false;
}
