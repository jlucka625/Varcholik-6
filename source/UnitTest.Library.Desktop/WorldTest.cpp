#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(RTTI, Enemy)
	ConcreteFactory(RTTI, Entity)

	TEST_CLASS(WorldTest)
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
		
		TEST_METHOD(WorldTestParseFromFile)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;

			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			IntegerXmlParseHelper intParseHelper;
			FloatXmlParseHelper floatParseHelper;
			VectorXmlParseHelper vectorParseHelper;
			WorldXmlParseHelper worldParseHelper;
			SectorXmlParseHelper sectorParseHelper;
			EntityXmlParseHelper entityParseHelper;

			parseMaster.AddHelper(intParseHelper);
			parseMaster.AddHelper(floatParseHelper);
			parseMaster.AddHelper(sectorParseHelper);
			parseMaster.AddHelper(vectorParseHelper);
			parseMaster.AddHelper(entityParseHelper);
			parseMaster.AddHelper(worldParseHelper);

			parseMaster.ParseFromFile("Content\\gamefile.xml");

			Datum& worlds = GetWorlds(parseMaster);
			Assert::AreEqual(1U, worlds.Size());

			World* world = worlds[0].As<World>();
			Assert::IsTrue(world != nullptr);
			Assert::IsTrue(world->IsAuxiliaryAttribute("AuxiliaryInt"));
			Assert::AreEqual(world->operator[]("AuxiliaryInt").Get<int>(), 55);

			Datum& sectors = world->Sectors();
			Assert::AreEqual(2U, sectors.Size());

			Sector* sector1 = sectors[0].As<Sector>();
			Assert::IsTrue(sector1 != nullptr);
			Assert::IsTrue(sector1->GetWorld() == world);
			Assert::IsTrue(sector1->IsAuxiliaryAttribute("AuxiliaryFloat"));
			Assert::AreEqual(sector1->operator[]("AuxiliaryFloat").Get<float>(), 56.56f);

			Sector* sector2 = sectors[1].As<Sector>();
			Assert::IsTrue(sector2 != nullptr);
			Assert::IsTrue(sector2->GetWorld() == world);

			Datum& entities = sector1->Entities();
			Assert::AreEqual(2U, entities.Size());

			Assert::IsTrue(entities[0].Is("Enemy"));
			Assert::IsTrue(entities[0].As<Entity>()->GetSector() == sectors[0].As<Sector>());
			Enemy& defaultEnemy = *(entities[0].As<Enemy>());
			Assert::AreEqual(defaultEnemy["Health"].Get<int>(), 100);
			Assert::IsTrue(defaultEnemy["Position"].Get<glm::vec4>() == glm::vec4(0, 0, 0, 0));
			Assert::AreEqual(defaultEnemy["DamageModifier"].Get<float>(), 1.0f);

			Assert::IsTrue(entities[1].Is("Enemy"));
			Assert::IsTrue(entities[1].As<Entity>()->GetSector() == sectors[0].As<Sector>());
			Enemy& heavyEnemy = *(entities[1].As<Enemy>());
			Assert::AreEqual(heavyEnemy["Health"].Get<int>(), 200);
			Assert::IsTrue(heavyEnemy["Position"].Get<glm::vec4>() == glm::vec4(10, 20, 30, 40));
			Assert::AreEqual(heavyEnemy["DamageModifier"].Get<float>(), 2.0f);

			Datum& entities2 = sector2->Entities();
			Assert::AreEqual(2U, entities.Size());

			Assert::IsTrue(entities2[0].Is("Enemy"));
			Assert::IsTrue(entities2[0].As<Entity>()->GetSector() == sectors[1].As<Sector>());
			Enemy& flyingEnemy = *(entities2[0].As<Enemy>());
			Assert::IsTrue(flyingEnemy.IsAuxiliaryAttribute("Weight"));
			Assert::AreEqual(flyingEnemy["Weight"].Get<float>(), 0.05f);

			Assert::IsFalse(entities2[1].Is("Enemy"));
			Assert::IsTrue(entities2[1].Is("Entity"));
		}

		TEST_METHOD(WorldTestInvalidParsingEntity)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;

			TableSharedData sharedData;
			XmlParseMaster parseMaster(sharedData);

			WorldXmlParseHelper worldHelper;
			parseMaster.AddHelper(worldHelper);

			SectorXmlParseHelper sectorHelper;
			parseMaster.AddHelper(sectorHelper);

			EntityXmlParseHelper entityHelper;
			parseMaster.AddHelper(entityHelper);

			std::string missingEntityClass = "<Universe><World name=\"Overworld\"><Sector name=\"Level1\"><Entity name=\"DefaultEnemy\"></Entity></Sector></World></Universe>";

			Assert::ExpectException<std::exception>([&] { parseMaster.Parse(missingEntityClass.c_str(), static_cast<int>(missingEntityClass.length()), true, true); });
		}

		TEST_METHOD(WorldTestNaming)
		{
			//World test
			World world(b2Vec2_zero);
			world.SetName("Overworld");
			Assert::IsTrue(world.Name() == "Overworld");

			world.SetName("NewWorldOrder");
			Assert::IsTrue(world.Name() != "Overworld");
			Assert::IsTrue(world.Name() == "NewWorldOrder");

			//Sector test
			Sector* sector = world.CreateSector("Sector1");
			Assert::IsTrue(sector->Name() == "Sector1");

			sector->SetName("SectorNumeroUno");
			Sector& sector1Ref = *(world.Sectors()[0].As<Sector>());
			Assert::IsTrue(sector1Ref["Name"] != "Sector1");
			Assert::IsTrue(sector1Ref["Name"] == "SectorNumeroUno");

			//Entity test
			EntityFactory entityFactory;

			Entity* entity = sector->CreateEntity("SomeEntity", "Entity");
			Assert::IsTrue(entity->Name() == "SomeEntity");

			entity->SetName("EnemyDefault");
			Assert::IsTrue(entity->Name() != "SomeEntity");
			Assert::IsTrue(entity->Name() == "EnemyDefault");
		}

		TEST_METHOD(WorldTestUpdate)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;

			GameClock gameClock;
			GameTime gameTime;

			World world(b2Vec2_zero);

			WorldState& worldState = world.GetWorldState();
			Sector* sector = new Sector();
			worldState.mSector = sector;
			worldState.SetGameTime(gameTime);

			gameClock.Reset();
			world.Update(gameClock);
			delete sector;
		}

		TEST_METHOD(WorldTestWorldRTTI)
		{
			World world(b2Vec2_zero);
			RTTI* testRTTI = &world;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("World"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			World* otherWorld = testRTTI->As<World>();
			Assert::IsTrue(otherWorld != nullptr);
			Assert::IsTrue(&world == otherWorld);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(World::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(World::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(WorldTestSectorRTTI)
		{
			Sector sector;
			RTTI* testRTTI = &sector;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("Sector"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			Sector* otherSector = testRTTI->As<Sector>();
			Assert::IsTrue(otherSector != nullptr);
			Assert::IsTrue(&sector == otherSector);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(Sector::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(Sector::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);
		}

		TEST_METHOD(WorldTestEntityRTTI)
		{
			EntityFactory entityFactory;
			EnemyFactory enemyFactory;

			World world(b2Vec2_zero);
			Sector* sector = world.CreateSector("Level1");

			//Entity test
			Entity* entity = sector->CreateEntity("SomeEntity", "Entity");

			RTTI* testRTTI = entity;

			Assert::IsFalse(testRTTI->Is(44));
			Assert::IsTrue(testRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testRTTI->Is("Unknown"));
			Assert::IsTrue(testRTTI->Is("Entity"));

			Foo* dummyPointer = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer == nullptr);

			Entity* otherEntity = testRTTI->As<Entity>();
			Assert::IsTrue(otherEntity != nullptr);
			Assert::IsTrue(entity == otherEntity);
			Assert::IsTrue(testRTTI->ToString() == "RTTI");

			Assert::IsTrue(testRTTI->Is(Entity::TypeIdClass()));
			Assert::IsTrue(testRTTI->Is(testRTTI->TypeIdInstance()));

			RTTI* otherTestRTTI = testRTTI->QueryInterface(Entity::TypeIdClass());
			Assert::IsTrue(otherTestRTTI != nullptr);

			//Enemy test
			RTTI* testEnemyRTTI = sector->CreateEntity("SomeEnemy", "Enemy");

			Assert::IsFalse(testEnemyRTTI->Is(44));
			Assert::IsTrue(testEnemyRTTI->QueryInterface(44) == nullptr);

			Assert::IsFalse(testEnemyRTTI->Is("Unknown"));
			Assert::IsTrue(testEnemyRTTI->Is("Enemy"));

			Foo* dummyPointer2 = testRTTI->As<Foo>();
			Assert::IsTrue(dummyPointer2 == nullptr);

			Enemy* enemy = testEnemyRTTI->As<Enemy>();
			Assert::IsTrue(enemy != nullptr);
			Assert::IsTrue(testEnemyRTTI->ToString() == "RTTI");

			Assert::IsTrue(testEnemyRTTI->Is(Enemy::TypeIdClass()));
			Assert::IsTrue(testEnemyRTTI->Is(testEnemyRTTI->TypeIdInstance()));

			RTTI* otherTestEnemyRTTI = testEnemyRTTI->QueryInterface(Enemy::TypeIdClass());
			Assert::IsTrue(otherTestEnemyRTTI != nullptr);
		}

	private:
#if defined(DEBUG) | defined(_DEBUG)
		static _CrtMemState startMemState;
#endif
	};

#if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState WorldTest::startMemState;
#endif
}