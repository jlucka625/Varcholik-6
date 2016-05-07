#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(ActionUnloadSector)

void ActionUnloadSector::Update(class WorldState& worldState)
{
	worldState.mAction = this;

	assert(worldState.mSector != nullptr);

	worldState.mSector = nullptr;
}
