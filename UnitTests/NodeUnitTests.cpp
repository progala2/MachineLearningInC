#include "CppUnitTest.h"
#include "stdafx.h"
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
			int class_number[2] {10, 20};
			const auto value_separator = 2;
			const auto node = CreateNodeParent(1, value_separator, true, class_number, CreateNodeLeaf(class_number), CreateNodeLeaf(class_number));
			Assert::AreEqual(value_separator, node->ParameterValueSeparator);
		}

	};
}