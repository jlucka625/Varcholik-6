#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(ScopeTestDefaultConstructor)
		{
			Scope testScope(0U);
			Assert::AreEqual(0U, testScope.Size());
			Assert::AreEqual(1U, testScope.Capacity());
			
			Scope otherTestScope;
			Assert::AreEqual(0U, otherTestScope.Size());
			Assert::AreEqual(6U, otherTestScope.Capacity());
		}

		TEST_METHOD(ScopeTestCopyConstructor)
		{
			Scope testScope;
			Datum& intDatum = testScope.Append("Health");
			Datum& floatDatum = testScope.Append("Money");
			Datum& stringDatum = testScope.Append("Name");

			intDatum = 100;
			floatDatum = 99.99f;
			stringDatum = "Charles";

			Scope otherTestScope = testScope;
			Assert::IsTrue(testScope == otherTestScope);
			Assert::AreEqual(3U, otherTestScope.Size());
			Assert::IsTrue(otherTestScope["Health"] == 100);
			Assert::IsTrue(otherTestScope["Money"] == 99.99f);
			Assert::IsTrue(otherTestScope["Name"] == "Charles");

			Scope& appended = otherTestScope.AppendScope("InnerScope");
			Datum& vectorDatum = appended.Append("Position");
			Datum& matrixDatum = appended.Append("FieldGrid");

			vectorDatum = glm::vec4(50, 20, 10, 1);
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Assert::IsTrue(otherTestScope["InnerScope"][0].GetParent() == &otherTestScope);

			Scope finalTestScope = otherTestScope;
			Assert::IsTrue(finalTestScope == otherTestScope);
			Assert::AreEqual(4U, finalTestScope.Size());
			Assert::IsTrue(finalTestScope["InnerScope"][0].GetParent() == &finalTestScope);
			Assert::IsTrue(finalTestScope["InnerScope"][0][0] == glm::vec4(50, 20, 10, 1));
			Assert::IsTrue(finalTestScope["InnerScope"][0][1] == glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
		}

		TEST_METHOD(ScopeTestAssignmentOperator)
		{
			Scope testScope;
			Datum& intDatum = testScope.Append("Health");
			Datum& floatDatum = testScope.Append("Money");
			Datum& stringDatum = testScope.Append("Name");

			intDatum = 100;
			floatDatum = 99.99f;
			stringDatum = "Charles";

			Scope otherTestScope;
			otherTestScope = testScope;
			Assert::IsTrue(testScope == otherTestScope);
			Assert::AreEqual(3U, otherTestScope.Size());
			Assert::IsTrue(otherTestScope["Health"] == 100);
			Assert::IsTrue(otherTestScope["Money"] == 99.99f);
			Assert::IsTrue(otherTestScope["Name"] == "Charles");

			Scope& appended = otherTestScope.AppendScope("InnerScope");
			Datum& vectorDatum = appended.Append("Position");
			Datum& matrixDatum = appended.Append("FieldGrid");

			vectorDatum = glm::vec4(50, 20, 10, 1);
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Assert::IsTrue(otherTestScope["InnerScope"][0].GetParent() == &otherTestScope);

			Scope finalTestScope;
			finalTestScope = otherTestScope;

			Assert::IsTrue(finalTestScope == otherTestScope);
			Assert::AreEqual(4U, finalTestScope.Size());
			Assert::IsTrue(finalTestScope["InnerScope"][0].GetParent() == &finalTestScope);
			Assert::IsTrue(finalTestScope["InnerScope"][0][0] == glm::vec4(50, 20, 10, 1));
			Assert::IsTrue(finalTestScope["InnerScope"][0][1] == glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
		}

		TEST_METHOD(ScopeTestMoveSemantics)
		{
			Scope testScope;
			Datum& intDatum = testScope.Append("Health");
			Datum& floatDatum = testScope.Append("Money");
			Datum& stringDatum = testScope.Append("Name");
			testScope.AppendScope("TestScope");

			intDatum = 100;
			floatDatum = 99.99f;
			stringDatum = "Charles";
			Assert::IsTrue(testScope["TestScope"][0].GetParent() == &testScope);

			Scope otherTestScope(std::move(testScope));
			Assert::AreEqual(4U, otherTestScope.Size());
			Assert::IsTrue(otherTestScope["Health"] == 100);
			Assert::IsTrue(otherTestScope["Money"] == 99.99f);
			Assert::IsTrue(otherTestScope["Name"] == "Charles");
			Assert::IsTrue(otherTestScope["TestScope"][0].GetParent() == &otherTestScope);

			Scope finalTestScope;
			finalTestScope = std::move(otherTestScope);
			Assert::AreEqual(4U, finalTestScope.Size());
			Assert::IsTrue(finalTestScope["Health"] == 100);
			Assert::IsTrue(finalTestScope["Money"] == 99.99f);
			Assert::IsTrue(finalTestScope["Name"] == "Charles");
			Assert::IsTrue(finalTestScope["TestScope"][0].GetParent() == &finalTestScope);
		}

		TEST_METHOD(ScopeTestEquality)
		{
			Scope testScope;
	
			Scope otherTestScope;
			Assert::IsTrue(testScope == otherTestScope);

			Datum& intDatum = testScope.Append("Health");
			Datum& floatDatum = testScope.Append("Money");
			Datum& stringDatum = testScope.Append("Name");

			otherTestScope.Append("Health");

			Assert::IsTrue(testScope != otherTestScope);

			otherTestScope.Append("Money");
			otherTestScope.Append("NAME");

			Assert::IsTrue(testScope != otherTestScope);

			otherTestScope.Clear();
			Datum& otherIntDatum = otherTestScope.Append("Health");
			Datum& otherFloatDatum = otherTestScope.Append("Money");
			Datum& otherStringDatum = otherTestScope.Append("Name");

			intDatum = 100;
			otherIntDatum = 100;

			floatDatum = 99.99f;
			otherFloatDatum = 66.66f;

			stringDatum = "John";
			otherStringDatum = "John";

			Assert::IsTrue(testScope != otherTestScope);

			otherFloatDatum = 99.99f;
			Assert::IsTrue(testScope == otherTestScope);
		}

		TEST_METHOD(ScopeTestAppend)
		{
			Scope testScope;

			testScope.Append("Health");
			Datum* intDatum = testScope.Find("Health");

			Assert::IsTrue(intDatum != nullptr);

			intDatum->PushBack(0);
			intDatum->PushBack(1);
			intDatum->PushBack(2);

			Datum otherIntDatum = testScope[0];

			Assert::IsTrue(*intDatum == otherIntDatum);

			Assert::ExpectException<std::exception>([&] { testScope.Append(""); });

			testScope.Append("Health");
			testScope.Append("Health");
			testScope.Append("Health");
			testScope.Append("Health");
			testScope.Append("Health");

			Assert::IsTrue(*intDatum == *(testScope.Find("Health")));
		}

		TEST_METHOD(ScopeTestAppendScope)
		{
			Datum intDatum;
			intDatum = 100;

			Datum floatDatum;
			floatDatum = 66.66f;

			Datum stringDatum;
			stringDatum = "Fred";

			Scope outerScope;

			outerScope["Health"] = intDatum;
			outerScope["Money"] = floatDatum;
			outerScope["Name"] = stringDatum;

			Assert::IsTrue(outerScope.GetParent() == nullptr);
			Assert::AreEqual(3U, outerScope.Size());

			Assert::ExpectException<std::exception>([&] {outerScope.AppendScope("Health"); });

			Assert::ExpectException<std::exception>([&] {outerScope.AppendScope(""); });

			Scope& innerScope = outerScope.AppendScope("InnerScope");
			Assert::IsTrue(innerScope.GetParent() == &outerScope);

			Datum vectorDatum;
			vectorDatum = glm::vec4(50, 20, 30, 1);

			Datum matrixDatum;
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			innerScope["Position"] = vectorDatum;
			innerScope["FieldGrid"] = matrixDatum;

			Assert::AreEqual(4U, outerScope.Size());
			Assert::AreEqual(2U, innerScope.Size());

			Scope& innerInnerScope = innerScope.AppendScope("InnerInnerScope");
			Scope& otherInnerInnerScope = innerScope.AppendScope("InnerInnerScope");
			Assert::IsTrue(innerScope.GetParent() == &outerScope);
			Assert::IsTrue(innerInnerScope.GetParent() == &innerScope);
			Assert::IsTrue(otherInnerInnerScope.GetParent() == &innerScope);

			Foo testFoo(13);

			Datum pointerDatum;
			pointerDatum = &testFoo;

			innerInnerScope["DummyVar"] = pointerDatum;

			Assert::AreEqual(4U, outerScope.Size());
			Assert::AreEqual(3U, innerScope.Size());
			Assert::AreEqual(1U, innerInnerScope.Size());
			Assert::AreEqual(2U, innerScope.Search("InnerInnerScope")->Size());
		}

		TEST_METHOD(ScopeTestAdopt)
		{
			Datum intDatum;
			intDatum = 100;

			Datum floatDatum;
			floatDatum = 66.66f;

			Datum vectorDatum;
			vectorDatum = glm::vec4(50, 20, 30, 1);

			Datum matrixDatum;
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Datum stringDatum;
			stringDatum = "Fred";

			Foo testFoo(13);

			Datum pointerDatum;
			pointerDatum = &testFoo;

			Scope outerScope;

			Scope otherOuterScope;
			Scope& innerScope = otherOuterScope.AppendScope("InnerScope");
			Assert::AreEqual(1U, otherOuterScope[0].Size());
			Assert::IsTrue(innerScope.GetParent() == &otherOuterScope);

			innerScope["Health"] = intDatum;
			innerScope["Money"] = floatDatum;
			innerScope["Name"] = stringDatum;

			Scope finalOtherOuterScope;
			Scope& otherInnerScope = finalOtherOuterScope.AppendScope("OtherInnerScope");
			Assert::AreEqual(1U, finalOtherOuterScope[0].Size());
			Assert::IsTrue(otherInnerScope.GetParent() == &finalOtherOuterScope);

			otherInnerScope["Position"] = vectorDatum;
			otherInnerScope["FieldGrid"] = matrixDatum;

			Scope& newScope = otherInnerScope.AppendScope("FlipFlopScope");
			Assert::AreEqual(1U, otherInnerScope[2].Size());
			Assert::IsTrue(newScope.GetParent() == &otherInnerScope);

			Assert::ExpectException<std::exception>([&] {outerScope.Adopt(innerScope, ""); });

			outerScope.Adopt(innerScope, "InnerScope");
			Assert::AreEqual(0U, otherOuterScope[0].Size());
			Assert::AreEqual(1U, outerScope.Size());
			Assert::IsTrue(innerScope.GetParent() == &outerScope);

			outerScope.Adopt(otherInnerScope, "OtherInnerScope");
			Assert::AreEqual(0U, finalOtherOuterScope[0].Size());
			Assert::AreEqual(2U, outerScope.Size());
			Assert::IsTrue(otherInnerScope.GetParent() == &outerScope);

			outerScope.Adopt(newScope, "FlipFlopScope");
			Assert::AreEqual(0U, otherInnerScope[2].Size());
			Assert::AreEqual(3U, outerScope.Size());
			Assert::IsTrue(newScope.GetParent() == &outerScope);
		}

		TEST_METHOD(ScopeTestFind)
		{
			Datum intDatum;
			intDatum = 100;

			Datum floatDatum;
			floatDatum = 66.66f;

			Datum vectorDatum;
			vectorDatum = glm::vec4(50, 20, 30, 1);

			Datum matrixDatum;
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Datum stringDatum;
			stringDatum = "Fred";

			Foo testFoo(13);

			Datum pointerDatum;
			pointerDatum = &testFoo;


			Scope outerScope;
			outerScope["Health"] = intDatum;
			outerScope["Money"] = floatDatum;
			outerScope["Name"] = stringDatum;

			Scope& innerScope = outerScope.AppendScope("InnerScope");
			innerScope["Position"] = vectorDatum;
			innerScope["FieldGrid"] = matrixDatum;

			Scope& innerInnerScope = innerScope.AppendScope("InnerInnerScope");
			Assert::IsTrue(innerScope.GetParent() == &outerScope);
			Assert::IsTrue(innerInnerScope.GetParent() == &innerScope);
			innerInnerScope["DummyVar"] = pointerDatum;


			Datum* found = outerScope.Find("Health");
			Assert::IsTrue(found != nullptr);

			found = outerScope.Find("Position");
			Assert::IsTrue(found == nullptr);

			found = innerScope.Find("FieldGrid");
			Assert::IsTrue(found != nullptr);

			found = innerScope.Find("Money");
			Assert::IsTrue(found == nullptr);

			found = innerInnerScope.Find("DummyVar");
			Assert::IsTrue(found != nullptr);

			found = innerInnerScope.Find("Name");
			Assert::IsTrue(found == nullptr);
		}

		TEST_METHOD(ScopeTestFindName)
		{
			Datum intDatum;
			intDatum = 100;

			Datum floatDatum;
			floatDatum = 66.66f;

			Datum vectorDatum;
			vectorDatum = glm::vec4(50, 20, 30, 1);

			Datum matrixDatum;
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Datum stringDatum;
			stringDatum = "Fred";

			Foo testFoo(13);

			Datum pointerDatum;
			pointerDatum = &testFoo;


			Scope outerScope;
			outerScope["Health"] = intDatum;
			outerScope["Money"] = floatDatum;
			outerScope["Name"] = stringDatum;

			std::string scopeName = outerScope.FindName(outerScope);
			Assert::IsTrue("" == scopeName);

			Scope& innerScope = outerScope.AppendScope("InnerScope");
			innerScope["Position"] = vectorDatum;
			innerScope["FieldGrid"] = matrixDatum;

			scopeName = outerScope.FindName(innerScope);
			Assert::IsTrue("InnerScope" == scopeName);

			Scope& innerInnerScope = innerScope.AppendScope("InnerInnerScope");
			Assert::IsTrue(innerScope.GetParent() == &outerScope);
			Assert::IsTrue(innerInnerScope.GetParent() == &innerScope);
			innerInnerScope["DummyVar"] = pointerDatum;

			scopeName = innerScope.FindName(innerInnerScope);
			Assert::IsTrue("InnerInnerScope" == scopeName);
		}

		TEST_METHOD(ScopeTestSearch)
		{
			Datum intDatum;
			intDatum = 100;

			Datum floatDatum;
			floatDatum = 66.66f;

			Datum vectorDatum;
			vectorDatum = glm::vec4(50, 20, 30, 1);

			Datum matrixDatum;
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Datum stringDatum;
			stringDatum = "Fred";

			Foo testFoo(13);

			Datum pointerDatum;
			pointerDatum = &testFoo;


			Scope outerScope;
			outerScope["Health"] = intDatum;
			outerScope["Money"] = floatDatum;
			outerScope["Name"] = stringDatum;

			Scope& innerScope = outerScope.AppendScope("InnerScope");
			innerScope["Position"] = vectorDatum;
			innerScope["FieldGrid"] = matrixDatum;

			Scope& innerInnerScope = innerScope.AppendScope("InnerInnerScope");
			Assert::IsTrue(innerScope.GetParent() == &outerScope);
			Assert::IsTrue(innerInnerScope.GetParent() == &innerScope);
			innerInnerScope["DummyVar"] = pointerDatum;


			Datum* found = outerScope.Search("Health");
			Assert::IsTrue(found != nullptr);

			found = outerScope.Search("DummyVar");
			Assert::IsTrue(found == nullptr);

			found = innerScope.Search("Position");
			Assert::IsTrue(found != nullptr);

			found = innerScope.Search("Money");
			Assert::IsTrue(found != nullptr);

			found = innerScope.Search("DummyVar");
			Assert::IsTrue(found == nullptr);

			found = innerInnerScope.Search("DummyVar");
			Assert::IsTrue(found != nullptr);

			found = innerInnerScope.Search("FieldGrid");
			Assert::IsTrue(found != nullptr);

			found = innerInnerScope.Search("Name");
			Assert::IsTrue(found != nullptr);
		}

		TEST_METHOD(ScopeTestClear)
		{
			Datum intDatum;
			intDatum = 100;

			Datum floatDatum;
			floatDatum = 66.66f;

			Datum vectorDatum;
			vectorDatum = glm::vec4(50, 20, 30, 1);

			Datum matrixDatum;
			matrixDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

			Datum stringDatum;
			stringDatum = "Fred";

			Foo testFoo(13);

			Datum pointerDatum;
			pointerDatum = &testFoo;


			Scope outerScope;
			outerScope["Health"] = intDatum;
			outerScope["Money"] = floatDatum;
			outerScope["Name"] = stringDatum;

			Scope& innerScope = outerScope.AppendScope("InnerScope");
			innerScope["Position"] = vectorDatum;
			innerScope["FieldGrid"] = matrixDatum;

			Scope& innerInnerScope = innerScope.AppendScope("InnerInnerScope");
			innerInnerScope["DummyVar"] = pointerDatum;

			Assert::AreEqual(1U, innerInnerScope.Size());
			delete &innerInnerScope;

			Assert::AreEqual(3U, innerScope.Size());
			delete &innerScope;

			Assert::AreEqual(4U, outerScope.Size());
			outerScope.Clear();
			Assert::AreEqual(0U, outerScope.Size());
		}

		TEST_METHOD(ScopeTestRTTIMethods)
		{
			Scope testScope;
			RTTI* testRTTI = &testScope;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("Scope"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			Scope* otherScope = testRTTI->As<Scope>();
			Assert::IsTrue(otherScope != nullptr);
			Assert::IsTrue(&testScope == otherScope);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(Scope::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(Scope::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}
	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ScopeTest::startMemState;
#endif
}