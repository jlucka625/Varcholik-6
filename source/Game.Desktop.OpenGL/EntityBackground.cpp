#include "pch.h"
#include "EntityBackground.h"
#include "WorldState.h"

RTTI_DEFINITIONS(EntityBackground);

const std::string EntityBackground::SCROLL_X_ATTRIBUTE = "ScrollSpeedX";
const std::string EntityBackground::SCROLL_Y_ATTRIBUTE = "ScrollSpeedY";


EntityBackground::EntityBackground() : mSprite(), mSpriteRenderer(ResourceManager::GetShader("SpriteShader"))
{
	AddExternalAttribute(SCROLL_X_ATTRIBUTE, 1, &mScrollSpeedX);
	AddExternalAttribute(SCROLL_Y_ATTRIBUTE, 1, &mScrollSpeedY);
}

Sprite & EntityBackground::GetSprite()
{
	return mSprite;
}

void EntityBackground::CreateSprite(const std::string & textureName, float u, float v, float width, float height)
{
	mSprite.SetTexture(ResourceManager::GetTexture(textureName));
	mSprite.SetFramePosition(glm::vec2(u, v));
	mSprite.SetSize(glm::vec2(width, height));
}

void EntityBackground::Update(WorldState & worldState)
{
	mSprite.Update(worldState.deltaTime);
}

void EntityBackground::Draw()
{
	mSpriteRenderer.DrawSprite(mSprite);
}


glm::vec2 EntityBackground::ScrollSpeed() const
{
	return mSprite.GetTranslationSpeed();
}

void EntityBackground::SetScrollSpeed(float x, float y)
{
	mScrollSpeedX = x;
	mScrollSpeedY = y;
	mSprite.SetTranslationSpeed(glm::vec2(x, y));

}
