#include "stdafx.h"
#include "CppUnitTest.h"
#include "Helpers.h"

extern "C" {
#include "../RandomTreeLib/ReadLearnData.h"
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

		TEST_METHOD(ReadLearnData_ReturnsLearnData)
		{
			const auto lines = Helpers::GenerateCsvFile("ReadCsvFile_ReturnsTable.txt");
			const auto lines2 = Helpers::GenerateCsvFile("ReadCsvFile_ReturnsTable2.txt");

			FILE* fp;
			fopen_s(&fp, "ReadCsvFile_ReturnsTable.txt", "r");
			auto table = TReadFile(fp, 5);
			fclose(fp);

			fopen_s(&fp, "ReadCsvFile_ReturnsTable2.txt", "r");
			auto table2 = TReadFile(fp, 5);
			fclose(fp);

			auto csvTable = LrnReadData(table, table2);

			TFreeMemory(&table, true);
			TFreeMemory(&table2, true);
			Assert::AreEqual("class", csvTable->ClassName);
			Assert::IsNotNull(csvTable->Classes);
			Assert::AreEqual(3u, csvTable->Classes->VecBase.Size, L"Classes count is wrong.");
			for (unsigned i = 0; i < lines.size(); ++i)
			{
				Assert::IsTrue(static_cast<int>(lines[i][0]) - csvTable->ClassesColumn->Data[i] == 0, L"Wrong class name");
				for (uint j = 0; j < 3; ++j)
				{
					if (csvTable->Classes->Table[j] == csvTable->Classes->Table[csvTable->ClassesColumn->Data[i]])
						Assert::IsTrue(csvTable->ClassesColumn->Data[i] == j, L"Wrong class value");
				}
				const double d = 0.00000001;
				Assert::IsTrue(fabs(lines[i][1] - csvTable->Parameters[0].Column[i]) < d);
				Assert::IsTrue(fabs(lines[i][2] - csvTable->Parameters[1].Column[i]) < d);
				Assert::IsTrue(fabs(lines[i][3] - csvTable->Parameters[2].Column[i]) < d);
				Assert::IsTrue(fabs(lines[i][4] - csvTable->Parameters[3].Column[i]) < d);
				
			}
			LrnFreeMemory(&csvTable);
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
			if (fp == nullptr)
				return;
			fprintf(fp, "%s", ss.str().c_str());
			fclose(fp);

			fopen_s(&fp, "ReadCsvFile_ReturnsTable.txt", "r");
			if (fp == nullptr)
				return;
			auto table = TReadFile(fp, 5);
			fclose(fp);

			for (auto i = 0; i < linesNr; ++i)
			{
				Assert::AreEqual(static_cast<const char*>(lines[i]), static_cast<const char*>(table->Table[i]->Data));
				free(lines[i]);
			}
			free(lines);

			TFreeMemory(&table, true);
		}

	};
}