#include "pch.h"

RTTI_DEFINITIONS(EntityTile)
using namespace LibraryDesktop;

const std::string EntityTile::IS_HARMFUL_ATTRIBUTE = "IsHarmful";
const std::string EntityTile::IS_CHECKPOINT_ATTRIBUTE = "IsCheckpoint";
const std::string EntityTile::IS_GRAVITY_FLIP_ATTRIBUTE = "IsGravityFlip";

EntityTile::EntityTile() : mSprite(), mIsHarmful(false), mIsCheckpoint(false), mIsGravityFlip(false), mIsExit(false)
{
	AddExternalAttribute(IS_HARMFUL_ATTRIBUTE, 1, &mIsHarmful);
	AddExternalAttribute(IS_CHECKPOINT_ATTRIBUTE, 1, &mIsCheckpoint);
	AddExternalAttribute(IS_GRAVITY_FLIP_ATTRIBUTE, 1, &mIsGravityFlip);
}

void EntityTile::CreateSprite(const std::string& textureName, float u, float v, float width, float height)
{
	mWidth = width;
	mHeight = height;

	mSprite.SetTextureName(std::wstring(textureName.begin(), textureName.end()));
	mSprite.SetFramePosition(glm::vec2(u, v));
	mSprite.SetSize(glm::vec2(width, height));
	mSprite.Initialize();
}

Sprite & EntityTile::GetSprite() const
{
	return const_cast<Sprite&>(mSprite);
}

void EntityTile::Update(WorldState & worldState)
{
	Entity::Update(worldState);
}

void EntityTile::Draw()
{	
	mSprite.Draw();	
}

bool EntityTile::IsHarmful() const
{
	return (mIsHarmful != 0);
}

void EntityTile::SetIsHarmful(int isHarmful)
{
	mIsHarmful = isHarmful;
}

bool EntityTile::IsCheckpoint() const
{
	return (mIsCheckpoint != 0);
}

void EntityTile::SetIsCheckpoint(int isCheckpoint)
{
	mIsCheckpoint = isCheckpoint;
}

bool EntityTile::IsGravityFlip() const
{
	return (mIsGravityFlip != 0);
}

void EntityTile::SetIsGravityFlip(int isGravityFlip)
{
	mIsGravityFlip = isGravityFlip;
}

bool EntityTile::IsExit()
{
	return mIsExit;
}

void EntityTile::SetExit(bool isExit)
{
	mIsExit = isExit;
}
