#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(RTTI, Enemy)
	ConcreteFactory(RTTI, Entity)
	ConcreteFactory(RTTI, ActionList)
	ConcreteFactory(RTTI, ActionIf)
	ConcreteFactory(RTTI, ActionListWhile)
	ConcreteFactory(RTTI, ActionIncrement)
	ConcreteFactory(RTTI, ActionDecrement)
	ConcreteFactory(RTTI, ActionLoadSector)

	TEST_CLASS(ActionLoadSectorTest)
	{
	public:
		static Datum& GetWorlds(XmlParseMaster& parseMaster)
		{
			TableSharedData* tableSharedData = parseMaster.GetSharedData()->As<TableSharedData>();
			assert(tableSharedData != nullptr);

			return tableSharedData->GetCurrentScope()["Worlds"];
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

		TEST_METHOD(ActionLoadSectorTestInvalidLoad)
		{
			EntityFactory entityFactory;
			ActionLoadSectorFactory aLoadSectorFactory;

			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper intParseHelper;
			StringXmlParseHelper stringParseHelper;
			WorldXmlParseHelper worldParseHelper;
			SectorXmlParseHelper sectorParseHelper;
			EntityXmlParseHelper entityParseHelper;
			ActionXmlParseHelper actionParseHelper;

			parseMaster.AddHelper(intParseHelper);
			parseMaster.AddHelper(actionParseHelper);
			parseMaster.AddHelper(stringParseHelper);
			parseMaster.AddHelper(sectorParseHelper);
			parseMaster.AddHelper(entityParseHelper);
			parseMaster.AddHelper(worldParseHelper);

			std::string worldSnippet =
				"<Universe>\
					<World>\
						<Integer name=\"CurrentLevel\" value=\"7\"/>\
						<Sector>\
							<Entity class=\"Entity\">\
								<Action class=\"ActionLoadSector\">\
									<String name = \"Target\" value = \"CurrentLevel\"></String>\
								</Action>\
							</Entity>\
						</Sector>\
					</World>\
				 </Universe>";

			parseMaster.Parse(worldSnippet.c_str(), static_cast<int>(worldSnippet.length()), true, true);

			Datum& worlds = GetWorlds(parseMaster);
			World& world = *(worlds[0].As<World>());
			GameClock gameClock;
			GameTime gameTime;
			WorldState& worldState = world.GetWorldState();
			worldState.SetGameTime(gameTime);
			worldState.mSector = world.Sectors()[0].As<Sector>();
			gameClock.Reset();

			Assert::ExpectException<std::exception>([&] { world.Update(gameClock); });
		}

		TEST_METHOD(ActionLoadUnloadSectorTestRTTI)
		{
			ActionLoadSector aLoadSector;
			ActionUnloadSector aUnloadSector;

			RTTI* testRTTI = &aLoadSector;
			RTTI* testRTTI2 = &aUnloadSector;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsFalse(testRTTI2->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);
			Assert::IsTrue(testRTTI2->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ActionLoadSector"));
			Assert::IsFalse(testRTTI2->Is("Unknown"));
			Assert::IsTrue(testRTTI2->Is("ActionUnloadSector"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			Foo* dummyPointer2 = testRTTI2->As<Foo>();
			Assert::IsTrue(dummyPointer2 == nullptr);

			ActionLoadSector* otherALoadSector = testRTTI->As<ActionLoadSector>();
			Assert::IsTrue(otherALoadSector != nullptr);
			Assert::IsTrue(&aLoadSector == otherALoadSector);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			ActionUnloadSector* otherAUnloadSector = testRTTI2->As<ActionUnloadSector>();
			Assert::IsTrue(otherAUnloadSector != nullptr);
			Assert::IsTrue(&aUnloadSector == otherAUnloadSector);
			Assert::IsTrue(testRTTI2->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(ActionLoadSector::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			Assert::IsTrue(testRTTI2->Is(ActionUnloadSector::TypeIdClass()));
			Assert::IsTrue(testRTTI2->Is(testRTTI2->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(ActionLoadSector::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			RTTI* otherTestRTTI2 = testRTTI2->QueryInterface(ActionUnloadSector::TypeIdClass());
			Assert::IsTrue(otherTestRTTI2 != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionLoadSectorTest::startMemState;
#endif
}