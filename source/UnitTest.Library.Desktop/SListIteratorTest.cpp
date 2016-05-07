#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListIteratorTest)
	{
	public:
		TEST_METHOD_INITIALIZE(PrepForTests)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&startMemState);
#endif
		}

#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_CLEANUP(CleanupTestObjects)
		{
			intSList.Clear();
			intPtrSList.Clear();
			fooSList.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(SListIteratorTestEquality)
		{
			//int tests
			SList<int>::Iterator intIterator, otherIntIterator;
			Assert::IsTrue(intIterator == otherIntIterator);

			intIterator = intSList.begin();
			otherIntIterator = intSList.end();

			Assert::IsTrue(intIterator == otherIntIterator);

			intSList.PushFront(20);
			intSList.PushBack(30);
			intSList.PushFront(40);

			intIterator = intSList.begin();
			otherIntIterator = intSList.Find(30);

			Assert::IsTrue(intIterator != otherIntIterator);

			intIterator = intSList.Find(30);
			Assert::IsTrue(intIterator == otherIntIterator);

			//int* tests
			intPtrSList.PushFront(&testArray[0]);
			intPtrSList.PushBack(&testArray[1]);
			
			auto intPtrIterator = intPtrSList.Find(&testArray[1]);

			intPtrSList.PushFront(&testArray[1]);

			auto otherIntPtrIterator = intPtrSList.Find(&testArray[1]);

			Assert::IsTrue(intPtrIterator != otherIntPtrIterator);

			intPtrIterator = intPtrSList.begin();

			Assert::IsTrue(intPtrIterator == otherIntPtrIterator);

			//Foo tests
			SList<Foo> otherFooSList;

			fooSList.PushFront(Foo(12));
			fooSList.PushBack(Foo(13));

			otherFooSList.PushFront(fooSList.Front());
			otherFooSList.PushBack(fooSList.Back());

			auto fooIterator = fooSList.begin();
			auto otherFooIterator = otherFooSList.begin();

			Assert::IsTrue(fooIterator != otherFooIterator);

			otherFooIterator = fooSList.begin();
			Assert::IsTrue(fooIterator == otherFooIterator);
		}

		TEST_METHOD(SListIteratorTestInvalidOperations)
		{
			SList<int>::Iterator intIterator;
			SList<int*>::Iterator intPtrIterator;
			SList<Foo>::Iterator fooIterator;

			auto badDereference = [&] { (*intIterator); };
			auto badPrefixIncrement = [&] { ++intPtrIterator;  };
			auto badPostfixIncrement = [&] { fooIterator++; };

			Assert::ExpectException<std::exception>(badDereference);
			Assert::ExpectException<std::exception>(badPrefixIncrement);
			Assert::ExpectException<std::exception>(badPostfixIncrement);

			intIterator = intSList.begin();
			intPtrIterator = intPtrSList.begin();
			fooIterator = fooSList.begin();

			Assert::IsTrue(intSList.IsEmpty());
			Assert::IsTrue(intPtrSList.IsEmpty());
			Assert::IsTrue(fooSList.IsEmpty());

			Assert::ExpectException<std::exception>(badDereference);
			Assert::ExpectException<std::exception>(badPrefixIncrement);
			Assert::ExpectException<std::exception>(badPostfixIncrement);
		}

		TEST_METHOD(SListIteratorTestPostfixIncrement)
		{
			int i;

			for (i = 1; i < 6; i++)
			{
				intSList.PushBack(i);
				intPtrSList.PushBack(&testArray[i - 1]);
				fooSList.PushBack(Foo(i));
			}

			//int test
			i = 0;

			for (auto intIterator = intSList.begin(); intIterator != intSList.end(); intIterator++)
			{
				Assert::AreEqual(testArray[i++], (*intIterator));
			}

			//int* test
			i = 0;

			for (auto intPtrIterator = intPtrSList.begin(); intPtrIterator != intPtrSList.end(); intPtrIterator++)
			{
				Assert::AreEqual(&testArray[i++], (*intPtrIterator));
			}

			//Foo test
			i = 0;

			for (auto fooIterator = fooSList.begin(); fooIterator != fooSList.end(); fooIterator++)
			{
				Assert::AreEqual(testArray[i++], (*fooIterator).Data());
			}
		}
	private:
		static SList<int> intSList;
		static SList<int*> intPtrSList;
		static SList<Foo> fooSList;
		static int testArray[5];

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

	SList<int> SListIteratorTest::intSList;
	SList<int*> SListIteratorTest::intPtrSList;
	SList<Foo> SListIteratorTest::fooSList;
	int SListIteratorTest::testArray[] = { 1, 2, 3, 4, 5 };

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState SListIteratorTest::startMemState;
#endif
}