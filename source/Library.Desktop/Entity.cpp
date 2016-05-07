#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(Entity)

const std::string Entity::NAME_ATTRIBUTE = "Name";

Entity::Entity() : ActionContainer(), mName()
{
	AddExternalAttribute(NAME_ATTRIBUTE, 1, &mName);
}

const std::string& Entity::Name() const
{
	return mName;
}

void Entity::SetName(const std::string & name)
{
	mName = name;
}

const Sector* Entity::GetSector() const
{
	assert(mParent->Is(Sector::TypeIdClass()));
	return static_cast<Sector*>(mParent);
}

void Entity::SetSector(Sector* sector)
{
	assert(sector != nullptr);
	sector->Adopt(*this, ENTITY_CONTAINER);
}

Action* Entity::CreateAction(const std::string& instanceName, const std::string & className, const std::string& parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Action::TypeIdClass()));

	Action* action = static_cast<Action*>(r);
	action->SetName(instanceName);
	Adopt(*action, parentName);

	return action;
}

Reaction* Entity::CreateReaction(const std::string & instanceName, const std::string & className, const std::string& parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Reaction::TypeIdClass()));

	Reaction* reaction = static_cast<Reaction*>(r);
	reaction->SetName(instanceName);
	Adopt(*reaction, parentName);

	return reaction;
}

void Entity::Update(WorldState& worldState)
{
	worldState.mEntity = this;

	Datum& actionsDatum = Actions();

	for (std::uint32_t i = 0; i < actionsDatum.Size(); i++)
	{
		Action* action = actionsDatum[i].As<Action>();
		assert(action != nullptr);
		action->Update(worldState);
	}
}
