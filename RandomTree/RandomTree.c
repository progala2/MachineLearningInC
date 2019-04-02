// RandomTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "Program.h"
#include <stdlib.h>

int main()
{
	printf("Hello Traveler!\n");

	Program* program;
	srand((unsigned)__TIME__);
	if ((program = PrgLoadData()) == NULL)
		return -1;
	return PrgMenuLoop(program) ? 0: -1;
}
