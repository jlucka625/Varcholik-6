#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(ActionLoadSector)

const std::string ActionLoadSector::LEVEL_ATTRIBUTE_NAME = "CurrentLevel";

ActionLoadSector::ActionLoadSector() : Action()
{
	
}

void ActionLoadSector::Update(class WorldState& worldState)
{
	worldState.mAction = this;
	int mLevel = (*worldState.mWorld)[LEVEL_ATTRIBUTE_NAME].Get<int>();

	Datum& sectors = worldState.mWorld->Sectors();

	if ((mLevel < 0) || (static_cast<std::uint32_t>(mLevel) >= sectors.Size()))
	{
		throw std::exception("Error: out-of-bounds sector ID");
	}

	Scope& sector = sectors[mLevel];
	assert(sector.Is(Sector::TypeIdClass()));
	worldState.mSector = static_cast<Sector*>(&sector);
}
