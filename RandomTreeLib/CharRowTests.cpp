#include "CppUnitTest.h"
#include "stdafx.h"
extern "C" {
#include "../RandomTreeLib/CharRow.h"
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{
	TEST_CLASS(CharRowTests)
	{

	public:
		TEST_METHOD(AppendCharRow_Twice_ReturnsProperString)
		{
			CharRow row;
			CrInit(&row);
			const auto fstChar = "class class class";
			CrAppend(&row, fstChar, strlen(fstChar));
			CrAppend(&row, fstChar, strlen(fstChar));
			Assert::AreEqual(std::string(fstChar) + std::string(fstChar), std::string(row.Data));
			CrFreeMemory(&row);
		}
	};
}