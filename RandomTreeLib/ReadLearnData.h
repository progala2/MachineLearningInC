#ifndef CsvReader_H_INCLUDE_GUARD
#define CsvReader_H_INCLUDE_GUARD
#include "CharsTable.h"
#include "CsvTable.h"

LearnData* LrnReadData(const CharsTable* trainingTable, const CharsTable* testTable);

#endif