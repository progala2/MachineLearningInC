#ifndef CsvReader_H_INCLUDE_GUARD
#define CsvReader_H_INCLUDE_GUARD

struct CsvTable
{
	int HeadersLen;
	long RowsNumber;
	char** Headers;
	char*** Parameters;
	char** ClassColumn;
	char* ClassName;
};

struct CsvTable* ReadCsv(const char* input);
void DestroyCsvTable(struct CsvTable** table);

#endif