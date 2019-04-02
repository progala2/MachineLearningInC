#include "Program.h"
#include <stdlib.h>
#include "../RandomTreeLib/Node.h"
#include "../RandomTreeLib/NodeGenerator.h"

#define CONFIG_DEFAULT_CONFIG_FILE "config.cfg"

Program* PrgLoadData()
{
	RtConfigs* configs = NULL;
	LearnData* lrnData;
	char buffer[255];
	while (1)
	{
		printf("Give me your config file! (Max path size: 255; Enter 'd' for default name - " CONFIG_DEFAULT_CONFIG_FILE  "): ");
		if (scanf_s("%254s", buffer, (size_t)255) < 1)
		{
			printf("Something wrong with your filename...\n");
			continue;
		}
		if (buffer[0] == 'd')
			strcpy_s(buffer, strlen(CONFIG_DEFAULT_CONFIG_FILE) + 1, CONFIG_DEFAULT_CONFIG_FILE);

		FILE* fp = NULL;
		if (fopen_s(&fp, buffer, "r") < 0 || fp == NULL)
		{
			printf("Something wrong with your config file...\n");
			fclose(fp);
			continue;
		}
		configs = RtReadConfig(fp);
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
			fclose(fp);
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
			TFreeMemory(&charsTable, true);
			fclose(fp);
			return NULL;
		}

		if (fopen_s(&fp, configs->TestFileName, "r") < 0 || fp == NULL)
		{
			printf("Something wrong with your test csv file...\n");
			fclose(fp);
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
	Program* program = malloc(sizeof(Program));
	program->Configs = configs;
	program->LearnData = lrnData;
	return program;
}

bool PrgMenuLoop(Program* program)
{
	char buffer[255];

	buffer[0] = 'n';
	printf("Print data?(y/n)\n");
	scanf_s("%1s", buffer, (size_t)255);
	if (buffer[0] == 'y')
	{
		LrnPrintTestAndTrainingData(program->LearnData);
	}
	Root** forest = NdGenerateForest(program->Configs, program->LearnData);
	ConfMatrix* matrix = NdCalculateOnTestData((const Root**)forest, program->LearnData, program->Configs->TreeCount);
	CmPrint(matrix);
	for (uint i = 0; i < program->Configs->TreeCount; ++i)
	{
		TreeFree(&forest[i]);
	}
	CmFree(&matrix);
	return true;
}
