#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(Action)

const std::string Action::NAME_ATTRIBUTE = "Name";

Action::Action() : ActionContainer(), mName()
{
	AddExternalAttribute(NAME_ATTRIBUTE, 1, &mName);
}

const std::string& Action::Name() const
{
	return mName;
}

void Action::SetName(const std::string & name)
{
	mName = name;
}

Action* Action::CreateAction(const std::string & instanceName, const std::string & className, const std::string & parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Action::TypeIdClass()));

	Action* action = static_cast<Action*>(r);
	action->SetName(instanceName);
	Adopt(*action, parentName);

	return action;
}

Reaction* Action::CreateReaction(const std::string & instanceName, const std::string & className, const std::string& parentName)
{
	RTTI* r = Factory<RTTI>::Create(className);
	assert(r != nullptr);
	assert(r->Is(Reaction::TypeIdClass()));

	Reaction* reaction = static_cast<Reaction*>(r);
	reaction->SetName(instanceName);
	Adopt(*reaction, parentName);

	return reaction;
}
