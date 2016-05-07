#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTest)
	{
	public:
#if defined(DEBUG) | defined(_DEBUG)
		TEST_METHOD_INITIALIZE(PrepForTests)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			//_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);
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

		TEST_METHOD(AttributedTestDefaultConstructor)
		{
			AttributedFoo testAFoo;
			Assert::AreEqual(14U, testAFoo.Size());

			Datum& thisDatum = testAFoo["this"];
			Assert::IsTrue(thisDatum.Get<RTTI*>() == &testAFoo);

			Datum& healthDatum = testAFoo["Health"];
			Assert::IsTrue(healthDatum.Get<int>() == 0);
			Assert::IsTrue(healthDatum.IsExternal());

			Datum& statsDatum = testAFoo["Stats"];
			Assert::IsTrue(!statsDatum.IsExternal());

			for (std::uint32_t i = 0; i < statsDatum.Size(); i++)
			{
				Assert::IsTrue(statsDatum.Get<int>(i) == 0);
			}

			Datum& percentsDatum = testAFoo["DamageModifiers"];

			for (std::uint32_t i = 0; i < percentsDatum.Size(); i++)
			{
				Assert::IsTrue(percentsDatum.Get<float>(i) == 1.0f);
			}

			AttributedBar testABar;
			Datum& otherThisDatum = testABar["this"];
			Assert::IsTrue(otherThisDatum.Get<RTTI*>() == &testABar);

			Datum& newPercentsDatum = testABar["DamageModifiers"];
			Assert::IsTrue(!newPercentsDatum.IsExternal());

			for (std::uint32_t i = 0; i < newPercentsDatum.Size(); i++)
			{
				Assert::IsTrue(newPercentsDatum.Get<float>(i) == 1.0f);
			}
		}

		TEST_METHOD(AttributedTestAppendAuxiliaryAttribute)
		{
			AttributedFoo testAFoo;
			
			Assert::ExpectException<std::exception>([&] {testAFoo.AppendAuxiliaryAttribute("Name"); });

			Assert::AreEqual(14U, testAFoo.Size());

			testAFoo.AppendAuxiliaryAttribute("Gender");
			Assert::AreEqual(15U, testAFoo.Size());
			Assert::IsTrue(testAFoo.IsAttribute("Gender"));
			Assert::IsTrue(testAFoo.IsAuxiliaryAttribute("Gender"));

			testAFoo.AppendAuxiliaryAttribute("Color");
			Assert::AreEqual(16U, testAFoo.Size());
			Assert::IsTrue(testAFoo.IsAttribute("Color"));
			Assert::IsTrue(testAFoo.IsAuxiliaryAttribute("Color"));
		}

		TEST_METHOD(AttributedTestAuxiliaryBegin)
		{
			std::uint64_t aFooClassID = AttributedFoo::TypeIdClass();
			std::cout << aFooClassID << std::endl;

			std::uint64_t attributedClassID = Attributed::TypeIdClass();
			std::cout << attributedClassID << std::endl;

			AttributedFoo testAFoo;
			Assert::AreEqual(14U, testAFoo.AuxiliaryBegin());

			AttributedBar testABar;
			Assert::AreEqual(16U, testABar.AuxiliaryBegin());
		}

		TEST_METHOD(AttributedTestIsAttribute)
		{
			AttributedFoo testAFoo;
			Assert::IsTrue(testAFoo.IsAttribute("Health"));
			Assert::IsTrue(testAFoo.IsAttribute("Money"));
			Assert::IsTrue(testAFoo.IsAttribute("Name"));
			Assert::IsTrue(testAFoo.IsAttribute("Position"));
			Assert::IsFalse(testAFoo.IsAttribute("Bogus"));
		}

		TEST_METHOD(AttributedTestIsPrescribedAttribute)
		{
			AttributedFoo testAFoo;
			testAFoo.AppendAuxiliaryAttribute("Gender");
			testAFoo.AppendAuxiliaryAttribute("Color");

			Assert::IsTrue(testAFoo.IsAttribute("Health"));
			Assert::IsTrue(testAFoo.IsAttribute("Money"));
			Assert::IsTrue(testAFoo.IsAttribute("Name"));
			Assert::IsTrue(testAFoo.IsAttribute("Position"));
			Assert::IsTrue(testAFoo.IsAttribute("Gender"));

			Assert::IsTrue(testAFoo.IsPrescribedAttribute("Health"));
			Assert::IsTrue(testAFoo.IsPrescribedAttribute("Money"));
			Assert::IsTrue(testAFoo.IsPrescribedAttribute("Name"));
			Assert::IsTrue(testAFoo.IsPrescribedAttribute("Position"));
			Assert::IsFalse(testAFoo.IsPrescribedAttribute("Gender"));
			Assert::IsFalse(testAFoo.IsPrescribedAttribute("Color"));
		}

		TEST_METHOD(AttributedTestCopyConstructor)
		{
			AttributedFoo testAFoo;
			Datum& healthDatum = testAFoo["Health"];
			Datum& moneyDatum = testAFoo["Money"];
			Datum& nameDatum = testAFoo["Name"];
			Datum& positionDatum = testAFoo["Position"];
			Datum& genderDatum = testAFoo.AppendAuxiliaryAttribute("Gender");
			
			healthDatum = 65;
			moneyDatum = 50.50f;
			nameDatum = "Fred";
			positionDatum = glm::vec4(50, 50, 50, 0);
			genderDatum = "Male";

			Assert::IsTrue(testAFoo["OuterScope"][0].GetParent() == &testAFoo);

			AttributedFoo otherTestAFoo = testAFoo;
			Assert::IsTrue(testAFoo == otherTestAFoo);
			Assert::AreEqual(otherTestAFoo.Size(), 15U);

			Assert::IsTrue(otherTestAFoo.IsAttribute("Health"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Money"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Name"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Position"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Gender"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("OuterScope"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Stats"));

			Datum& statsDatum = otherTestAFoo["Stats"];
			Assert::IsTrue(!statsDatum.IsExternal());

			for (std::uint32_t i = 0; i < statsDatum.Size(); i++)
			{
				Assert::IsTrue(statsDatum.Get<int>(i) == 0);
			}

			Assert::IsTrue(otherTestAFoo["this"] == &otherTestAFoo);
			Assert::IsTrue(otherTestAFoo["Health"] == 65);
			Assert::IsTrue(otherTestAFoo["Money"] == 50.50f);
			Assert::IsTrue(otherTestAFoo["Name"] == "Fred");
			Assert::IsTrue(otherTestAFoo["Position"] == glm::vec4(50, 50, 50, 0));
			Assert::IsTrue(otherTestAFoo["Gender"] == "Male");
			Assert::IsTrue(otherTestAFoo["OuterScope"][0].GetParent() == &otherTestAFoo);
		}

		TEST_METHOD(AttributedTestAssignmentOperator)
		{
			AttributedFoo testAFoo;
			Datum& healthDatum = testAFoo["Health"];
			Datum& moneyDatum = testAFoo["Money"];
			Datum& nameDatum = testAFoo["Name"];
			Datum& positionDatum = testAFoo["Position"];
			Datum& genderDatum = testAFoo.AppendAuxiliaryAttribute("Gender");

			healthDatum = 65;
			moneyDatum = 50.50f;
			nameDatum = "Fred";
			positionDatum = glm::vec4(50, 50, 50, 0);
			genderDatum = "Male";

			Assert::IsTrue(testAFoo["OuterScope"][0].GetParent() == &testAFoo);

			AttributedFoo otherTestAFoo;
			otherTestAFoo = testAFoo;
			Assert::IsTrue(testAFoo == otherTestAFoo);
			Assert::AreEqual(otherTestAFoo.Size(), 15U);

			Assert::IsTrue(otherTestAFoo.IsAttribute("Health"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Money"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Name"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Position"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Gender"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("OuterScope"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Stats"));

			Datum& statsDatum = otherTestAFoo["Stats"];
			Assert::IsTrue(!statsDatum.IsExternal());

			for (std::uint32_t i = 0; i < statsDatum.Size(); i++)
			{
				Assert::IsTrue(statsDatum.Get<int>(i) == 0);
			}

			Assert::IsTrue(otherTestAFoo["this"] == &otherTestAFoo);
			Assert::IsTrue(otherTestAFoo["Health"] == 65);
			Assert::IsTrue(otherTestAFoo["Money"] == 50.50f);
			Assert::IsTrue(otherTestAFoo["Name"] == "Fred");
			Assert::IsTrue(otherTestAFoo["Position"] == glm::vec4(50, 50, 50, 0));
			Assert::IsTrue(otherTestAFoo["Gender"] == "Male");
			Assert::IsTrue(otherTestAFoo["OuterScope"][0].GetParent() == &otherTestAFoo);
		}

		TEST_METHOD(AttributedTestMoveSemantics)
		{
			AttributedFoo testAFoo;
			Datum& healthDatum = testAFoo["Health"];
			Datum& moneyDatum = testAFoo["Money"];
			Datum& nameDatum = testAFoo["NameOfFoo"];
			Datum& positionDatum = testAFoo["Position"];
			Datum& genderDatum = testAFoo.AppendAuxiliaryAttribute("Gender");

			healthDatum = 65;
			moneyDatum = 50.50f;
			nameDatum = "Fred";
			positionDatum = glm::vec4(50, 50, 50, 0);
			genderDatum = "Male";

			Assert::IsTrue(testAFoo["OuterScope"][0].GetParent() == &testAFoo);

			AttributedFoo otherTestAFoo(std::move(testAFoo));
			Assert::AreEqual(otherTestAFoo.Size(), 16U);

			Assert::IsTrue(otherTestAFoo.IsAttribute("Health"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Money"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("NameOfFoo"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Position"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Gender"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("OuterScope"));
			Assert::IsTrue(otherTestAFoo.IsAttribute("Stats"));

			Datum& statsDatum = otherTestAFoo["Stats"];
			Assert::IsTrue(!statsDatum.IsExternal());

			for (std::uint32_t i = 0; i < statsDatum.Size(); i++)
			{
				Assert::IsTrue(statsDatum.Get<int>(i) == 0);
			}

			Assert::IsTrue(otherTestAFoo["this"] == &otherTestAFoo);
			Assert::IsTrue(otherTestAFoo["Health"] == 65);
			Assert::IsTrue(otherTestAFoo["Money"] == 50.50f);
			Assert::IsTrue(otherTestAFoo["NameOfFoo"] == "Fred");
			Assert::IsTrue(otherTestAFoo["Position"] == glm::vec4(50, 50, 50, 0));
			Assert::IsTrue(otherTestAFoo["Gender"] == "Male");
			Assert::IsTrue(otherTestAFoo["OuterScope"][0].GetParent() == &otherTestAFoo);

			AttributedFoo finalTestAFoo;
			finalTestAFoo = std::move(otherTestAFoo);
			Assert::AreEqual(finalTestAFoo.Size(), 16U);

			Assert::IsTrue(finalTestAFoo.IsAttribute("Health"));
			Assert::IsTrue(finalTestAFoo.IsAttribute("Money"));
			Assert::IsTrue(finalTestAFoo.IsAttribute("NameOfFoo"));
			Assert::IsTrue(finalTestAFoo.IsAttribute("Position"));
			Assert::IsTrue(finalTestAFoo.IsAttribute("Gender"));
			Assert::IsTrue(finalTestAFoo.IsAttribute("OuterScope"));
			Assert::IsTrue(finalTestAFoo.IsAttribute("Stats"));

			Datum& newStatsDatum = finalTestAFoo["Stats"];
			Assert::IsTrue(!newStatsDatum.IsExternal());

			for (std::uint32_t i = 0; i < newStatsDatum.Size(); i++)
			{
				Assert::IsTrue(newStatsDatum.Get<int>(i) == 0);
			}

			Assert::IsTrue(finalTestAFoo["this"] == &finalTestAFoo);
			Assert::IsTrue(finalTestAFoo["Health"] == 65);
			Assert::IsTrue(finalTestAFoo["Money"] == 50.50f);
			Assert::IsTrue(finalTestAFoo["NameOfFoo"] == "Fred");
			Assert::IsTrue(finalTestAFoo["Position"] == glm::vec4(50, 50, 50, 0));
			Assert::IsTrue(finalTestAFoo["Gender"] == "Male");
			Assert::IsTrue(finalTestAFoo["OuterScope"][0].GetParent() == &finalTestAFoo);
		}

		TEST_METHOD(AttributedTestEquality)
		{
			AttributedFoo testAFoo;

			AttributedFoo otherTestAFoo;
			otherTestAFoo.AppendAuxiliaryAttribute("NewAttrib1");

			Assert::IsTrue(testAFoo != otherTestAFoo);

			testAFoo.AppendAuxiliaryAttribute("NewAttrib1");
			testAFoo["Name"] = "Ted";

			Assert::IsTrue(testAFoo != otherTestAFoo);

			otherTestAFoo = testAFoo;
			Assert::IsTrue(testAFoo == otherTestAFoo);
		}

		TEST_METHOD(AttributedFooTestRTTIMethods)
		{
			AttributedFoo testAFoo;
			RTTI* testRTTI = &testAFoo;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("AttributedFoo"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			AttributedFoo* otherAFoo = testRTTI->As<AttributedFoo>();
			Assert::IsTrue(otherAFoo != nullptr);
			Assert::IsTrue(&testAFoo == otherAFoo);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(AttributedFoo::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(AttributedBarTestRTTIMethods)
		{
			AttributedBar testABar;
			RTTI* testRTTI = &testABar;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("AttributedBar"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			AttributedBar* otherABar = testRTTI->As<AttributedBar>();
			Assert::IsTrue(otherABar != nullptr);
			Assert::IsTrue(&testABar == otherABar);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(AttributedBar::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(AttributedBar::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState AttributedTest::startMemState;
#endif
}