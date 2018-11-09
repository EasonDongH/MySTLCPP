#include "stdafx.h"
#include "CppUnitTest.h"

#include "../list/List.h"
#include "../list/List.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest_List)
	{
		
	public:
		
		TEST_METHOD(Test_insert)
		{
			// TODO: 在此输入测试代码
			List<int> L;
			for (int i = 0; i < 10; i++)
				L.insertAsLast(i);
			bool flag = true;
			for (int i = 0; i < 10; i++)
				flag = L[i] == i;
			
			Assert::IsTrue(flag);
		}

		TEST_METHOD(Test_reverse)
		{
			// TODO: 在此输入测试代码
			List<int> L;
			for (int i = 0; i < 10; i++)
				L.insertAsLast(i);
			L.reverse();
			bool flag = true;
			for (int i = 0; i < 10; i++)
				flag = L[i] == 10-i-1;

			Assert::IsTrue(flag);
		}

	};
}