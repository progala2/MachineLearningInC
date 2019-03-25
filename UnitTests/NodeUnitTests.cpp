#include "stdafx.h"
#include "CppUnitTest.h"
extern "C" {
#include "../RandomTreeLib/Node.h"
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(NodeUnitTests)
	{
	public:
		
		TEST_METHOD(CreateNodeParent_StructFilled)
		{
			int classNumber[2] {10, 20};
			const auto valueSeparator = 2.;
			const auto node = NdCreateParent(1, valueSeparator, classNumber, 2, NdCreateLeaf(classNumber, 2) , NdCreateLeaf(classNumber, 2));
			Assert::AreEqual(valueSeparator, node->ParameterValueSeparator);
		}

	};
}