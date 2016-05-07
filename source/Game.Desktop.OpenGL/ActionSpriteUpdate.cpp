#include "pch.h"
#include "ActionSpriteUpdate.h"
#include "EntityPlayer.h"
#include "EntityTile.h"
#include "EntityMovable.h"
#include "EntityBackground.h"

RTTI_DEFINITIONS(ActionSpriteUpdate);

ActionSpriteUpdate::ActionSpriteUpdate() : Action()
{
}

void ActionSpriteUpdate::Update(WorldState & worldState)
{
	if (worldState.mEntity->Is(EntityPlayer::TypeIdClass()))
	{
		reinterpret_cast<EntityPlayer*>(worldState.mEntity)->GetSprite().Update(worldState.deltaTime);
	}
	/*else if (worldState.mEntity->Is(EntityTile::TypeIdClass()))
	{
		reinterpret_cast<EntityTile*>(worldState.mEntity)->GetSprite().Update(worldState.deltaTime);
	}*/
	else if (worldState.mEntity->Is(EntityMovable::TypeIdClass()))
	{
		reinterpret_cast<EntityMovable*>(worldState.mEntity)->GetSprite().Update(worldState.deltaTime);
	}
	else if (worldState.mEntity->Is(EntityBackground::TypeIdClass()))
	{
		reinterpret_cast<EntityBackground*>(worldState.mEntity)->GetSprite().Update(worldState.deltaTime);
	}

}


