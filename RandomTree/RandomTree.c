// RandomTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "../RandomTreeLib/CsvReader.h"
#include "../RandomTreeLib/NodeGenerator.h"

int main()
{
	char buffer[255];
	printf("Hello Traveler!\n");
	CsvTable * table;
	RtConfigs* configs;

	srand((unsigned)buffer);
	while (1)
	{
		printf("Give me your config file! (Max path size: 255): ");
		if (scanf_s("%254s", buffer, (size_t)255) < 1)
		{
			printf("Something wrong with your filename...\n");
			continue;
		}
		FILE* fp;
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

		if (fopen_s(&fp, configs->FileName, "r") < 0 || fp == NULL)
		{
			printf("Something wrong with your csv file...\n");
			fclose(fp);
			free(configs);
			continue;
		}
		CharsTable* charsTable;
		if ((charsTable = TReadFile(fp, 1024)) < 0 ||
			 (table = CsvReadTable(charsTable)) < 0)
		{
			printf("Something wrong with you file...\nEnsure that commas are used as separators and the numbers of columns in each row\n");
			printf("You have to provide at least two columns: first is class columns and the rest are used as parameters\n");
			fclose(fp);
			free(configs);
			free(charsTable);
			continue;
		}
		TFreeMemory(charsTable, true);
		free(charsTable);
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
			CsvNormalize(table);
		}
		printf("%s, ", table->ClassName);
		for (uint j = 0; j < table->ParametersCount; j++)
		{
			printf("%s, ", table->Headers[j]);
		}
		printf("\n");
		break;
	}
	for (uint i = 0; i < table->RowsCount; i++)
	{
		printf("%d, ", table->ClassesColumn[i].Value);
		for (uint j = 0; j < table->ParametersCount; j++)
		{
			printf("%lf, ", table->Parameters[j].Column[i]);
		}
		printf("\n");
	}
	Root** forest = NdGenerateForest(configs, table);
	for (uint i = 0; i < configs->TreeCount; ++i)
	{
		TreeFree(&forest[i]);	
	}
	

}