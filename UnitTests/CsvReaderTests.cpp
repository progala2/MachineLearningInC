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
			auto lines = static_cast<double**>(malloc(sizeof(double*) * linesNr));
			srand(1);
			for (auto i = 0; i < linesNr; ++i)
			{
				lines[i] = static_cast<double*>(malloc(sizeof(double) * 5));
				for (auto j = 0; j < 5; ++j)
				{
					lines[i][j] = rand()*0.001;
					ss << lines[i][j] << ",";
				}
				ss.seekp(-1, ss.cur);
				ss << "\n";
			}

			FILE* fp;
			fopen_s(&fp,"ReadCsvFile_ReturnsTable.txt", "w+");
			fprintf(fp, ss.str().c_str());
			fclose(fp);

			fopen_s(&fp,"ReadCsvFile_ReturnsTable.txt", "r");
			const auto table = TReadFile(fp, 5);
			fclose(fp);

			const auto csvTable = CsvReadTable(table);

			TFreeMemory(table, true);
			Assert::AreEqual("class", csvTable->ClassName);
			for (auto i = 0; i < linesNr; ++i)
			{
				Assert::IsTrue(fabs(lines[i][0] - csvTable->ClassColumn[i]) < 0.00000001);
				Assert::IsTrue(fabs(lines[i][1] - csvTable->Parameters[i][0]) < 0.00000001);
				Assert::IsTrue(fabs(lines[i][2] - csvTable->Parameters[i][1]) < 0.00000001);

				free(lines[i]);
			}
			free(lines);
			CsvFreeMemory(csvTable);
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