#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
	{
	public:
		TEST_METHOD_INITIALIZE(PrepForTests)
		{
			intVector.SetReserveFunctor(defaultFunctor);
			intPtrVector.SetReserveFunctor(defaultFunctor);
			fooVector.SetReserveFunctor(defaultFunctor);

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

		TEST_METHOD(VectorTestDefaultConstructor)
		{
			Assert::IsTrue(intVector.IsEmpty());
			Assert::IsTrue(intPtrVector.IsEmpty());
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(VectorTestIncrementFunctor)
		{
			int i;

			//int tests (uses default increment functor, which increments the capacity by 5 each time)
			Assert::IsTrue(intVector.IsEmpty());

			intVector.PushBack(5);
			Assert::AreEqual(1U, intVector.Capacity());

			intVector.PushBack(7);
			Assert::AreEqual(2U, intVector.Capacity());
			
			//int* tests
			ZeroIncrementFunctor zeroFunctor;
			intPtrVector.SetReserveFunctor(zeroFunctor);

			Assert::IsTrue(intPtrVector.IsEmpty());

			intPtrVector.Reserve(1);
			intPtrVector.PushBack(&testArray[0]);
			Assert::AreEqual(1U, intPtrVector.Capacity());

			std::uint32_t currentCapacity;

			for (i = 1; i < 5; i++)
			{
				currentCapacity = intPtrVector.Capacity();
				intPtrVector.PushBack(&testArray[i]);
				Assert::AreEqual(++currentCapacity, intPtrVector.Capacity());
			}

			//Foo tests
			DoubleSizeIncrementFunctor doubleFunctor;
			fooVector.SetReserveFunctor(doubleFunctor);

			Assert::IsTrue(fooVector.IsEmpty());

			fooVector.PushBack(Foo(2));
			Assert::AreEqual(1U, fooVector.Capacity());

			fooVector.PushBack(Foo(3));
			Assert::AreEqual(2U, fooVector.Capacity());

			fooVector.PushBack(Foo(4));
			Assert::AreEqual(4U, fooVector.Capacity());

			currentCapacity = fooVector.Capacity();

			for (i = 0; i < 2; i++)
			{
				fooVector.PushBack(Foo(testArray[i]));
			}

			Assert::AreEqual(8U, fooVector.Capacity());
		}

		TEST_METHOD(VectorTestReserve)
		{
			//int tests
			intVector.Reserve(2);
			Assert::AreEqual(2U, intVector.Capacity());

			intVector.Reserve(5);
			Assert::AreEqual(5U, intVector.Capacity());

			intVector.Reserve(3);
			Assert::AreNotEqual(3U, intVector.Capacity());

			//int* tests
			intPtrVector.Reserve(1);
			Assert::AreEqual(1U, intPtrVector.Capacity());

			intPtrVector.Reserve(1);
			Assert::AreEqual(1U, intPtrVector.Capacity());

			intPtrVector.Reserve(7);
			Assert::AreNotEqual(1U, intPtrVector.Capacity());

			//Foo tests
			fooVector.Reserve(0);
			Assert::AreEqual(0U, fooVector.Capacity());

			fooVector.Reserve(10);
			Assert::AreEqual(10U, fooVector.Capacity());
			
			fooVector.Reserve(0);
			Assert::AreNotEqual(0U, fooVector.Capacity());
		}

		TEST_METHOD(VectorTestPushBack)
		{
			std::uint32_t i;

			//int tests
			intVector.Reserve(2);

			intVector.PushBack(5);
			Assert::AreEqual(5, intVector.Back());
			Assert::AreEqual(intVector.Front(), intVector.Back());

			intVector.PushBack(6);
			Assert::AreEqual(6, intVector.Back());
			Assert::AreNotEqual(intVector.Front(), intVector.Back());

			intVector.PushBack(7);
			Assert::AreEqual(7, intVector.Back());
			Assert::AreEqual(3U, intVector.Capacity());

			Assert::AreEqual(5, intVector[0]);
			Assert::AreEqual(6, intVector[1]);
			Assert::AreEqual(7, intVector[2]);

			//int* tests
			intPtrVector.Reserve(4);

			for (i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
				Assert::AreEqual(&testArray[i], intPtrVector.Back());
				Assert::AreEqual((i + 1), intPtrVector.Size());
				Assert::IsTrue(intPtrVector.Size() <= intPtrVector.Capacity());
			}

			intPtrVector.PushBack(&testArray[i]);
			Assert::AreEqual(&testArray[i], intPtrVector.Back());
			Assert::AreEqual(5U, intPtrVector.Capacity());

			for (i = 0; i < 5; i++)
			{
				Assert::AreEqual(&testArray[i], intPtrVector[i]);
			}

			//Foo tests
			Foo testFoo(12);
			Foo otherTestFoo(21);

			fooVector.Reserve(3);

			fooVector.PushBack(testFoo);
			fooVector.PushBack(otherTestFoo);
			fooVector.PushBack(testFoo);

			fooVector.PushBack(otherTestFoo);
			Assert::AreEqual(4U, fooVector.Capacity());

			for (i = 0; i < 4; i++)
			{
				if (i % 2 == 0)
				{
					Assert::AreEqual(12, fooVector[i].Data());
				}
				else
				{
					Assert::AreEqual(21, fooVector[i].Data());
				}
			}
		}

		TEST_METHOD(VectorTestPopBack)
		{
			auto intVectorPopBack = [&] { intVector.PopBack(); };
			auto intPtrVectorPopBack = [&] { intVector.PopBack(); };
			auto fooVectorPopBack = [&] { intVector.PopBack(); };

			Assert::ExpectException<std::exception>(intVectorPopBack);
			Assert::ExpectException<std::exception>(intPtrVectorPopBack);
			Assert::ExpectException<std::exception>(fooVectorPopBack);

			//int tests
			intVector.Reserve(4);

			intVector.PushBack(11);
			intVector.PushBack(22);
			intVector.PushBack(35);
			intVector.PushBack(67);

			intVector.PopBack();
			Assert::AreEqual(3U, intVector.Size());
			Assert::AreEqual(35, intVector.Back());

			intVector.PopBack();
			Assert::AreEqual(2U, intVector.Size());
			Assert::AreEqual(22, intVector.Back());

			intVector.PopBack();
			Assert::AreEqual(1U, intVector.Size());
			Assert::AreEqual(11, intVector.Back());
			Assert::AreEqual(intVector.Front(), intVector.Back());

			intVector.PopBack();
			Assert::IsTrue(intVector.IsEmpty());
			Assert::AreEqual(4U, intVector.Capacity());

			//int* tests
			int i;

			intPtrVector.Reserve(5);

			for (i = 0; i < 5; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			std::uint32_t currentSize = intPtrVector.Size();

			for (i = 3; i >= 0; i--)
			{
				intPtrVector.PopBack();
				Assert::AreEqual(&testArray[i], intPtrVector.Back());
				Assert::AreEqual((--currentSize), intPtrVector.Size());
			}

			intPtrVector.PopBack();
			Assert::IsTrue(intPtrVector.IsEmpty());
			Assert::AreEqual(5U, intPtrVector.Capacity());

			//Foo tests
			fooVector.Reserve(3);

			fooVector.PushBack(Foo(19));
			fooVector.PushBack(Foo(17));
			fooVector.PushBack(Foo(19));

			Assert::AreEqual(fooVector.Front().Data(), fooVector.Back().Data());

			fooVector.PopBack();

			Assert::AreEqual(2U, fooVector.Size());

		}

		TEST_METHOD(VectorTestConstFront)
		{
			//int tests
			intVector.Reserve(3);

			intVector.PushBack(7);
			intVector.PushBack(6);
			intVector.PushBack(7);

			const Vector<int> otherIntVector = intVector;

			Assert::AreEqual(otherIntVector.Front(), intVector.Front());
			Assert::AreEqual(otherIntVector.Front(), intVector.Back());

			//int* tests
			intPtrVector.Reserve(4);

			for (int i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[0]);
			}

			const Vector<int*> otherIntPtrVector = intPtrVector;

			Assert::AreEqual(otherIntPtrVector.Front(), intPtrVector.Front());
			Assert::AreEqual(otherIntPtrVector.Front(), intPtrVector.Back());

			//Foo tests
			fooVector.Reserve(3);

			fooVector.PushBack(Foo(14));
			fooVector.PushBack(Foo(13));
			fooVector.PushBack(Foo(14));

			const Vector<Foo> otherFooVector = fooVector;

			Assert::IsTrue(fooVector.Front() == otherFooVector.Front());
			Assert::IsTrue(fooVector.Back() == otherFooVector.Front());
		}

		TEST_METHOD(VectorTestNonConstFront)
		{
			auto intVectorFront = ([&] { intVector.Front(); });
			auto intPtrVectorFront = ([&] { intPtrVector.Front(); });
			auto fooVectorFront = ([&] { fooVector.Front(); });

			Assert::ExpectException<std::exception>(intVectorFront);
			Assert::ExpectException<std::exception>(intPtrVectorFront);
			Assert::ExpectException<std::exception>(fooVectorFront);

			//int tests
			intVector.PushBack(12);
			Assert::AreEqual(12, intVector.Front());
			Assert::AreEqual(intVector.Front(), intVector.Back());

			intVector.PushBack(21);
			Assert::AreNotEqual(intVector.Front(), intVector.Back());

			//int* tests
			intPtrVector.PushBack(&testArray[0]);

			Assert::AreEqual(&testArray[0], intPtrVector.Front());
			Assert::AreEqual(intPtrVector.Front(), intPtrVector.Back());

			intPtrVector.PushBack(&testArray[0]);
			Assert::AreEqual(intPtrVector.Front(), intPtrVector.Back());

			intPtrVector.PushBack(&testArray[1]);
			Assert::AreNotEqual(intPtrVector.Front(), intPtrVector.Back());

			//Foo tests
			fooVector.PushBack(Foo(7));
			fooVector.PushBack(Foo(8));

			Assert::IsTrue(fooVector.Front() != fooVector.Back());

			fooVector.Back().Data() = 7;

			Assert::AreEqual(fooVector.Front().Data(), fooVector.Back().Data());
		}

		TEST_METHOD(VectorTestConstBack)
		{
			//int tests
			intVector.Reserve(3);

			intVector.PushBack(7);
			intVector.PushBack(6);
			intVector.PushBack(7);

			const Vector<int> otherIntVector = intVector;

			Assert::AreEqual(otherIntVector.Back(), intVector.Front());
			Assert::AreEqual(otherIntVector.Back(), intVector.Back());

			//int* tests
			intPtrVector.Reserve(4);

			for (int i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[0]);
			}

			const Vector<int*> otherIntPtrVector = intPtrVector;

			Assert::AreEqual(otherIntPtrVector.Back(), intPtrVector.Front());
			Assert::AreEqual(otherIntPtrVector.Back(), intPtrVector.Back());

			//Foo tests
			fooVector.Reserve(3);

			fooVector.PushBack(Foo(14));
			fooVector.PushBack(Foo(13));
			fooVector.PushBack(Foo(14));

			const Vector<Foo> otherFooVector = fooVector;

			Assert::IsTrue(fooVector.Front() == otherFooVector.Back());
			Assert::IsTrue(fooVector.Back() == otherFooVector.Back());
		}

		TEST_METHOD(VectorTestNonConstBack)
		{
			auto intVectorBack = ([&] { intVector.Back(); });
			auto intPtrVectorBack = ([&] { intPtrVector.Back(); });
			auto fooVectorBack = ([&] { fooVector.Back(); });

			Assert::ExpectException<std::exception>(intVectorBack);
			Assert::ExpectException<std::exception>(intPtrVectorBack);
			Assert::ExpectException<std::exception>(fooVectorBack);

			//int tests
			intVector.PushBack(12);
			Assert::AreEqual(12, intVector.Front());
			Assert::AreEqual(intVector.Front(), intVector.Back());

			intVector.PushBack(21);
			Assert::AreEqual(21, intVector.Back());
			Assert::AreNotEqual(intVector.Front(), intVector.Back());

			//int* tests
			intPtrVector.PushBack(&testArray[0]);

			Assert::AreEqual(&testArray[0], intPtrVector.Front());
			Assert::AreEqual(intPtrVector.Front(), intPtrVector.Back());

			intPtrVector.PushBack(&testArray[0]);
			Assert::AreEqual(intPtrVector.Front(), intPtrVector.Back());

			intPtrVector.PushBack(&testArray[1]);
			Assert::AreEqual(&testArray[1], intPtrVector.Back());
			Assert::AreNotEqual(intPtrVector.Front(), intPtrVector.Back());

			//Foo tests
			fooVector.PushBack(Foo(7));
			fooVector.PushBack(Foo(8));

			Assert::IsTrue(fooVector.Front() != fooVector.Back());

			fooVector.Back().Data() = 7;

			Assert::AreEqual(fooVector.Front().Data(), fooVector.Back().Data());
		}

		TEST_METHOD(VectorTestCopyConstructor)
		{
			//int test
			int i;

			intVector.Reserve(5);

			for (i = 0; i < 5; i++)
			{
				intVector.PushBack(testArray[i]);
			}

			Vector<int> otherIntVector = intVector;
			Assert::IsTrue(intVector == otherIntVector);

			//int* test
			intPtrVector.Reserve(4);

			for (i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			Vector<int*> otherIntPtrVector = intPtrVector;
			Assert::IsTrue(otherIntPtrVector == intPtrVector);

			//Foo test
			fooVector.Reserve(3);

			for (i = 0; i < 4; i++)
			{
				fooVector.PushBack(Foo(testArray[i]));
			}

			Vector<Foo> otherFooVector = fooVector;
			Assert::IsTrue(fooVector == otherFooVector);
		}

		TEST_METHOD(VectorTestAssignmentOperator)
		{
			//int tests
			Vector<int> otherIntVector;

			otherIntVector = intVector;
			Assert::IsTrue(intVector == otherIntVector);

			intVector.Reserve(3);
			intVector.PushBack(7);
			intVector.PushBack(4);
			intVector.PushBack(12);

			intVector = intVector;
			Assert::IsTrue(intVector == intVector);

			otherIntVector = intVector;
			Assert::IsTrue(intVector == otherIntVector);

			//int* tests
			Vector<int *> otherIntPtrVector;

			otherIntPtrVector = intPtrVector;
			Assert::IsTrue(intPtrVector == otherIntPtrVector);

			otherIntPtrVector.Reserve(4);

			for (int i = 0; i < 4; i++)
			{
				otherIntPtrVector.PushBack(&testArray[i]);
			}

			intPtrVector = otherIntPtrVector;

			Assert::IsTrue(otherIntPtrVector == intPtrVector);

			//Foo tests
			fooVector.Reserve(3);
			fooVector.PushBack(Foo(55));
			fooVector.PushBack(Foo(56));
			fooVector.PushBack(Foo(57));

			Vector<Foo> otherFooVector(fooVector);

			Vector<Foo> finalFooVector;
			finalFooVector = fooVector;

			Assert::IsTrue(fooVector == otherFooVector);
			Assert::IsTrue(fooVector == finalFooVector);
			Assert::IsTrue(otherFooVector == finalFooVector);
		}

		TEST_METHOD(VectorTestMoveSemantics)
		{
			//int test
			intVector.Reserve(3);

			intVector.PushBack(5);
			intVector.PushBack(15);
			intVector.PushBack(15);

			intVector = std::move(intVector);
			Assert::IsTrue(intVector == intVector);

			Vector<int> otherIntVector = std::move(intVector);

			Assert::AreEqual(3U, otherIntVector.Size());
			Assert::AreEqual(otherIntVector.Size(), otherIntVector.Capacity());
			Assert::AreEqual(5, otherIntVector[0]);
			Assert::AreEqual(15, otherIntVector[1]);
			Assert::AreEqual(15, otherIntVector.At(2));

			Vector<int> yetAnotherIntVector;
			yetAnotherIntVector = std::move(otherIntVector);

			Assert::AreEqual(3U, yetAnotherIntVector.Size());
			Assert::AreEqual(yetAnotherIntVector.Size(), yetAnotherIntVector.Capacity());
			Assert::AreEqual(5, yetAnotherIntVector[0]);
			Assert::AreEqual(15, yetAnotherIntVector[1]);
			Assert::AreEqual(15, yetAnotherIntVector.At(2));

			//int* test
			intPtrVector.Reserve(3);

			intPtrVector.PushBack(&testArray[0]);
			intPtrVector.PushBack(&testArray[1]);
			intPtrVector.PushBack(&testArray[2]);

			intPtrVector = std::move(intPtrVector);
			Assert::IsTrue(intPtrVector == intPtrVector);

			Vector<int*> otherIntPtrVector = std::move(intPtrVector);

			Assert::AreEqual(3U, otherIntPtrVector.Size());
			Assert::AreEqual(otherIntPtrVector.Size(), otherIntPtrVector.Capacity());
			Assert::AreEqual(&testArray[0], otherIntPtrVector[0]);
			Assert::AreEqual(&testArray[1], otherIntPtrVector[1]);
			Assert::AreEqual(&testArray[2], otherIntPtrVector.At(2));

			Vector<int*> yetAnotherIntPtrVector;
			yetAnotherIntPtrVector = std::move(otherIntPtrVector);

			Assert::AreEqual(3U, yetAnotherIntPtrVector.Size());
			Assert::AreEqual(yetAnotherIntPtrVector.Size(), yetAnotherIntPtrVector.Capacity());
			Assert::AreEqual(&testArray[0], yetAnotherIntPtrVector[0]);
			Assert::AreEqual(&testArray[1], yetAnotherIntPtrVector[1]);
			Assert::AreEqual(&testArray[2], yetAnotherIntPtrVector.At(2));

			//Foo test
			fooVector.Reserve(3);

			fooVector.PushBack(Foo(5));
			fooVector.PushBack(Foo(15));
			fooVector.PushBack(Foo(15));

			fooVector = std::move(fooVector);
			Assert::IsTrue(fooVector == fooVector);

			Vector<Foo> otherFooVector = std::move(fooVector);

			Assert::AreEqual(3U, otherFooVector.Size());
			Assert::AreEqual(otherFooVector.Size(), otherFooVector.Capacity());
			Assert::AreEqual(5, otherFooVector[0].Data());
			Assert::AreEqual(15, otherFooVector[1].Data());
			Assert::AreEqual(15, otherFooVector.At(2).Data());

			Vector<Foo> yetAnotherFooVector;
			yetAnotherFooVector = std::move(otherFooVector);

			Assert::AreEqual(3U, yetAnotherIntVector.Size());
			Assert::AreEqual(yetAnotherIntVector.Size(), yetAnotherIntVector.Capacity());
			Assert::AreEqual(5, yetAnotherIntVector[0]);
			Assert::AreEqual(15, yetAnotherIntVector[1]);
			Assert::AreEqual(15, yetAnotherIntVector.At(2));
		}

		TEST_METHOD(VectorTestEqualityOperator)
		{
			//int tests
			Vector<int> otherIntVector;

			intVector.Reserve(2);
			otherIntVector.Reserve(3);

			Assert::IsTrue(intVector == otherIntVector);

			for (std::uint32_t i = 0; i < intVector.Capacity(); i++)
			{
				intVector.PushBack(testArray[i]);
				otherIntVector.PushBack(testArray[i]);
			}

			otherIntVector.PushBack(testArray[2]);

			Assert::IsTrue(intVector != otherIntVector);

			intVector.PushBack(testArray[2]);

			Assert::IsTrue(intVector == otherIntVector);

			//int* tests
			Vector<int*> otherIntPtrVector;

			intPtrVector.Reserve(5);
			otherIntPtrVector.Reserve(intPtrVector.Capacity());

			Assert::IsTrue(intPtrVector == otherIntPtrVector);

			for (std::uint32_t i = 0; i < intPtrVector.Capacity(); i++)
			{
				intPtrVector.PushBack(&testArray[i]);
				otherIntPtrVector.PushBack(&testArray[i]);
			}

			otherIntPtrVector.PopBack();

			Assert::IsTrue(intPtrVector != otherIntPtrVector);

			otherIntPtrVector.PushBack(&testArray[2]);

			Assert::IsTrue(intPtrVector != otherIntPtrVector);

			otherIntPtrVector.PopBack();
			otherIntPtrVector.PushBack(&testArray[4]);

			Assert::IsTrue(intPtrVector == otherIntPtrVector);

			//Foo tests
			Vector<Foo> otherFooVector;

			fooVector.Reserve(4);
			otherFooVector.Reserve(3);

			Assert::IsTrue(fooVector == otherFooVector);

			for (std::uint32_t i = 0; i < otherFooVector.Capacity(); i++)
			{
				fooVector.PushBack(Foo(testArray[i]));
				otherFooVector.PushBack(fooVector.Back());
			}

			fooVector.PushBack(Foo(testArray[4]));

			Assert::IsTrue(fooVector != otherFooVector);

			otherFooVector.Reserve(4);
			otherFooVector.PushBack(fooVector.Back());

			Assert::IsTrue(fooVector == otherFooVector);
		}

		TEST_METHOD(VectorTestAt)
		{
			std::uint32_t testIndex = 0;

			auto intVectorAt = [&] { intVector.At(testIndex); };
			auto intPtrVectorAt = [&] { intPtrVector.At(testIndex); };
			auto fooVectorAt = [&] { fooVector.At(testIndex); };

			Assert::ExpectException<std::exception>(intVectorAt);
			Assert::ExpectException<std::exception>(intPtrVectorAt);
			Assert::ExpectException<std::exception>(fooVectorAt);

			//int tests
			intVector.Reserve(3);

			intVector.PushBack(3);
			intVector.PushBack(4);
			intVector.PushBack(5);

			testIndex = 5;

			Assert::ExpectException<std::exception>(intVectorAt);

			Assert::AreEqual(intVector.At(0), intVector.Front());
			Assert::AreEqual(intVector.At(1), 4);
			Assert::AreEqual(intVector.At(2), intVector.Back());

			//int* tests
			intPtrVector.Reserve(5);

			for (int i = 0; i < 5; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
				Assert::AreEqual(intPtrVector.At(i), &testArray[i]);
			}

			testIndex = 8;

			Assert::ExpectException<std::exception>(intPtrVectorAt);

			//Foo tests
			fooVector.Reserve(4);

			fooVector.PushBack(Foo(12));
			fooVector.PushBack(Foo(21));
			fooVector.PushBack(fooVector[0]);
			fooVector.PushBack(fooVector[1]);

			testIndex = 13;

			Assert::ExpectException<std::exception>(fooVectorAt);

			Assert::IsTrue(fooVector.At(0) == fooVector.At(2));
			Assert::IsTrue(fooVector.At(1) == fooVector.At(3));
		}

		TEST_METHOD(VectorTestConstSubscriptOperator)
		{
			std::uint32_t testIndex;
			int i;

			//int test
			intVector.Reserve(3);

			intVector.PushBack(2);
			intVector.PushBack(3);
			intVector.PushBack(4);

			const Vector<int> otherIntVector = intVector;

			testIndex = 800;

			auto intVectorSubscript = [&] { otherIntVector[testIndex]; };
			Assert::ExpectException<std::exception>(intVectorSubscript);

			Assert::AreEqual(2, otherIntVector[0]);
			Assert::AreEqual(3, otherIntVector[1]);
			Assert::AreEqual(4, otherIntVector[2]);

			//int* tests
			for (i = 0; i < 5; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			const Vector<int*> otherIntPtrVector = intPtrVector;

			testIndex = 17;

			auto intPtrVectorSubscript = [&] { intPtrVector[testIndex]; };
			Assert::ExpectException<std::exception>(intPtrVectorSubscript);

			for (i = 0; i < 5; i++)
			{
				Assert::AreEqual(&testArray[i], otherIntPtrVector[i]);
			}

			//Foo tests
			fooVector.PushBack(Foo(7));
			fooVector.PushBack(Foo(9));
			fooVector.PushBack(Foo(11));

			const Vector<Foo> otherFooVector = fooVector;

			testIndex = 32;

			auto fooVectorSubscript = [&] { fooVector[testIndex]; };
			Assert::ExpectException<std::exception>(fooVectorSubscript);

			Assert::IsTrue(fooVector[0] == otherFooVector[0]);
			Assert::IsTrue(fooVector[1] == otherFooVector[1]);
			Assert::IsTrue(fooVector[2] == otherFooVector[2]);
		}

		TEST_METHOD(VectorTestNonConstSubscriptOperator)
		{
			std::uint32_t testIndex = 0;

			auto intVectorSubscript = [&] { intVector[testIndex]; };
			auto intPtrVectorSubscript = [&] { intPtrVector[testIndex]; };
			auto fooVectorSubscript = [&] { fooVector[testIndex]; };
			
			Assert::ExpectException<std::exception>(intVectorSubscript);
			Assert::ExpectException<std::exception>(intPtrVectorSubscript);
			Assert::ExpectException<std::exception>(fooVectorSubscript);

			//int tests
			intVector.Reserve(3);

			intVector.PushBack(3);
			intVector.PushBack(4);
			intVector.PushBack(5);

			testIndex = 5;
			Assert::ExpectException<std::exception>(intVectorSubscript);

			Assert::AreEqual(intVector[0], intVector.Front());
			Assert::AreEqual(intVector[1], 4);
			Assert::AreEqual(intVector[2], intVector.Back());

			//int* tests
			intPtrVector.Reserve(5);

			for (int i = 0; i < 5; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
				Assert::AreEqual(intPtrVector[i], &testArray[i]);
			}

			testIndex = 66;
			Assert::ExpectException<std::exception>(intPtrVectorSubscript);

			//Foo tests
			fooVector.Reserve(4);

			fooVector.PushBack(Foo(12));
			fooVector.PushBack(Foo(21));
			fooVector.PushBack(fooVector[0]);
			fooVector.PushBack(fooVector[1]);

			testIndex = 7000000;
			Assert::ExpectException<std::exception>(fooVectorSubscript);

			Assert::IsTrue(fooVector[0] == fooVector[2]);
			Assert::IsTrue(fooVector[1] == fooVector[3]);
		}

		TEST_METHOD(VectorTestFind)
		{
			//int tests
			intVector.Reserve(4);

			intVector.PushBack(2);
			intVector.PushBack(3);
			intVector.PushBack(7);
			intVector.PushBack(9);

			Assert::IsTrue(intVector.Find(2) == intVector.begin());
			Assert::IsTrue(*(intVector.Find(9)) == intVector.Back());
			Assert::IsTrue(*(intVector.Find(3)) == intVector[1]);
			Assert::IsTrue(*(intVector.Find(7)) == intVector.At(2));

			//int* tests
			intPtrVector.Reserve(3);

			for (int i = 0; i < 3; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			Assert::IsTrue(intPtrVector.Find(&testArray[4]) == intPtrVector.end());
			Assert::IsTrue(*(intPtrVector.Find(&testArray[0])) == intPtrVector.Front());

			//Foo tests
			fooVector.Reserve(4);

			Foo testFoo(44);

			fooVector.PushBack(testFoo);
			fooVector.PushBack(Foo(55));
			fooVector.PushBack(Foo(66));
			fooVector.PushBack(testFoo);

			Assert::IsTrue(fooVector.Find(testFoo) == fooVector.begin());

			fooVector.Remove(testFoo);

			Assert::IsTrue(fooVector.Find(testFoo) != fooVector.end());

			fooVector.Remove(testFoo);

			Assert::IsTrue(fooVector.Find(testFoo) == fooVector.end());
		}

		TEST_METHOD(VectorTestSingleRemoveInt)
		{
			int i;

			intVector.Reserve(5);

			for (i = 0; i < 5; i++)
			{
				intVector.PushBack(testArray[i]);
				Assert::AreEqual((i + 1), intVector.At(i));
			}

			intVector.Remove(3);

			Assert::AreEqual(4U, intVector.Size());
			Assert::AreEqual(1, intVector.At(0));
			Assert::AreEqual(2, intVector.At(1));
			Assert::AreEqual(4, intVector.At(2));
			Assert::AreEqual(5, intVector.At(3));

			intVector.Remove(1);

			Assert::AreEqual(3U, intVector.Size());
			Assert::AreEqual(2, intVector.At(0));
			Assert::AreEqual(4, intVector.At(1));
			Assert::AreEqual(5, intVector.At(2));

			intVector.Remove(5);

			Assert::AreEqual(2U, intVector.Size());
			Assert::AreEqual(2, intVector.At(0));
			Assert::AreEqual(4, intVector.At(1));

			Assert::AreEqual(5U, intVector.Capacity());
		}

		TEST_METHOD(VectorTestSingleRemoveIntPtr)
		{
			int i;

			intPtrVector.Reserve(4);

			for (i = 0; i < 4; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
			}

			intPtrVector.Remove(&testArray[3]);

			Assert::AreEqual(&testArray[0], intPtrVector[0]);
			Assert::AreEqual(&testArray[1], intPtrVector[1]);
			Assert::AreEqual(&testArray[2], intPtrVector[2]);

			Assert::ExpectException<std::exception>(([&] { intPtrVector[3]; }));

			intPtrVector.Remove(&testArray[0]);

			Assert::AreEqual(&testArray[1], intPtrVector.Front());

			intPtrVector.Remove(&testArray[1]);

			Assert::AreEqual(intPtrVector.Front(), intPtrVector.Back());

			intPtrVector.Remove(&testArray[2]);

			Assert::IsTrue(intPtrVector.IsEmpty());
		}

		TEST_METHOD(VectorTestSingleRemoveFoo)
		{
			Foo testFoo(46);
			Foo otherFoo(64);

			fooVector.PushBack(testFoo);
			fooVector.PushBack(otherFoo);
			fooVector.PushBack(testFoo);

			fooVector.Remove(otherFoo);

			Assert::IsTrue(fooVector.Front() == fooVector.Back());

			fooVector.Remove(testFoo);

			Assert::IsTrue(fooVector.Front() == fooVector.Back());

			fooVector.Remove(testFoo);

			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(VectorTestRangeRemoveInt)
		{
			std::uint32_t i;
			int rangeRemoveTestData[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

			Vector<int> otherIntVector;
			otherIntVector.Reserve(1);
			otherIntVector.PushBack(1);

			intVector.Reserve(10);

			for (i = 0; i < 10; i++)
			{
				intVector.PushBack(rangeRemoveTestData[i]);
			}

			Vector<int>::Iterator begin = intVector.Find(3);
			Vector<int>::Iterator end = otherIntVector.begin();

			Assert::ExpectException<std::exception>([&] {intVector.Remove(begin, end); });

			end = intVector.Find(6);

			Assert::ExpectException<std::exception>([&] {intVector.Remove(end, begin); });

			begin = intVector.Find(6);
			intVector.Remove(begin, end);
			Assert::AreEqual(10U, intVector.Size());

			begin = intVector.Find(3);
			intVector.Remove(begin, end);
			Assert::AreEqual(7U, intVector.Size());

			for (i = 0; i < intVector.Size(); i++)
			{
				if (i >= 2)
				{
					Assert::AreEqual(rangeRemoveTestData[i + 3], intVector[i]);
				}
				else
				{
					Assert::AreEqual(rangeRemoveTestData[i], intVector[i]);
				}
			}

			intVector.Remove(intVector.begin(), intVector.end());

			Assert::IsTrue(intVector.IsEmpty());
		}

		TEST_METHOD(VectorTestRangeRemoveIntPtr)
		{
			std::uint32_t i;
			int rangeRemoveTestData[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

			Vector<int*> otherIntPtrVector;
			otherIntPtrVector.Reserve(1);
			otherIntPtrVector.PushBack(&rangeRemoveTestData[0]);

			intPtrVector.Reserve(5);

			for (i = 0; i < 5; i++)
			{
				intPtrVector.PushBack(&rangeRemoveTestData[i]);
			}

			Vector<int*>::Iterator begin = intPtrVector.begin();
			Vector<int*>::Iterator end = otherIntPtrVector.begin();

			Assert::ExpectException<std::exception>([&] {intPtrVector.Remove(begin, end); });

			begin = intPtrVector.Find(&rangeRemoveTestData[3]);

			Assert::ExpectException<std::exception>([&] {intPtrVector.Remove(begin, end); });

			end = intPtrVector.Find(&rangeRemoveTestData[3]);
			intPtrVector.Remove(begin, end);
			Assert::AreEqual(5U, intPtrVector.Size());

			end = intPtrVector.end();
			intPtrVector.Remove(begin, end);
			Assert::AreEqual(3U, intPtrVector.Size());

			for (i = 0; i < intPtrVector.Size(); i++)
			{
				Assert::AreEqual(&rangeRemoveTestData[i], intPtrVector[i]);
			}
		}

		TEST_METHOD(VectorTestRangeRemoveFoo)
		{
			std::uint32_t i;
			int rangeRemoveTestData[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

			Vector<Foo> otherFooVector;
			otherFooVector.Reserve(1);
			otherFooVector.PushBack(Foo(rangeRemoveTestData[0]));

			Vector<Foo>::Iterator begin = fooVector.begin();
			Vector<Foo>::Iterator end = fooVector.end();

			fooVector.Reserve(6);

			Foo targetFoo(rangeRemoveTestData[5]);

			for (i = 0; i < 6; i++)
			{
				fooVector.PushBack(Foo(rangeRemoveTestData[i]));
			}

			begin = fooVector.end();
			end = otherFooVector.end();

			Assert::ExpectException<std::exception>([&] {fooVector.Remove(begin, end); });

			end = fooVector.Find(targetFoo);

			Assert::ExpectException<std::exception>([&] {fooVector.Remove(begin, end); });

			end = fooVector.end();

			Assert::AreEqual(6U, fooVector.Size());

			begin = fooVector.begin();
			end = fooVector.Find(targetFoo);
			fooVector.Remove(begin, end);

			Assert::AreEqual(1U, fooVector.Size());
			Assert::IsTrue(fooVector.Front() == fooVector.Back());

			begin = fooVector.begin();
			
			fooVector.Remove(begin, fooVector.end());
			Assert::IsTrue(fooVector.IsEmpty());
		}

		TEST_METHOD(VectorTestClear)
		{
			//int tests
			intVector.Reserve(3);

			intVector.PushBack(3);
			intVector.PushBack(4);
			intVector.PushBack(5);
			
			intVector.Clear();
			Assert::IsTrue(intVector.IsEmpty());

			//int* tests
			intPtrVector.Reserve(5);

			for (int i = 0; i < 5; i++)
			{
				intPtrVector.PushBack(&testArray[i]);
				Assert::AreEqual(intPtrVector[i], &testArray[i]);
			}

			intPtrVector.Clear();
			Assert::IsTrue(intPtrVector.IsEmpty());

			//Foo tests
			fooVector.Reserve(4);

			fooVector.PushBack(Foo(12));
			fooVector.PushBack(Foo(21));
			fooVector.PushBack(fooVector[0]);
			fooVector.PushBack(fooVector[1]);

			fooVector.Clear();
			Assert::IsTrue(fooVector.IsEmpty());
		}
	private:
		static Vector<int> intVector;
		static Vector<int*> intPtrVector;
		static Vector<Foo> fooVector;
		static int testArray[5];
		static DefaultIncrementFunctor defaultFunctor;

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

	Vector<int> VectorTest::intVector;
	Vector<int*> VectorTest::intPtrVector;
	Vector<Foo> VectorTest::fooVector;
	int VectorTest::testArray[] = { 1, 2, 3, 4, 5 };
	DefaultIncrementFunctor VectorTest::defaultFunctor;

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState VectorTest::startMemState;
#endif
}