#include "pch.h"
#include "ActionFlipPlayer.h"


	RTTI_DEFINITIONS(ActionFlipPlayer)

	const std::string ActionFlipPlayer::TARGET_ATTRIBUTE_NAME = "Target";


	ActionFlipPlayer::ActionFlipPlayer()
	{
		AddExternalAttribute(TARGET_ATTRIBUTE_NAME, 1, &mTarget);
	}


	void ActionFlipPlayer::Update(WorldState & worldState)
	{
		worldState.mAction = this;

		if (mTarget == "")
		{
			throw std::exception("Error: you must provide the name of the target value you wish to modify.");
		}

		
		Scope* grandparent = GetParent()->GetParent();
		if (grandparent == nullptr)
		{
			throw std::exception("Error: Entity is null");
		}

		if (!grandparent->Is(EntityPlayer::TypeIdClass()))
		{
			throw std::exception("Error: Entity found is not of type EntityPlayer");
		}

		EntityPlayer* player = grandparent->As<EntityPlayer>();
		//TODO Invoke EntityPlayer.Flip();
		player->FlipPlayer();
	}
