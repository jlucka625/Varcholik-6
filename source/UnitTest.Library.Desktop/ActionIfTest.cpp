#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(RTTI, Entity)
	ConcreteFactory(RTTI, ActionList)
	ConcreteFactory(RTTI, ActionIf)
	ConcreteFactory(RTTI, ActionIncrement)
	ConcreteFactory(RTTI, ActionDecrement)

	TEST_CLASS(ActionIfTest)
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

		TEST_METHOD(ActionIfTestInvalidIfBlock)
		{
			EntityFactory entityFactory;
			ActionListFactory aListFactory;
			ActionIfFactory aIfFactory;
			ActionIncrementFactory aIncrementFactory;
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

			std::string ifNoThen = 
				"<Universe>\
					<World>\
						<Sector>\
							<Entity class=\"Entity\">\
								<Action class=\"ActionIf\">\
									<Integer name=\"Condition\" value=\"0\"></Integer>\
									<Action class=\"ActionList\" parentName=\"Else\">\
										<Action class=\"ActionIncrement\">\
											<String name = \"Target\" value = \"Condition\"></String>\
										</Action>\
									</Action>\
								</Action>\
							</Entity>\
						</Sector>\
					</World>\
				 </Universe>";

			parseMaster.Parse(ifNoThen.c_str(), static_cast<int>(ifNoThen.length()), true, true);

			Datum& worlds = GetWorlds(parseMaster);
			World& world = *(worlds[0].As<World>());
			GameClock gameClock;
			GameTime gameTime;
			WorldState& worldState = world.GetWorldState();
			worldState.mSector = world.Sectors()[0].As<Sector>();
			worldState.SetGameTime(gameTime);
			gameClock.Reset();

			Assert::ExpectException<std::exception>([&] { world.Update(gameClock); });
		}

		TEST_METHOD(ActionIfTestRTTI)
		{
			ActionIf actionIf;

			RTTI* testRTTI = &actionIf;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ActionIf"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			ActionIf* otherActionIf = testRTTI->As<ActionIf>();
			Assert::IsTrue(otherActionIf != nullptr);
			Assert::IsTrue(&actionIf == otherActionIf);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(ActionIf::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(ActionIf::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ActionIfTest::startMemState;
#endif
}