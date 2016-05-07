#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(XmlParseMasterTest)
	{
	public:
		static Scope& GetSharedDataScope(XmlParseMaster& parseMaster)
		{
			TableSharedData* tableSharedData = parseMaster.GetSharedData()->As<TableSharedData>();

			if (tableSharedData == nullptr)
			{
				throw std::exception("Error: the shared data does not support table parsing functionality");
			}

			return tableSharedData->GetCurrentScope();
		}

		static void ClearSharedDataScope(XmlParseMaster& parseMaster)
		{
			TableSharedData* tableSharedData = parseMaster.GetSharedData()->As<TableSharedData>();

			if (tableSharedData == nullptr)
			{
				throw std::exception("Error: the shared data does not support table parsing functionality");
			}

			tableSharedData->Clear();
		}

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
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#endif

		TEST_METHOD(XmlParseMasterTestConstructor)
		{
			FooSharedData sharedData;
			Assert::IsTrue(sharedData.Depth() == 0);
			

			XmlParseMaster parseMaster(sharedData);
			Assert::AreEqual(0U, parseMaster.GetSharedData()->Depth());	
			Assert::ExpectException<std::exception>([&] {GetSharedDataScope(parseMaster); });
			Assert::ExpectException<std::exception>([&] {ClearSharedDataScope(parseMaster); });
		}

		TEST_METHOD(XmlParseMasterTestClone)
		{
			FooSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper intHelper;
			parseMaster.AddHelper(intHelper);

			FloatXmlParseHelper floatHelper;
			parseMaster.AddHelper(floatHelper);

			VectorXmlParseHelper vectorHelper;
			parseMaster.AddHelper(vectorHelper);

			MatrixXmlParseHelper matrixHelper;
			parseMaster.AddHelper(matrixHelper);

			TableXmlParseHelper tableHelper;
			parseMaster.AddHelper(tableHelper);

			StringXmlParseHelper stringHelper;
			parseMaster.AddHelper(stringHelper);

			WorldXmlParseHelper worldHelper;
			parseMaster.AddHelper(worldHelper);

			SectorXmlParseHelper sectorHelper;
			parseMaster.AddHelper(sectorHelper);

			EntityXmlParseHelper entityHelper;
			parseMaster.AddHelper(entityHelper);

			ActionXmlParseHelper actionHelper;
			parseMaster.AddHelper(actionHelper);

			ReactionXmlParseHelper reactionHelper;
			parseMaster.AddHelper(reactionHelper);

			XmlParseMaster* parseMasterClone = parseMaster.Clone();
			Assert::AreEqual(parseMaster.GetHelperCount(), parseMasterClone->GetHelperCount());
			Assert::ExpectException<std::exception>([&] {parseMasterClone->SetSharedData(sharedData); });

			TableSharedData otherSharedData;
			XmlParseMaster otherParseMaster(otherSharedData);

			XmlParseMaster* otherParseMasterClone = otherParseMaster.Clone();
			Assert::AreEqual(otherParseMaster.GetHelperCount(), otherParseMasterClone->GetHelperCount());
			Assert::ExpectException<std::exception>([&] {otherParseMasterClone->SetSharedData(sharedData); });

			delete parseMasterClone;
			delete otherParseMasterClone;
		}

		TEST_METHOD(XmlParseMasterTestAddHelper)
		{
			FooSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper defaultHelper;

			parseMaster.AddHelper(defaultHelper);
			Assert::AreEqual(1U, parseMaster.GetHelperCount());

			Assert::ExpectException<std::exception>([&] {parseMaster.AddHelper(defaultHelper); });
			
			XmlParseMaster* parseMasterClone = parseMaster.Clone();
			Assert::ExpectException<std::exception>([&] {parseMasterClone->AddHelper(defaultHelper); });

			delete parseMasterClone;
		}

		TEST_METHOD(XmlParseMasterTestRemoveHelper)
		{
			FooSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper defaultHelper;
			parseMaster.AddHelper(defaultHelper);

			XmlParseMaster* parseMasterClone = parseMaster.Clone();

			Assert::AreEqual(1U, parseMaster.GetHelperCount());

			parseMaster.RemoveHelper(defaultHelper);
			Assert::AreEqual(0U, parseMaster.GetHelperCount());

			Assert::ExpectException<std::exception>([&] {parseMasterClone->RemoveHelper(defaultHelper); });

			delete parseMasterClone;
		}

		TEST_METHOD(XmlParseMasterTestParseFromFile)
		{
			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			Assert::ExpectException<std::exception>([&] { parseMaster.ParseFromFile("nonexistent_file.xml"); });

			Assert::ExpectException<std::exception>([&] {parseMaster.Parse("Test file", 13, true, false); });

			IntegerXmlParseHelper intHelper;
			parseMaster.AddHelper(intHelper);

			FloatXmlParseHelper floatHelper;
			parseMaster.AddHelper(floatHelper);

			VectorXmlParseHelper vectorHelper;
			parseMaster.AddHelper(vectorHelper);

			MatrixXmlParseHelper matrixHelper;
			parseMaster.AddHelper(matrixHelper);

			TableXmlParseHelper tableHelper;
			parseMaster.AddHelper(tableHelper);

			StringXmlParseHelper stringHelper;
			parseMaster.AddHelper(stringHelper);

			WorldXmlParseHelper worldHelper;
			parseMaster.AddHelper(worldHelper);

			parseMaster.ParseFromFile("Content\\testfile.xml");

			Assert::IsTrue(parseMaster.GetFileName() == "Content\\testfile.xml");
			Scope& scope = GetSharedDataScope(parseMaster);

			Assert::AreEqual(6U, scope.Size());

			Datum* healthDatum = scope.Find("Health");
			Datum* moneyDatum = scope.Find("Money");
			Datum* nameDatum = scope.Find("Name");
			Datum* positionDatum = scope.Find("Position");
			Datum* fieldGridDatum = scope.Find("FieldGrid");
			Datum* innerScopeDatum = scope.Find("InnerScope");

			Assert::IsTrue(healthDatum != nullptr);
			Assert::AreEqual(100, healthDatum->Get<int>());

			Assert::IsTrue(moneyDatum != nullptr);
			Assert::AreEqual(2U, moneyDatum->Size());
			Assert::AreEqual(12.34f, moneyDatum->Get<float>());
			Assert::AreEqual(56.78f, moneyDatum->Get<float>(1));

			Assert::IsTrue(nameDatum != nullptr);
			Assert::AreEqual(2U, nameDatum->Size());
			Assert::IsTrue("Logan" == nameDatum->Get<std::string>());
			Assert::IsTrue("Paul" == nameDatum->Get<std::string>(1));

			Assert::IsTrue(positionDatum != nullptr);
			Assert::IsTrue(glm::vec4(50, 50, 50, 0) == positionDatum->Get<glm::vec4>());

			Assert::IsTrue(fieldGridDatum != nullptr);
			Assert::IsTrue(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) == fieldGridDatum->Get<glm::mat4>());

			Assert::IsTrue(innerScopeDatum != nullptr);
			Assert::AreEqual(2U, innerScopeDatum->Size());
			Assert::AreEqual(2U, innerScopeDatum->operator[](0).Size());
			Assert::AreEqual(3U, innerScopeDatum->operator[](1).Size());

			Datum* testScopeDatum = innerScopeDatum->operator[](1).Search("InnerInnerScope");
			Assert::IsTrue(testScopeDatum != nullptr);
			Assert::AreEqual(1U, testScopeDatum->operator[](0).Size());
			Assert::AreEqual(1U, testScopeDatum->operator[](0)[0].Size());

			Datum* positionDatum2 = testScopeDatum->operator[](0).Search("Position2");
			Assert::IsTrue(positionDatum2 != nullptr);
			Assert::IsTrue(glm::vec4(0, 0, 0, 50) == positionDatum2->Get<glm::vec4>());

			ClearSharedDataScope(parseMaster);
			Assert::AreEqual(GetSharedDataScope(parseMaster).Size(), 0U);

			parseMaster.ParseFromFile("Content\\testfile.xml");
			Assert::AreEqual(GetSharedDataScope(parseMaster).Size(), 6U);
		}

		TEST_METHOD(XmlParseMasterTestInvalidParsing)
		{
			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			TableXmlParseHelper tableHelper;
			parseMaster.AddHelper(tableHelper);

			IntegerXmlParseHelper intHelper;
			parseMaster.AddHelper(intHelper);

			std::string missingIntName = "<Integer value=\"13\"></Integer>";
			std::string missingIntValue = "<Integer name=\"Health\"></Integer>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingIntName.c_str(), static_cast<int>(missingIntName.length()), true, true); });
			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingIntValue.c_str(), static_cast<int>(missingIntValue.length()), true, true); });

			FloatXmlParseHelper floatHelper;
			parseMaster.AddHelper(floatHelper);

			std::string missingFloatName = "<Float value=\"13.13\"></Float>";
			std::string missingFloatValue = "<Float name=\"Money\"></Float>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingFloatName.c_str(), static_cast<int>(missingFloatName.length()), true, true); });
			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingFloatValue.c_str(), static_cast<int>(missingFloatValue.length()), true, true); });

			VectorXmlParseHelper vectorHelper;
			parseMaster.AddHelper(vectorHelper);

			std::string missingVectorName = "<Vector value=\"0 0 0 0\"></Vector>";
			std::string missingVectorValue = "<Vector name=\"Position\"></Vector>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingVectorName.c_str(), static_cast<int>(missingVectorName.length()), true, true); });
			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingVectorValue.c_str(), static_cast<int>(missingVectorValue.length()), true, true); });

			MatrixXmlParseHelper matrixHelper;
			parseMaster.AddHelper(matrixHelper);

			std::string missingMatrixName = "<Matrix value=\"0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\"></Matrix>";
			std::string missingMatrixValue = "<Matrix name=\"Grid\"></Matrix>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingMatrixName.c_str(), static_cast<int>(missingMatrixName.length()), true, true); });
			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingMatrixValue.c_str(), static_cast<int>(missingMatrixValue.length()), true, true); });

			std::string missingTableName = "<Table value=\"13.13\"></Table>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingTableName.c_str(), static_cast<int>(missingTableName.length()), true, true); });

			StringXmlParseHelper stringHelper;
			parseMaster.AddHelper(stringHelper);

			std::string missingStringName = "<String value=\"Fred\"></String>";
			std::string missingStringValue = "<String name=\"Name\"></String>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingStringName.c_str(), static_cast<int>(missingStringName.length()), true, true); });
			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingStringValue.c_str(), static_cast<int>(missingStringValue.length()), true, true); });
		}

		TEST_METHOD(XmlParseMasterTestRTTI)
		{
			//FooSharedData and SharedData (base) test
			FooSharedData testDefaultSharedData;

			XmlParseMaster::SharedData* testSharedData = testDefaultSharedData.Clone();
			RTTI* testRTTI = testSharedData;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("SharedData"));
			Assert::IsTrue(testRTTI->Is("FooSharedData"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			XmlParseMaster::SharedData* otherSharedData = testRTTI->As<XmlParseMaster::SharedData>();
			Assert::IsTrue(otherSharedData != nullptr);
			Assert::IsTrue(testSharedData == otherSharedData);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			FooSharedData* otherOtherSharedData = testRTTI->As<FooSharedData>();
			Assert::IsTrue(otherOtherSharedData != nullptr);

			Assert::IsTrue(testRTTI->Is(XmlParseMaster::SharedData::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(FooSharedData::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(XmlParseMaster::SharedData::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			//TableSharedData test
			TableSharedData testDatumTagSharedData;

			RTTI* finalTestRTTI = &testDatumTagSharedData;

			Assert::IsFalse(finalTestRTTI->Is(44));
			Assert::IsTrue(finalTestRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(finalTestRTTI->Is("Unknown"));
			Assert::IsTrue(finalTestRTTI->Is("TableSharedData"));

			Foo* otherDummyPointer = finalTestRTTI->As<Foo>();
			Assert::IsTrue(otherDummyPointer == nullptr);

			TableSharedData* finalTestSharedData = finalTestRTTI->As<TableSharedData>();
			Assert::IsTrue(finalTestSharedData != nullptr);
			Assert::IsTrue(&testDatumTagSharedData == finalTestSharedData);
			Assert::IsTrue(finalTestRTTI->ToString() == "RTTI");

			Assert::IsTrue(finalTestRTTI->Is(TableSharedData::TypeIdClass()));
			Assert::IsTrue(finalTestRTTI->Is(finalTestRTTI->TypeIdInstance()));

			RTTI* finalFinalTestRTTI = finalTestRTTI->QueryInterface(TableSharedData::TypeIdClass());
			Assert::IsTrue(finalFinalTestRTTI != nullptr);

			delete testSharedData;
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState XmlParseMasterTest::startMemState;
#endif
}