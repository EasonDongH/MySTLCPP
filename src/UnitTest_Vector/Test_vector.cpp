#include "stdafx.h"
#include "CppUnitTest.h"
#include "../vector/vector.h"
#include "../vector/vector.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest_Vector
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: �ڴ�������Դ���
			Vector<int> V;
			Assert::IsTrue(true);
		}

	};
}