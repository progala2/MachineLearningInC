#ifndef CsvReader_H_INCLUDE_GUARD
#define CsvReader_H_INCLUDE_GUARD
#include "CharsTable.h"
#include "CsvTable.h"

LearnData* LrnReadData(const CharsTable* trainingTable, const CharsTable* testTable);

void LrnPrintTestAndTrainingData_F(FILE* const stream, const LearnData* const table);

void LrnPrintTestAndTrainingData(const LearnData* const table);
#endif
