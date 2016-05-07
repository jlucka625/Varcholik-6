#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapIteratorTest)
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
		TEST_METHOD(HashMapIteratorTestCopyConstructor)
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
			}

			HashMap<int, int>::Iterator intMapIterator = intMap.begin();
			Assert::IsTrue(intMapIterator == intMap.begin());

			HashMap<int, int>::Iterator otherIntMapIterator = intMapIterator;
			Assert::IsTrue(intMapIterator == intMapIterator);
			Assert::IsTrue(intMapIterator == intMap.begin());

			//int* tests
			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
			}

			HashMap<int*, int>::Iterator found = intPtrMap.Find(&testIntArray[13]);
			Assert::AreEqual((*found).first, &testIntArray[13]);
			Assert::AreEqual((*found).second, testIntArray[13]);

			HashMap<int*, int>::Iterator intPtrMapIterator = found;
			Assert::IsTrue(intPtrMapIterator == found);

			//char* tests
			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
			}

			HashMap<char*, int>::Iterator insertion = charPtrMap.Insert(HashMap<char*, int>::PairType("Stellar", i), wasInserted);
			Assert::AreEqual(strcmp((*insertion).first, "Stellar"), 0);
			Assert::AreEqual((*insertion).second, 20);

			HashMap<char*, int>::Iterator charPtrMapIterator = insertion;
			Assert::IsTrue(charPtrMapIterator == insertion);

			//std::string tests
			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
			}

			HashMap<std::string, int>::Iterator newInsertion = stringMap.Insert(HashMap<std::string, int>::PairType("Groovy", 27), wasInserted);
			Assert::AreEqual(strcmp((*newInsertion).first.c_str(), "Groovy"), 0);
			Assert::AreEqual((*newInsertion).second, 27);

			HashMap<std::string, int>::Iterator stringMapIterator = newInsertion;
			Assert::IsTrue(stringMapIterator == newInsertion);

			//Foo tests
			Foo fooKey(6);

			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
			}

			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator fooMapIterator = fooMap.end();
			Assert::IsTrue(fooMapIterator == fooMap.end());

			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator otherFooMapIterator = fooMapIterator;
			Assert::IsTrue(otherFooMapIterator == fooMapIterator);
			Assert::IsTrue(otherFooMapIterator == fooMap.end());
		}

		TEST_METHOD(HashMapIteratorTestAssignmentOperator)
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
			}

			HashMap<int, int>::Iterator intMapIterator;
			intMapIterator = intMap.begin();
			Assert::IsTrue(intMapIterator == intMap.begin());

			HashMap<int, int>::Iterator otherIntMapIterator;
			otherIntMapIterator = intMapIterator;
			Assert::IsTrue(intMapIterator == intMapIterator);
			Assert::IsTrue(intMapIterator == intMap.begin());

			//int* tests
			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
			}

			HashMap<int*, int>::Iterator found;
			found = intPtrMap.Find(&testIntArray[13]);
			Assert::AreEqual((*found).first, &testIntArray[13]);
			Assert::AreEqual((*found).second, testIntArray[13]);

			HashMap<int*, int>::Iterator intPtrMapIterator;
			intPtrMapIterator = found;
			Assert::IsTrue(intPtrMapIterator == found);

			//char* tests
			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
			}

			HashMap<char*, int>::Iterator insertion;
			insertion = charPtrMap.Insert(HashMap<char*, int>::PairType("Stellar", i), wasInserted);
			Assert::AreEqual(strcmp((*insertion).first, "Stellar"), 0);
			Assert::AreEqual((*insertion).second, 20);

			HashMap<char*, int>::Iterator charPtrMapIterator;
			charPtrMapIterator = insertion;
			Assert::IsTrue(charPtrMapIterator == insertion);

			//std::string tests
			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
			}

			HashMap<std::string, int>::Iterator newInsertion;
			newInsertion = stringMap.Insert(HashMap<std::string, int>::PairType("Groovy", 27), wasInserted);
			Assert::AreEqual(strcmp((*newInsertion).first.c_str(), "Groovy"), 0);
			Assert::AreEqual((*newInsertion).second, 27);

			HashMap<std::string, int>::Iterator stringMapIterator;
			stringMapIterator = newInsertion;
			Assert::IsTrue(stringMapIterator == newInsertion);

			//Foo tests
			Foo fooKey(6);

			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
			}

			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator fooMapIterator;
			fooMapIterator = fooMap.end();
			Assert::IsTrue(fooMapIterator == fooMap.end());

			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator otherFooMapIterator;
			otherFooMapIterator = fooMapIterator;
			Assert::IsTrue(otherFooMapIterator == fooMapIterator);
			Assert::IsTrue(otherFooMapIterator == fooMap.end());
		}

		TEST_METHOD(HashMapIteratorTestMoveSemantics)
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
			}

			auto intMapIterator = intMap.Find(6);

			auto otherIntMapIterator = std::move(intMapIterator);
			Assert::AreEqual(6, otherIntMapIterator->first);
			Assert::AreEqual(7, otherIntMapIterator->second);

			HashMap<int, int>::Iterator yetAnotherIntMapIterator;
			yetAnotherIntMapIterator = std::move(otherIntMapIterator);
			Assert::AreEqual(6, yetAnotherIntMapIterator->first);
			Assert::AreEqual(7, yetAnotherIntMapIterator->second);

			//int* tests
			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
			}

			int* testKey = new int(45);

			auto intPtrMapIterator = intPtrMap.Insert(HashMap<int*, int>::PairType(testKey, 45), wasInserted);

			auto otherIntPtrMapIterator = std::move(intPtrMapIterator);
			Assert::AreEqual(testKey, otherIntPtrMapIterator->first);
			Assert::AreEqual(45, otherIntPtrMapIterator->second);

			HashMap<int*, int>::Iterator yetAnotherIntPtrMapIterator;
			yetAnotherIntPtrMapIterator = std::move(otherIntPtrMapIterator);
			Assert::AreEqual(testKey, yetAnotherIntPtrMapIterator->first);
			Assert::AreEqual(45, yetAnotherIntPtrMapIterator->second);

			delete testKey;

			//char* tests
			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
			}

			auto charPtrMapIterator = charPtrMap.Insert(HashMap<char*, int>::PairType("Help Me Please", 505), wasInserted);

			auto otherCharPtrMapIterator = std::move(charPtrMapIterator);
			Assert::AreEqual("Help Me Please", otherCharPtrMapIterator->first);
			Assert::AreEqual(505, otherCharPtrMapIterator->second);

			HashMap<char*, int>::Iterator yetAnotherCharPtrMapIterator;
			yetAnotherCharPtrMapIterator = std::move(otherCharPtrMapIterator);
			Assert::AreEqual("Help Me Please", yetAnotherCharPtrMapIterator->first);
			Assert::AreEqual(505, yetAnotherCharPtrMapIterator->second);

			//std::string tests
			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
			}

			auto stringMapIterator = stringMap.Insert(HashMap<std::string, int>::PairType("Crying", 77), wasInserted);

			auto otherStringMapIterator = std::move(stringMapIterator);
			Assert::IsTrue("Crying" == otherStringMapIterator->first);
			Assert::AreEqual(77, otherStringMapIterator->second);

			HashMap<std::string, int>::Iterator yetAnotherStringMapIterator;
			yetAnotherStringMapIterator = std::move(otherStringMapIterator);
			Assert::IsTrue("Crying" == yetAnotherStringMapIterator->first);
			Assert::AreEqual(77, yetAnotherStringMapIterator->second);

			//Foo tests
			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int, CustomHashFunctor<Foo>>::PairType(testFooArray[i], i), wasInserted);
			}

			auto fooMapIterator = fooMap.Find(testFooArray[15]);

			auto otherFooMapIterator = std::move(fooMapIterator);
			Assert::IsTrue(testFooArray[15] == otherFooMapIterator->first);
			Assert::AreEqual(15, otherFooMapIterator->second);

			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator yetAnotherFooMapIterator;
			yetAnotherFooMapIterator = std::move(otherFooMapIterator);
			Assert::IsTrue(testFooArray[15] == yetAnotherFooMapIterator->first);
			Assert::AreEqual(15, yetAnotherFooMapIterator->second);
		}

		TEST_METHOD(HashMapIteratorTestEquality)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			//int tests
			HashMap<int, int>::Iterator intMapIterator, otherIntMapIterator;
			Assert::IsTrue(intMapIterator == otherIntMapIterator);

			HashMap<int, int> otherIntMap(20);

			for (i = 0; i < 20; i++)
			{
				intMap.Insert(HashMap<int, int>::PairType(i, (i + 1)), wasInserted);
				otherIntMap.Insert(HashMap<int, int>::PairType(i, (i + 1)), wasInserted);
			}

			intMapIterator = intMap.begin();
			otherIntMapIterator = otherIntMap.begin();
			Assert::IsTrue(intMapIterator != otherIntMapIterator);

			intMapIterator = intMap.Insert(HashMap<int, int>::PairType(64, 46), wasInserted);
			otherIntMapIterator = intMap.Insert(HashMap<int, int>::PairType(46, 64), wasInserted);
			Assert::IsTrue(intMapIterator != otherIntMapIterator);

			otherIntMapIterator = intMap.Find(64);
			Assert::IsTrue(intMapIterator == otherIntMapIterator);

			//int* tests
			HashMap<int*, int>::Iterator intPtrMapIterator, otherIntPtrMapIterator;
			Assert::IsTrue(intPtrMapIterator == otherIntPtrMapIterator);

			HashMap<int*, int> otherIntPtrMap(30);

			for (i = 0; i < 30; i++)
			{
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
				otherIntPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
			}

			intPtrMapIterator = intPtrMap.end();
			otherIntPtrMapIterator = otherIntPtrMap.end();
			Assert::IsTrue(intPtrMapIterator != otherIntPtrMapIterator);

			int* newKey = new int(62);
			int* otherNewKey = new int(44);

			intPtrMapIterator = intPtrMap.Insert(HashMap<int*, int>::PairType(newKey, 46), wasInserted);
			otherIntPtrMapIterator = intPtrMap.Insert(HashMap<int*, int>::PairType(otherNewKey, 64), wasInserted);
			Assert::IsTrue(intPtrMapIterator != otherIntPtrMapIterator);

			intPtrMapIterator = intPtrMap.Find(otherNewKey);
			Assert::IsTrue(intMapIterator == otherIntMapIterator);

			delete newKey;
			delete otherNewKey;

			//char* tests
			HashMap<char*, int>::Iterator charPtrMapIterator, otherCharPtrMapIterator;
			Assert::IsTrue(charPtrMapIterator == otherCharPtrMapIterator);

			HashMap<char*, int> otherCharPtrMap(20);

			for (i = 0; i < 20; i++)
			{
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
				otherCharPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
			}

			charPtrMapIterator = charPtrMap.Find(testStringArray1[14]);
			otherCharPtrMapIterator = otherCharPtrMap.Find(testStringArray1[14]);
			Assert::IsTrue(charPtrMapIterator != otherCharPtrMapIterator);

			otherCharPtrMapIterator = charPtrMap.Find(testStringArray1[14]);
			Assert::IsTrue(charPtrMapIterator == otherCharPtrMapIterator);

			//std::string tests
			HashMap<std::string, int>::Iterator stringMapIterator, otherStringMapIterator;
			Assert::IsTrue(stringMapIterator == otherStringMapIterator);

			HashMap<std::string, int> otherStringMap(20);

			for (i = 0; i < 20; i++)
			{
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
				otherStringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], (i + 1)), wasInserted);
			}

			stringMapIterator = stringMap.Find(testStringArray2[6]);
			otherStringMapIterator = otherStringMap.Find(testStringArray2[6]);
			Assert::IsTrue(stringMapIterator != otherStringMapIterator);

			stringMapIterator = otherStringMap.Find(testStringArray2[6]);
			Assert::IsTrue(stringMapIterator == otherStringMapIterator);

			//Foo tests
			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator fooMapIterator, otherFooMapIterator;
			Assert::IsTrue(fooMapIterator == otherFooMapIterator);

			HashMap<Foo, int, CustomHashFunctor<Foo>> otherFooMap(25);

			for (i = 0; i < 25; i++)
			{
				fooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
				otherFooMap.Insert(HashMap<Foo, int>::PairType(testFooArray[i], i), wasInserted);
			}

			fooMapIterator = fooMap.Find(testFooArray[7]);
			otherFooMapIterator = otherFooMap.Find(testFooArray[7]);
			Assert::IsTrue(fooMapIterator != otherFooMapIterator);

			fooMapIterator = otherFooMap.Find(testFooArray[7]);
			Assert::IsTrue(fooMapIterator == otherFooMapIterator);
		}

		TEST_METHOD(HashMapIteratorTestInvalidOperations)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			//int tests
			HashMap<int, int>::Iterator intMapIterator;

			Assert::ExpectException<std::exception>([&] {++intMapIterator; });
			Assert::ExpectException<std::exception>([&] {intMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*intMapIterator; });
			Assert::ExpectException<std::exception>([&] {intMapIterator->first; });

			intMapIterator = intMap.begin();

			Assert::ExpectException<std::exception>([&] {++intMapIterator; });
			Assert::ExpectException<std::exception>([&] {intMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*intMapIterator; });
			Assert::ExpectException<std::exception>([&] {intMapIterator->first; });

			//int* tests
			HashMap<int*, int>::Iterator intPtrMapIterator;

			Assert::ExpectException<std::exception>([&] {++intPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {intPtrMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*intPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {intPtrMapIterator->first; });

			intPtrMapIterator = intPtrMap.begin();

			Assert::ExpectException<std::exception>([&] {++intPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {intPtrMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*intPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {intPtrMapIterator->first; });

			//char* tests
			HashMap<char*, int>::Iterator charPtrMapIterator;

			Assert::ExpectException<std::exception>([&] {++charPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {charPtrMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*charPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {charPtrMapIterator->first; });

			charPtrMapIterator = charPtrMap.begin();

			Assert::ExpectException<std::exception>([&] {++charPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {charPtrMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*charPtrMapIterator; });
			Assert::ExpectException<std::exception>([&] {charPtrMapIterator->first; });

			//std::string tests
			HashMap<std::string, int>::Iterator stringMapIterator;

			Assert::ExpectException<std::exception>([&] {++stringMapIterator; });
			Assert::ExpectException<std::exception>([&] {stringMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*stringMapIterator; });
			Assert::ExpectException<std::exception>([&] {stringMapIterator->first; });

			stringMapIterator = stringMap.begin();

			Assert::ExpectException<std::exception>([&] {++stringMapIterator; });
			Assert::ExpectException<std::exception>([&] {stringMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*stringMapIterator; });
			Assert::ExpectException<std::exception>([&] {stringMapIterator->first; });

			//Foo tests
			HashMap<Foo, int, CustomHashFunctor<Foo>>::Iterator fooMapIterator;

			Assert::ExpectException<std::exception>([&] {++fooMapIterator; });
			Assert::ExpectException<std::exception>([&] {fooMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*fooMapIterator; });
			Assert::ExpectException<std::exception>([&] {fooMapIterator->first; });

			fooMapIterator = fooMap.begin();

			Assert::ExpectException<std::exception>([&] {++fooMapIterator; });
			Assert::ExpectException<std::exception>([&] {fooMapIterator++; });
			Assert::ExpectException<std::exception>([&] {*fooMapIterator; });
			Assert::ExpectException<std::exception>([&] {fooMapIterator->first; });
		}

		TEST_METHOD(HashMapIteratorTestPostfixIncrement)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			for (i = 0; i < 10; i++)
			{
				intMap.Insert(HashMap<int, int>::PairType(i, i), wasInserted);
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], testIntArray[i]), wasInserted);
				fooMap.Insert(HashMap<Foo, int, CustomHashFunctor<Foo>>::PairType(Foo(i), i), wasInserted);
			}

			//int tests
			for (auto it = intMap.begin(); it != intMap.end(); it++)
			{
				if (it->first <= 5)
				{
					Assert::IsTrue(it->first <= 5);
				}
				else
				{
					Assert::IsTrue(it->first > 5);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}

			//int* tests
			for (auto it = intPtrMap.begin(); it != intPtrMap.end(); it++)
			{
				if (it->first <= &testIntArray[i])
				{
					Assert::IsTrue(it->first <= &testIntArray[i]);
				}
				else
				{
					Assert::IsTrue(it->first > &testIntArray[i]);
				}

				if (it->second <= testIntArray[5])
				{
					Assert::IsTrue(it->second <= testIntArray[5]);
				}
				else
				{
					Assert::IsTrue(it->second > testIntArray[5]);
				}
			}

			//char* tests
			for (auto it = charPtrMap.begin(); it != charPtrMap.end(); it++)
			{
				int strCmpResult = strcmp(it->first, testStringArray1[5]);

				if (strCmpResult <= 0)
				{
					Assert::IsTrue(strCmpResult <= 0);
				}
				else
				{
					Assert::IsTrue(strCmpResult > 0);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}

			//std::string tests
			for (auto it = stringMap.begin(); it != stringMap.end(); it++)
			{
				int strCmpResult = strcmp(it->first.c_str(), testStringArray2[5]);

				if (strCmpResult <= 0)
				{
					Assert::IsTrue(strCmpResult <= 0);
				}
				else
				{
					Assert::IsTrue(strCmpResult > 0);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}

			//Foo tests
			for (auto it = fooMap.begin(); it != fooMap.end(); it++)
			{
				if ((it->first).Data() <= 5)
				{
					Assert::IsTrue((it->first).Data() <= 5);
				}
				else
				{
					Assert::IsTrue((it->first).Data() > 5);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}
		}

		TEST_METHOD(HashMapIteratorTestArrowOperator)
		{
			HashMap<int, int> intMap(20);
			HashMap<int*, int> intPtrMap(30);
			HashMap<char*, int> charPtrMap(20);
			HashMap<std::string, int> stringMap(20);
			HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap(25);

			int i;
			bool wasInserted;

			for (i = 0; i < 10; i++)
			{
				intMap.Insert(HashMap<int, int>::PairType(i, i), wasInserted);
				intPtrMap.Insert(HashMap<int*, int>::PairType(&testIntArray[i], testIntArray[i]), wasInserted);
				charPtrMap.Insert(HashMap<char*, int>::PairType(testStringArray1[i], i), wasInserted);
				stringMap.Insert(HashMap<std::string, int>::PairType(testStringArray2[i], testIntArray[i]), wasInserted);
				fooMap.Insert(HashMap<Foo, int, CustomHashFunctor<Foo>>::PairType(Foo(i), i), wasInserted);
			}

			//int tests
			for (auto it = intMap.begin(); it != intMap.end(); ++it)
			{
				if (it->first <= 5)
				{
					Assert::IsTrue(it->first <= 5);
				}
				else
				{
					Assert::IsTrue(it->first > 5);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}

			//int* tests
			for (auto it = intPtrMap.begin(); it != intPtrMap.end(); ++it)
			{
				if (it->first <= &testIntArray[i])
				{
					Assert::IsTrue(it->first <= &testIntArray[i]);
				}
				else
				{
					Assert::IsTrue(it->first > &testIntArray[i]);
				}

				if (it->second <= testIntArray[5])
				{
					Assert::IsTrue(it->second <= testIntArray[5]);
				}
				else
				{
					Assert::IsTrue(it->second > testIntArray[5]);
				}
			}

			//char* tests
			for (auto it = charPtrMap.begin(); it != charPtrMap.end(); ++it)
			{
				int strCmpResult = strcmp(it->first, testStringArray1[5]);

				if (strCmpResult <= 0)
				{
					Assert::IsTrue(strCmpResult <= 0);
				}
				else
				{
					Assert::IsTrue(strCmpResult > 0);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}

			//std::string tests
			for (auto it = stringMap.begin(); it != stringMap.end(); ++it)
			{
				int strCmpResult = strcmp(it->first.c_str(), testStringArray2[5]);

				if (strCmpResult <= 0)
				{
					Assert::IsTrue(strCmpResult <= 0);
				}
				else
				{
					Assert::IsTrue(strCmpResult > 0);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}

			//Foo tests
			for (auto it = fooMap.begin(); it != fooMap.end(); ++it)
			{
				if ((it->first).Data() <= 5)
				{
					Assert::IsTrue((it->first).Data() <= 5);
				}
				else
				{
					Assert::IsTrue((it->first).Data() > 5);
				}

				if (it->second <= 5)
				{
					Assert::IsTrue(it->second <= 5);
				}
				else
				{
					Assert::IsTrue(it->second > 5);
				}
			}
		}
	private:
		static int testIntArray[30];
		static char testStringArray1[][20];
		static char testStringArray2[][20];
		static Foo testFooArray[25];

		/*static HashMap<int, int> intMap;
		static HashMap<int*, int> intPtrMap;
		static HashMap<char*, int> charPtrMap;
		static HashMap<std::string, int> stringMap;
		static HashMap<Foo, int, CustomHashFunctor<Foo>> fooMap;*/

#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

	int HashMapIteratorTest::testIntArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };
	char HashMapIteratorTest::testStringArray1[][20] = { "This", "is", "just", "an", "array", "of", "character", "arrays", "for", "Hash", "Map", "testing", "purposes", "For", "Assignnment", "6", "fOr", "Paul's", "Prog2", "class" };
	char HashMapIteratorTest::testStringArray2[][20] = { "How", "about", "another", "array", "of", "character", "arrays", "just", "for", "fun?", "Sounds", "good", "to", "me!", "HOW", "abOUt", "you?", "Is", "that", "fine?!?" };
	Foo HashMapIteratorTest::testFooArray[25] = { Foo(2), Foo(4), Foo(6), Foo(8), Foo(10), Foo(12), Foo(14), Foo(16), Foo(18), Foo(20), Foo(22), Foo(24), Foo(26), Foo(28), Foo(30), Foo(32), Foo(34), Foo(36), Foo(38), Foo(40), Foo(42), Foo(44), Foo(46), Foo(48), Foo(50) };

	/*HashMap<int, int> HashMapIteratorTest::intMap(20);
	HashMap<int*, int> HashMapIteratorTest::intPtrMap(30);
	HashMap<char*, int> HashMapIteratorTest::charPtrMap(20);
	HashMap<std::string, int> HashMapIteratorTest::stringMap(20);
	HashMap<Foo, int, CustomHashFunctor<Foo>> HashMapIteratorTest::fooMap(25);*/

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState HashMapIteratorTest::startMemState;
#endif
}