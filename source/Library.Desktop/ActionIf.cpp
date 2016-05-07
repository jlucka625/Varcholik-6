#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(ActionIf)

const std::string ActionIf::CONDITION_ATTRIBUTE_NAME = "Condition";
const std::string ActionIf::THEN_ATTRIBUTE_NAME = "Then";
const std::string ActionIf::ELSE_ATTRIBUTE_NAME = "Else";

ActionIf::ActionIf() : Action(), mCondition(0)
{
	AddExternalAttribute(CONDITION_ATTRIBUTE_NAME, 1, &mCondition);
	AddNestedScopeAttribute(THEN_ATTRIBUTE_NAME);
	AddNestedScopeAttribute(ELSE_ATTRIBUTE_NAME);
}

void ActionIf::Update(class WorldState& worldState)
{
	worldState.mAction = this;

	Datum& thenBlock = (*this)[THEN_ATTRIBUTE_NAME];
	Datum& elseBlock = (*this)[ELSE_ATTRIBUTE_NAME];

	if (thenBlock.Size() <= 0)
	{
		throw std::exception("Error: cannot have an empty then block");
	}

	assert(thenBlock[0].Is(Action::TypeIdClass()));

	if (mCondition != 0)
	{
		static_cast<Action*>(&thenBlock[0])->Update(worldState);
	}
	else if (elseBlock.Size() > 0)
	{
		assert(elseBlock[0].Is(Action::TypeIdClass()));
		static_cast<Action*>(&elseBlock[0])->Update(worldState);
	}
}
