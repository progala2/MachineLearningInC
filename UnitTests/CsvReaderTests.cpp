#include "CppUnitTest.h"
#include "stdafx.h"
extern "C" {
#include "../RandomTreeLib/CsvReader.h"
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{
	TEST_CLASS(CsvReaderTests)
	{
		template<typename T>
		static bool ThrowsException(T* pointer)
		{
			try
			{
				auto test = *pointer;
			}
			catch (...)
			{
				return true;
			}
			return false;
		}


	public:

		TEST_METHOD(ReadCsvTable_ReturnsCsvTable)
		{
			const auto linesNr = 50;
			std::stringstream ss;
			const auto headerLine = "class, a, b, c, d";
			ss << headerLine << "\n";
			const auto lines = static_cast<char**>(malloc(sizeof(char*) * linesNr));
			for (auto i = 0; i < linesNr; ++i)
			{
				lines[i] = static_cast<char*>(malloc(sizeof(char) * 20));
				snprintf(lines[i], 20, "1, 2.3, 3, 4.567, 5");
				ss << lines[i] << "\n";
			}

			FILE* fp;
			fopen_s(&fp,"ReadCsvFile_ReturnsTable.txt", "w+");
			fprintf(fp, ss.str().c_str());
			fclose(fp);

			fopen_s(&fp,"ReadCsvFile_ReturnsTable.txt", "r");
			const auto table = TReadFile(fp, 5);
			fclose(fp);

			const auto csvTable = CsvReadTable(table);
			Assert::AreEqual("class", csvTable->ClassName);
			/*for (auto i = 0; i < linesNr; ++i)
			{
				Assert::AreEqual(static_cast<const char*>(lines[i]), static_cast<const char*>(table->Data[i]->Data));
				free(lines[i]);
			}*/
			free(lines);

			TFreeMemory(table, true);
		}

		TEST_METHOD(ReadCsvFile_ReturnsCharsTable)
		{
			const auto linesNr = 50;
			std::stringstream ss;
			const auto lines = static_cast<char**>(malloc(sizeof(char*) * linesNr));
			for (auto i = 0; i < linesNr; ++i)
			{
				lines[i] = static_cast<char*>(malloc(sizeof(char) * 20));
				snprintf(lines[i], 20, "long line %d", i);
				ss << lines[i] << "\n";
			}

			FILE* fp;
			fopen_s(&fp,"ReadCsvFile_ReturnsTable.txt", "w+");
			fprintf(fp, ss.str().c_str());
			fclose(fp);

			fopen_s(&fp,"ReadCsvFile_ReturnsTable.txt", "r");
			const auto table = TReadFile(fp, 5);
			fclose(fp);

			for (auto i = 0; i < linesNr; ++i)
			{
				Assert::AreEqual(static_cast<const char*>(lines[i]), static_cast<const char*>(table->Data[i]->Data));
				free(lines[i]);
			}
			free(lines);

			TFreeMemory(table, true);
		}

	};
}