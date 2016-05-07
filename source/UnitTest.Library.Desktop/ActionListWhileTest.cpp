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

	TEST_CLASS(ActionListWhileTest)
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

		TEST_METHOD(ActionListWhileTestUpdate)
		{
			EntityFactory entityFactory;
			ActionListFactory aListFactory;
			ActionListWhileFactory aListWhileFactory;
			ActionIfFactory aIfFactory;
			ActionDecrementFactory aDecrementFactory;

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

			std::string whileSnippet =
				"<Universe>\
					<World>\
						<Sector>\
							<Entity class=\"Entity\">\
								<Action class=\"ActionListWhile\">\
									<Integer name=\"Condition\" value=\"10\"></Integer>\
									<Action class=\"ActionList\">\
										<String name = \"Target\" value = \"Condition\"></String>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
										<Action class=\"ActionDecrement\">\
											<String name=\"Target\" value=\"Condition\"></String>\
										</Action>\
									</Action>\
								</Action>\
							</Entity>\
						</Sector>\
					</World>\
				 </Universe>";

			parseMaster.Parse(whileSnippet.c_str(), static_cast<int>(whileSnippet.length()), true, true);

			Datum& worlds = GetWorlds(parseMaster);
			World& world = *(worlds[0].As<World>());
			Sector* sector1 = world.Sectors()[0].As<Sector>();
			Datum& entities = sector1->Entities();
			Entity& emptyEntity = *(entities[0].As<Entity>());
			Datum& entityActions = emptyEntity.Actions();
			ActionListWhile& actionListWhile = *(static_cast<ActionListWhile*>(&entityActions[0]));
			Assert::AreEqual(10, actionListWhile["Condition"].Get<int>());

			GameClock gameClock;
			GameTime gameTime;
			WorldState& worldState = world.GetWorldState();
			worldState.mSector = sector1;
			worldState.SetGameTime(gameTime);
			gameClock.Reset();

			world.Update(gameClock);

			Assert::AreEqual(0, actionListWhile["Condition"].Get<int>());
		}

		TEST_METHOD(ActionListWhileTestRTTI)
		{
			ActionListWhile aListWhile;

			RTTI* testRTTI = &aListWhile;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ActionListWhile"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			ActionListWhile* otherAListWhile = testRTTI->As<ActionListWhile>();
			Assert::IsTrue(otherAListWhile != nullptr);
			Assert::IsTrue(&aListWhile == otherAListWhile);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(ActionListWhile::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(ActionListWhile::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionListWhileTest::startMemState;
#endif
}