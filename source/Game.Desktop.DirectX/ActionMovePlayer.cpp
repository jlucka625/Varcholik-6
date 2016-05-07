#include "pch.h"
#include "ActionMovePlayer.h"

RTTI_DEFINITIONS(ActionMovePlayer)

const  std::string ActionMovePlayer::TARGET_ATTRIBUTE_NAME = "Target";
const std::string ActionMovePlayer::DIRECTION_ATTRIBUTE_NAME = "Direction";


ActionMovePlayer::ActionMovePlayer()
{
	AddExternalAttribute(DIRECTION_ATTRIBUTE_NAME, 1, &mDirection);
	AddExternalAttribute(TARGET_ATTRIBUTE_NAME, 1, &mTarget);

}

void ActionMovePlayer::Update(WorldState & worldState)
{
	worldState.mAction = this;

	if (mTarget == "")
	{
		throw std::exception("Error: you must provide the name of the target value you wish to modify.");
	}
	if (mDirection == "")
	{
		throw std::exception("Error: you must provide the name of the target value you wish to modify.");
	}
	if (mDirection != "Left" && mDirection != "Right" && mDirection != "LeftStop" && mDirection != "RightStop")
	{
		throw std::exception("Error: valid directions are 'Left' or 'Right'.  Check spelling.");
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
	if (mDirection == "LeftStop" || mDirection == "RightStop")
	{
		player->StopPlayer();
	}

	//direction indicates left or right movement.  If left then set to 1, if right set to -1
	std::int8_t direction = 0;
	if (mDirection == "Left")
	{
		player->GetSprite().SetIsXFlipped(true);
	}
	else if (mDirection == "Right")
	{
		player->GetSprite().SetIsXFlipped(false);
	}

	if (mDirection == "Left")
	{
		direction = -1;
		player->SetLeftPressed(true);
	}
	else if (mDirection == "RightStop")
	{
		direction = -1;
		player->SetRightPressed(false);
	}
	else if (mDirection == "Right")
	{
		direction = 1;
		player->SetRightPressed(true);
	}
	else if (mDirection == "LeftStop")
	{
		direction = 1;
		player->SetLeftPressed(false);
	}

	// Prevents player movement from being added or negated after level change
	if ((mDirection == "RightStop" || mDirection == "LeftStop") && abs(player->GetCurrentSpeed()) <= 0.01f && (!player->GetRightPressed() && !player->GetLeftPressed()))
	{
		return;
	}

	player->MovePlayer(direction);

}





