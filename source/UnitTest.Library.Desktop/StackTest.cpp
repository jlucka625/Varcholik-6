#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(StackTest)
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
			intStack.Clear();
			intPtrStack.Clear();
			fooStack.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(StackTestDefaultConstructor)
		{
			Assert::IsTrue(intStack.IsEmpty());
			Assert::IsTrue(intPtrStack.IsEmpty());
			Assert::IsTrue(fooStack.IsEmpty());
		}

		TEST_METHOD(StackTestCopyConstructor)
		{
			//int tests
			intStack.Push(5);
			intStack.Push(6);
			intStack.Push(7);

			Stack<int> otherIntStack = intStack;
			Assert::IsTrue(otherIntStack == intStack);

			//int* tests
			for (int i = 0; i < 4; i++)
			{
				intPtrStack.Push(&testArray[i]);
			}

			Stack<int*> otherIntPtrStack = intPtrStack;
			Assert::IsTrue(otherIntPtrStack == intPtrStack);

			//Foo tests
			fooStack.Push(Foo(5));
			fooStack.Push(Foo(6));
			fooStack.Push(Foo(7));
			fooStack.Push(Foo(8));

			Stack<Foo> otherFooStack = fooStack;
			Assert::IsTrue(otherFooStack == fooStack);
		}

		TEST_METHOD(StackTestAssignmentOperator)
		{
			//int tests
			intStack.Push(5);
			intStack.Push(6);
			intStack.Push(7);

			Stack<int> otherIntStack;
			otherIntStack = intStack;
			Assert::IsTrue(otherIntStack == intStack);

			//int* tests
			for (int i = 0; i < 4; i++)
			{
				intPtrStack.Push(&testArray[i]);
			}

			Stack<int*> otherIntPtrStack;
			otherIntPtrStack = intPtrStack;
			Assert::IsTrue(otherIntPtrStack == intPtrStack);

			//Foo tests
			fooStack.Push(Foo(5));
			fooStack.Push(Foo(6));
			fooStack.Push(Foo(7));
			fooStack.Push(Foo(8));

			Stack<Foo> otherFooStack;
			otherFooStack = fooStack;
			Assert::IsTrue(otherFooStack == fooStack);
		}

		TEST_METHOD(StackTestMoveSemantics)
		{
			//int tests
			Stack<int> intStackCopy = intStack;

			intStackCopy.Push(5);
			intStackCopy.Push(6);
			intStackCopy.Push(7);

			Stack<int> otherIntStack = std::move(intStackCopy);

			Assert::AreEqual(3U, otherIntStack.Size());
			Assert::AreEqual(7, otherIntStack.Top());

			Stack<int> yetAnotherIntStack;
			yetAnotherIntStack = std::move(otherIntStack);

			Assert::AreEqual(3U, yetAnotherIntStack.Size());
			Assert::AreEqual(7, yetAnotherIntStack.Top());

			//int* tests
			Stack<int*> intPtrStackCopy = intPtrStack;

			for (int i = 0; i < 5; i++)
			{
				intPtrStackCopy.Push(&testArray[i]);
			}

			Stack<int*> otherIntPtrStack = std::move(intPtrStackCopy);

			Assert::AreEqual(5U, otherIntPtrStack.Size());
			Assert::AreEqual(&testArray[4], otherIntPtrStack.Top());

			Stack<int*> yetAnotherIntPtrStack;
			yetAnotherIntPtrStack = std::move(otherIntPtrStack);

			Assert::AreEqual(5U, yetAnotherIntPtrStack.Size());
			Assert::AreEqual(&testArray[4], yetAnotherIntPtrStack.Top());

			//Foo tests
			Stack<Foo> fooStackCopy = fooStack;

			fooStackCopy.Push(Foo(5));
			fooStackCopy.Push(Foo(6));
			fooStackCopy.Push(Foo(7));
			fooStackCopy.Push(Foo(8));

			Stack<Foo> otherFooStack = std::move(fooStackCopy);

			Assert::AreEqual(4U, otherFooStack.Size());
			Assert::AreEqual(8, otherFooStack.Top().Data());

			Stack<Foo> yetAnotherFooStack;
			yetAnotherFooStack = std::move(otherFooStack);

			Assert::AreEqual(4U, yetAnotherFooStack.Size());
			Assert::AreEqual(8, yetAnotherFooStack.Top().Data());
		}

		TEST_METHOD(StackTestEqualityOperator)
		{
			//int tests
			Stack<int> otherIntStack;

			Assert::IsTrue(otherIntStack == intStack);

			intStack.Push(12);
			intStack.Push(13);

			otherIntStack.Push(12);

			Assert::IsTrue(otherIntStack != intStack);

			otherIntStack.Push(14);

			Assert::IsTrue(otherIntStack != intStack);

			otherIntStack.Pop();
			otherIntStack.Push(13);

			Assert::IsTrue(otherIntStack == intStack);

			//int* tests
			Stack<int*> otherIntPtrStack;

			Assert::IsTrue(otherIntPtrStack == intPtrStack);

			for (int i = 0; i < 4; i++)
			{
				intPtrStack.Push(&testArray[i]);
			}

			for (int j = 0; j < 5; j++)
			{
				otherIntPtrStack.Push(&testArray[j]);
			}

			Assert::IsTrue(intPtrStack != otherIntPtrStack);

			otherIntPtrStack.Pop();

			Assert::IsTrue(intPtrStack == otherIntPtrStack);

			//Foo tests
			Stack<Foo> otherFooStack;

			Assert::IsTrue(otherFooStack == fooStack);

			fooStack.Push(Foo(22));
			fooStack.Push(Foo(33));
			fooStack.Push(Foo(44));

			otherFooStack.Push(Foo(22));
			otherFooStack.Push(Foo(33));
			otherFooStack.Push(Foo(55));

			Assert::IsTrue(fooStack != otherFooStack);

			otherFooStack.Pop();
			otherFooStack.Push(fooStack.Top());

			Assert::IsTrue(fooStack == otherFooStack);
		}

		TEST_METHOD(StackTestPush)
		{
			//int tests
			Assert::IsTrue(intStack.IsEmpty());

			intStack.Push(5);
			Assert::AreEqual(5, intStack.Top());
			Assert::AreEqual(1U, intStack.Size());

			intStack.Push(6);
			Assert::AreEqual(6, intStack.Top());
			Assert::AreEqual(2U, intStack.Size());

			intStack.Push(7);
			Assert::AreEqual(7, intStack.Top());
			Assert::AreEqual(3U, intStack.Size());

			//int* tests
			std::uint32_t currentSize = intPtrStack.Size();

			Assert::AreEqual(0U, currentSize);

			for (int i = 0; i < 4; i++)
			{
				intPtrStack.Push(&testArray[i]);
				Assert::AreEqual(&testArray[i], intPtrStack.Top());
				Assert::AreEqual(++currentSize, intPtrStack.Size());
			}

			//Foo tests
			Assert::IsTrue(fooStack.IsEmpty());

			fooStack.Push(Foo(5));
			Assert::AreEqual(5, fooStack.Top().Data());
			Assert::AreEqual(1U, fooStack.Size());

			fooStack.Push(Foo(6));
			Assert::AreEqual(6, fooStack.Top().Data());
			Assert::AreEqual(2U, fooStack.Size());

			Foo testFoo(7);

			fooStack.Push(testFoo);
			Assert::IsTrue(testFoo == fooStack.Top());
			Assert::AreEqual(3U, fooStack.Size());
		}

		TEST_METHOD(StackTestPop)
		{
			//int tests
			Assert::ExpectException<std::exception>([&] {intStack.Pop(); });

			intStack.Push(5);
			intStack.Push(6);
			intStack.Push(7);

			Assert::AreEqual(7, intStack.Top());

			intStack.Pop();
			Assert::AreEqual(6, intStack.Top());
			intStack.Pop();
			Assert::AreEqual(5, intStack.Top());
			intStack.Pop();

			Assert::IsTrue(intStack.IsEmpty());

			//int* tests
			Assert::ExpectException<std::exception>([&] {intPtrStack.Pop(); });

			int i;

			for (i = 0; i < 5; i++)
			{
				intPtrStack.Push(&testArray[i]);
				Assert::AreEqual(&testArray[i], intPtrStack.Top());
			}

			for (i = 4; i >= 0; i--)
			{
				intPtrStack.Pop();

				if (i == 0)
				{
					Assert::IsTrue(intPtrStack.IsEmpty());
				}
				else
				{
					Assert::AreEqual(&testArray[i - 1], intPtrStack.Top());
				}
			}

			//Foo tests
			Assert::ExpectException<std::exception>([&] {fooStack.Pop(); });

			Foo dummyFoo1(8), dummyFoo2(9), dummyFoo3(10);

			fooStack.Push(dummyFoo1);
			fooStack.Push(dummyFoo2);
			fooStack.Push(dummyFoo3);

			Assert::IsTrue(dummyFoo3 == fooStack.Top());
			fooStack.Pop();
			Assert::IsTrue(dummyFoo2 == fooStack.Top());
			fooStack.Pop();
			Assert::IsTrue(dummyFoo1 == fooStack.Top());
			fooStack.Pop();
			Assert::IsTrue(fooStack.IsEmpty());
		}

		TEST_METHOD(StackTestConstTop)
		{
			//int tests
			intStack.Push(5);
			intStack.Push(6);
			intStack.Push(7);

			const Stack<int> constIntStack = intStack;
			Assert::AreEqual(7, constIntStack.Top());

			//int* tests
			for (int i = 0; i < 4; i++)
			{
				intPtrStack.Push(&testArray[i]);
			}

			const Stack<int*> constIntPtrStack = intPtrStack;
			Assert::AreEqual(&testArray[3], constIntPtrStack.Top());

			//Foo tests
			fooStack.Push(Foo(5));
			fooStack.Push(Foo(6));

			Foo testFoo(7);
			fooStack.Push(testFoo);

			Assert::IsTrue(testFoo == fooStack.Top());
		}

		TEST_METHOD(StackTestClear)
		{
			//int tests
			intStack.Push(5);
			intStack.Push(6);
			intStack.Push(7);

			Assert::IsFalse(intStack.IsEmpty());

			intStack.Clear();

			Assert::IsTrue(intStack.IsEmpty());

			//int* tests
			for (int i = 0; i < 5; i++)
			{
				intPtrStack.Push(&testArray[i]);
			}

			Assert::IsFalse(intPtrStack.IsEmpty());

			intPtrStack.Clear();

			Assert::IsTrue(intPtrStack.IsEmpty());

			//Foo tests
			fooStack.Push(Foo(5));
			fooStack.Push(Foo(6));
			fooStack.Push(Foo(7));
			fooStack.Push(Foo(8));

			Assert::IsFalse(fooStack.IsEmpty());

			fooStack.Clear();

			Assert::IsTrue(fooStack.IsEmpty());
		}
	private:
		static Stack<int> intStack;
		static Stack<int*> intPtrStack;
		static Stack<Foo> fooStack;
		static int testArray[5];

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

	Stack<int> StackTest::intStack;
	Stack<int*> StackTest::intPtrStack;
	Stack<Foo> StackTest::fooStack;
	int StackTest::testArray[] = { 1, 2, 3, 4, 5 };

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState StackTest::startMemState;
#endif
}