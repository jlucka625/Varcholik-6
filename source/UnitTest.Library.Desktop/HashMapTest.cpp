#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
	{
	public:
#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(PrepForTests)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&startMemState);
		}

		TEST_METHOD_CLEANUP(CleanupTestObjects)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(HashMapTestDefaultConstructor)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			Assert::IsTrue(intMap.IsEmpty());
			Assert::IsTrue(intPtrMap.IsEmpty());
			Assert::IsTrue(charPtrMap.IsEmpty());
			Assert::IsTrue(stringMap.IsEmpty());
			Assert::IsTrue(fooMap.IsEmpty());
		}

		TEST_METHOD(HashMapTestCopyConstructor)
		{
			HashMap<int, int> intMap(3);
			intMap[0] = 1;
			intMap[1] = 2;
			intMap[2] = 3;

			HashMap<int, int> otherIntMap(std::move(intMap));
			Assert::IsTrue(otherIntMap.Size() == 3U);
			Assert::IsTrue(otherIntMap[0] == 1);
			Assert::IsTrue(otherIntMap[1] == 2);
			Assert::IsTrue(otherIntMap[2] == 3);
		}

		TEST_METHOD(HashMapTestAssignmentOperator)
		{
			HashMap<int, int> intMap(3);
			intMap[0] = 1;
			intMap[1] = 2;
			intMap[2] = 3;

			HashMap<int, int> otherIntMap(3);
			otherIntMap = intMap;
			Assert::IsTrue(otherIntMap.Size() == 3U);
			Assert::IsTrue(otherIntMap[0] == 1);
			Assert::IsTrue(otherIntMap[1] == 2);
			Assert::IsTrue(otherIntMap[2] == 3);
		}

		TEST_METHOD(HashMapTestSubscriptOperator)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;

			//int tests
			int previousTestValue;

			for (i = 0; i < 20; i++)
			{
				intMap[i] = (i + 1);

				if (i == 0)
				{
					previousTestValue = (i + 1);
				}

				Assert::IsTrue(intMap.ContainsKey(i));
				Assert::AreEqual(intMap[i], (i + 1));
				intMap[i] = 24;
			}

			intMap[0] = 24;
			Assert::AreNotEqual(previousTestValue, intMap[0]);
			Assert::AreEqual(intMap[0], 24);

			//int* tests
			for (i = 0; i < 30; i++)
			{
				intPtrMap[&testIntArray[i]] = testIntArray[i];

				if (i == 6)
				{
					previousTestValue = testIntArray[i];
				}

				Assert::IsTrue(intPtrMap.ContainsKey(&testIntArray[i]));
				Assert::AreEqual(intPtrMap[&testIntArray[i]], testIntArray[i]);
			}

			intPtrMap[&testIntArray[6]] = 55;
			Assert::AreNotEqual(previousTestValue, intPtrMap[&testIntArray[6]]);
			Assert::AreEqual(intPtrMap[&testIntArray[6]], 55);

			//char* tests
			for (i = 0; i < 20; i++)
			{
				charPtrMap[testStringArray1[i]] = i;

				if (i == 15)
				{
					previousTestValue = i;
				}

				Assert::IsTrue(charPtrMap.ContainsKey(testStringArray1[i]));
				Assert::AreEqual(charPtrMap[testStringArray1[i]], i);
			}

			charPtrMap[testStringArray1[15]] = 9001;
			Assert::AreNotEqual(previousTestValue, charPtrMap[testStringArray1[15]]);
			Assert::AreEqual(charPtrMap[testStringArray1[15]], 9001);

			//std::string tests
			for (i = 0; i < 20; i++)
			{
				stringMap[testStringArray2[i]] = (i + 1);

				if (i == 8)
				{
					previousTestValue = (i + 1);
				}

				Assert::IsTrue(stringMap.ContainsKey(testStringArray2[i]));
			}

			stringMap[testStringArray2[8]] = 123;
			Assert::AreNotEqual(previousTestValue, stringMap[testStringArray2[8]]);
			Assert::AreEqual(stringMap[testStringArray2[8]], 123);

			//Foo tests
			for (i = 0; i < 25; i++)
			{
				if (i == 24)
				{
					previousTestValue = i;
				}

				fooMap[testFooArray[i]] = i;
				Assert::IsTrue(fooMap.ContainsKey(testFooArray[i]));
				Assert::IsTrue(fooMap[testFooArray[i]] == i);
			}

			fooMap[testFooArray[24]] = 777;
			Assert::AreNotEqual(previousTestValue, fooMap[testFooArray[24]]);
			Assert::AreEqual(fooMap[testFooArray[24]], 777);
		}

		TEST_METHOD(HashMapTestMoveSemantics)
		{
			HashMap<int, int> intMap(3);
			intMap[0] = 1;
			intMap[1] = 2;
			intMap[2] = 3;

			HashMap<int, int> otherIntMap(std::move(intMap));
			Assert::IsTrue(otherIntMap.Size() == 3U);
			Assert::IsTrue(otherIntMap[0] == 1);
			Assert::IsTrue(otherIntMap[1] == 2);
			Assert::IsTrue(otherIntMap[2] == 3);

			HashMap<int, int> yetAnotherIntMap(3);
			yetAnotherIntMap = std::move(otherIntMap);
			Assert::IsTrue(yetAnotherIntMap.Size() == 3U);
			Assert::IsTrue(yetAnotherIntMap[0] == 1);
			Assert::IsTrue(yetAnotherIntMap[1] == 2);
			Assert::IsTrue(yetAnotherIntMap[2] == 3);
		}

		TEST_METHOD(HashMapTestEquality)
		{
			HashMap<int, int> intMap(3);
			HashMap<int, int> otherIntMap(3);

			Assert::IsTrue(intMap == otherIntMap);

			intMap[0] = 1;
			intMap[1] = 2;
			intMap[2] = 3;
			Assert::IsTrue(intMap != otherIntMap);

			otherIntMap[0] = 1;
			otherIntMap[1] = 2;
			otherIntMap[2] = 5;
			Assert::IsTrue(intMap != otherIntMap);

			otherIntMap[2] = 3;
			Assert::IsTrue(intMap == otherIntMap);
		}

		TEST_METHOD(HashMapTestFind)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			//int tests
			for (i = 0; i < 20; i++)
			{
				intMap[i * 2] = (i + 1);
				Assert::IsTrue(intMap.Find((i * 2)) != intMap.end());
			}

			//int* tests
			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
				Assert::IsTrue(intPtrMap.Find(&testIntArray[i]) != intPtrMap.end());
			}

			//char* tests
			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
				Assert::IsTrue(charPtrMap.Find(testStringArray1[i]) != charPtrMap.end());
			}

			//std::string tests
			for (i = 0; i < 20; i++)
			{
				stringMap[testStringArray2[i]] = (i + 1);
				Assert::IsTrue(stringMap.ContainsKey(testStringArray2[i]));
			}

			//Foo tests
			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
				Assert::IsTrue(fooMap.Find(testFooArray[i]) != fooMap.end());
			}
		}

		TEST_METHOD(HashMapTestInsert)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			//int tests
			for (i = 0; i < 20; i++)
			{
				intMap.Insert(HashMap<int, int>::PairType(i, (i + 1)), wasInserted);
				Assert::IsTrue(intMap.ContainsKey(i));
				Assert::AreEqual(intMap[i], (i + 1));
			}

			intMap.Insert(HashMap<int, int>::PairType(14, 72), wasInserted);
			Assert::AreNotEqual(intMap[14], 20);
			Assert::AreEqual(intMap[14], 15);

			//int* tests
			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
				Assert::IsTrue(intPtrMap.ContainsKey(&testIntArray[i]));
				Assert::AreEqual(intPtrMap[&testIntArray[i]], testIntArray[i]);
			}

			intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[6], 89), wasInserted);
			Assert::AreNotEqual(intPtrMap[&testIntArray[6]], 89);
			Assert::AreEqual(intPtrMap[&testIntArray[6]], testIntArray[6]);

			//char* tests
			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
				Assert::IsTrue(charPtrMap.ContainsKey(testStringArray1[i]));
				Assert::AreEqual(charPtrMap[testStringArray1[i]], i);
			}

			charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[10], 43), wasInserted);
			Assert::AreNotEqual(charPtrMap[testStringArray1[10]], 43);
			Assert::AreEqual(charPtrMap[testStringArray1[10]], 10);

			//std::string tests
			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
				Assert::IsTrue(stringMap.ContainsKey(testStringArray2[i]));
				Assert::AreEqual(stringMap[testStringArray2[i]], (i + 1));
			}

			stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[7], 66), wasInserted);
			Assert::AreNotEqual(stringMap[testStringArray2[7]], 66);
			Assert::AreEqual(stringMap[testStringArray2[7]], 8);

			//Foo tests
			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
				Assert::IsTrue(fooMap.ContainsKey(testFooArray[i]));
				Assert::AreEqual(fooMap[testFooArray[i]], i);
			}

			fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[24], 9867), wasInserted);
			Assert::AreNotEqual(fooMap[testFooArray[24]], 9867);
			Assert::AreEqual(fooMap[testFooArray[24]], 24);
		}

		TEST_METHOD(HashMapTestRemove)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			//int tests
			intMap.Remove(84);
			Assert::IsTrue(intMap.Find(84) == intMap.end());

			for (i = 0; i < 20; i++)
			{
				intMap.Insert(HashMap<int, int>::PairType(testIntArray[i], i), wasInserted);
				Assert::IsTrue(intMap.ContainsKey(testIntArray[i]));
			}

			for (i = 0; i < 20; i++)
			{
				intMap.Remove(testIntArray[i]);
				Assert::IsFalse(intMap.ContainsKey(testIntArray[i]));
			}

			//int* tests
			intPtrMap.Remove(&testIntArray[15]);
			Assert::IsTrue(intPtrMap.Find(&testIntArray[15]) == intPtrMap.end());

			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], (i * 2)), wasInserted);
				Assert::IsTrue(intPtrMap.ContainsKey(&testIntArray[i]));
			}

			for (i = 0; i < 30; i++)
			{
				intPtrMap.Remove(&testIntArray[i]);
				Assert::IsFalse(intPtrMap.ContainsKey(&testIntArray[i]));
			}

			//char* tests
			charPtrMap.Remove("Dude!");
			Assert::IsTrue(charPtrMap.Find("Dude!") == charPtrMap.end());

			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
				Assert::IsTrue(charPtrMap.ContainsKey(testStringArray1[i]));
			}

			for (i = 0; i < 20; i++)
			{
				charPtrMap.Remove(testStringArray1[i]);
				Assert::IsFalse(charPtrMap.ContainsKey(testStringArray1[i]));
			}

			//std::string tests
			stringMap.Remove("Flibbitydoo");
			Assert::IsTrue(stringMap.Find("Flibbitydoo") == stringMap.end());

			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
				Assert::IsTrue(stringMap.ContainsKey(testStringArray2[i]));
			}

			for (i = 0; i < 20; i++)
			{
				stringMap.Remove(testStringArray2[i]);
				Assert::IsFalse(stringMap.ContainsKey(testStringArray2[i]));
			}

			//Foo tests
			Foo dummyFoo(1847);

			fooMap.Remove(dummyFoo);
			Assert::IsTrue(fooMap.Find(dummyFoo) == fooMap.end());

			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
				Assert::IsTrue(fooMap.ContainsKey(testFooArray[i]));
			}

			for (i = 0; i < 25; i++)
			{
				fooMap.Remove(testFooArray[i]);
				Assert::IsFalse(fooMap.ContainsKey(testFooArray[i]));
			}
		}

		TEST_METHOD(HashMapTestClear)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			for (i = 0; i < 20; i++)
			{
				intMap.Insert(HashMap<int, int>::PairType(i, (i + 1)), wasInserted);
			}

			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
			}

			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
			}

			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
			}

			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
			}

			Assert::AreEqual(20U, intMap.Size());
			Assert::AreEqual(30U, intPtrMap.Size());
			Assert::AreEqual(20U, charPtrMap.Size());
			Assert::AreEqual(20U, stringMap.Size());
			Assert::AreEqual(25U, fooMap.Size());

			intMap.Clear();
			intPtrMap.Clear();
			charPtrMap.Clear();
			stringMap.Clear();
			fooMap.Clear();

			Assert::AreEqual(0U, intMap.Size());
			Assert::AreEqual(0U, intPtrMap.Size());
			Assert::AreEqual(0U, charPtrMap.Size());
			Assert::AreEqual(0U, stringMap.Size());
			Assert::AreEqual(0U, fooMap.Size());
		}
	private:
		static int testIntArray[30];
		static char testStringArray1[][20];
		static char testStringArray2[][20];
		static Foo testFooArray[25];

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

	int HashMapTest::testIntArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };
	char HashMapTest::testStringArray1[][20] = { "This", "is", "just", "an", "array", "of", "character", "arrays", "for", "Hash", "Map", "testing", "purposes", "For", "Assignnment", "6", "fOr", "Paul's", "Prog2", "class" };
	char HashMapTest::testStringArray2[][20] = { "How", "about", "another", "array", "of", "character", "arrays", "just", "for", "fun?", "Sounds", "good", "to", "me!", "HOW", "abOUt", "you?", "Is", "that", "fine?!?" };
	Foo HashMapTest::testFooArray[25] = { Foo(2), Foo(4), Foo(6), Foo(8), Foo(10), Foo(12), Foo(14), Foo(16), Foo(18), Foo(20), Foo(22), Foo(24), Foo(26), Foo(28), Foo(30), Foo(32), Foo(34), Foo(36), Foo(38), Foo(40), Foo(42), Foo(44), Foo(46), Foo(48), Foo(50) };

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState HashMapTest::startMemState;
#endif
}