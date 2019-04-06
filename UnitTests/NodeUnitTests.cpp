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
		
		/*TEST_METHOD(CreateNodeParent_StructFilled)
		{
			double classNumber[2] {10, 20};
			const double valueSeparator[2] = { 2. };
			const auto node = TreeCreateRoot(1, valueSeparator, TreeCreateLeaf(classNumber, 2, 1, 15), TreeCreateLeaf(classNumber, 2, 1, 15));
			Assert::AreEqual(valueSeparator, node->ParameterValueSeparators);
		}*/

	};
}