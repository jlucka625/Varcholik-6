#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
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

		TEST_METHOD(SListTestDefaultConstructor)
		{
			Assert::IsTrue(intSList.IsEmpty());
			Assert::IsTrue(intPtrSList.IsEmpty());
			Assert::IsTrue(fooSList.IsEmpty());
		}

		TEST_METHOD(SListTestPushFront)
		{
			//int list
			intSList.PushFront(5);
			Assert::AreEqual(5, intSList.Front());
			Assert::AreEqual(intSList.Front(), intSList.Back());

			intSList.PushFront(13);
			Assert::AreEqual(13, intSList.Front());
			Assert::AreNotEqual(intSList.Front(), intSList.Back());

			//int* list
			int *testPointer = new int(69);

			intPtrSList.PushFront(testPointer);
			Assert::AreEqual(testPointer, intPtrSList.Front());
			Assert::AreEqual(intPtrSList.Front(), intPtrSList.Back());

			int *newTestPointer = new int(69);
			intPtrSList.PushFront(newTestPointer);
			Assert::AreEqual(newTestPointer, intPtrSList.Front());
			Assert::AreNotEqual(intPtrSList.Front(), intPtrSList.Back());

			delete testPointer;
			delete newTestPointer;

			//Foo list
			Foo testFoo(30);

			fooSList.PushFront(testFoo);
			Assert::IsTrue(testFoo == fooSList.Front());
			Assert::IsTrue(fooSList.Front() == fooSList.Back());

			testFoo.Data() = 24;
			fooSList.PushFront(testFoo);
			Assert::IsTrue(testFoo == fooSList.Front());
			Assert::IsFalse(fooSList.Front() == fooSList.Back());
		}

		TEST_METHOD(SListTestPopFront)
		{
			auto intPopFront = [&] { intSList.PopFront(); };
			auto intPtrPopFront = [&] { intPtrSList.PopFront(); };
			auto fooPopFront = [&] { fooSList.PopFront(); };

			Assert::ExpectException<std::exception>(intPopFront);
			Assert::ExpectException<std::exception>(intPtrPopFront);
			Assert::ExpectException<std::exception>(fooPopFront);

			//int list
			intSList.PushBack(10);
			Assert::IsFalse(intSList.IsEmpty());
			intSList.PopFront();
			Assert::IsTrue(intSList.IsEmpty());

			//int* list
			int* testPointer = new int(20);
			int* newTestPointer = new int(21);

			intPtrSList.PushFront(testPointer);
			intPtrSList.PushBack(newTestPointer);
			Assert::IsFalse(intPtrSList.IsEmpty());
			intPtrSList.PopFront();
			Assert::IsTrue(intPtrSList.Front() == intPtrSList.Back());
			intPtrSList.PopFront();
			Assert::IsTrue(intPtrSList.IsEmpty());

			delete testPointer;
			delete newTestPointer;

			//Foo list
			fooSList.PushFront(Foo(14));
			Assert::IsFalse(fooSList.IsEmpty());
			fooSList.PopFront();
			Assert::IsTrue(fooSList.IsEmpty());
		}

		TEST_METHOD(SListTestPushBack)
		{
			//int list
			int testInteger = 10;

			intSList.PushBack(testInteger);
			Assert::AreEqual(testInteger, intSList.Back());
			Assert::AreEqual(intSList.Front(), intSList.Back());

			testInteger = 15;
			intSList.PushBack(testInteger);
			Assert::AreEqual(testInteger, intSList.Back());
			Assert::AreNotEqual(intSList.Front(), intSList.Back());

			//int* list
			int *testPointer = new int(20);

			intPtrSList.PushBack(testPointer);
			Assert::AreEqual(testPointer, intPtrSList.Back());
			Assert::AreEqual(intPtrSList.Front(), intPtrSList.Back());

			intPtrSList.PushBack(&testInteger);
			Assert::AreEqual(&testInteger, intPtrSList.Back());
			Assert::AreNotEqual(intPtrSList.Front(), intPtrSList.Back());

			delete testPointer;

			//Foo list
			Foo testFoo(30);

			fooSList.PushBack(testFoo);
			Assert::IsTrue(testFoo == fooSList.Back());
			Assert::IsTrue(fooSList.Front() == fooSList.Back());

			testFoo.Data() = 50;
			fooSList.PushBack(testFoo);
			Assert::IsTrue(testFoo == fooSList.Back());
			Assert::IsFalse(fooSList.Front() == fooSList.Back());
		}

		TEST_METHOD(SListTestFind)
		{
			Foo testFoo(666);
			Foo otherTestFoo(555);

			Assert::IsTrue(intSList.Find(20) == intSList.end());
			Assert::IsTrue(intPtrSList.Find(&testArray[3]) == intPtrSList.end());
			Assert::IsTrue(fooSList.Find(testFoo) == fooSList.end());

			//int test
			intSList.PushFront(10);
			intSList.PushBack(15);
			intSList.PushFront(5);
			intSList.PushBack(20);

			Assert::IsTrue(intSList.Find(5) == intSList.begin());

			//int* tests
			intPtrSList.PushBack(&testArray[1]);
			intPtrSList.PushBack(&testArray[2]);
			intPtrSList.PushBack(&testArray[4]);

			auto intPtrIterator = intPtrSList.Find(&testArray[3]);

			Assert::IsTrue(intPtrIterator == intPtrSList.end());

			intPtrIterator = intPtrSList.Find(&testArray[2]);
			intPtrSList.InsertAfter(&testArray[3], intPtrIterator);

			Assert::IsTrue(intPtrSList.Find(&testArray[3]) != intPtrSList.end());

			//Foo tests
			fooSList.PushFront(Foo(111));
			fooSList.PushFront(Foo(222));
			fooSList.PushFront(Foo(333));
			fooSList.PushFront(Foo(444));
			fooSList.PushFront(otherTestFoo);

			SList<Foo> otherFooSList(fooSList);
			Assert::IsTrue(otherFooSList.Find(testFoo) == otherFooSList.end());

			otherFooSList.InsertAfter(testFoo, otherFooSList.Find(otherTestFoo));
			auto fooIterator = otherFooSList.Find(testFoo);

			Assert::IsTrue(otherFooSList.Find(testFoo) != otherFooSList.end());
		}

		TEST_METHOD(SListTestInsertAfter)
		{
			Foo testFoo(5);

			Assert::IsTrue(intSList.IsEmpty());
			Assert::IsTrue(intPtrSList.IsEmpty());
			Assert::IsTrue(fooSList.IsEmpty());

			//Attempts to insert with iterators of other lists
			SList<int> dummyIntSList;
			SList<int*> dummyIntPtrSList;
			SList<Foo> dummyFooSList;

			auto dummyIntIterator = dummyIntSList.begin();
			auto dummyIntPtrIterator = dummyIntPtrSList.begin();
			auto dummyFooIterator = dummyFooSList.begin();

			auto badIntInsertAfter = [&] { intSList.InsertAfter(0, dummyIntIterator); };
			auto badIntPtrInsertAfter = [&] { intPtrSList.InsertAfter(&testArray[4], dummyIntPtrIterator); };
			auto badFooInsertAfter = [&] { fooSList.InsertAfter(testFoo, dummyFooIterator); };

			Assert::ExpectException<std::exception>(badIntInsertAfter);
			Assert::ExpectException<std::exception>(badIntPtrInsertAfter);
			Assert::ExpectException<std::exception>(badFooInsertAfter);

			//int tests
			auto testIntIterator = intSList.begin();
			intSList.InsertAfter(5, testIntIterator);
			Assert::AreEqual(1U, intSList.Size());
			Assert::AreEqual(5, intSList.Front());

			testIntIterator = intSList.end();
			intSList.InsertAfter(6, testIntIterator);
			Assert::AreEqual(2U, intSList.Size());
			Assert::AreEqual(6, intSList.Back());

			testIntIterator = intSList.begin();
			intSList.InsertAfter(7, testIntIterator);
			Assert::AreEqual(3U, intSList.Size());
			
			testIntIterator = intSList.Find(7);
			Assert::IsTrue(testIntIterator != intSList.end());

			//int* tests
			auto testIntPtrIterator = intPtrSList.begin();
			intPtrSList.InsertAfter(&testArray[0], testIntPtrIterator);
			Assert::AreEqual(1U, intPtrSList.Size());
			Assert::AreEqual(&testArray[0], intPtrSList.Front());

			testIntPtrIterator = intPtrSList.end();
			intPtrSList.InsertAfter(&testArray[1], testIntPtrIterator);
			Assert::AreEqual(2U, intPtrSList.Size());
			Assert::AreEqual(&testArray[1], intPtrSList.Back());

			testIntPtrIterator = intPtrSList.begin();
			intPtrSList.InsertAfter(&testArray[2], testIntPtrIterator);
			Assert::AreEqual(3U, intPtrSList.Size());

			testIntPtrIterator = intPtrSList.Find(&testArray[2]);
			Assert::IsTrue(testIntPtrIterator != intPtrSList.end());

			//Foo tests
			auto testFooIterator = fooSList.begin();
			fooSList.InsertAfter(testFoo, testFooIterator);
			Assert::AreEqual(1U, fooSList.Size());
			Assert::IsTrue(testFoo == fooSList.Front());

			Foo newTestFoo(6);
			testFooIterator = fooSList.end();
			fooSList.InsertAfter(newTestFoo, testFooIterator);
			Assert::AreEqual(2U, fooSList.Size());
			Assert::IsTrue(newTestFoo == fooSList.Back());

			Foo newNewTestFoo(7);
			testFooIterator = fooSList.begin();
			fooSList.InsertAfter(newNewTestFoo, testFooIterator);
			Assert::AreEqual(3U, fooSList.Size());

			testFooIterator = fooSList.Find(newNewTestFoo);
			Assert::IsTrue(testFooIterator != fooSList.end());
		}

		TEST_METHOD(SListTestFrontOnEmptyList)
		{
			auto intFront = [&] { intSList.Front(); };
			auto intPtrFront = [&] { intPtrSList.Front(); };
			auto fooFront = [&] { fooSList.Front(); };

			Assert::ExpectException<std::exception>(intFront);
			Assert::ExpectException<std::exception>(intPtrFront);
			Assert::ExpectException<std::exception>(fooFront);
		}

		TEST_METHOD(SListTestBackOnEmptyList)
		{
			auto intBack = [&] { intSList.Back(); };
			auto intPtrBack = [&] { intPtrSList.Back(); };
			auto fooBack = [&] { fooSList.Back(); };

			Assert::ExpectException<std::exception>(intBack);
			Assert::ExpectException<std::exception>(intPtrBack);
			Assert::ExpectException<std::exception>(fooBack);
		}

		TEST_METHOD(SListTestSize)
		{
			std::uint32_t i;

			//int list
			Assert::IsTrue(intSList.IsEmpty());

			for (i = 0; i < 3; i++)
			{
				intSList.PushBack(i);
			}

			Assert::AreEqual(i, intSList.Size());

			//int* list
			Assert::IsTrue(intPtrSList.IsEmpty());

			int* testPointer = new int(7);

			for (i = 0; i < 6; i++)
			{
				intPtrSList.PushFront(testPointer);
			}

			Assert::AreEqual(i, intPtrSList.Size());

			delete testPointer;

			//Foo list
			for (i = 0; i < 4; i++)
			{
				fooSList.PushBack(Foo(21));
			}

			Assert::AreEqual(i, fooSList.Size());
		}

		TEST_METHOD(SListTestCopyConstructor)
		{
			//int list
			int testInteger = 100;

			intSList.PushFront(testInteger);
			intSList.PushBack(200);
			intSList.PushBack(300);

			SList<int> intSListCopy(intSList);

			Assert::IsTrue(intSList == intSListCopy);

			//int* list
			int* testPointer = new int(50);

			intPtrSList.PushBack(testPointer);
			intPtrSList.PushFront(&testInteger);
			intPtrSList.PushFront(testPointer);
			intPtrSList.PushBack(&(intSList.Back()));

			SList<int*> intPtrSListCopy(intPtrSList);
			Assert::IsTrue(intPtrSList == intPtrSListCopy);

			delete testPointer;

			//Foo list
			fooSList.PushBack(Foo(2));
			fooSList.PushFront(Foo(4));
			fooSList.PushFront(Foo(6));
			fooSList.PushBack(Foo(8));
			fooSList.PushFront(Foo(10));

			SList<Foo> fooSListCopy(fooSList);
			Assert::IsTrue(fooSList == fooSListCopy);
		}

		TEST_METHOD(SListTestEqualityOperator)
		{
			//int lists
			SList<int> newIntSList;

			Assert::IsTrue(intSList == newIntSList);

			intSList.PushBack(20);
			intSList.PushFront(17);

			newIntSList.PushFront(17);
			newIntSList.PushFront(13);
			newIntSList.PushBack(20);

			Assert::IsTrue(intSList != newIntSList);

			intSList.PushFront(14);

			Assert::IsTrue(intSList != newIntSList);

			intSList.PopFront();
			intSList.PushFront(13);

			Assert::IsTrue(intSList == newIntSList);

			//int* lists
			int testInteger = 10;

			SList<int*> newIntPtrSList;

			Assert::IsTrue(intPtrSList == newIntPtrSList);

			intPtrSList.PushFront(&intSList.Back());
			intPtrSList.PushBack(&intSList.Front());
			intPtrSList.PushFront(&newIntSList.Front());

			newIntPtrSList.PushFront(&intSList.Back());
			newIntPtrSList.PushBack(&testInteger);
			newIntPtrSList.PushFront(&newIntSList.Front());

			Assert::IsTrue(intPtrSList != newIntPtrSList);

			newIntPtrSList.Clear();
			newIntPtrSList.PushFront(&intSList.Back());
			newIntPtrSList.PushBack(&intSList.Front());
			newIntPtrSList.PushFront(&newIntSList.Front());

			Assert::IsTrue(intPtrSList == newIntPtrSList);

			//Foo lists
			SList<Foo> newFooSList;

			Assert::IsTrue(fooSList == newFooSList);

			fooSList.PushBack(Foo(12));
			fooSList.PushBack(Foo(13));
			fooSList.PushBack(Foo(14));

			Foo testFoo(15);

			newFooSList.PushBack(Foo(12));
			newFooSList.PushBack(Foo(13));
			newFooSList.PushBack(testFoo);

			Assert::IsTrue(fooSList != newFooSList);

			fooSList.Clear();
			fooSList = newFooSList;

			Assert::IsTrue(fooSList == newFooSList);
		}

		TEST_METHOD(SListTestAssignmentOperator)
		{
			//int lists
			intSList.PushFront(100);
			intSList.PushBack(200);
			intSList.PushBack(300);

			intSList = intSList;
			Assert::IsTrue(intSList == intSList);

			SList<int> newIntSList;
			newIntSList = intSList;
			Assert::IsTrue(intSList == newIntSList);

			newIntSList.PopFront();
			intSList.PushFront(400);
			newIntSList = intSList;
			Assert::IsTrue(intSList == newIntSList);

			//int* lists
			int* testPointer = new int(13);

			intPtrSList.PushBack(testPointer);
			intPtrSList.PushFront(testPointer);

			intPtrSList = intPtrSList;
			Assert::IsTrue(intPtrSList == intPtrSList);

			SList<int*> newIntPtrSList;
			newIntPtrSList = intPtrSList;
			Assert::IsTrue(intPtrSList == newIntPtrSList);

			*testPointer = 14;
			intPtrSList.PopFront();
			newIntPtrSList = intPtrSList;
			Assert::IsTrue(intPtrSList == newIntPtrSList);

			delete testPointer;

			//Foo lists
			fooSList.PushBack(24);
			fooSList.PushBack(26);
			fooSList.PushBack(28);
			fooSList.PushFront(30);

			fooSList = fooSList;
			Assert::IsTrue(fooSList == fooSList);

			SList<Foo> newFooSList;
			newFooSList = fooSList;
			Assert::IsTrue(fooSList == newFooSList);

			newFooSList.Front() = 32;
			newFooSList.Back() = 34;

			fooSList = newFooSList;
			Assert::IsTrue(newFooSList == fooSList);
		}

		TEST_METHOD(SListTestRemove)
		{
			//int tests
			intSList.PushBack(testArray[0]);
			intSList.Remove(testArray[0]);
			Assert::IsTrue(intSList.IsEmpty());

			intSList.PushBack(testArray[1]);
			intSList.PushBack(testArray[2]);
			intSList.Remove(testArray[2]);
			Assert::AreEqual(1U, intSList.Size());
			Assert::IsTrue(intSList.Front() == intSList.Back());

			intSList.PushFront(testArray[3]);
			intSList.PushBack(testArray[4]);
			intSList.Remove(testArray[1]);
			Assert::AreEqual(2U, intSList.Size());
			Assert::IsTrue(intSList.Find(testArray[1]) == intSList.end());

			//int* tests
			intPtrSList.PushBack(&testArray[0]);
			intPtrSList.PushFront(&testArray[1]);
			intPtrSList.PushFront(&testArray[2]);
			intPtrSList.PushBack(&testArray[3]);
			intPtrSList.PushFront(&testArray[4]);

			intPtrSList.Remove(&testArray[4]);
			Assert::AreEqual(&testArray[2], intPtrSList.Front());

			intPtrSList.Remove(&testArray[0]);
			Assert::IsTrue(intPtrSList.Find(&testArray[0]) == intPtrSList.end());

			intPtrSList.Remove(&testArray[1]);
			Assert::IsTrue(intPtrSList.Find(&testArray[1]) == intPtrSList.end());

			intPtrSList.Remove(&testArray[2]);
			Assert::AreEqual(intPtrSList.Front(), intPtrSList.Back());

			intPtrSList.Remove(&testArray[3]);
			Assert::IsTrue(intPtrSList.IsEmpty());

			//Foo tests
			Foo testFoo(testArray[0]);
			Foo newTestFoo(testArray[4]);

			fooSList.PushFront(testFoo);
			fooSList.PushFront(newTestFoo);
			fooSList.PushBack(newTestFoo);
			fooSList.PushBack(testFoo);

			fooSList.Remove(testFoo);
			Assert::IsTrue(testFoo == fooSList.Back());

			fooSList.Remove(newTestFoo);
			Assert::IsTrue(fooSList.Find(newTestFoo) != fooSList.end());
		}

		TEST_METHOD(SListTestClear)
		{
			//int list
			intSList.PushBack(10);
			intSList.PushBack(-300);
			intSList.PushBack(6);
			Assert::IsFalse(intSList.IsEmpty());
			intSList.Clear();
			Assert::IsTrue(intSList.IsEmpty());

			//int* list
			int *testPointer = new int(7);
			intPtrSList.PushFront(testPointer);
			intPtrSList.PushFront(testPointer);
			intPtrSList.PushFront(testPointer);
			Assert::IsFalse(intPtrSList.IsEmpty());
			intPtrSList.Clear();
			Assert::IsTrue(intPtrSList.IsEmpty());

			delete testPointer;

			//Foo list
			fooSList.PushFront(Foo(1));
			fooSList.PushBack(Foo(2));
			fooSList.PushFront(Foo(99999));
			Assert::IsFalse(fooSList.IsEmpty());
			fooSList.Clear();
			Assert::IsTrue(fooSList.IsEmpty());
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

	SList<int> SListTest::intSList;
	SList<int*> SListTest::intPtrSList;
	SList<Foo> SListTest::fooSList;
	int SListTest::testArray[] = { 1, 2, 3, 4, 5 };

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState SListTest::startMemState;
#endif
}