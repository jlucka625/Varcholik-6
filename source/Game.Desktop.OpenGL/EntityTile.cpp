#include "pch.h"

RTTI_DEFINITIONS(EntityTile)
using namespace LibraryDesktop;

const std::string EntityTile::IS_HARMFUL_ATTRIBUTE = "IsHarmful";
const std::string EntityTile::IS_CHECKPOINT_ATTRIBUTE = "IsCheckpoint";
const std::string EntityTile::IS_GRAVITY_FLIP_ATTRIBUTE = "IsGravityFlip";

EntityTile::EntityTile() : mSprite(nullptr), mSpriteRenderer(ResourceManager::GetShader("SpriteShader")), mIsHarmful(false), mIsCheckpoint(false), mIsGravityFlip(false), keepStill(false), originalPosition(glm::vec2())
{
	AddExternalAttribute(IS_HARMFUL_ATTRIBUTE, 1, &mIsHarmful);
	AddExternalAttribute(IS_CHECKPOINT_ATTRIBUTE, 1, &mIsCheckpoint);
	AddExternalAttribute(IS_GRAVITY_FLIP_ATTRIBUTE, 1, &mIsGravityFlip);
}

EntityTile::~EntityTile()
{
	if (mSprite)
	{
		delete mSprite;
	}
}

void EntityTile::CreateSprite(const std::string& textureName, float u, float v, float width, float height)
{
	if (!mSprite)
	{
		mWidth = width;
		mHeight = height;

		mSprite = new Sprite();
		mSprite->SetTexture(ResourceManager::GetTexture(textureName));
		mSprite->SetSize(glm::vec2(width, height));
		mSprite->SetFramePosition(glm::vec2(u, v));
	}
}

Sprite & EntityTile::GetSprite()
{
	if (mSprite == nullptr)
	{
		throw std::exception("This object needs a sprite");
	}

	return *mSprite;
}

void EntityTile::Update(WorldState & worldState)
{
	Entity::Update(worldState);

	//if (mName == "Barrier")
	//{
	//	if (keepStill)
	//	{
	//		mBody->SetType(b2_staticBody);
	//	}
	//	else
	//	{
	//		mBody->SetType(b2_dynamicBody);
	//	}
	//}
	//mSprite->Update((float)worldState.GetGameTime().ElapsedGameTime().count());
}

void EntityTile::Draw()
{
	if (mSprite && mSprite->IsVisible())
	{
		mSpriteRenderer.DrawSprite(*mSprite);
	}	
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

void EntityTile::ToggleStillness()
{
	keepStill = !keepStill;
}
