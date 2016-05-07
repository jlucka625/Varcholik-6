#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorIteratorTest)
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
			intVector.Clear();
			intPtrVector.Clear();
			fooVector.Clear();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(VectorIteratorTestCopyConstructor)
		{
			//int test
			intVector.Reserve(3);

			intVector.PushBack(7);
			intVector.PushBack(8);
			intVector.PushBack(9);

			Vector<int>::Iterator intIterator = intVector.begin();
			Assert::IsTrue(intIterator == intVector.begin());

			Vector<int>::Iterator otherIntIterator = intIterator;
			Assert::IsTrue(otherIntIterator == intIterator);
			Assert::IsTrue(otherIntIterator == intVector.begin());

			//int* test
			intPtrVector.Reserve(4);

			for (int i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			Vector<int*>::Iterator intPtrIterator = intPtrVector.end();
			Assert::IsTrue(intPtrIterator == intPtrVector.end());

			Vector<int*>::Iterator otherIntPtrIterator = intPtrIterator;
			Assert::IsTrue(otherIntPtrIterator == intPtrIterator);
			Assert::IsTrue(otherIntPtrIterator == intPtrVector.end());

			//Foo test
			fooVector.Reserve(3);

			Foo targetFoo(47);

			fooVector.PushBack(Foo(23));
			fooVector.PushBack(targetFoo);
			fooVector.PushBack(Foo(16));

			Vector<Foo>::Iterator fooIterator = fooVector.Find(targetFoo);
			Assert::IsTrue(*fooIterator == fooVector[1]);

			Vector<Foo>::Iterator otherFooIterator = fooIterator;
			Assert::IsTrue(otherFooIterator == fooIterator);
			Assert::IsTrue(*otherFooIterator == fooVector[1]);
		}

		TEST_METHOD(VectorIteratorTestAssignment)
		{
			//int test
			intVector.Reserve(3);

			intVector.PushBack(7);
			intVector.PushBack(8);
			intVector.PushBack(9);

			Vector<int>::Iterator intIterator;
			intIterator = intVector.begin();
			Assert::IsTrue(intIterator == intVector.begin());

			Vector<int>::Iterator otherIntIterator;
			otherIntIterator = intIterator;
			Assert::IsTrue(otherIntIterator == intIterator);
			Assert::IsTrue(otherIntIterator == intVector.begin());

			//int* test
			intPtrVector.Reserve(4);

			for (int i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			Vector<int*>::Iterator intPtrIterator;
			intPtrIterator = intPtrVector.end();
			Assert::IsTrue(intPtrIterator == intPtrVector.end());

			Vector<int*>::Iterator otherIntPtrIterator;
			otherIntPtrIterator = intPtrIterator;
			Assert::IsTrue(otherIntPtrIterator == intPtrIterator);
			Assert::IsTrue(otherIntPtrIterator == intPtrVector.end());

			//Foo test
			fooVector.Reserve(3);

			Foo targetFoo(47);

			fooVector.PushBack(Foo(23));
			fooVector.PushBack(targetFoo);
			fooVector.PushBack(Foo(16));

			Vector<Foo>::Iterator fooIterator;
			fooIterator = fooVector.Find(targetFoo);
			Assert::IsTrue(*fooIterator == fooVector[1]);

			Vector<Foo>::Iterator otherFooIterator;
			otherFooIterator = fooIterator;
			Assert::IsTrue(otherFooIterator == fooIterator);
			Assert::IsTrue(*otherFooIterator == fooVector[1]);
		}

		TEST_METHOD(VectorIteratorTestMoveSemantics)
		{
			//int test
			intVector.Reserve(3);

			intVector.PushBack(7);
			intVector.PushBack(8);
			intVector.PushBack(9);

			Vector<int>::Iterator intIterator = intVector.begin();
			Vector<int>::Iterator otherIntIterator = std::move(intIterator);
			Assert::IsTrue(otherIntIterator == intVector.begin());

			Vector<int>::Iterator yetAnotherIntIterator;
			yetAnotherIntIterator = std::move(otherIntIterator);
			Assert::IsTrue(yetAnotherIntIterator == intVector.begin());

			//int* test
			intPtrVector.Reserve(4);

			for (int i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			Vector<int*>::Iterator intPtrIterator = intPtrVector.end();
			Vector<int*>::Iterator otherIntPtrIterator = std::move(intPtrIterator);
			Assert::IsTrue(otherIntPtrIterator == intPtrVector.end());

			Vector<int*>::Iterator yetAnotherIntPtrIterator;
			yetAnotherIntPtrIterator = std::move(otherIntPtrIterator);
			Assert::IsTrue(yetAnotherIntPtrIterator == intPtrVector.end());

			//Foo test
			fooVector.Reserve(3);

			Foo targetFoo(47);

			fooVector.PushBack(Foo(23));
			fooVector.PushBack(targetFoo);
			fooVector.PushBack(Foo(16));

			Vector<Foo>::Iterator fooIterator = fooVector.Find(targetFoo);
			Vector<Foo>::Iterator otherFooIterator = std::move(fooIterator);
			Assert::IsTrue(*otherFooIterator == fooVector[1]);

			Vector<Foo>::Iterator yetAnotherFooIterator;
			yetAnotherFooIterator = std::move(otherFooIterator);
			Assert::IsTrue(*yetAnotherFooIterator == fooVector.At(1));
		}

		TEST_METHOD(VectorIteratorTestEquality)
		{
			//int tests
			Vector<int>::Iterator intIterator, otherIntIterator;
			Assert::IsTrue(intIterator == otherIntIterator);

			intIterator = intVector.begin();
			otherIntIterator = intVector.end();

			Assert::IsTrue(intIterator == otherIntIterator);
			intVector.PushBack(30);
			intVector.PushBack(20);

			otherIntIterator = intVector.end();

			Assert::IsTrue(intIterator != otherIntIterator);

			intIterator = intVector.begin();
			otherIntIterator = intVector.Find(30);

			Assert::IsTrue(intIterator == otherIntIterator);

			//int* tests
			intPtrVector.PushBack(&testArray[1]);
			intPtrVector.PushBack(&testArray[2]);
			intPtrVector.PushBack(&testArray[0]);
			intPtrVector.PushBack(&testArray[4]);

			auto intPtrIterator = intPtrVector.Find(&testArray[2]);
			auto otherIntPtrIterator = intPtrVector.Find(&testArray[1]);

			Assert::IsTrue(intPtrIterator != otherIntPtrIterator);

			intPtrIterator = intPtrVector.begin();

			Assert::IsTrue(intPtrIterator == otherIntPtrIterator);

			//Foo tests
			Vector<Foo> otherFooVector;

			fooVector.PushBack(Foo(13));

			otherFooVector.PushBack(fooVector.Back());

			auto fooIterator = fooVector.begin();
			auto otherFooIterator = otherFooVector.begin();

			Assert::IsTrue(fooIterator != otherFooIterator);

			otherFooIterator = fooVector.begin();
			Assert::IsTrue(fooIterator == otherFooIterator);
		}

		TEST_METHOD(VectorIteratorTestInvalidOperations)
		{
			Vector<int>::Iterator intIterator;
			Vector<int*>::Iterator intPtrIterator;
			Vector<Foo>::Iterator fooIterator;

			auto badIntDereference = [&] { (*intIterator); };
			auto badIntPtrDereference = [&] { (*intPtrIterator); };
			auto badFooDereference = [&] { (*fooIterator); };

			auto badIntPrefixIncrement = [&] { ++intIterator;  };
			auto badIntPtrPrefixIncrement = [&] { ++intPtrIterator;  };
			auto badFooPrefixIncrement = [&] { ++fooIterator;  };

			auto badIntPostfixIncrement = [&] { intIterator++; };
			auto badIntPtrPostfixIncrement = [&] { intPtrIterator++; };
			auto badFooPostfixIncrement = [&] { fooIterator++; };

			Assert::ExpectException<std::exception>(badIntDereference);
			Assert::ExpectException<std::exception>(badIntPtrDereference);
			Assert::ExpectException<std::exception>(badFooDereference);
			Assert::ExpectException<std::exception>(badIntPrefixIncrement);
			Assert::ExpectException<std::exception>(badIntPtrPrefixIncrement);
			Assert::ExpectException<std::exception>(badFooPrefixIncrement);
			Assert::ExpectException<std::exception>(badIntPostfixIncrement);
			Assert::ExpectException<std::exception>(badIntPtrPostfixIncrement);
			Assert::ExpectException<std::exception>(badFooPostfixIncrement);

			intIterator = intVector.begin();
			intPtrIterator = intPtrVector.begin();
			fooIterator = fooVector.begin();

			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(intPtrVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());

			Assert::ExpectException<std::exception>(badIntDereference);
			Assert::ExpectException<std::exception>(badIntPtrDereference);
			Assert::ExpectException<std::exception>(badFooDereference);
			Assert::ExpectException<std::exception>(badIntPrefixIncrement);
			Assert::ExpectException<std::exception>(badIntPtrPrefixIncrement);
			Assert::ExpectException<std::exception>(badFooPrefixIncrement);
			Assert::ExpectException<std::exception>(badIntPostfixIncrement);
			Assert::ExpectException<std::exception>(badIntPtrPostfixIncrement);
			Assert::ExpectException<std::exception>(badFooPostfixIncrement);
		}

		TEST_METHOD(VectorIteratorTestPostfixIncrement)
		{
			int i;

			for (i = 1; i < 6; i++)
			{
				intVector.PushBack(i);
				intPtrVector.PushBack(&testArray[i - 1]);
				fooVector.PushBack(Foo(i));
			}

			//int test
			i = 0;

			for (auto intIterator = intVector.begin(); intIterator != intVector.end(); intIterator++)
			{
				Assert::AreEqual(testArray[i++], (*intIterator));
			}

			//int* test
			i = 0;

			for (auto intPtrIterator = intPtrVector.begin(); intPtrIterator != intPtrVector.end(); intPtrIterator++)
			{
				Assert::AreEqual(&testArray[i++], (*intPtrIterator));
			}

			//Foo test
			i = 0;

			for (auto fooIterator = fooVector.begin(); fooIterator != fooVector.end(); fooIterator++)
			{
				Assert::AreEqual(testArray[i++], (*fooIterator).Data());
			}
		}
	private:
		static Vector<int> intVector;
		static Vector<int*> intPtrVector;
		static Vector<Foo> fooVector;
		static int testArray[5];

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

	Vector<int> VectorIteratorTest::intVector;
	Vector<int*> VectorIteratorTest::intPtrVector;
	Vector<Foo> VectorIteratorTest::fooVector;
	int VectorIteratorTest::testArray[] = { 1, 2, 3, 4, 5 };

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState VectorIteratorTest::startMemState;
#endif
}