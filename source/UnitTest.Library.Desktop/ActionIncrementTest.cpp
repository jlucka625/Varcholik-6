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

	TEST_CLASS(ActionIncrementTest)
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

		TEST_METHOD(ActionIncrementTestInvalidIncrement)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;
			ActionListFactory aListFactory;
			ActionIncrementFactory aIncrementFactory;

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

			std::string incrementNoTarget = "<Universe><World name=\"Overworld\"><Sector name=\"Level1\"><Entity name=\"DefaultEnemy\" class=\"Enemy\"><Integer name=\"test\" value=\"50\"></Integer><Action name=\"increment\" class=\"ActionIncrement\"></Action></Entity></Sector></World></Universe>";
			parseMaster.Parse(incrementNoTarget.c_str(), static_cast<int>(incrementNoTarget.length()), true, true);

			Datum& worlds = GetWorlds(parseMaster);
			World& world = *(worlds[0].As<World>());
			GameClock gameClock;
			GameTime gameTime;
			WorldState& worldState = world.GetWorldState();
			worldState.SetGameTime(gameTime);
			worldState.mSector = world.Sectors()[0].As<Sector>();
			gameClock.Reset();

			Assert::ExpectException<std::exception>([&] { world.Update(gameClock); });

			XmlParseMaster* parseMasterClone2 = parseMaster.Clone();

			std::string incrementMIATarget = "<Universe><World name=\"Overworld\"><Sector name=\"Level1\"><Entity name=\"DefaultEnemy\" class=\"Enemy\"><Integer name=\"test\" value=\"50\"></Integer><Action name=\"increment\" class=\"ActionIncrement\"><String name=\"Target\" value=\"bogus\"></String></Action></Entity></Sector></World></Universe>";
			parseMasterClone2->Parse(incrementMIATarget.c_str(), static_cast<int>(incrementMIATarget.length()), true, true);

			Datum& worlds3 = GetWorlds(*parseMasterClone2);
			World& world3 = *(worlds3[0].As<World>());

			WorldState& worldState3 = world3.GetWorldState();
			worldState3.mSector = world3.Sectors()[0].As<Sector>();
			worldState3.SetGameTime(gameTime);
			gameClock.Reset();

			Assert::ExpectException<std::exception>([&] { world3.Update(gameClock); });

			XmlParseMaster* parseMasterClone = parseMaster.Clone();
			std::string incrementBadTarget = "<Universe><World name=\"Overworld\"><Sector name=\"Level1\"><Entity name=\"DefaultEnemy\" class=\"Enemy\"><String name=\"test\" value=\"Oops!\"></String><Action name=\"increment\" class=\"ActionIncrement\"><String name=\"Target\" value=\"test\"></String></Action></Entity></Sector></World></Universe>";
			parseMasterClone->Parse(incrementBadTarget.c_str(), static_cast<int>(incrementBadTarget.length()), true, true);

			Datum& worlds2 = GetWorlds(*parseMasterClone);
			World& world2 = *(worlds2[0].As<World>());

			WorldState& worldState2 = world2.GetWorldState();
			worldState2.mSector = world2.Sectors()[0].As<Sector>();
			worldState2.SetGameTime(gameTime);
			gameClock.Reset();

			Assert::ExpectException<std::exception>([&] { world2.Update(gameClock); });

			delete parseMasterClone;
			delete parseMasterClone2;
		}

		TEST_METHOD(ActionIncrementTestRTTI)
		{
			ActionIncrement actionIncrement;

			RTTI* testRTTI = &actionIncrement;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ActionIncrement"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			ActionIncrement* otherActionIncrement = testRTTI->As<ActionIncrement>();
			Assert::IsTrue(otherActionIncrement != nullptr);
			Assert::IsTrue(&actionIncrement == otherActionIncrement);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(ActionIncrement::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(ActionIncrement::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionIncrementTest::startMemState;
#endif
}