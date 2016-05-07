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
	ConcreteFactory(RTTI, ActionUnloadSector)
	ConcreteFactory(RTTI, ReactionAttributed)
	ConcreteFactory(RTTI, ActionEvent)

	TEST_CLASS(ReactionTest)
	{
	public:
		static World& GetWorldFromXML(XmlParseMaster& parseMaster, const std::string& filename)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;
			ActionListFactory aListFactory;
			ActionIfFactory aListIfFactory;
			ActionDecrementFactory aDecrementFactory;
			ActionIncrementFactory aIncrementFactory;
			ActionListWhileFactory aListWhileFactory;
			ActionLoadSectorFactory aLoadSectorFactory;
			ActionUnloadSectorFactory aUnloadSectorFactory;
			ReactionAttributedFactory aReactionFactory;
			ActionEventFactory aEventFactory;

			IntegerXmlParseHelper intParseHelper;
			FloatXmlParseHelper floatParseHelper;
			StringXmlParseHelper stringParseHelper;
			WorldXmlParseHelper worldParseHelper;
			SectorXmlParseHelper sectorParseHelper;
			EntityXmlParseHelper entityParseHelper;
			ActionXmlParseHelper actionParseHelper;
			ReactionXmlParseHelper reactionParseHelper;

			parseMaster.AddHelper(intParseHelper);
			parseMaster.AddHelper(actionParseHelper);
			parseMaster.AddHelper(floatParseHelper);
			parseMaster.AddHelper(stringParseHelper);
			parseMaster.AddHelper(sectorParseHelper);
			parseMaster.AddHelper(reactionParseHelper);
			parseMaster.AddHelper(entityParseHelper);
			parseMaster.AddHelper(worldParseHelper);

			parseMaster.ParseFromFile(filename);
			return *(parseMaster.GetSharedData()->As<TableSharedData>()->GetCurrentScope()["Worlds"][0].As<World>());
		}

		TEST_METHOD_INITIALIZE(PrepForTests)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(CleanupTestObjects)
		{
#if defined(DEBUG) | defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(ReactionTestParseFromFile)
		{
			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			GameClock gameClock;
			GameTime gameTime;

			World& world = GetWorldFromXML(parseMaster, "Content\\gamefile_reaction.xml");
			Assert::AreEqual(1U, world.Reactions().Size());

			Assert::AreEqual(4U, Event<EventMessageAttributed>::Subscribers().Size());

			ReactionAttributed aReaction;
			Event<EventMessageAttributed>::Subscribe(aReaction);
			Assert::IsFalse(aReaction.IsAttribute("AuxiliaryString"));
			Assert::IsFalse(aReaction.IsAttribute("AuxiliaryFloat"));

			world.GetWorldState().mSector = world.Sectors()[0].As<Sector>();
			world.GetWorldState().SetGameTime(gameTime);

			gameClock.Reset();

			for (int i = 0; i < 2; i++)
			{
				world.Update(gameClock);
			}

			EventQueue& eventQueue = world.GetEventQueue();

			Assert::AreEqual(1U, eventQueue.Size());

			Assert::IsTrue(aReaction.IsAttribute("AuxiliaryString"));
			Assert::IsTrue(aReaction["AuxiliaryString"].Get<std::string>() == "Hello");
			Assert::IsTrue(aReaction.IsAttribute("AuxiliaryFloat"));
			Assert::AreEqual(aReaction["AuxiliaryFloat"].Get<float>(), 12.34f);

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ReactionTestUpdate)
		{
			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			GameClock gameClock;
			gameClock.Reset();

			GameTime gameTime;

			World& world = GetWorldFromXML(parseMaster, "Content\\gamefile_reaction.xml");
			world.GetWorldState().mSector = world.Sectors()[0].As<Sector>();
			world.GetWorldState().SetGameTime(gameTime);

			for (int i = 0; i < 5000; i++)
			{
				world.Update(gameClock);
			}

			Assert::IsTrue(world.GetWorldState().mSector == world.Sectors()[1].As<Sector>());
			Assert::IsTrue(world.GetEventQueue().IsEmpty());

			Event<EventMessageAttributed>::UnsubscribeAll();
		}

		TEST_METHOD(ReactionTestEventMessageAttributedRTTI)
		{
			EventMessageAttributed aEventMessage;
			RTTI* testRTTI = &aEventMessage;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("EventMessageAttributed"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			EventMessageAttributed* otherWorld = testRTTI->As<EventMessageAttributed>();
			Assert::IsTrue(otherWorld != nullptr);
			Assert::IsTrue(&aEventMessage == otherWorld);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(EventMessageAttributed::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(EventMessageAttributed::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(ReactionTestReactionAttributedRTTI)
		{
			ReactionAttributed aReaction;
			RTTI* testRTTI = &aReaction;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ReactionAttributed"));
			Assert::IsTrue(testRTTI->Is("Reaction"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			ReactionAttributed* otherWorld = testRTTI->As<ReactionAttributed>();
			Assert::IsTrue(otherWorld != nullptr);
			Assert::IsTrue(&aReaction == otherWorld);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Reaction* basePtr = testRTTI->As<Reaction>();
			Assert::IsTrue(basePtr != nullptr);

			Assert::IsTrue(testRTTI->Is(ReactionAttributed::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(Reaction::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(ReactionAttributed::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			otherTestRTTI = testRTTI->QueryInterface(Reaction::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(ReactionTestActionEventRTTI)
		{
			ActionEvent aEvent;
			RTTI* testRTTI = &aEvent;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("ActionEvent"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			ActionEvent* otherWorld = testRTTI->As<ActionEvent>();
			Assert::IsTrue(otherWorld != nullptr);
			Assert::IsTrue(&aEvent == otherWorld);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(ActionEvent::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(ActionEvent::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState ReactionTest::startMemState;
#endif
}