#include "pch.h"

RTTI_DEFINITIONS(ActionIncrement)

const std::string ActionIncrement::TARGET_ATTRIBUTE_NAME = "Target";

ActionIncrement::ActionIncrement() : Action(), mTarget()
{
	AddExternalAttribute(TARGET_ATTRIBUTE_NAME, 1, &mTarget);
}

void ActionIncrement::Update(WorldState& worldState)
{
	worldState.mAction = this;

	if (mTarget == "")
	{
		throw std::exception("Error: you must provide the name of the target value you wish to modify");
	}

	Datum* target = Search(mTarget);

	if (target == nullptr)
	{
		throw std::exception("Error: no Datum with this name was found");
	}

	if (target->Type() != Datum::DatumType::Integer)
	{
		throw std::exception("Error: the target of an increment or decrement operation must be of type Integer");
	}

	target->Set((target->Get<int>() + 1));
	//value += 1;
}
