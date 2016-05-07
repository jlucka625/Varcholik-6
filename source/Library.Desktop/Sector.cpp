#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(Sector)

const std::string Sector::NAME_ATTRIBUTE = "Name";

Sector::Sector() : mName(), ActionContainer()
{
	AddExternalAttribute(NAME_ATTRIBUTE, 1, &mName);
	AddNestedScopeAttribute(ENTITY_CONTAINER);
}

const std::string& Sector::Name() const
{
	return mName;
}

void Sector::SetName(const std::string& name)
{
	mName = name;
}

Datum& Sector::Entities()
{
	return (*this)[ENTITY_CONTAINER];
}

Action* Sector::CreateAction(const std::string & instanceName, const std::string & className, const std::string & parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Action::TypeIdClass()));

	Action* action = static_cast<Action*>(r);
	action->SetName(instanceName);
	Adopt(*action, parentName);

	return action;
}

Reaction* Sector::CreateReaction(const std::string & instanceName, const std::string & className, const std::string& parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Reaction::TypeIdClass()));

	Reaction* reaction = static_cast<Reaction*>(r);
	reaction->SetName(instanceName);
	Adopt(*reaction, parentName);

	return reaction;
}

Entity* Sector::CreateEntity(const std::string& instanceName, const std::string& className)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Entity::TypeIdClass()));

	Entity* entity = static_cast<Entity*>(r);
	entity->SetName(instanceName);
	entity->SetSector(this);
	return entity;
}

const World* Sector::GetWorld() const
{
	assert(mParent->Is(World::TypeIdClass()));
	return static_cast<World*>(mParent);
}

void Sector::SetWorld(World* world)
{
	assert(world != nullptr);
	world->Adopt(*this, SECTOR_CONTAINER);
}

void Sector::Update(WorldState& worldState)
{
	worldState.mSector = this;

	Datum& actionsDatum = Actions();

	for (std::uint32_t i = 0; i < actionsDatum.Size(); i++)
	{
		Action* action = actionsDatum[i].As<Action>();
		assert(action != nullptr);
		action->Update(worldState);
	}

	Datum& entitiesDatum = Entities();

	for (std::uint32_t i = 0; i < entitiesDatum.Size(); i++)
	{
		Entity* entity = entitiesDatum[i].As<Entity>();
		assert(entity != nullptr);
		entity->Update(worldState);
	}
}