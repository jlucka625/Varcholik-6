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

	TEST_CLASS(ActionTest)
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

		TEST_METHOD(ActionTestParseFromFile)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;
			ActionListFactory aListFactory;
			ActionIfFactory aListIfFactory;
			ActionDecrementFactory aDecrementFactory;
			ActionIncrementFactory aIncrementFactory;
			ActionListWhileFactory aListWhileFactory;

			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper intParseHelper;
			FloatXmlParseHelper floatParseHelper;
			VectorXmlParseHelper vectorParseHelper;
			StringXmlParseHelper stringParseHelper;
			WorldXmlParseHelper worldParseHelper;
			SectorXmlParseHelper sectorParseHelper;
			EntityXmlParseHelper entityParseHelper;
			ActionXmlParseHelper actionParseHelper;

			parseMaster.AddHelper(intParseHelper);
			parseMaster.AddHelper(actionParseHelper);
			parseMaster.AddHelper(floatParseHelper);
			parseMaster.AddHelper(stringParseHelper);
			parseMaster.AddHelper(sectorParseHelper);
			parseMaster.AddHelper(vectorParseHelper);
			parseMaster.AddHelper(entityParseHelper);
			parseMaster.AddHelper(worldParseHelper);

			parseMaster.ParseFromFile("Content\\gamefile_action.xml");

			Datum& worlds = GetWorlds(parseMaster);
			World* world = worlds[0].As<World>();
			Assert::IsTrue(world->Name() == "World0");

			Datum& sectors = world->Sectors();
			Sector* sector1 = sectors[0].As<Sector>();
			Sector* sector2 = sectors[1].As<Sector>();
			Datum& entities = sector1->Entities();
			Enemy& heavyEnemy = *(entities[0].As<Enemy>());
			Datum& entities2 = sector2->Entities();
			Enemy& flyingEnemy = *(entities2[0].As<Enemy>());

			Datum& heavyEnemyActions = heavyEnemy.Actions();
			Assert::AreEqual(1U, heavyEnemyActions.Size());
			Assert::IsTrue(heavyEnemyActions[0].Is(ActionIf::TypeIdClass()));

			ActionIf& actionIf = *(heavyEnemyActions[0].As<ActionIf>());
			Assert::IsTrue(actionIf.Name() == "TestIf");
			Assert::AreEqual(0, actionIf["Condition"].Get<int>());
			Assert::AreEqual(1U, actionIf["Then"].Size());
			Assert::IsTrue(actionIf["Then"][0].Is(ActionList::TypeIdClass()));

			ActionList& thenBlock = *(actionIf["Then"][0].As<ActionList>());
			Assert::IsTrue(thenBlock.Actions()[0].Is("ActionDecrement"));

			Assert::AreEqual(1U, actionIf["Else"].Size());
			Assert::IsTrue(actionIf["Else"][0].Is(ActionList::TypeIdClass()));

			ActionList& elseBlock = *(actionIf["Else"][0].As<ActionList>());
			Assert::IsTrue(elseBlock.Actions()[0].Is("ActionIncrement"));

			Datum& flyingEnemyActions = flyingEnemy.Actions();
			Assert::AreEqual(1U, flyingEnemyActions.Size());
			Assert::IsTrue(flyingEnemyActions[0].Is("ActionListWhile"));

			
		}

		TEST_METHOD(ActionTestUpdate)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;
			ActionListFactory aListFactory;
			ActionIfFactory aListIfFactory;
			ActionDecrementFactory aDecrementFactory;
			ActionIncrementFactory aIncrementFactory;
			ActionListWhileFactory aListWhileFactory;

			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper intParseHelper;
			FloatXmlParseHelper floatParseHelper;
			VectorXmlParseHelper vectorParseHelper;
			StringXmlParseHelper stringParseHelper;
			WorldXmlParseHelper worldParseHelper;
			SectorXmlParseHelper sectorParseHelper;
			EntityXmlParseHelper entityParseHelper;
			ActionXmlParseHelper actionParseHelper;

			parseMaster.AddHelper(intParseHelper);
			parseMaster.AddHelper(actionParseHelper);
			parseMaster.AddHelper(floatParseHelper);
			parseMaster.AddHelper(stringParseHelper);
			parseMaster.AddHelper(sectorParseHelper);
			parseMaster.AddHelper(vectorParseHelper);
			parseMaster.AddHelper(entityParseHelper);
			parseMaster.AddHelper(worldParseHelper);

			parseMaster.ParseFromFile("Content\\gamefile_action.xml");

			Datum& worlds = GetWorlds(parseMaster);
			World& world = *(worlds[0].As<World>());
			Datum& sectors = world.Sectors();
			Sector* sector1 = sectors[0].As<Sector>();
			Assert::IsTrue(sector1->Name() == "Level1");
			Datum& entities = sector1->Entities();
			Enemy& heavyEnemy = *(entities[0].As<Enemy>());
			Datum& heavyEnemyActions = heavyEnemy.Actions();
			ActionIf& actionIf = *(static_cast<ActionIf*>(&heavyEnemyActions[0]));
			Assert::AreEqual(0, actionIf["Condition"].Get<int>());

			Sector* sector2 = sectors[1].As<Sector>();
			Datum& entities2 = sector2->Entities();
			Enemy& flyingEnemy = *(entities2[0].As<Enemy>());
			Datum& flyingEnemyActions = flyingEnemy.Actions();
			ActionListWhile& actionWhile = *(static_cast<ActionListWhile*>(&flyingEnemyActions[0]));
			Assert::AreEqual(10, actionWhile["Condition"].Get<int>());

			GameClock gameClock;
			GameTime gameTime;

			WorldState& worldState = world.GetWorldState();
			worldState.mSector = sector1;
			worldState.SetGameTime(gameTime);

			gameClock.Reset();

			for (int i = 0; i < 2; i++)
			{
				world.Update(gameClock);

				if (i % 2 == 0)
				{
					Assert::AreEqual(1, actionIf["Condition"].Get<int>());
				}
				else
				{
					Assert::AreEqual(0, actionIf["Condition"].Get<int>());
				}
			}

			worldState.mSector = sector2;
			world.Update(gameClock);

			Assert::AreEqual(0, actionWhile["Condition"].Get<int>());
		}

		TEST_METHOD(ActionListTestRTTI)
		{
			ActionList actionList;

			RTTI* testRTTI = &actionList;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ActionList"));
			Assert::IsTrue(testRTTI->Is("Action"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			ActionList* otherActionList = testRTTI->As<ActionList>();
			Assert::IsTrue(otherActionList != nullptr);
			Assert::IsTrue(&actionList == otherActionList);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Action* actionBasePointer = testRTTI->As<Action>();
			Assert::IsTrue(actionBasePointer != nullptr);

			Assert::IsTrue(testRTTI->Is(Action::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(ActionList::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(Action::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			otherTestRTTI = testRTTI->QueryInterface(ActionList::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionTest::startMemState;
#endif
}