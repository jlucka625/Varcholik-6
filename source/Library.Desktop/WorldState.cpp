#include "pch.h"
#include "WorldState.h"

using namespace LibraryDesktop;

WorldState::WorldState() : mGameTime(nullptr), mEntity(nullptr), mSector(nullptr), mWorld(nullptr), mAction(nullptr), mUnloadSector(false), mReloadSector(false)
{
}

GameTime& WorldState::GetGameTime()
{
	assert(mGameTime != nullptr);
	return *mGameTime;
}

void WorldState::SetGameTime(GameTime& gameTime)
{
	mGameTime = &gameTime;
}

bool WorldState::GetUnloadSector()
{
	return mUnloadSector;
}

void WorldState::SetUnloadSector(bool unload)
{
	mUnloadSector = unload;
}

bool WorldState::GetReloadSector()
{
	return mReloadSector;
}

void WorldState::SetReloadSector(bool reload)
{
	mReloadSector = reload;
}