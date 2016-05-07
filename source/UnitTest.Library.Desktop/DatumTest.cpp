#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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
		TEST_METHOD(DatumTestDefaultConstructor)
		{
			Datum intDatum, floatDatum, vectorDatum, matrixDatum, stringDatum, pointerDatum;

			Assert::IsTrue(intDatum.Type() == Datum::DatumType::Unknown);
			Assert::IsTrue(floatDatum.Type() == Datum::DatumType::Unknown);
			Assert::IsTrue(vectorDatum.Type() == Datum::DatumType::Unknown);
			Assert::IsTrue(matrixDatum.Type() == Datum::DatumType::Unknown);
			Assert::IsTrue(stringDatum.Type() == Datum::DatumType::Unknown);
			Assert::IsTrue(pointerDatum.Type() == Datum::DatumType::Unknown);
		}

		TEST_METHOD(DatumTestIntScalarAssignment)
		{
			Datum intDatum;

			Assert::AreEqual(0U, intDatum.Size());
			Assert::AreEqual(0U, intDatum.Capacity());

			intDatum = 5;

			Assert::IsTrue(intDatum.Type() == Datum::DatumType::Integer);

			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(1U, intDatum.Capacity());
			Assert::AreEqual(intDatum.Get<int>(), 5);

			intDatum = -13;

			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(1U, intDatum.Capacity());
			Assert::AreEqual(intDatum.Get<int>(), -13);

			Assert::ExpectException<std::exception>([&] {intDatum.SetType(Datum::DatumType::Matrix); });
		}

		TEST_METHOD(DatumTestFloatScalarAssignment)
		{
			Datum floatDatum;

			Assert::AreEqual(0U, floatDatum.Size());
			Assert::AreEqual(0U, floatDatum.Capacity());

			floatDatum = 6.3f;

			Assert::IsTrue(floatDatum.Type() == Datum::DatumType::Float);

			Assert::AreEqual(1U, floatDatum.Size());
			Assert::AreEqual(1U, floatDatum.Capacity());
			Assert::AreEqual(floatDatum.Get<float>(), 6.3f);

			floatDatum = -73.76f;

			Assert::AreEqual(1U, floatDatum.Size());
			Assert::AreEqual(1U, floatDatum.Capacity());
			Assert::AreEqual(floatDatum.Get<float>(), -73.76f);

			Assert::ExpectException<std::exception>([&] {floatDatum.SetType(Datum::DatumType::Integer); });
		}

		TEST_METHOD(DatumTestVectorScalarAssignment)
		{
			Datum vectorDatum;

			Assert::AreEqual(0U, vectorDatum.Size());
			Assert::AreEqual(0U, vectorDatum.Capacity());

			glm::vec4 testVec = glm::vec4(1, 2, 3, 4);
			vectorDatum = testVec;

			Assert::IsTrue(vectorDatum.Type() == Datum::DatumType::Vector);

			Assert::AreEqual(1U, vectorDatum.Size());
			Assert::AreEqual(1U, vectorDatum.Capacity());
			Assert::IsTrue(vectorDatum.Get<glm::vec4>() == testVec);

			glm::vec4 newTestVec = glm::vec4(-5, -6, -7, -8);
			vectorDatum = newTestVec;

			Assert::AreEqual(1U, vectorDatum.Size());
			Assert::AreEqual(1U, vectorDatum.Capacity());
			Assert::IsTrue(vectorDatum.Get<glm::vec4>() == newTestVec);

			Assert::ExpectException<std::exception>([&] {vectorDatum.SetType(Datum::DatumType::Float); });
		}

		TEST_METHOD(DatumTestMatrixScalarAssignment)
		{
			Datum matrixDatum;

			Assert::AreEqual(0U, matrixDatum.Size());
			Assert::AreEqual(0U, matrixDatum.Capacity());

			glm::mat4 testMat = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
			matrixDatum = testMat;

			Assert::IsTrue(matrixDatum.Type() == Datum::DatumType::Matrix);

			Assert::AreEqual(1U, matrixDatum.Size());
			Assert::AreEqual(1U, matrixDatum.Capacity());
			Assert::IsTrue(matrixDatum.Get<glm::mat4>() == testMat);

			glm::mat4 newTestMat = glm::mat4(-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16);
			matrixDatum = newTestMat;

			Assert::AreEqual(1U, matrixDatum.Size());
			Assert::AreEqual(1U, matrixDatum.Capacity());
			Assert::IsTrue(matrixDatum.Get<glm::mat4>() == newTestMat);

			Assert::ExpectException<std::exception>([&] {matrixDatum.SetType(Datum::DatumType::Vector); });
		}

		TEST_METHOD(DatumTestStringScalarAssignment)
		{
			Datum stringDatum;

			Assert::AreEqual(0U, stringDatum.Size());
			Assert::AreEqual(0U, stringDatum.Capacity());

			stringDatum = "Hello";

			Assert::IsTrue(stringDatum.Type() == Datum::DatumType::String);

			Assert::AreEqual(1U, stringDatum.Size());
			Assert::AreEqual(1U, stringDatum.Capacity());
			Assert::IsTrue(stringDatum.Get<std::string>() == "Hello");

			stringDatum = "Goodbye";

			Assert::AreEqual(1U, stringDatum.Size());
			Assert::AreEqual(1U, stringDatum.Capacity());
			Assert::IsTrue(stringDatum.Get<std::string>() == "Goodbye");

			Assert::ExpectException<std::exception>([&] {stringDatum.SetType(Datum::DatumType::Pointer); });
		}

		TEST_METHOD(DatumTestPointerScalarAssignment)
		{
			Datum pointerDatum;

			Assert::AreEqual(0U, pointerDatum.Size());
			Assert::AreEqual(0U, pointerDatum.Capacity());

			Foo testFoo(17);
			pointerDatum = &testFoo;
			Assert::AreEqual(1U, pointerDatum.Size());
			Assert::AreEqual(1U, pointerDatum.Capacity());
			Assert::IsTrue(testFoo.Equals(pointerDatum.Get<RTTI*>()));

			Foo otherTestFoo(24);
			pointerDatum = &otherTestFoo;
			Assert::AreEqual(1U, pointerDatum.Size());
			Assert::AreEqual(1U, pointerDatum.Capacity());
			Assert::IsTrue(otherTestFoo.Equals(pointerDatum.Get<RTTI*>()));
		}

		TEST_METHOD(DatumTestGetSetInteger)
		{
			Datum intDatum;

			Assert::ExpectException<std::exception>([&] {intDatum.Get<int>(3); });
			Assert::AreEqual(0U, intDatum.Size());
			Assert::AreEqual(0U, intDatum.Capacity());

			Assert::ExpectException<std::exception>([&] {intDatum.SetSize(3); });

			intDatum.SetType(Datum::DatumType::Integer);
			Assert::ExpectException<std::exception>([&] {intDatum.Get<int>(3); });

			intDatum = 99;
			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(1U, intDatum.Capacity());
			Assert::AreEqual(99, intDatum.Get<int>());

			intDatum.SetSize(3);
			Assert::AreEqual(3U, intDatum.Size());
			Assert::AreEqual(3U, intDatum.Capacity());

			intDatum.SetSize(0);
			Assert::AreEqual(0U, intDatum.Size());
			Assert::AreEqual(3U, intDatum.Capacity());

			intDatum.SetSize(1);
			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(3U, intDatum.Capacity());

			intDatum.SetSize(3);
			Assert::AreEqual(3U, intDatum.Size());
			Assert::AreEqual(3U, intDatum.Capacity());

			intDatum.Set(-67, 1);
			Assert::AreEqual(99, intDatum.Get<int>());
			Assert::AreEqual(-67, intDatum.Get<int>(1));

			intDatum.Set(14, 2);
			Assert::AreEqual(99, intDatum.Get<int>());
			Assert::AreEqual(-67, intDatum.Get<int>(1));
			Assert::AreEqual(14, intDatum.Get<int>(2));

			Assert::ExpectException<std::exception>([&] {intDatum.Get<std::string>(); });
		}

		TEST_METHOD(DatumTestGetSetFloat)
		{
			Datum floatDatum;

			Assert::ExpectException<std::exception>([&] {floatDatum.Get<float>(7); });
			Assert::AreEqual(0U, floatDatum.Size());
			Assert::AreEqual(0U, floatDatum.Capacity());

			Assert::ExpectException<std::exception>([&] {floatDatum.SetSize(3); });

			floatDatum.SetType(Datum::DatumType::Float);
			Assert::ExpectException<std::exception>([&] { floatDatum.Get<float>(3); });

			floatDatum = 12.34f;
			Assert::AreEqual(1U, floatDatum.Size());
			Assert::AreEqual(1U, floatDatum.Capacity());
			Assert::AreEqual(12.34f, floatDatum.Get<float>());

			floatDatum.SetSize(3);
			Assert::AreEqual(3U, floatDatum.Size());
			Assert::AreEqual(3U, floatDatum.Capacity());

			floatDatum.Set(56.78f, 1);
			Assert::AreEqual(12.34f, floatDatum.Get<float>());
			Assert::AreEqual(56.78f, floatDatum.Get<float>(1));

			floatDatum.Set(98.10f, 2);
			Assert::AreEqual(12.34f, floatDatum.Get<float>());
			Assert::AreEqual(56.78f, floatDatum.Get<float>(1));
			Assert::AreEqual(98.10f, floatDatum.Get<float>(2));

			Assert::ExpectException<std::exception>([&] {floatDatum.Get<std::string>(); });
		}

		TEST_METHOD(DatumTestGetSetVector)
		{
			Datum vectorDatum;

			Assert::ExpectException<std::exception>([&] {vectorDatum.Get<glm::vec4>(1); });
			Assert::AreEqual(0U, vectorDatum.Size());
			Assert::AreEqual(0U, vectorDatum.Capacity());

			Assert::ExpectException<std::exception>([&] {vectorDatum.SetSize(3); });

			vectorDatum.SetType(Datum::DatumType::Vector);
			Assert::ExpectException<std::exception>([&] {vectorDatum.Get<glm::vec4>(1); });

			glm::vec4 testVec1 = glm::vec4(1, 2, 3, 4);
			glm::vec4 testVec2 = glm::vec4(5, 6, 7, 8);
			glm::vec4 testVec3 = glm::vec4(9, 10, 11, 12);

			vectorDatum = testVec1;
			Assert::AreEqual(1U, vectorDatum.Size());
			Assert::AreEqual(1U, vectorDatum.Capacity());
			Assert::IsTrue(testVec1 == vectorDatum.Get<glm::vec4>());

			vectorDatum.SetSize(3);
			Assert::AreEqual(3U, vectorDatum.Size());
			Assert::AreEqual(3U, vectorDatum.Capacity());

			vectorDatum.Set(testVec2, 1);
			Assert::IsTrue(testVec1 == vectorDatum.Get<glm::vec4>());
			Assert::IsTrue(testVec2 == vectorDatum.Get<glm::vec4>(1));

			vectorDatum.Set(testVec3, 2);
			Assert::IsTrue(testVec1 == vectorDatum.Get<glm::vec4>());
			Assert::IsTrue(testVec2 == vectorDatum.Get<glm::vec4>(1));
			Assert::IsTrue(testVec3 == vectorDatum.Get<glm::vec4>(2));

			Assert::ExpectException<std::exception>([&] {vectorDatum.Get<std::string>(); });
		}

		TEST_METHOD(DatumTestGetSetMatrix)
		{
			Datum matrixDatum;

			Assert::ExpectException<std::exception>([&] { matrixDatum.Get<glm::mat4>(1); });
			Assert::AreEqual(0U, matrixDatum.Size());
			Assert::AreEqual(0U, matrixDatum.Capacity());

			Assert::ExpectException<std::exception>([&] { matrixDatum.SetSize(3); });

			matrixDatum.SetType(Datum::DatumType::Matrix);
			Assert::ExpectException<std::exception>([&] { matrixDatum.Get<glm::mat4>(1); });

			glm::mat4 testMat1 = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
			glm::mat4 testMat2 = glm::mat4(17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32);
			glm::mat4 testMat3 = glm::mat4(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48);

			matrixDatum = testMat1;
			Assert::AreEqual(1U, matrixDatum.Size());
			Assert::AreEqual(1U, matrixDatum.Capacity());
			Assert::IsTrue(testMat1 == matrixDatum.Get<glm::mat4>());

			matrixDatum.SetSize(3);
			Assert::AreEqual(3U, matrixDatum.Size());
			Assert::AreEqual(3U, matrixDatum.Capacity());

			matrixDatum.Set(testMat2, 1);
			Assert::IsTrue(testMat1 == matrixDatum.Get<glm::mat4>());
			Assert::IsTrue(testMat2 == matrixDatum.Get<glm::mat4>(1));

			matrixDatum.Set(testMat3, 2);
			Assert::IsTrue(testMat1 == matrixDatum.Get<glm::mat4>());
			Assert::IsTrue(testMat2 == matrixDatum.Get<glm::mat4>(1));
			Assert::IsTrue(testMat3 == matrixDatum.Get<glm::mat4>(2));

			Assert::ExpectException<std::exception>([&] { matrixDatum.Get<Scope*>(); });
		}

		TEST_METHOD(DatumTestGetSetTable)
		{
			Datum tableDatum;

			Assert::ExpectException<std::exception>([&] {tableDatum.Get<Scope*>(12); });
			Assert::AreEqual(0U, tableDatum.Size());
			Assert::AreEqual(0U, tableDatum.Capacity());

			Assert::ExpectException<std::exception>([&] {tableDatum.SetSize(3); });

			tableDatum.SetType(Datum::DatumType::Table);
			Assert::ExpectException<std::exception>([&] { tableDatum.Get<Scope*>(1); });

			Scope testScope;
			testScope["Health"] = 100;
			testScope["Name"] = "Cindy";

			Scope otherTestScope;
			testScope["Position"] = glm::vec4(50, 50, 50, 1);
			testScope["FieldGrid"] = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Foo testFoo(13);
			Scope finalTestScope;
			testScope["TestFoo"] = &testFoo;

			tableDatum.Set(testScope);
			tableDatum.Set(otherTestScope, 1);
			tableDatum.Set(finalTestScope, 2);

			Assert::AreEqual(3U, tableDatum.Size());
			Assert::AreEqual(3U, tableDatum.Capacity());

			Assert::IsTrue(testScope == tableDatum[0]);
			Assert::IsTrue(otherTestScope == tableDatum[1]);
			Assert::IsTrue(finalTestScope == tableDatum[2]);

			Assert::ExpectException<std::exception>([&] { tableDatum.Get<std::string>(); });
		}

		TEST_METHOD(DatumTestGetSetString)
		{
			Datum stringDatum;

			Assert::ExpectException<std::exception>([&] {stringDatum.Get<std::string>(12); });
			Assert::AreEqual(0U, stringDatum.Size());
			Assert::AreEqual(0U, stringDatum.Capacity());

			Assert::ExpectException<std::exception>([&] {stringDatum.SetSize(3); });

			stringDatum.SetType(Datum::DatumType::String);
			Assert::ExpectException<std::exception>([&] {stringDatum.Get<std::string>(1); });

			std::string testString1 = "Hello";
			std::string testString2 = "there,";
			std::string testString3 = "Paul!";

			stringDatum = testString1;
			Assert::AreEqual(1U, stringDatum.Size());
			Assert::AreEqual(1U, stringDatum.Capacity());
			Assert::IsTrue(testString1 == stringDatum.Get<std::string>());

			stringDatum.SetSize(3);
			Assert::AreEqual(3U, stringDatum.Size());
			Assert::AreEqual(3U, stringDatum.Capacity());

			stringDatum.Set(testString2, 1);
			Assert::IsTrue(testString1 == stringDatum.Get<std::string>());
			Assert::IsTrue(testString2 == stringDatum.Get<std::string>(1));

			stringDatum.Set(testString3, 2);
			Assert::IsTrue(testString1 == stringDatum.Get<std::string>());
			Assert::IsTrue(testString2 == stringDatum.Get<std::string>(1));
			Assert::IsTrue(testString3 == stringDatum.Get<std::string>(2));

			Assert::ExpectException<std::exception>([&] {stringDatum.Get<glm::mat4>(); });
		}

		TEST_METHOD(DatumTestGetSetPointer)
		{
			Foo testFoo(404);
			Foo otherTestFoo(505);
			Foo yetAnotherTestFoo(606);

			Datum pointerDatum;

			Assert::ExpectException<std::exception>([&] {pointerDatum.Get<RTTI*>(999); });
			Assert::AreEqual(0U, pointerDatum.Size());
			Assert::AreEqual(0U, pointerDatum.Capacity());

			Assert::ExpectException<std::exception>([&] {pointerDatum.SetSize(3); });

			pointerDatum.SetType(Datum::DatumType::Pointer);
			Assert::ExpectException<std::exception>([&] {pointerDatum.Get<RTTI*>(999); });

			pointerDatum = &testFoo;
			Assert::AreEqual(1U, pointerDatum.Size());
			Assert::AreEqual(1U, pointerDatum.Capacity());
			Assert::IsTrue(testFoo.Equals(pointerDatum.Get<RTTI*>()));

			pointerDatum.SetSize(3);
			Assert::AreEqual(3U, pointerDatum.Size());
			Assert::AreEqual(3U, pointerDatum.Capacity());

			pointerDatum.Set(&otherTestFoo, 1);
			Assert::IsTrue(testFoo.Equals(pointerDatum.Get<RTTI*>()));
			Assert::IsTrue(otherTestFoo.Equals(pointerDatum.Get<RTTI*>(1)));

			pointerDatum.Set(&yetAnotherTestFoo, 2);
			Assert::IsTrue(testFoo.Equals(pointerDatum.Get<RTTI*>()));
			Assert::IsTrue(otherTestFoo.Equals(pointerDatum.Get<RTTI*>(1)));
			Assert::IsTrue(yetAnotherTestFoo.Equals(pointerDatum.Get<RTTI*>(2)));

			Assert::ExpectException<std::exception>([&] {pointerDatum.Get<int>(); });
		}

		TEST_METHOD(DatumTestIntSetStorage)
		{
			int testArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
			int otherTestArray[] = { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 };
			int finalTestArray[] = { 31, 32, 33, 34, 35 };

			float dummyArray[] = { 1.1f, 2.2f, 3.3f };

			Datum intDatum;
			Assert::IsFalse(intDatum.IsExternal());
			Assert::AreEqual(intDatum.Size(), 0U);
			Assert::AreEqual(intDatum.Capacity(), 0U);
			Assert::IsTrue(intDatum.Type() == Datum::DatumType::Unknown);

			intDatum.SetStorage(testArray, 10);
			Assert::ExpectException<std::exception>([&] {intDatum.Remove(5); });

			Assert::IsTrue(intDatum.IsExternal());
			Assert::AreEqual(intDatum.Size(), 10U);
			Assert::AreEqual(intDatum.Capacity(), 10U);
			Assert::IsTrue(intDatum.Type() == Datum::DatumType::Integer);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(intDatum.Get<int>(i), testArray[i]);
				Assert::AreEqual(&intDatum.Get<int>(i), &testArray[i]);
			}

			intDatum.SetStorage(otherTestArray, 20);
			Assert::AreEqual(intDatum.Size(), 20U);
			Assert::AreEqual(intDatum.Capacity(), 20U);

			for (int i = 0; i < 20; i++)
			{
				Assert::AreEqual(intDatum.Get<int>(i), otherTestArray[i]);
				Assert::AreEqual(&intDatum.Get<int>(i), &otherTestArray[i]);
			}

			intDatum.SetStorage(finalTestArray, 5);
			Assert::AreEqual(intDatum.Size(), 5U);
			Assert::AreEqual(intDatum.Capacity(), 5U);

			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(intDatum.Get<int>(i), finalTestArray[i]);
				Assert::AreEqual(&intDatum.Get<int>(i), &finalTestArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { intDatum.SetSize(3); });
			Assert::ExpectException<std::exception>([&] { intDatum.SetStorage(dummyArray, 3); });

			Datum otherIntDatum;
			Assert::IsFalse(otherIntDatum.IsExternal());

			otherIntDatum.Set(5, 4);
			otherIntDatum.Set(4, 3);
			otherIntDatum.Set(3, 2);
			otherIntDatum.Set(2, 1);
			otherIntDatum.Set(1, 0);

			otherIntDatum.SetStorage(testArray, 10);

			Assert::IsTrue(otherIntDatum.IsExternal());
			Assert::AreEqual(otherIntDatum.Size(), 10U);
			Assert::AreEqual(otherIntDatum.Capacity(), 10U);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(otherIntDatum.Get<int>(i), testArray[i]);
				Assert::AreEqual(&otherIntDatum.Get<int>(i), &testArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { otherIntDatum.SetSize(5); });
			Assert::ExpectException<std::exception>([&] { otherIntDatum.SetStorage(dummyArray, 3); });
		}

		TEST_METHOD(DatumTestFloatSetStorage)
		{
			float testArray[] = { 1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f, 10.10f };
			float otherTestArray[] = { 11.11f, 12.12f, 13.13f, 14.14f, 15.15f, 16.16f, 17.17f, 18.18f, 19.19f, 20.20f, 21.21f, 22.22f, 23.23f, 24.24f, 25.25f, 26.26f, 27.27f, 28.29f, 29.29f, 30.30f };
			float finalTestArray[] = { 31.31f, 32.32f, 33.33f, 34.34f, 35.35f };

			int dummyArray[] = { 1, 2, 3 };

			Datum floatDatum;
			Assert::IsFalse(floatDatum.IsExternal());
			Assert::AreEqual(floatDatum.Size(), 0U);
			Assert::AreEqual(floatDatum.Capacity(), 0U);
			Assert::IsTrue(floatDatum.Type() == Datum::DatumType::Unknown);

			floatDatum.SetStorage(testArray, 10);
			Assert::ExpectException<std::exception>([&] {floatDatum.Remove(3.3f); });

			Assert::IsTrue(floatDatum.IsExternal());
			Assert::AreEqual(floatDatum.Size(), 10U);
			Assert::AreEqual(floatDatum.Capacity(), 10U);
			Assert::IsTrue(floatDatum.Type() == Datum::DatumType::Float);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(floatDatum.Get<float>(i), testArray[i]);
				Assert::AreEqual(&floatDatum.Get<float>(i), &testArray[i]);
			}

			floatDatum.SetStorage(otherTestArray, 20);
			Assert::AreEqual(floatDatum.Size(), 20U);
			Assert::AreEqual(floatDatum.Capacity(), 20U);

			for (int i = 0; i < 20; i++)
			{
				Assert::AreEqual(floatDatum.Get<float>(i), otherTestArray[i]);
				Assert::AreEqual(&floatDatum.Get<float>(i), &otherTestArray[i]);
			}

			floatDatum.SetStorage(finalTestArray, 5);
			Assert::AreEqual(floatDatum.Size(), 5U);
			Assert::AreEqual(floatDatum.Capacity(), 5U);

			for (int i = 0; i < 5; i++)
			{
				Assert::AreEqual(floatDatum.Get<float>(i), finalTestArray[i]);
				Assert::AreEqual(&floatDatum.Get<float>(i), &finalTestArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { floatDatum.SetSize(3); });
			Assert::ExpectException<std::exception>([&] {floatDatum.SetStorage(dummyArray, 3); });

			Datum otherFloatDatum;

			otherFloatDatum.Set(5.6f, 4);
			otherFloatDatum.Set(4.5f, 3);
			otherFloatDatum.Set(3.4f, 2);
			otherFloatDatum.Set(2.3f, 1);
			otherFloatDatum.Set(1.2f, 0);

			Assert::IsFalse(otherFloatDatum.IsExternal());

			otherFloatDatum.SetStorage(testArray, 10);

			Assert::IsTrue(otherFloatDatum.IsExternal());
			Assert::AreEqual(otherFloatDatum.Size(), 10U);
			Assert::AreEqual(otherFloatDatum.Capacity(), 10U);

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(otherFloatDatum.Get<float>(i), testArray[i]);
				Assert::AreEqual(&otherFloatDatum.Get<float>(i), &testArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { otherFloatDatum.SetSize(5); });
			Assert::ExpectException<std::exception>([&] { otherFloatDatum.SetStorage(dummyArray, 3); });
		}

		TEST_METHOD(DatumTestVectorSetStorage)
		{
			glm::vec4 testArray[] = { glm::vec4(1, 2, 3, 4), glm::vec4(5, 6, 7, 8), glm::vec4(9, 10, 11, 12) };
			glm::vec4 otherTestArray[] = { glm::vec4(13, 14, 15, 16), glm::vec4(17, 18, 19, 20), glm::vec4(21, 22, 23, 24), glm::vec4(25, 26, 27, 28) };
			glm::vec4 finalTestArray[] = { glm::vec4(29, 30, 31, 32), glm::vec4(33, 34, 35, 36), glm::vec4(37, 38, 39, 40), glm::vec4(41, 42, 43, 44), glm::vec4(45, 46, 47, 48) };

			glm::mat4 dummyArray[] = { glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16) };

			Datum vectorDatum;
			Assert::IsFalse(vectorDatum.IsExternal());
			Assert::AreEqual(vectorDatum.Size(), 0U);
			Assert::AreEqual(vectorDatum.Capacity(), 0U);
			Assert::IsTrue(vectorDatum.Type() == Datum::DatumType::Unknown);

			vectorDatum.SetStorage(testArray, 3);
			Assert::ExpectException<std::exception>([&] {vectorDatum.Remove(testArray[2]); });

			Assert::IsTrue(vectorDatum.IsExternal());
			Assert::AreEqual(vectorDatum.Size(), 3U);
			Assert::AreEqual(vectorDatum.Capacity(), 3U);
			Assert::IsTrue(vectorDatum.Type() == Datum::DatumType::Vector);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(vectorDatum.Get<glm::vec4>(i) == testArray[i]);
				Assert::IsTrue(&vectorDatum.Get<glm::vec4>(i) == &testArray[i]);
			}

			vectorDatum.SetStorage(otherTestArray, 4);
			Assert::AreEqual(vectorDatum.Size(), 4U);
			Assert::AreEqual(vectorDatum.Capacity(), 4U);

			for (int i = 0; i < 4; i++)
			{
				Assert::IsTrue(vectorDatum.Get<glm::vec4>(i) == otherTestArray[i]);
				Assert::IsTrue(&vectorDatum.Get<glm::vec4>(i) == &otherTestArray[i]);
			}

			vectorDatum.SetStorage(finalTestArray, 5);
			Assert::AreEqual(vectorDatum.Size(), 5U);
			Assert::AreEqual(vectorDatum.Capacity(), 5U);

			for (int i = 0; i < 5; i++)
			{
				Assert::IsTrue(vectorDatum.Get<glm::vec4>(i) == finalTestArray[i]);
				Assert::IsTrue(&vectorDatum.Get<glm::vec4>(i) == &finalTestArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { vectorDatum.SetSize(3); });
			Assert::ExpectException<std::exception>([&] { vectorDatum.SetStorage(dummyArray, 3); });

			Datum otherVectorDatum;

			otherVectorDatum.Set(glm::vec4(29, 30, 31, 32), 4);
			otherVectorDatum.Set(glm::vec4(33, 34, 35, 36), 3);
			otherVectorDatum.Set(glm::vec4(37, 38, 39, 40), 2);
			otherVectorDatum.Set(glm::vec4(41, 42, 43, 44), 1);
			otherVectorDatum.Set(glm::vec4(45, 46, 47, 48), 0);

			Assert::IsFalse(otherVectorDatum.IsExternal());

			otherVectorDatum.SetStorage(testArray, 3);

			Assert::IsTrue(otherVectorDatum.IsExternal());
			Assert::AreEqual(otherVectorDatum.Size(), 3U);
			Assert::AreEqual(otherVectorDatum.Capacity(), 3U);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(otherVectorDatum.Get<glm::vec4>(i) == testArray[i]);
				Assert::IsTrue(&otherVectorDatum.Get<glm::vec4>(i) == &testArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { otherVectorDatum.SetSize(5); });
			Assert::ExpectException<std::exception>([&] { otherVectorDatum.SetStorage(dummyArray, 3); });
		}

		TEST_METHOD(DatumTestMatrixSetStorage)
		{
			glm::mat4 testArray[] = { glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), glm::mat4(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20), glm::mat4(9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24) };
			glm::mat4 otherTestArray[] = { glm::mat4(13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28), glm::mat4(17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32), glm::mat4(21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36), glm::mat4(25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40) };
			glm::mat4 finalTestArray[] = { glm::mat4(29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44), glm::mat4(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48), glm::mat4(37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52), glm::mat4(41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56), glm::mat4(45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60) };

			std::string dummyArray[] = { "Oops", "I", "did it again" };

			Datum matrixDatum;
			Assert::IsFalse(matrixDatum.IsExternal());
			Assert::AreEqual(matrixDatum.Size(), 0U);
			Assert::AreEqual(matrixDatum.Capacity(), 0U);
			Assert::IsTrue(matrixDatum.Type() == Datum::DatumType::Unknown);

			matrixDatum.SetStorage(testArray, 3);
			Assert::ExpectException<std::exception>([&] {matrixDatum.Remove(testArray[0]); });

			Assert::IsTrue(matrixDatum.IsExternal());
			Assert::AreEqual(matrixDatum.Size(), 3U);
			Assert::AreEqual(matrixDatum.Capacity(), 3U);
			Assert::IsTrue(matrixDatum.Type() == Datum::DatumType::Matrix);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(matrixDatum.Get<glm::mat4>(i) == testArray[i]);
				Assert::IsTrue(&matrixDatum.Get<glm::mat4>(i) == &testArray[i]);
			}

			matrixDatum.SetStorage(otherTestArray, 4);
			Assert::AreEqual(matrixDatum.Size(), 4U);
			Assert::AreEqual(matrixDatum.Capacity(), 4U);

			for (int i = 0; i < 4; i++)
			{
				Assert::IsTrue(matrixDatum.Get<glm::mat4>(i) == otherTestArray[i]);
				Assert::IsTrue(&matrixDatum.Get<glm::mat4>(i) == &otherTestArray[i]);
			}

			matrixDatum.SetStorage(finalTestArray, 5);
			Assert::AreEqual(matrixDatum.Size(), 5U);
			Assert::AreEqual(matrixDatum.Capacity(), 5U);

			for (int i = 0; i < 5; i++)
			{
				Assert::IsTrue(matrixDatum.Get<glm::mat4>(i) == finalTestArray[i]);
				Assert::IsTrue(&matrixDatum.Get<glm::mat4>(i) == &finalTestArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { matrixDatum.SetSize(3); });
			Assert::ExpectException<std::exception>([&] { matrixDatum.SetStorage(dummyArray, 1); });

			Datum otherMatrixDatum;

			otherMatrixDatum.Set(glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16), 4);
			otherMatrixDatum.Set(glm::mat4(13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28), 3);
			otherMatrixDatum.Set(glm::mat4(29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44), 2);
			otherMatrixDatum.Set(glm::mat4(5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20), 1);
			otherMatrixDatum.Set(glm::mat4(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48), 0);

			Assert::IsFalse(otherMatrixDatum.IsExternal());

			otherMatrixDatum.SetStorage(testArray, 3);

			Assert::IsTrue(otherMatrixDatum.IsExternal());
			Assert::AreEqual(otherMatrixDatum.Size(), 3U);
			Assert::AreEqual(otherMatrixDatum.Capacity(), 3U);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(otherMatrixDatum.Get<glm::mat4>(i) == testArray[i]);
				Assert::IsTrue(&otherMatrixDatum.Get<glm::mat4>(i) == &testArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { otherMatrixDatum.SetSize(5); });
			Assert::ExpectException<std::exception>([&] { otherMatrixDatum.SetStorage(dummyArray, 3); });
		}

		TEST_METHOD(DatumTestStringSetStorage)
		{
			std::string testArray[] = { "I", "am", "very", "tired" };
			std::string otherTestArray[] = { "of", "this", "dumb", "assignment" };
			std::string finalTestArray[] = { "Please", "send", "help!" };

			Foo dummyFoo(13);

			RTTI* dummyArray[] = { &dummyFoo };

			Datum stringDatum;
			Assert::IsFalse(stringDatum.IsExternal());
			Assert::AreEqual(stringDatum.Size(), 0U);
			Assert::AreEqual(stringDatum.Capacity(), 0U);
			Assert::IsTrue(stringDatum.Type() == Datum::DatumType::Unknown);

			stringDatum.SetStorage(testArray, 4);
			Assert::ExpectException<std::exception>([&] {stringDatum.Remove("very"); });

			Assert::IsTrue(stringDatum.IsExternal());
			Assert::AreEqual(stringDatum.Size(), 4U);
			Assert::AreEqual(stringDatum.Capacity(), 4U);
			Assert::IsTrue(stringDatum.Type() == Datum::DatumType::String);

			for (int i = 0; i < 4; i++)
			{
				Assert::IsTrue(stringDatum.Get<std::string>(i) == testArray[i]);
				Assert::IsTrue(&(stringDatum.Get<std::string>(i)) == &testArray[i]);
			}

			stringDatum.SetStorage(otherTestArray, 4);
			Assert::AreEqual(stringDatum.Size(), 4U);
			Assert::AreEqual(stringDatum.Capacity(), 4U);

			for (int i = 0; i < 4; i++)
			{
				Assert::IsTrue(stringDatum.Get<std::string>(i) == otherTestArray[i]);
				Assert::IsTrue(&(stringDatum.Get<std::string>(i)) == &otherTestArray[i]);
			}

			stringDatum.SetStorage(finalTestArray, 3);
			Assert::AreEqual(stringDatum.Size(), 3U);
			Assert::AreEqual(stringDatum.Capacity(), 3U);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(stringDatum.Get<std::string>(i) == finalTestArray[i]);
				Assert::IsTrue(&(stringDatum.Get<std::string>(i)) == &finalTestArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { stringDatum.SetSize(12); });
			Assert::ExpectException<std::exception>([&] { stringDatum.SetStorage(dummyArray, 1); });

			Datum otherStringDatum;

			otherStringDatum.Set(finalTestArray[0], 0);
			otherStringDatum.Set(finalTestArray[1], 1);
			otherStringDatum.Set(finalTestArray[2], 2);


			Assert::IsFalse(otherStringDatum.IsExternal());

			otherStringDatum.SetStorage(testArray, 4);

			Assert::IsTrue(otherStringDatum.IsExternal());
			Assert::AreEqual(otherStringDatum.Size(), 4U);
			Assert::AreEqual(otherStringDatum.Capacity(), 4U);

			for (int i = 0; i < 4; i++)
			{
				Assert::IsTrue(otherStringDatum.Get<std::string>(i) == testArray[i]);
				Assert::IsTrue(&otherStringDatum.Get<std::string>(i) == &testArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { otherStringDatum.SetSize(5); });
			Assert::ExpectException<std::exception>([&] { otherStringDatum.SetStorage(dummyArray, 3); });
		}

		TEST_METHOD(DatumTestPointerSetStorage)
		{
			Foo fooArray[] = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9), Foo(10), Foo(11), Foo(12) };

			RTTI* testArray[] = { &fooArray[0], &fooArray[1], &fooArray[2] };
			RTTI* otherTestArray[] = { &fooArray[3], &fooArray[4], &fooArray[5], &fooArray[6] };
			RTTI* finalTestArray[] = { &fooArray[7], &fooArray[8], &fooArray[9], &fooArray[10], &fooArray[11] };

			glm::vec4 dummyArray[] = { glm::vec4(1, 2, 3, 4) };

			Datum pointerDatum;
			Assert::IsFalse(pointerDatum.IsExternal());
			Assert::AreEqual(pointerDatum.Size(), 0U);
			Assert::AreEqual(pointerDatum.Capacity(), 0U);
			Assert::IsTrue(pointerDatum.Type() == Datum::DatumType::Unknown);

			pointerDatum.SetStorage(testArray, 3);
			Assert::ExpectException<std::exception>([&] {pointerDatum.Remove(testArray[1]); });

			Assert::IsTrue(pointerDatum.IsExternal());
			Assert::AreEqual(pointerDatum.Size(), 3U);
			Assert::AreEqual(pointerDatum.Capacity(), 3U);
			Assert::IsTrue(pointerDatum.Type() == Datum::DatumType::Pointer);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(pointerDatum.Get<RTTI*>(i) == testArray[i]);
				Assert::IsTrue(&(pointerDatum.Get<RTTI*>(i)) == &testArray[i]);
			}

			pointerDatum.SetStorage(otherTestArray, 4);
			Assert::AreEqual(pointerDatum.Size(), 4U);
			Assert::AreEqual(pointerDatum.Capacity(), 4U);

			for (int i = 0; i < 4; i++)
			{
				Assert::IsTrue(pointerDatum.Get<RTTI*>(i) == otherTestArray[i]);
				Assert::IsTrue(&(pointerDatum.Get<RTTI*>(i)) == &otherTestArray[i]);
			}

			pointerDatum.SetStorage(finalTestArray, 5);
			Assert::AreEqual(pointerDatum.Size(), 5U);
			Assert::AreEqual(pointerDatum.Capacity(), 5U);

			for (int i = 0; i < 5; i++)
			{
				Assert::IsTrue(pointerDatum.Get<RTTI*>(i) == finalTestArray[i]);
				Assert::IsTrue(&(pointerDatum.Get<RTTI*>(i)) == &finalTestArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { pointerDatum.SetSize(12); });
			Assert::ExpectException<std::exception>([&] { pointerDatum.SetStorage(dummyArray, 3); });

			Datum otherPointerDatum;

			otherPointerDatum.Set(&fooArray[7], 4);
			otherPointerDatum.Set(&fooArray[8], 3);
			otherPointerDatum.Set(&fooArray[9], 2);
			otherPointerDatum.Set(&fooArray[10], 1);
			otherPointerDatum.Set(&fooArray[11], 0);

			Assert::IsFalse(otherPointerDatum.IsExternal());

			otherPointerDatum.SetStorage(testArray, 3);

			Assert::IsTrue(otherPointerDatum.IsExternal());
			Assert::AreEqual(otherPointerDatum.Size(), 3U);
			Assert::AreEqual(otherPointerDatum.Capacity(), 3U);

			for (int i = 0; i < 3; i++)
			{
				Assert::IsTrue(otherPointerDatum.Get<RTTI*>(i) == testArray[i]);
				Assert::IsTrue(&otherPointerDatum.Get<RTTI*>(i) == &testArray[i]);
			}

			Assert::ExpectException<std::exception>([&] { otherPointerDatum.SetSize(5); });
			Assert::ExpectException<std::exception>([&] { otherPointerDatum.SetStorage(dummyArray, 3); });
		}

		TEST_METHOD(DatumTestCopyConstructor)
		{
			//Integer test
			Datum otherIntDatum;
			otherIntDatum.Set(3, 2);
			otherIntDatum.Set(2, 1);
			otherIntDatum.Set(1, 0);

			Datum intDatum(otherIntDatum);
			Assert::IsTrue(intDatum == otherIntDatum);

			//Float test
			Datum otherFloatDatum;
			otherFloatDatum.Set(3.2f, 2);
			otherFloatDatum.Set(2.2f, 1);
			otherFloatDatum.Set(1.15f, 0);

			Datum floatDatum = otherFloatDatum;
			Assert::IsTrue(floatDatum == otherFloatDatum);

			//Vector test
			Datum otherVectorDatum;
			otherVectorDatum.Set(glm::vec4(1, 2, 3, 4), 2);
			otherVectorDatum.Set(glm::vec4(5, 6, 7, 8), 1);
			otherVectorDatum.Set(glm::vec4(9, 10, 11, 12), 0);

			Datum vectorDatum(otherVectorDatum);
			Assert::IsTrue(vectorDatum == otherVectorDatum);

			//Matrix test
			Datum otherMatrixDatum;
			otherMatrixDatum.Set(glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32), 2);
			otherMatrixDatum.Set(glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48), 1);
			otherMatrixDatum.Set(glm::mat4(49, 50.50, 51, 52.52, 53, 54.54, 55, 56.56, 57, 58.58, 59, 60.60, 61, 62.62, 63, 64.64), 0);

			Datum matrixDatum(otherMatrixDatum);
			Assert::IsTrue(matrixDatum == otherMatrixDatum);

			//Table test
			Datum tableDatum;

			Scope* testScope = new Scope();

			tableDatum = testScope;

			Datum& healthDatum = testScope->Append("Health");
			Datum& moneyDatum = testScope->Append("Money");
			healthDatum = 100;
			moneyDatum = 99.05f;

			Datum otherTableDatum = tableDatum;

			Assert::IsTrue(tableDatum == otherTableDatum);

			delete testScope;

			//String test
			Datum otherStringDatum;
			otherStringDatum.Set("I'm", 2);
			otherStringDatum.Set("almost", 1);
			otherStringDatum.Set("done!", 0);

			Datum stringDatum(otherStringDatum);
			Assert::IsTrue(stringDatum == otherStringDatum);

			//Pointer test
			Foo testFoo(14);
			Foo otherTestFoo(19);
			Foo finalTestFoo(23);

			Datum otherPointerDatum;
			otherPointerDatum.Set(&testFoo, 2);
			otherPointerDatum.Set(&otherTestFoo, 1);
			otherPointerDatum.Set(&finalTestFoo, 0);

			Datum pointerDatum(otherPointerDatum);
			Assert::IsTrue(pointerDatum == otherPointerDatum);
		}

		TEST_METHOD(DatumTestAssignmentOperatorIntDatum)
		{
			int testArray[] = { 1, 3, 5, 7, 9 };
			int otherTestArray[] = { 2, 4, 6, 8, 10 };

			//Internal and internal
			Datum intDatum;
			
			intDatum.Set(6, 2);
			intDatum.Set(5, 1);
			intDatum.Set(4);

			Assert::IsFalse(intDatum.IsExternal());

			Datum otherIntDatum;
			otherIntDatum.Set(3, 2);
			otherIntDatum.Set(2, 1);
			otherIntDatum.Set(1, 0);

			Assert::IsFalse(otherIntDatum.IsExternal());

			intDatum = otherIntDatum;
			Assert::IsTrue(intDatum == otherIntDatum);
			Assert::IsFalse(intDatum.IsExternal());

			//External and external
			Datum externalDatum;

			externalDatum.SetStorage(testArray, 5);
			Assert::IsTrue(externalDatum.IsExternal());

			otherIntDatum.SetStorage(otherTestArray, 5);
			Assert::IsTrue(otherIntDatum.IsExternal());

			otherIntDatum = externalDatum;
			Assert::IsTrue(otherIntDatum == externalDatum);
			Assert::IsTrue(otherIntDatum.IsExternal());

			//External and internal
			Assert::IsTrue(otherIntDatum.IsExternal());
			Assert::IsFalse(intDatum.IsExternal());

			otherIntDatum = intDatum;
			Assert::IsTrue(intDatum == otherIntDatum);
			Assert::IsFalse(otherIntDatum.IsExternal());

			//Internal and external
			Assert::IsTrue(externalDatum.IsExternal());
			Assert::IsFalse(intDatum.IsExternal());

			intDatum = externalDatum;
			Assert::IsTrue(intDatum == externalDatum);
			Assert::IsTrue(intDatum.IsExternal());
		}

		TEST_METHOD(DatumTestAssignmentOperatorFloatDatum)
		{
			float testArray[] = { 1.1f, 3.3f, 5.5f, 7.7f, 9.9f };
			float otherTestArray[] = { 2.2f, 4.4f, 6.6f, 8.8f, 10.10f };

			//Internal and internal
			Datum floatDatum;

			floatDatum.Set(4.4f);
			floatDatum.Set(5.5f, 1);
			floatDatum.Set(6.6f, 2);

			Assert::IsFalse(floatDatum.IsExternal());

			Datum otherFloatDatum;
			otherFloatDatum.Set(1.1f, 0);
			otherFloatDatum.Set(2.2f, 1);
			otherFloatDatum.Set(3.3f, 2);

			Assert::IsFalse(otherFloatDatum.IsExternal());

			floatDatum = otherFloatDatum;
			Assert::IsTrue(floatDatum == otherFloatDatum);
			Assert::IsFalse(floatDatum.IsExternal());

			//External and external
			Datum externalDatum;

			externalDatum.SetStorage(testArray, 5);
			Assert::IsTrue(externalDatum.IsExternal());

			otherFloatDatum.SetStorage(otherTestArray, 5);
			Assert::IsTrue(otherFloatDatum.IsExternal());

			otherFloatDatum = externalDatum;
			Assert::IsTrue(otherFloatDatum == externalDatum);
			Assert::IsTrue(otherFloatDatum.IsExternal());

			//External and internal
			Assert::IsTrue(otherFloatDatum.IsExternal());
			Assert::IsFalse(floatDatum.IsExternal());

			otherFloatDatum = floatDatum;
			Assert::IsTrue(floatDatum == otherFloatDatum);
			Assert::IsFalse(otherFloatDatum.IsExternal());

			//Internal and external
			Assert::IsTrue(externalDatum.IsExternal());
			Assert::IsFalse(floatDatum.IsExternal());

			floatDatum = externalDatum;
			Assert::IsTrue(floatDatum == externalDatum);
			Assert::IsTrue(floatDatum.IsExternal());
		}

		TEST_METHOD(DatumTestAssignmentOperatorVectorDatum)
		{
			glm::vec4 testArray[] = { glm::vec4(29, 30, 31, 32), glm::vec4(33, 34, 35, 36), glm::vec4(37, 38, 39, 40), glm::vec4(41, 42, 43, 44), glm::vec4(45, 46, 47, 48) };
			glm::vec4 otherTestArray[] = { glm::vec4(-29, -30, -31, -32), glm::vec4(-33, -34, -35, -36), glm::vec4(-37, -38, -39, -40), glm::vec4(-41, -42, -43, -44), glm::vec4(-45, -46, -47, -48) };

			//Internal and internal
			Datum vectorDatum;

			vectorDatum.Set(glm::vec4(29, 30, 31, 32), 2);
			vectorDatum.Set(glm::vec4(-29, -30, -31, -32), 1);
			vectorDatum.Set(glm::vec4(37, 38, 39, 40));

			Assert::IsFalse(vectorDatum.IsExternal());

			Datum otherVectorDatum;
			otherVectorDatum.Set(glm::vec4(45, 46, 47, 48), 2);
			otherVectorDatum.Set(glm::vec4(41, 42, 43, 44), 1);
			otherVectorDatum.Set(glm::vec4(-41, -42, -43, -44), 0);

			Assert::IsFalse(otherVectorDatum.IsExternal());

			vectorDatum = otherVectorDatum;
			Assert::IsTrue(vectorDatum == otherVectorDatum);
			Assert::IsFalse(vectorDatum.IsExternal());

			//External and external
			Datum externalDatum;

			externalDatum.SetStorage(testArray, 5);
			Assert::IsTrue(externalDatum.IsExternal());

			otherVectorDatum.SetStorage(otherTestArray, 5);
			Assert::IsTrue(otherVectorDatum.IsExternal());

			otherVectorDatum = externalDatum;
			Assert::IsTrue(otherVectorDatum == externalDatum);
			Assert::IsTrue(otherVectorDatum.IsExternal());

			//External and internal
			Assert::IsTrue(otherVectorDatum.IsExternal());
			Assert::IsFalse(vectorDatum.IsExternal());

			otherVectorDatum = vectorDatum;
			Assert::IsTrue(vectorDatum == otherVectorDatum);
			Assert::IsFalse(otherVectorDatum.IsExternal());

			//Internal and external
			Assert::IsTrue(externalDatum.IsExternal());
			Assert::IsFalse(vectorDatum.IsExternal());

			vectorDatum = externalDatum;
			Assert::IsTrue(vectorDatum == externalDatum);
			Assert::IsTrue(vectorDatum.IsExternal());
		}

		TEST_METHOD(DatumTestAssignmentOperatorMatrixDatum)
		{
			glm::mat4 testArray[] = { glm::mat4(29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44), glm::mat4(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48), glm::mat4(37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52), glm::mat4(41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56), glm::mat4(45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60) };
			glm::mat4 otherTestArray[] = { glm::mat4(-29, -30, -31, -32, -33, -34, -35, -36, -37, -38, -39, -40, -41, -42, -43, -44), glm::mat4(-33, -34, -35, -36, -37, -38, -39, -40, -41, -42, -43, -44, -45, -46, -47, -48), glm::mat4(-37, -38, -39, -40, -41, -42, -43, -44, -45, -46, -47, -48, -49, -50, -51, -52), glm::mat4(-41, -42, -43, -44, -45, -46, -47, -48, -49, -50, -51, -52, -53, -54, -55, -56), glm::mat4(-45, -46, -47, -48, -49, -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60) };

			//Internal and internal
			Datum matrixDatum;

			matrixDatum.Set(glm::mat4(29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44), 2);
			matrixDatum.Set(glm::mat4(-29, -30, -31, -32, -33, -34, -35, -36, -37, -38, -39, -40, -41, -42, -43, -44), 1);
			matrixDatum.Set(glm::mat4(33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48));

			Assert::IsFalse(matrixDatum.IsExternal());

			Datum otherMatrixDatum;
			otherMatrixDatum.Set(glm::mat4(-33, -34, -35, -36, -37, -38, -39, -40, -41, -42, -43, -44, -45, -46, -47, -48), 2);
			otherMatrixDatum.Set(glm::mat4(45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60), 1);
			otherMatrixDatum.Set(glm::mat4(-45, -46, -47, -48, -49, -50, -51, -52, -53, -54, -55, -56, -57, -58, -59, -60), 0);

			Assert::IsFalse(otherMatrixDatum.IsExternal());

			matrixDatum = otherMatrixDatum;
			Assert::IsTrue(matrixDatum == otherMatrixDatum);
			Assert::IsFalse(matrixDatum.IsExternal());

			//External and external
			Datum externalDatum;

			externalDatum.SetStorage(testArray, 5);
			Assert::IsTrue(externalDatum.IsExternal());

			otherMatrixDatum.SetStorage(otherTestArray, 5);
			Assert::IsTrue(otherMatrixDatum.IsExternal());

			otherMatrixDatum = externalDatum;
			Assert::IsTrue(otherMatrixDatum == externalDatum);
			Assert::IsTrue(otherMatrixDatum.IsExternal());

			//External and internal
			Assert::IsTrue(otherMatrixDatum.IsExternal());
			Assert::IsFalse(matrixDatum.IsExternal());

			otherMatrixDatum = matrixDatum;
			Assert::IsTrue(matrixDatum == otherMatrixDatum);
			Assert::IsFalse(otherMatrixDatum.IsExternal());

			//Internal and external
			Assert::IsTrue(externalDatum.IsExternal());
			Assert::IsFalse(matrixDatum.IsExternal());

			matrixDatum = externalDatum;
			Assert::IsTrue(matrixDatum == externalDatum);
			Assert::IsTrue(matrixDatum.IsExternal());
		}

		TEST_METHOD(DatumTestAssignmentOperatorStringDatum)
		{
			std::string testArray[] = { "When", "will", "the", "madness", "end?!?" };
			std::string otherTestArray[] = { "Please", "let", "it", "be", "soon!!!" };

			//Internal and internal
			Datum stringDatum;

			stringDatum.Set("When", 2);
			stringDatum.Set("will", 1);
			stringDatum.Set("the");

			Assert::IsFalse(stringDatum.IsExternal());

			Datum otherStringDatum;
			otherStringDatum.Set("madness", 2);
			otherStringDatum.Set("end?!?", 1);
			otherStringDatum.Set("Soon!!!", 0);

			Assert::IsFalse(otherStringDatum.IsExternal());

			stringDatum = otherStringDatum;
			Assert::IsTrue(stringDatum == otherStringDatum);
			Assert::IsFalse(stringDatum.IsExternal());

			//External and external
			Datum externalDatum;

			externalDatum.SetStorage(testArray, 5);
			Assert::IsTrue(externalDatum.IsExternal());

			otherStringDatum.SetStorage(otherTestArray, 5);
			Assert::IsTrue(otherStringDatum.IsExternal());

			otherStringDatum = externalDatum;
			Assert::IsTrue(otherStringDatum == externalDatum);
			Assert::IsTrue(otherStringDatum.IsExternal());

			//External and internal
			Assert::IsTrue(otherStringDatum.IsExternal());
			Assert::IsFalse(stringDatum.IsExternal());

			otherStringDatum = stringDatum;
			Assert::IsTrue(stringDatum == otherStringDatum);
			Assert::IsFalse(otherStringDatum.IsExternal());

			//Internal and external
			Assert::IsTrue(externalDatum.IsExternal());
			Assert::IsFalse(stringDatum.IsExternal());

			stringDatum = externalDatum;
			Assert::IsTrue(stringDatum == externalDatum);
			Assert::IsTrue(stringDatum.IsExternal());
		}

		TEST_METHOD(DatumTestAssignmentOperatorPointerDatum)
		{
			Foo fooArray[] = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
			Foo otherFooArray[] = { Foo(6), Foo(7), Foo(8), Foo(9), Foo(10) };

			RTTI* testArray[] = { &fooArray[0], &fooArray[1], &fooArray[2], &fooArray[3], &fooArray[4] };
			RTTI* otherTestArray[] = { &otherFooArray[0], &otherFooArray[1], &otherFooArray[2], &otherFooArray[3], &otherFooArray[4] };

			//Internal and internal
			Datum pointerDatum;

			pointerDatum.Set(&fooArray[0], 2);
			pointerDatum.Set(&fooArray[1], 1);
			pointerDatum.Set(&fooArray[2]);

			Assert::IsFalse(pointerDatum.IsExternal());

			Datum otherPointerDatum;
			otherPointerDatum.Set(&otherFooArray[0], 2);
			otherPointerDatum.Set(&otherFooArray[1], 1);
			otherPointerDatum.Set(&otherFooArray[2], 0);

			Assert::IsFalse(otherPointerDatum.IsExternal());

			pointerDatum = otherPointerDatum;
			Assert::IsTrue(pointerDatum == otherPointerDatum);
			Assert::IsFalse(pointerDatum.IsExternal());

			//External and external
			Datum externalDatum;

			externalDatum.SetStorage(testArray, 5);
			Assert::IsTrue(externalDatum.IsExternal());

			otherPointerDatum.SetStorage(otherTestArray, 5);
			Assert::IsTrue(otherPointerDatum.IsExternal());

			otherPointerDatum = externalDatum;
			Assert::IsTrue(otherPointerDatum == externalDatum);
			Assert::IsTrue(otherPointerDatum.IsExternal());

			//External and internal
			Assert::IsTrue(otherPointerDatum.IsExternal());
			Assert::IsFalse(pointerDatum.IsExternal());

			otherPointerDatum = pointerDatum;
			Assert::IsTrue(pointerDatum == otherPointerDatum);
			Assert::IsFalse(otherPointerDatum.IsExternal());

			//Internal and external
			Assert::IsTrue(externalDatum.IsExternal());
			Assert::IsFalse(pointerDatum.IsExternal());

			pointerDatum = externalDatum;
			Assert::IsTrue(pointerDatum == externalDatum);
			Assert::IsTrue(pointerDatum.IsExternal());
		}

		TEST_METHOD(DatumTestIntMoveSemantics)
		{
			Datum intDatum;
			intDatum.Set(1, 0);
			intDatum.Set(2, 1);
			intDatum.Set(3, 2);

			Datum otherIntDatum(std::move(intDatum));
			Assert::IsTrue(otherIntDatum.Size() == 3U);

			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual((i + 1), otherIntDatum.Get<int>(i));
			}

			otherIntDatum.Set(5, 1);
			Assert::AreEqual(otherIntDatum.Get<int>(1), 5);

			Datum yetAnotherIntDatum;
			yetAnotherIntDatum = std::move(otherIntDatum);

			Assert::IsTrue(yetAnotherIntDatum.Size() == 3U);

			yetAnotherIntDatum.Set(2, 1);

			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual((i + 1), yetAnotherIntDatum.Get<int>(i));
			}
		}

		TEST_METHOD(DatumTestFloatMoveSemantics)
		{
			float testArray[] = { 4.4f, 5.5f, 6.6f };

			Datum floatDatum;
			
			floatDatum.Set(1.1f, 0);
			floatDatum.Set(2.2f, 1);
			floatDatum.Set(3.3f, 2);

			Datum otherFloatDatum(std::move(floatDatum));
			Assert::IsTrue(otherFloatDatum.Size() == 3U);
			Assert::IsTrue(otherFloatDatum.Capacity() == 3U);
			Assert::IsFalse(otherFloatDatum.IsExternal());

			Assert::IsTrue(1.1f == otherFloatDatum.Get<float>());
			Assert::IsTrue(2.2f == otherFloatDatum.Get<float>(1));
			Assert::IsTrue(3.3f == otherFloatDatum.Get<float>(2));

			otherFloatDatum.SetStorage(testArray, 3);

			Datum yetAnotherFloatDatum;
			yetAnotherFloatDatum = std::move(otherFloatDatum);

			Assert::IsTrue(yetAnotherFloatDatum.Size() == 3U);
			Assert::IsTrue(yetAnotherFloatDatum.Capacity() == 3U);
			Assert::IsTrue(yetAnotherFloatDatum.IsExternal());

			Assert::IsTrue(4.4f == yetAnotherFloatDatum.Get<float>());
			Assert::IsTrue(5.5f == yetAnotherFloatDatum.Get<float>(1));
			Assert::IsTrue(6.6f == yetAnotherFloatDatum.Get<float>(2));
		}

		TEST_METHOD(DatumTestVectortMoveSemantics)
		{
			glm::vec4 testVec1 = glm::vec4(1, 2.2, 3, 4.4);
			glm::vec4 testVec2 = glm::vec4(5, 6.6, 7, 8.8);
			glm::vec4 testVec3 = glm::vec4(9, 10.10, 11, 12.12);

			Datum vectorDatum;
			vectorDatum.Set(testVec1, 0);
			vectorDatum.Set(testVec2, 1);
			vectorDatum.Set(testVec3, 2);

			Datum otherVectorDatum(std::move(vectorDatum));
			Assert::IsTrue(otherVectorDatum.Size() == 3U);

			Assert::IsTrue(testVec1 == otherVectorDatum.Get<glm::vec4>());
			Assert::IsTrue(testVec2 == otherVectorDatum.Get<glm::vec4>(1));
			Assert::IsTrue(testVec3 == otherVectorDatum.Get<glm::vec4>(2));

			Datum yetAnotherVectorDatum;
			yetAnotherVectorDatum = std::move(otherVectorDatum);

			Assert::IsTrue(yetAnotherVectorDatum.Size() == 3U);

			Assert::IsTrue(testVec1 == yetAnotherVectorDatum.Get<glm::vec4>());
			Assert::IsTrue(testVec2 == yetAnotherVectorDatum.Get<glm::vec4>(1));
			Assert::IsTrue(testVec3 == yetAnotherVectorDatum.Get<glm::vec4>(2));
		}

		TEST_METHOD(DatumTestMatrixMoveSemantics)
		{
			glm::mat4 testMat1 = glm::mat4(1, 2.2, 3, 4.4, 5, 6.6, 7, 8.8, 9, 10.10, 11, 12.12, 13, 14.14, 15, 16.16);
			glm::mat4 testMat2 = glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32);
			glm::mat4 testMat3 = glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48);

			Datum matrixDatum;
			matrixDatum.Set(testMat1, 0);
			matrixDatum.Set(testMat2, 1);
			matrixDatum.Set(testMat3, 2);

			Datum otherMatrixDatum(std::move(matrixDatum));
			Assert::IsTrue(otherMatrixDatum.Size() == 3U);
			
			Assert::IsTrue(testMat1 == otherMatrixDatum.Get<glm::mat4>());
			Assert::IsTrue(testMat2 == otherMatrixDatum.Get<glm::mat4>(1));
			Assert::IsTrue(testMat3 == otherMatrixDatum.Get<glm::mat4>(2));

			Datum yetAnotherMatrixDatum;
			yetAnotherMatrixDatum = std::move(otherMatrixDatum);

			Assert::IsTrue(yetAnotherMatrixDatum.Size() == 3U);

			Assert::IsTrue(testMat1 == yetAnotherMatrixDatum.Get<glm::mat4>());
			Assert::IsTrue(testMat2 == yetAnotherMatrixDatum.Get<glm::mat4>(1));
			Assert::IsTrue(testMat3 == yetAnotherMatrixDatum.Get<glm::mat4>(2));
		}

		TEST_METHOD(DatumTestStringMoveSemantics)
		{
			Datum stringDatum;
			stringDatum.Set("Oy!", 0);
			stringDatum.Set("Hola", 1);
			stringDatum.Set("Yo", 2);

			Datum otherStringDatum(std::move(stringDatum));
			Assert::IsTrue(otherStringDatum.Size() == 3U);

			Assert::IsTrue("Oy!" == otherStringDatum.Get<std::string>());
			Assert::IsTrue("Hola" == otherStringDatum.Get<std::string>(1));
			Assert::IsTrue("Yo" == otherStringDatum.Get<std::string>(2));

			Datum yetAnotherStringDatum;
			yetAnotherStringDatum = std::move(otherStringDatum);

			Assert::IsTrue(yetAnotherStringDatum.Size() == 3U);

			Assert::IsTrue("Oy!" == yetAnotherStringDatum.Get<std::string>());
			Assert::IsTrue("Hola" == yetAnotherStringDatum.Get<std::string>(1));
			Assert::IsTrue("Yo" == yetAnotherStringDatum.Get<std::string>(2));
		}

		TEST_METHOD(DatumTestPointerMoveSemantics)
		{
			Foo testFoo(1);
			Foo otherTestFoo(2);
			Foo finalTestFoo(3);

			Datum pointerDatum;
			pointerDatum.Set(&testFoo, 0);
			pointerDatum.Set(&otherTestFoo, 1);
			pointerDatum.Set(&finalTestFoo, 2);

			Datum otherPointerDatum(std::move(pointerDatum));
			Assert::IsTrue(otherPointerDatum.Size() == 3U);

			Assert::IsTrue(&testFoo == otherPointerDatum.Get<RTTI*>());
			Assert::IsTrue(&otherTestFoo == otherPointerDatum.Get<RTTI*>(1));
			Assert::IsTrue(&finalTestFoo == otherPointerDatum.Get<RTTI*>(2));

			Datum yetAnotherPointerDatum;
			yetAnotherPointerDatum = std::move(otherPointerDatum);

			Assert::IsTrue(yetAnotherPointerDatum.Size() == 3U);

			Assert::IsTrue(&testFoo == yetAnotherPointerDatum.Get<RTTI*>());
			Assert::IsTrue(&otherTestFoo == yetAnotherPointerDatum.Get<RTTI*>(1));
			Assert::IsTrue(&finalTestFoo == yetAnotherPointerDatum.Get<RTTI*>(2));
		}

		TEST_METHOD(DatumTestIntDatumEquality)
		{
			Datum intDatum, otherIntDatum;

			Assert::IsTrue(intDatum == otherIntDatum);

			intDatum.SetType(Datum::DatumType::Integer);
			Assert::IsTrue(intDatum != otherIntDatum);

			intDatum.SetSize(3);

			otherIntDatum.SetType(Datum::DatumType::Integer);
			otherIntDatum.SetSize(2);
			Assert::IsTrue(intDatum != otherIntDatum);

			otherIntDatum.Set(14, 2);
			otherIntDatum.Set(13, 1);
			otherIntDatum.Set(12);

			intDatum.Set(14, 2);
			intDatum.Set(31, 1);
			intDatum.Set(12);

			Assert::IsTrue(intDatum != otherIntDatum);

			intDatum.Set(13, 1);
			Assert::IsTrue(intDatum == otherIntDatum);
		}

		TEST_METHOD(DatumTestFloatDatumEquality)
		{
			Datum floatDatum, otherFloatDatum;

			Assert::IsTrue(floatDatum == otherFloatDatum);

			floatDatum.SetType(Datum::DatumType::Float);
			Assert::IsTrue(floatDatum != otherFloatDatum);

			floatDatum.SetSize(3);

			otherFloatDatum.SetType(Datum::DatumType::Float);
			otherFloatDatum.SetSize(2);
			Assert::IsTrue(floatDatum != otherFloatDatum);

			otherFloatDatum.Set(14.4f, 2);
			otherFloatDatum.Set(13.3f, 1);
			otherFloatDatum.Set(12.2f);

			floatDatum.Set(41.1f, 2);
			floatDatum.Set(13.3f, 1);
			floatDatum.Set(12.2f);

			Assert::IsTrue(floatDatum != otherFloatDatum);

			floatDatum.Set(14.4f, 2);
			Assert::IsTrue(floatDatum == otherFloatDatum);
		}

		TEST_METHOD(DatumTestVectorDatumEquality)
		{
			Datum vectorDatum, otherVectorDatum;

			Assert::IsTrue(vectorDatum == otherVectorDatum);

			vectorDatum.SetType(Datum::DatumType::Vector);
			Assert::IsTrue(vectorDatum != otherVectorDatum);

			vectorDatum.SetSize(3);

			otherVectorDatum.SetType(Datum::DatumType::Vector);
			otherVectorDatum.SetSize(2);
			Assert::IsTrue(vectorDatum != otherVectorDatum);

			otherVectorDatum.Set(glm::vec4(1, 2, 3, 4), 2);
			otherVectorDatum.Set(glm::vec4(5, 6, 7, 8), 1);
			otherVectorDatum.Set(glm::vec4(9, 10, 11, 12));

			vectorDatum.Set(glm::vec4(1, 2, 3, 4), 2);
			vectorDatum.Set(glm::vec4(5, 6, 7, 8), 1);
			vectorDatum.Set(glm::vec4(13, 14, 15, 16));

			Assert::IsTrue(vectorDatum != otherVectorDatum);

			vectorDatum.Set(glm::vec4(9, 10, 11, 12));
			Assert::IsTrue(vectorDatum == otherVectorDatum);
		}

		TEST_METHOD(DatumTestMatrixDatumEquality)
		{
			glm::mat4 testMat1 = glm::mat4(1, 2.2, 3, 4.4, 5, 6.6, 7, 8.8, 9, 10.10, 11, 12.12, 13, 14.14, 15, 16.16);
			glm::mat4 testMat2 = glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32);
			glm::mat4 testMat3 = glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48);
			glm::mat4 testMat4 = glm::mat4(49, 50.50, 51, 52.52, 53, 54.54, 55, 56.56, 57, 58.58, 59, 60.60, 61, 62.62, 63, 64.64);

			Datum matrixDatum, otherMatrixDatum;

			Assert::IsTrue(matrixDatum == otherMatrixDatum);

			matrixDatum.SetType(Datum::DatumType::Matrix);
			Assert::IsTrue(matrixDatum != otherMatrixDatum);

			matrixDatum.SetSize(3);

			otherMatrixDatum.SetType(Datum::DatumType::Matrix);
			otherMatrixDatum.SetSize(2);
			Assert::IsTrue(matrixDatum != otherMatrixDatum);

			otherMatrixDatum.Set(testMat1, 2);
			otherMatrixDatum.Set(testMat2, 1);
			otherMatrixDatum.Set(testMat3);

			matrixDatum.Set(testMat4, 2);
			matrixDatum.Set(testMat2, 1);
			matrixDatum.Set(testMat3);

			Assert::IsTrue(matrixDatum != otherMatrixDatum);

			matrixDatum.Set(testMat1, 2);
			Assert::IsTrue(matrixDatum == otherMatrixDatum);
		}

		TEST_METHOD(DatumTestTableEquality)
		{
			Datum tableDatum;
			Datum otherTableDatum;

			Assert::IsTrue(tableDatum == otherTableDatum);

			Scope testScope;
			Scope otherTestScope;

			tableDatum = testScope;
			Assert::IsTrue(tableDatum == testScope);

			otherTableDatum = otherTestScope;
			Assert::IsTrue(tableDatum == otherTestScope);

			Assert::IsTrue(tableDatum == otherTableDatum);

			Datum& intDatum = testScope.Append("Health");
			Datum& floatDatum = testScope.Append("Money");

			Datum& otherIntDatum = otherTestScope.Append("Health");

			Assert::IsTrue(tableDatum != otherTableDatum);
			Assert::IsTrue(tableDatum != otherTestScope);

			Datum& otherFloatDatum = otherTestScope.Append("Money");

			intDatum = 100;
			otherIntDatum = 95;

			floatDatum = 1.0f;
			otherFloatDatum = 1.0f;

			Assert::IsTrue(tableDatum != otherTableDatum);
			Assert::IsTrue(tableDatum != otherTestScope);

			otherIntDatum = 100;

			Assert::IsTrue(tableDatum == otherTableDatum);
			Assert::IsTrue(tableDatum == otherTestScope);
		}

		TEST_METHOD(DatumTestStringDatumEquality)
		{
			Datum stringDatum, otherStringDatum;

			Assert::IsTrue(stringDatum == otherStringDatum);

			stringDatum.SetType(Datum::DatumType::String);
			Assert::IsTrue(stringDatum != otherStringDatum);

			stringDatum.SetSize(3);

			otherStringDatum.SetType(Datum::DatumType::String);
			otherStringDatum.SetSize(2);
			Assert::IsTrue(stringDatum != otherStringDatum);

			otherStringDatum.Set("Finally!", 2);
			otherStringDatum.Set("My", 1);
			otherStringDatum.Set("trouble");

			stringDatum.Set("Finally!", 2);
			stringDatum.Set("MY", 1);
			stringDatum.Set("trouble");

			Assert::IsTrue(stringDatum != otherStringDatum);

			stringDatum.Set("My", 1);
			Assert::IsTrue(stringDatum == otherStringDatum);
		}

		TEST_METHOD(DatumTestPointerDatumEquality)
		{
			Foo fooArray[] = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6), Foo(7), Foo(8), Foo(9), Foo(10), Foo(11), Foo(12) };

			RTTI* testArray[] = { &fooArray[0], &fooArray[1], &fooArray[2] };
			RTTI* otherTestArray[] = { &fooArray[3], &fooArray[4], &fooArray[5], &fooArray[6] };

			Datum pointerDatum, otherPointerDatum;

			Assert::IsTrue(pointerDatum == otherPointerDatum);

			pointerDatum.SetType(Datum::DatumType::Pointer);
			Assert::IsTrue(pointerDatum != otherPointerDatum);

			pointerDatum.SetSize(3);

			otherPointerDatum.SetType(Datum::DatumType::Pointer);
			otherPointerDatum.SetSize(2);
			Assert::IsTrue(pointerDatum != otherPointerDatum);

			otherPointerDatum.SetStorage(testArray, 3);

			pointerDatum.Set(testArray[2], 2);
			pointerDatum.Set(otherTestArray[1], 1);
			pointerDatum.Set(testArray[0]);

			Assert::IsTrue(pointerDatum != otherPointerDatum);

			pointerDatum.Set(testArray[1], 1);
			Assert::IsTrue(pointerDatum == otherPointerDatum);
		}

		TEST_METHOD(DatumTestIntEquality)
		{
			Datum intDatum;

			intDatum.SetType(Datum::DatumType::Integer);

			intDatum.SetSize(4);
			intDatum.Set(2);
			intDatum.Set(4, 1);
			intDatum.Set(6, 2);
			intDatum.Set(8, 3);

			Assert::IsTrue(intDatum != 3);
			Assert::IsTrue(intDatum == 2);

			Datum otherIntDatum;
			otherIntDatum.Set(2);
			otherIntDatum.Set(6, 2);

			Assert::IsTrue(intDatum == otherIntDatum.Get<int>());
			Assert::IsTrue(intDatum != otherIntDatum.Get<int>(1));
		}

		TEST_METHOD(DatumTestFloatEquality)
		{
			Datum floatDatum;
			Datum otherFloatDatum;

			floatDatum.SetType(Datum::DatumType::Float);

			floatDatum.SetSize(4);

			floatDatum.Set(2.34f);
			floatDatum.Set(4.35f, 1);
			floatDatum.Set(6.99f, 2);
			floatDatum.Set(8.12f, 3);

			Assert::IsTrue(floatDatum != 3.42f);
			Assert::IsTrue(floatDatum == 2.34f);

			otherFloatDatum.Set(2.34f);
			otherFloatDatum.Set(6.99f, 2);

			Assert::IsTrue(floatDatum == otherFloatDatum.Get<float>());
			Assert::IsTrue(floatDatum != otherFloatDatum.Get<float>(1));
		}

		TEST_METHOD(DatumTestVectorEquality)
		{
			glm::vec4 testVec1 = glm::vec4(1, 2.2, 3, 4.4);
			glm::vec4 testVec2 = glm::vec4(5, 6.6, 7, 8.8);
			glm::vec4 testVec3 = glm::vec4(9, 10.10, 11, 12.12);
			glm::vec4 testVec4 = glm::vec4(13, 14.14, 15, 16.16);

			Datum vectorDatum;

			vectorDatum.Set(testVec1);
			vectorDatum.Set(testVec2, 1);
			vectorDatum.Set(testVec3, 2);
			vectorDatum.Set(testVec4, 3);

			Assert::IsTrue(vectorDatum != testVec2);
			Assert::IsTrue(vectorDatum == testVec1);

			Datum otherVectorDatum;
			vectorDatum.Set(testVec1);
			vectorDatum.Set(testVec3, 2);

			Assert::IsTrue(vectorDatum == vectorDatum.Get<glm::vec4>());
			Assert::IsTrue(vectorDatum != vectorDatum.Get<glm::vec4>(1));
		}

		TEST_METHOD(DatumTestMatrixEquality)
		{
			glm::mat4 testMat1 = glm::mat4(1, 2.2, 3, 4.4, 5, 6.6, 7, 8.8, 9, 10.10, 11, 12.12, 13, 14.14, 15, 16.16);
			glm::mat4 testMat2 = glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32);
			glm::mat4 testMat3 = glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48);
			glm::mat4 testMat4 = glm::mat4(49, 50.50, 51, 52.52, 53, 54.54, 55, 56.56, 57, 58.58, 59, 60.60, 61, 62.62, 63, 64.64);

			Datum matrixDatum;

			matrixDatum.Set(testMat1);
			matrixDatum.Set(testMat2, 1);
			matrixDatum.Set(testMat3, 2);
			matrixDatum.Set(testMat4, 3);

			Assert::IsTrue(matrixDatum != testMat4);
			Assert::IsTrue(matrixDatum == testMat1);

			Datum otherMatrixDatum;
			otherMatrixDatum.Set(testMat1);
			otherMatrixDatum.Set(testMat4, 2);

			Assert::IsTrue(matrixDatum == otherMatrixDatum.Get<glm::mat4>());
			Assert::IsTrue(matrixDatum != otherMatrixDatum.Get<glm::mat4>(1));
		}

		TEST_METHOD(DatumTestStringEquality)
		{
			Datum stringDatum;

			stringDatum.SetType(Datum::DatumType::String);

			stringDatum.SetSize(4);

			stringDatum.Set("Is");
			stringDatum.Set("it", 1);
			stringDatum.Set("over", 2);
			stringDatum.Set("yet?", 3);

			Assert::IsTrue(stringDatum != "over");
			Assert::IsTrue(stringDatum == "Is");

			Datum otherStringDatum;
			otherStringDatum.Set("Is");
			otherStringDatum.Set("over", 2);

			Assert::IsTrue(stringDatum == otherStringDatum.Get<std::string>());
			Assert::IsTrue(stringDatum != otherStringDatum.Get<std::string>(1));
		}

		TEST_METHOD(DatumTestPointerEquality)
		{
			Datum pointerDatum;

			pointerDatum.SetType(Datum::DatumType::Pointer);

			pointerDatum.SetSize(4);

			Foo testFoo(1);
			Foo otherTestFoo(2);
			Foo yetAnotherTestFoo(3);
			Foo finalTestFoo(4);

			pointerDatum.Set(&testFoo);
			pointerDatum.Set(&otherTestFoo, 1);
			pointerDatum.Set(&yetAnotherTestFoo, 2);
			pointerDatum.Set(&finalTestFoo, 3);

			Assert::IsTrue(pointerDatum != &finalTestFoo);
			Assert::IsTrue(pointerDatum == &testFoo);

			Datum otherPointerDatum;

			otherPointerDatum.Set(&testFoo);
			otherPointerDatum.Set(&yetAnotherTestFoo, 2);
		}

		TEST_METHOD(DatumTestIntToString)
		{
			Datum intDatum;

			intDatum.Set(4, 0);
			intDatum.Set(3, 1);
			intDatum.Set(2, 2);
			intDatum.Set(1, 3);

			Assert::IsTrue("4" == intDatum.ToString());
			Assert::IsTrue("3" == intDatum.ToString(1));
			Assert::IsTrue("2" == intDatum.ToString(2));
			Assert::IsTrue("1" == intDatum.ToString(3));

			intDatum.Set(7, 2);
			Assert::IsTrue("7" == intDatum.ToString(2));
		}

		TEST_METHOD(DatumTestFloatToString)
		{
			Datum floatDatum;

			float testFloat1 = 1.14f;
			float testFloat2 = 2.730f;
			float testFloat3 = 3.14159f;
			float testFloat4 = 4.42f;

			floatDatum.Set(testFloat1);
			floatDatum.Set(testFloat2, 1);
			floatDatum.Set(testFloat3, 2);
			floatDatum.Set(testFloat4, 3);

			Assert::IsTrue(std::to_string(testFloat1) == floatDatum.ToString());
			Assert::IsTrue(std::to_string(testFloat2) == floatDatum.ToString(1));
			Assert::IsTrue(std::to_string(testFloat3) == floatDatum.ToString(2));
			Assert::IsTrue(std::to_string(testFloat4) == floatDatum.ToString(3));

			float testFloat5 = 7.0f;

			floatDatum.Set(testFloat5, 1);
			Assert::IsTrue(std::to_string(testFloat5) == floatDatum.ToString(1));
		}

		TEST_METHOD(DatumTestVectorToString)
		{
			glm::vec4 testVec1 = glm::vec4(1, 2.2, 3, 4.4);
			glm::vec4 testVec2 = glm::vec4(5, 6.6, 7, 8.8);
			glm::vec4 testVec3 = glm::vec4(9, 10.10, 11, 12.12);
			glm::vec4 testVec4 = glm::vec4(13, 14.14, 15, 16.16);

			Datum vectorDatum;

			vectorDatum.Set(testVec1);
			vectorDatum.Set(testVec2, 1);
			vectorDatum.Set(testVec3, 2);
			vectorDatum.Set(testVec4, 3);

			Assert::IsTrue(glm::to_string(testVec1) == vectorDatum.ToString());
			Assert::IsTrue(glm::to_string(testVec2) == vectorDatum.ToString(1));
			Assert::IsTrue(glm::to_string(testVec3) == vectorDatum.ToString(2));
			Assert::IsTrue(glm::to_string(testVec4) == vectorDatum.ToString(3));
		}

		TEST_METHOD(DatumTestMatrixToString)
		{
			glm::mat4 testMat1 = glm::mat4(1, 2.2, 3, 4.4, 5, 6.6, 7, 8.8, 9, 10.10, 11, 12.12, 13, 14.14, 15, 16.16);
			glm::mat4 testMat2 = glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32);
			glm::mat4 testMat3 = glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48);
			glm::mat4 testMat4 = glm::mat4(49, 50.50, 51, 52.52, 53, 54.54, 55, 56.56, 57, 58.58, 59, 60.60, 61, 62.62, 63, 64.64);

			Datum matrixDatum;

			matrixDatum.Set(testMat1);
			matrixDatum.Set(testMat2, 1);
			matrixDatum.Set(testMat3, 2);
			matrixDatum.Set(testMat4, 3);

			Assert::IsTrue(glm::to_string(testMat1) == matrixDatum.ToString());
			Assert::IsTrue(glm::to_string(testMat2) == matrixDatum.ToString(1));
			Assert::IsTrue(glm::to_string(testMat3) == matrixDatum.ToString(2));
			Assert::IsTrue(glm::to_string(testMat4) == matrixDatum.ToString(3));
		}

		TEST_METHOD(DatumTestStringToString)
		{
			Datum stringDatum;

			stringDatum.SetType(Datum::DatumType::String);

			stringDatum.SetSize(4);

			stringDatum.Set("Well");
			stringDatum.Set("well", 1);
			stringDatum.Set("WELL", 2);
			stringDatum.Set("WeLl", 3);

			Assert::IsTrue("Well" == stringDatum.ToString());
			Assert::IsTrue("well" == stringDatum.ToString(1));
			Assert::IsTrue("WELL" == stringDatum.ToString(2));
			Assert::IsTrue("WeLl" == stringDatum.ToString(3));
		}

		TEST_METHOD(DatumTestPointerToString)
		{
			Datum pointerDatum;

			pointerDatum.SetType(Datum::DatumType::Pointer);

			pointerDatum.SetSize(4);

			Foo testFoo(1);
			Foo otherTestFoo(2);
			Foo yetAnotherTestFoo(3);
			Foo finalTestFoo(4);

			pointerDatum.Set(&testFoo);
			pointerDatum.Set(&otherTestFoo, 1);
			pointerDatum.Set(&yetAnotherTestFoo, 2);
			pointerDatum.Set(&finalTestFoo, 3);

			Assert::IsTrue(testFoo.ToString() == pointerDatum.ToString());
			Assert::IsTrue(otherTestFoo.ToString() == pointerDatum.ToString(1));
			Assert::IsTrue(yetAnotherTestFoo.ToString() == pointerDatum.ToString(2));
			Assert::IsTrue(finalTestFoo.ToString() == pointerDatum.ToString(3));
		}

		TEST_METHOD(DatumTestIntSetFromString)
		{
			Datum intDatum;

			intDatum.SetFromString("11", Datum::DatumType::Integer);
			intDatum.SetFromString("12", Datum::DatumType::Integer, 1);
			intDatum.SetFromString("13", Datum::DatumType::Integer, 2);
			intDatum.SetFromString("14", Datum::DatumType::Integer, 3);

			Assert::AreEqual(11, intDatum.Get<int>());
			Assert::AreEqual(12, intDatum.Get<int>(1));
			Assert::AreEqual(13, intDatum.Get<int>(2));
			Assert::AreEqual(14, intDatum.Get<int>(3));
		}

		TEST_METHOD(DatumTestFloatSetFromString)
		{
			Datum floatDatum;

			floatDatum.SetFromString("1.11", Datum::DatumType::Float);
			floatDatum.SetFromString("2.22", Datum::DatumType::Float, 1);
			floatDatum.SetFromString("3.33", Datum::DatumType::Float, 2);
			floatDatum.SetFromString("4.44", Datum::DatumType::Float, 3);

			Assert::AreEqual(1.11f, floatDatum.Get<float>());
			Assert::AreEqual(2.22f, floatDatum.Get<float>(1));
			Assert::AreEqual(3.33f, floatDatum.Get<float>(2));
			Assert::AreEqual(4.44f, floatDatum.Get<float>(3));
		}

		TEST_METHOD(DatumTestVectorSetFromString)
		{
			glm::vec4 testVec1 = glm::vec4(1, 2.2, 3, 4.4);
			glm::vec4 testVec2 = glm::vec4(5, 6.6, 7, 8.8);
			glm::vec4 testVec3 = glm::vec4(9, 10.10, 11, 12.12);
			glm::vec4 testVec4 = glm::vec4(13, 14.14, 15, 16.16);

			Datum vectorDatum;

			vectorDatum.SetFromString("1 2.2 3 4.4", Datum::DatumType::Vector);
			vectorDatum.SetFromString("5 6.6 7 8.8", Datum::DatumType::Vector, 1);
			vectorDatum.SetFromString("9 10.10 11 12.12", Datum::DatumType::Vector, 2);
			vectorDatum.SetFromString("13 14.14 15 16.16", Datum::DatumType::Vector, 3);

			Assert::IsTrue(testVec1 == vectorDatum.Get<glm::vec4>());
			Assert::IsTrue(testVec2 == vectorDatum.Get<glm::vec4>(1));
			Assert::IsTrue(testVec3 == vectorDatum.Get<glm::vec4>(2));
			Assert::IsTrue(testVec4 == vectorDatum.Get<glm::vec4>(3));
		}

		TEST_METHOD(DatumTestMatrixSetFromString)
		{
			glm::mat4 testMat1 = glm::mat4(1, 2.2, 3, 4.4, 5, 6.6, 7, 8.8, 9, 10.10, 11, 12.12, 13, 14.14, 15, 16.16);
			glm::mat4 testMat2 = glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32);
			glm::mat4 testMat3 = glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48);
			glm::mat4 testMat4 = glm::mat4(49, 50.50, 51, 52.52, 53, 54.54, 55, 56.56, 57, 58.58, 59, 60.60, 61, 62.62, 63, 64.64);

			Datum matrixDatum;

			matrixDatum.SetFromString("1 2.2 3 4.4 5 6.6 7 8.8 9 10.10 11 12.12 13 14.14 15 16.16", Datum::DatumType::Matrix);
			matrixDatum.SetFromString("17 18.18 19 20.20 21 22.22 23 24.24 25 26.26 27 28.28 29 30.30 31 32.32", Datum::DatumType::Matrix, 1);
			matrixDatum.SetFromString("33 34.34 35 36.36 37 38.38 39 40.40 41 42.42 43 44.44 45 46.46 47 48.48", Datum::DatumType::Matrix, 2);
			matrixDatum.SetFromString("49 50.50 51 52.52 53 54.54 55 56.56 57 58.58 59 60.60 61 62.62 63 64.64", Datum::DatumType::Matrix, 3);

			Assert::IsTrue(testMat1 == matrixDatum.Get<glm::mat4>());
			Assert::IsTrue(testMat2 == matrixDatum.Get<glm::mat4>(1));
			Assert::IsTrue(testMat3 == matrixDatum.Get<glm::mat4>(2));
			Assert::IsTrue(testMat4 == matrixDatum.Get<glm::mat4>(3));
		}

		TEST_METHOD(DatumTestStringSetFromString)
		{
			Datum stringDatum;

			stringDatum.SetFromString("I", Datum::DatumType::String);
			stringDatum.SetFromString("really", Datum::DatumType::String, 1);
			stringDatum.SetFromString("hate", Datum::DatumType::String, 2);
			stringDatum.SetFromString("Datum", Datum::DatumType::String, 3);

			Assert::IsTrue("I" == stringDatum.Get<std::string>());
			Assert::IsTrue("really" == stringDatum.Get<std::string>(1));
			Assert::IsTrue("hate" == stringDatum.Get<std::string>(2));
			Assert::IsTrue("Datum" == stringDatum.Get<std::string>(3));
		}

		TEST_METHOD(DatumTestPointerSetFromString)
		{
			Foo testFoo(13);

			std::string fooString = testFoo.ToString();

			Datum pointerDatum;

			Assert::ExpectException<std::exception>([&] { pointerDatum.SetFromString(fooString, Datum::DatumType::Pointer); });
		}

		TEST_METHOD(DatumTestRemoveInt)
		{
			Datum intDatum;

			intDatum.PushBack(1);
			intDatum.PushBack(2);
			intDatum.PushBack(3);

			Assert::AreEqual(3U, intDatum.Size());

			intDatum.Remove(2);
			Assert::AreEqual(2U, intDatum.Size());
			Assert::AreEqual(1, intDatum.Get<int>());
			Assert::AreEqual(3, intDatum.Get<int>(1));

			intDatum.Remove(3);
			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(1, intDatum.Get<int>());

			intDatum.Remove(1);
			Assert::AreEqual(0U, intDatum.Size());

			Assert::ExpectException<std::exception>([&] { intDatum.Remove(1.1f); });
		}

		TEST_METHOD(DatumTestRemoveFloat)
		{
			Datum floatDatum;

			floatDatum.PushBack(1.1f);
			floatDatum.PushBack(2.2f);
			floatDatum.PushBack(3.3f);

			Assert::AreEqual(3U, floatDatum.Size());

			floatDatum.Remove(2.2f);
			Assert::AreEqual(2U, floatDatum.Size());
			Assert::AreEqual(1.1f, floatDatum.Get<float>());
			Assert::AreEqual(3.3f, floatDatum.Get<float>(1));

			floatDatum.Remove(3.3f);
			Assert::AreEqual(1U, floatDatum.Size());
			Assert::AreEqual(1.1f, floatDatum.Get<float>());

			floatDatum.Remove(1.1f);
			Assert::AreEqual(0U, floatDatum.Size());

			Assert::ExpectException<std::exception>([&] { floatDatum.Remove("Hi"); });
		}

		TEST_METHOD(DatumTestRemoveVector)
		{
			glm::vec4 testVec1(1, 2, 3, 4);
			glm::vec4 testVec2(11, 12, 13, 14);
			glm::vec4 testVec3(21, 22, 23, 24);

			Datum vectorDatum;

			vectorDatum.PushBack(testVec1);
			vectorDatum.PushBack(testVec2);
			vectorDatum.PushBack(testVec3);

			Assert::AreEqual(3U, vectorDatum.Size());

			vectorDatum.Remove(testVec2);
			Assert::AreEqual(2U, vectorDatum.Size());
			Assert::IsTrue(testVec1 == vectorDatum.Get<glm::vec4>());
			Assert::IsTrue(testVec3 == vectorDatum.Get<glm::vec4>(1));

			vectorDatum.Remove(testVec3);
			Assert::AreEqual(1U, vectorDatum.Size());
			Assert::IsTrue(testVec1 == vectorDatum.Get<glm::vec4>());

			vectorDatum.Remove(testVec1);
			Assert::AreEqual(0U, vectorDatum.Size());

			Foo testFoo(44);

			Assert::ExpectException<std::exception>([&] { vectorDatum.Remove(&testFoo); });
		}

		TEST_METHOD(DatumTestRemoveMatrix)
		{
			glm::mat4 testMat1 = glm::mat4(1, 2.2, 3, 4.4, 5, 6.6, 7, 8.8, 9, 10.10, 11, 12.12, 13, 14.14, 15, 16.16);
			glm::mat4 testMat2 = glm::mat4(17, 18.18, 19, 20.20, 21, 22.22, 23, 24.24, 25, 26.26, 27, 28.28, 29, 30.30, 31, 32.32);
			glm::mat4 testMat3 = glm::mat4(33, 34.34, 35, 36.36, 37, 38.38, 39, 40.40, 41, 42.42, 43, 44.44, 45, 46.46, 47, 48.48);

			Datum matrixDatum;
			matrixDatum.PushBack(testMat1);
			matrixDatum.PushBack(testMat2);
			matrixDatum.PushBack(testMat3);

			Assert::AreEqual(3U, matrixDatum.Size());

			matrixDatum.Remove(testMat2);
			Assert::AreEqual(2U, matrixDatum.Size());
			Assert::IsTrue(testMat1 == matrixDatum.Get<glm::mat4>());
			Assert::IsTrue(testMat3 == matrixDatum.Get<glm::mat4>(1));

			matrixDatum.Remove(testMat1);
			Assert::AreEqual(1U, matrixDatum.Size());
			Assert::IsTrue(testMat3 == matrixDatum.Get<glm::mat4>());

			matrixDatum.Remove(testMat3);
			Assert::AreEqual(0U, matrixDatum.Size());

			Scope* testScope = new Scope();
			Assert::ExpectException<std::exception>([&] { matrixDatum.Remove(testScope); });

			delete testScope;
		}

		TEST_METHOD(DatumTestRemoveTable)
		{
			Datum tableDatum;

			Scope testScope;
			Scope otherTestScope;
			Scope finalTestScope;

			tableDatum.PushBack(testScope);
			tableDatum.PushBack(otherTestScope);
			tableDatum.PushBack(finalTestScope);

			Assert::AreEqual(3U, tableDatum.Size());

			tableDatum.Remove(otherTestScope);
			Assert::AreEqual(2U, tableDatum.Size());
			Assert::IsTrue(tableDatum[0] == testScope);
			Assert::IsTrue(tableDatum[1] == finalTestScope);

			tableDatum.Remove(finalTestScope);
			Assert::AreEqual(1U, tableDatum.Size());
			Assert::IsTrue(tableDatum[0] == testScope);

			tableDatum.Remove(testScope);
			Assert::AreEqual(0U, tableDatum.Size());

			Assert::ExpectException<std::exception>([&] { tableDatum.Remove(glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16)); });
		}

		TEST_METHOD(DatumTestRemoveString)
		{
			Datum stringDatum;

			stringDatum.PushBack("Welp");
			stringDatum.PushBack("this");
			stringDatum.PushBack("sucks");

			Assert::AreEqual(3U, stringDatum.Size());

			stringDatum.Remove("this");
			Assert::AreEqual(2U, stringDatum.Size());
			Assert::IsTrue("Welp" == stringDatum.Get<std::string>());
			Assert::IsTrue("sucks" == stringDatum.Get<std::string>(1));

			stringDatum.Remove("sucks");
			Assert::AreEqual(1U, stringDatum.Size());
			Assert::IsTrue("Welp" == stringDatum.Get<std::string>());

			stringDatum.Remove("Welp");
			Assert::AreEqual(0U, stringDatum.Size());

			Assert::ExpectException<std::exception>([&] {stringDatum.Remove(1); });
		}

		TEST_METHOD(DatumTestRemovePointer)
		{
			Foo testFoo(13);
			Foo otherTestFoo(15);
			Foo finalTestFoo(17);

			Datum pointerDatum;

			pointerDatum.PushBack(&testFoo);
			pointerDatum.PushBack(&otherTestFoo);
			pointerDatum.PushBack(&finalTestFoo);

			Assert::AreEqual(3U, pointerDatum.Size());

			pointerDatum.Remove(&otherTestFoo);
			Assert::AreEqual(2U, pointerDatum.Size());
			Assert::IsTrue(&testFoo == pointerDatum.Get<RTTI*>());
			Assert::IsTrue(&finalTestFoo == pointerDatum.Get<RTTI*>(1));

			pointerDatum.Remove(&finalTestFoo);
			Assert::AreEqual(1U, pointerDatum.Size());
			Assert::IsTrue(&testFoo == pointerDatum.Get<RTTI*>());

			pointerDatum.Remove(&testFoo);
			Assert::AreEqual(0U, pointerDatum.Size());

			Assert::ExpectException<std::exception>([&] {pointerDatum.Remove(glm::vec4(1, 2, 3, 4)); });
		}
	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState DatumTest::startMemState;
#endif
}