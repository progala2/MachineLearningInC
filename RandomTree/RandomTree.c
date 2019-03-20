// RandomTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include "../RandomTreeLib/CsvReader.h"

int Comp(const void * elem1, const void * elem2)
{
	int* f = (int*)elem1;
	int* s = (int*)elem2;
	if (f[0] > s[0]) return  1;
	if (f[0] < s[0]) return -1;
	return 0;
}

int main()
{
	char buffer[255];
	printf("Hello Traveler!\n");
	CharsTable* charsTable = NULL;
	while (1)
	{
		printf("Give me your CSV file! (Max path size: 255): ");
		if (scanf_s("%254s", buffer, (size_t)255) < 1)
		{
			printf("Something wrong with your filename...\n");
			continue;
		}
		FILE* fp;
		if (fopen_s(&fp, buffer, "r") < 0)
		{
			printf("Something wrong with you file...\n");
			continue;
		}
		if ((charsTable = TReadFile(fp, 1024)) < 0 ||
			CsvTable * table = CsvReadTable(charsTable);)
		{
			printf("Something wrong with you file...\nEnsure that commas are used as separators and the numbers of columns in each row\n");
			printf("You have to provide at least two columns: first is class columns and the rest are used as parameters\n");
			continue;
		}
	}
	while (1)
	{
		printf("Do you want to normalize the data?(y/n)\n");
		if (scanf_s("%1s", buffer, (size_t)255) < 1 || buffer[0] != 'y' && buffer[0] != 'n')
		{
			printf("YES OR NO?!\n");
			continue;
		}

	}

}
//
//int data[45][2];
//int classNumbers[2];
//int classNumbers1[2];
//int classNumbers2[2];
//classNumbers[0] = 0;
//classNumbers[1] = 0;
//time_t tt;
//const int seed = time(&tt);
//srand(seed);
//for (int i = 0; i < 45; ++i)
//{
//	data[i][0] = rand() % 20;
//	data[i][1] = rand() % 2;
//	if (data[i][1] == 1)
//		classNumbers[1]++;
//	else if (data[i][1] == 0)
//		classNumbers[0]++;
//}
//printf("%d\n %d", sizeof(data) / sizeof(*data), sizeof(*data));
//qsort(data, sizeof(data) / sizeof(*data), sizeof(*data), Comp);
//
//int count0 = 0, count1 = 0;
//for (int i = 0; i < 23; ++i)
//{
//	if (data[i][1] == 1)
//		count1++;
//	else if (data[i][1] == 0)
//		count0++;
//}
//
//classNumbers1[0] = count0;
//classNumbers1[1] = count1;
//classNumbers2[0] = classNumbers[0] - count0;
//classNumbers2[1] = classNumbers[1] - count1;
//
//struct Node* nd;
//if (count1 > count0)
//{
//	nd = CreateNodeParent(0, data[22][0], false, classNumbers, CreateNodeLeaf(classNumbers2), CreateNodeLeaf(classNumbers1));
//}
//else
//{
//	nd = CreateNodeParent(0, data[22][0], true, classNumbers, CreateNodeLeaf(classNumbers1), CreateNodeLeaf(classNumbers2));
//}
//DestroyNode(nd);