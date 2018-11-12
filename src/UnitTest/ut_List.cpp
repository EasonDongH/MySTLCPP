#include "stdafx.h"
#include "CppUnitTest.h"

#include "../list/List.h"
#include "../list/List.cpp"

#include <random>
#include <time.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest_List)
	{
		
	public:
		
		TEST_METHOD(Test_insert)
		{
			// TODO: �ڴ�������Դ���
			List<int> L;
			int i;
			const int size = 10;
			for ( i = 0; i < size; i++)
				L.insertAsLast(i);
			for ( i = 0; i < size; i++)
				if (L[i] != i) 
					break;
			
			Assert::IsTrue(i==size);
		}

		TEST_METHOD(Test_sort_And_disOrdered)
		{
			// TODO: �ڴ�������Դ���
			List<int> L;
			const int size = 10000;
			srand(unsigned(time(NULL)));
			for (int i = 0; i < size; i++)
				L.insertAsLast(rand()% (size*10));

			L.sort();

			Assert::IsTrue(!L.disOrdered());
		}

		TEST_METHOD(Test_reverse)
		{
			// TODO: �ڴ�������Դ���
			int j, size[] = { 1,2,3,5,8,10,51,100,1337 };//��Ƽ�����ģ���漰��С��ż��������������
			bool flag = true;
			for (int i = 0; i < sizeof(size) / sizeof(size[0]); ++i) {
				List<int> L;
				for ( j = 1; j <= size[i]; ++j)
					L.insertAsLast(j);
				L.reverse();
				ListNodePosi(int) trail = L.last()->succ;
				--j;
				for (auto p = L.first(); p != trail; p = p->succ, --j) {
					if (p->data != j) {
						flag = false;
						break;
					}
				}
				if (!flag)
					break;
			}

			Assert::IsTrue(flag);
		}

	};
}