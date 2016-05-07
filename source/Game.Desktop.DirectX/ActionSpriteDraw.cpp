#include "pch.h"
#include "ActionSpriteDraw.h"
#include "WorldState.h"
#include "EntityPlayer.h"
#include "EntityMovable.h"
#include "EntityTile.h"
#include "EntityBackground.h"
#include "EntityText.h"

RTTI_DEFINITIONS(ActionSpriteDraw);

ActionSpriteDraw::ActionSpriteDraw() : Action()
{
}

void ActionSpriteDraw::Update(WorldState & worldState)
{
	assert(worldState.mEntity != nullptr);

	if (worldState.mEntity->Is(EntityPlayer::TypeIdClass()))
	{
		reinterpret_cast<EntityPlayer*>(worldState.mEntity)->Draw();
	}
	else if (worldState.mEntity->Is(EntityTile::TypeIdClass()))
	{
		reinterpret_cast<EntityTile*>(worldState.mEntity)->Draw();
	}
	else if (worldState.mEntity->Is(EntityMovable::TypeIdClass()))
	{
		reinterpret_cast<EntityMovable*>(worldState.mEntity)->Draw();
	}

}



