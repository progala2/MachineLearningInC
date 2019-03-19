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

		TEST_METHOD(CsvTableDestroy_HeadersDestroyed)
		{
			const auto headersNr = 5;
			auto table = static_cast<struct CsvTable*>(malloc(sizeof(struct CsvTable)));
			const auto headers = static_cast<char**>(malloc(sizeof(char*) * headersNr));
			for (int i = 0; i < headersNr; ++i)
			{
				headers[i] = static_cast<char*>(malloc(sizeof(char) * headersNr));
				headers[i][0] = 'a';
			}
			table->HeadersLen = headersNr;
			table->Headers = headers;
			DestroyCsvTable(&table);
			Assert::IsNull(table);
			for (int i = 0; i < headersNr; ++i)
			{
				Assert::IsTrue(ThrowsException(headers[i]));
			}
		}

		TEST_METHOD(Read_OnlyHeaders_ReturnsTableWithHeaders)
		{
			auto className = "class";
			std::stringstream ss;
			ss << className << ",a,b\n";
			auto table = ReadCsv(ss.str().c_str());
			Assert::AreEqual("class", table->ClassName);

			DestroyCsvTable(&table);
		}

	};
}