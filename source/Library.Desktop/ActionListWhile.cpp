#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(ActionListWhile)

const std::string ActionListWhile::CONDITION_ATTRIBUTE_NAME = "Condition";

ActionListWhile::ActionListWhile() : ActionList(), mCondition(0)
{
	AddExternalAttribute(CONDITION_ATTRIBUTE_NAME, 1, &mCondition);
}

void ActionListWhile::Update(WorldState& worldState)
{
	worldState.mAction = this;

	while (mCondition)
	{
		Datum& actionsDatum = Actions();

		for (std::uint32_t i = 0; i < actionsDatum.Size(); i++)
		{
			Action* action = actionsDatum[i].As<Action>();
			assert(action != nullptr);
			worldState.mAction = action;
			action->Update(worldState);
		}

		worldState.mAction = this;
	}
}
