#include "stdafx.h"
#include "CppUnitTest.h"
#include "Helpers.h"

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
			const auto lines = Helpers::GenerateCsvFile("ReadCsvFile_ReturnsTable.txt");

			FILE* fp;
			fopen_s(&fp, "ReadCsvFile_ReturnsTable.txt", "r");
			const auto table = TReadFile(fp, 5);
			fclose(fp);

			auto csvTable = CsvReadTable(table);

			TFreeMemory(table, true);
			Assert::AreEqual("class", csvTable->ClassName);
			for (unsigned i = 0; i < lines.size(); ++i)
			{
				Assert::IsTrue(static_cast<int>(lines[i][0]) - strtol(csvTable->ClassesColumn[i].Name, nullptr, 10) == 0);
				Assert::IsTrue(static_cast<int>(lines[i][0]) - csvTable->ClassesColumn[i].Value == 0);
				Assert::IsTrue(fabs(lines[i][1] - csvTable->Parameters[0].Column[i]) < 0.00000001);
				Assert::IsTrue(fabs(lines[i][2] - csvTable->Parameters[1].Column[i]) < 0.00000001);
				Assert::IsTrue(fabs(lines[i][3] - csvTable->Parameters[1].Column[i]) < 0.00000001);
				Assert::IsTrue(fabs(lines[i][4] - csvTable->Parameters[1].Column[i]) < 0.00000001);
				//Assert::AreEqual(0, csvTable->ClassColumn)
			}
			CsvFreeMemory(&csvTable);
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
			fopen_s(&fp, "ReadCsvFile_ReturnsTable.txt", "w+");
			fprintf(fp, "%s", ss.str().c_str());
			fclose(fp);

			fopen_s(&fp, "ReadCsvFile_ReturnsTable.txt", "r");
			const auto table = TReadFile(fp, 5);
			fclose(fp);

			for (auto i = 0; i < linesNr; ++i)
			{
				Assert::AreEqual(static_cast<const char*>(lines[i]), static_cast<const char*>(table->Table[i]->Data));
				free(lines[i]);
			}
			free(lines);

			TFreeMemory(table, true);
		}

	};
}