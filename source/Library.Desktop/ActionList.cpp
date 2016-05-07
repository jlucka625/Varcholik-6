#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(ActionList)

ActionList::ActionList() : Action()
{
}

void ActionList::Update(WorldState& worldState)
{
	Datum& actionsDatum = Actions();

	for (std::uint32_t i = 0; i < actionsDatum.Size(); i++)
	{
		Action* action = actionsDatum[i].As<Action>();
		assert(action != nullptr);
		worldState.mAction = action;
		action->Update(worldState);
	}
}
