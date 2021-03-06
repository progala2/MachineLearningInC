#ifndef CsvReader_H_INCLUDE_GUARD
#define CsvReader_H_INCLUDE_GUARD
#include "CharsTable.h"
#include "LearnData.h"


LearnData* LrnReadData(const CharsTable* trainingTable, const CharsTable* testTable);
LearnData* LrnReadData_NoTest(const CharsTable* trainingTable);

void LrnPrintTestAndTrainingData(const LearnData* const table);
#endif
