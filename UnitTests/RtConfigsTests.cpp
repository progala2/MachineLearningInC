#include "stdafx.h"
#include "CppUnitTest.h"

extern "C" {
	#include "../RandomTreeLib/RtConfigs.h"
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{
	TEST_CLASS(CsvReaderTests)
	{

	public:

		TEST_METHOD(RtReadConfig_ReturnsConfigs)
		{
			const auto fileName = "RtReadConfig_ReturnsConfigs.txt";
			const auto fileNameConfig = "RtReadConfig_ReturnsConfigs.config";
			std::stringstream ss;		

			ss << "FileName=" << fileName << "\n";
			ss << "TreeCount=" << 14 << "\n";
			FILE* fp;
			fopen_s(&fp,fileNameConfig, "w+");
			fprintf(fp, "%s", ss.str().c_str());
			fclose(fp);

			fopen_s(&fp,fileNameConfig, "r");
			
			auto config = RtReadConfig(fp);
			fclose(fp);
			
			Assert::AreEqual("RtReadConfig_ReturnsConfigs.txt", config->TrainingFileName);
			Assert::AreEqual(14u, config->TreeCount);
			RtFreeMemory(&config);
		}

	};
}