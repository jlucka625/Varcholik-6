#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(World)

const std::string World::NAME_ATTRIBUTE = "Name";
const std::string World::LEVEL_ATTRIBUTE_NAME = "CurrentLevel";

World::World(const b2Vec2& gravity) : ActionContainer(), mName(), mLevel(0), mWorldState(), mEventQueue(), b2DWorld(new b2World(gravity))
{
	AddExternalAttribute(NAME_ATTRIBUTE, 1, &mName);
	AddExternalAttribute(LEVEL_ATTRIBUTE_NAME, 1, &mLevel);
	AddNestedScopeAttribute(SECTOR_CONTAINER);
	mWorldState.mWorld = this;
}

World::~World()
{
	delete b2DWorld;
}

const std::string& World::Name() const
{
	return mName;
}

void World::SetName(const std::string & name)
{
	mName = name;
}

WorldState& World::GetWorldState()
{
	return mWorldState;
}

EventQueue& World::GetEventQueue()
{
	return mEventQueue;
}

b2World* World::GetB2DWorld() const
{
	return b2DWorld;
}

void World::FlipGravity()
{
	b2Vec2 gravity = b2DWorld->GetGravity();
	b2DWorld->SetGravity(b2Vec2(gravity.x, (gravity.y * -1)));
}

Action* World::CreateAction(const std::string & instanceName, const std::string & className, const std::string & parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Action::TypeIdClass()));

	Action* action = static_cast<Action*>(r);
	action->SetName(instanceName);
	Adopt(*action, parentName);

	return action;
}

Reaction* World::CreateReaction(const std::string & instanceName, const std::string & className, const std::string& parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Reaction::TypeIdClass()));

	Reaction* reaction = static_cast<Reaction*>(r);
	reaction->SetName(instanceName);
	Adopt(*reaction, parentName);

	return reaction;
}

Datum& World::Sectors()
{
	return (*this)[SECTOR_CONTAINER];
}

Sector* World::CreateSector(const std::string& name)
{
	Sector* sector = new Sector();
	sector->SetName(name);
	sector->SetWorld(this);
	return sector;
}

void World::Update(GameClock& gameClock)
{
	mWorldState.mWorld = this;
	GameTime gameTime = mWorldState.GetGameTime();
	gameClock.UpdateGameTime(gameTime);
	b2DWorld->Step(TIME_STEP, VELOCITY_STEP_RATE, POSITION_STEP_RATE);

	mEventQueue.Update(gameTime);

	Datum& actionsDatum = Actions();

	for (std::uint32_t i = 0; i < actionsDatum.Size(); i++)
	{
		Action* action = actionsDatum[i].As<Action>();
		assert(action != nullptr);
		action->Update(mWorldState);
	}

	Datum& sectorsDatum = Sectors();

	for (std::uint32_t i = 0; i < sectorsDatum.Size(); i++)
	{
		Sector* sector = sectorsDatum[i].As<Sector>();
		assert(sector != nullptr);
		sector->Update(mWorldState);
	}
	//assert(mWorldState.mSector != nullptr);
	//mWorldState.mSector->Update(mWorldState);
}
