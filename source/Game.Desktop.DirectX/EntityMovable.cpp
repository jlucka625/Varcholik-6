#include "pch.h"
#include "EntityMovable.h"

const std::string EntityMovable::SPEED_X_ATTRIBUTE = "SpeedX";
const std::string EntityMovable::SPEED_Y_ATTRIBUTE = "SpeedY";
const std::string EntityMovable::IS_POSITIVE_X_ATTRIBUTE = "IsPositiveX";
const std::string EntityMovable::IS_POSITIVE_Y_ATTRIBUTE = "IsPositiveY";
const std::string EntityMovable::IS_HARMFUL_ATTRIBUTE = "IsHarmful";
const std::string EntityMovable::MIN_MAX_ATTRIBUTE = "MinMaxData";

RTTI_DEFINITIONS(EntityMovable);

EntityMovable::EntityMovable() : mSprite(), mIsPositiveX(true), mIsPositiveY(true)
{
	AddExternalAttribute(SPEED_X_ATTRIBUTE, 1, &mSpeedX);
	AddExternalAttribute(SPEED_Y_ATTRIBUTE, 1, &mSpeedY);
	AddExternalAttribute(IS_POSITIVE_X_ATTRIBUTE, 1, &mIsPositiveX);
	AddExternalAttribute(IS_POSITIVE_Y_ATTRIBUTE, 1, &mIsPositiveY);
	AddExternalAttribute(IS_HARMFUL_ATTRIBUTE, 1, &mIsHarmful);
	AddExternalAttribute(MIN_MAX_ATTRIBUTE, 1, &mMinMaxData);
}

Sprite & EntityMovable::GetSprite() const
{
	return const_cast<Sprite&>(mSprite);
}


void EntityMovable::Update(WorldState & worldState)
{
	Entity::Update(worldState);
	Sector& level = *worldState.mSector;
	Datum& entities = *level.Find("Entities");
	for (std::uint32_t i = 0; i < entities.Size(); ++i)
	{
		EntityTile* tile = entities.Get<Scope*>(i)->As<EntityTile>();
		if (tile && tile->Name() == "Barrier")
		{
			if (mSprite.IsColliding(tile->GetSprite()))
			{
				mIsPositiveX = !mIsPositiveX;
				mIsPositiveY = !mIsPositiveY;
				break;
			}
		}
	}

	//Now update positions
	if (mSpeedX != 0.0f)
		if (mIsPositiveX)
		{
			mSprite.SetPosition(glm::vec2(mSprite.GetPosition().x + (worldState.deltaTime * mSpeedX), mSprite.GetPosition().y));
		}
		else
		{
			mSprite.SetPosition(glm::vec2(mSprite.GetPosition().x - (worldState.deltaTime * mSpeedX), mSprite.GetPosition().y));
		}

	if (mSpeedY != 0.0f)
		if (mIsPositiveY)
		{
			mSprite.SetPosition(glm::vec2(mSprite.GetPosition().x, mSprite.GetPosition().y + (worldState.deltaTime * mSpeedY)));
		}
		else
		{
			mSprite.SetPosition(glm::vec2(mSprite.GetPosition().x, mSprite.GetPosition().y - (worldState.deltaTime * mSpeedY)));
		}
}

void EntityMovable::Draw()
{
	mSprite.Draw();
}

void EntityMovable::CreateSprite(const std::string & textureName, float u, float v, float width, float height)
{
	mWidth = width;
	mHeight = height;

	mSprite.SetTextureName(std::wstring(textureName.begin(), textureName.end()));
	mSprite.SetFramePosition(glm::vec2(u, v));
	mSprite.SetSize(glm::vec2(width, height));
	mSprite.Initialize();
}

glm::vec2 EntityMovable::Speed() const
{
	return glm::vec2(mSpeedX, mSpeedY);
}

void EntityMovable::SetSpeed(float speedX, float speedY)
{
	mSpeedX = speedX;
	mSpeedY = speedY;
}

glm::vec4 EntityMovable::MinMaxData() const
{
	return mMinMaxData;
}

void EntityMovable::SetMinMaxData(float minX, float maxX, float minY, float maxY)
{
	mMinMaxData = glm::vec4(minX, maxX, minY, maxY);
}

bool EntityMovable::IsPositiveX() const
{
	return (mIsPositiveX != 0);
}

void EntityMovable::SetIsPositiveX(int isPositive)
{
	mIsPositiveX = isPositive;
}

bool EntityMovable::IsPositiveY() const
{
	return (mIsPositiveY != 0);
}

void EntityMovable::SetIsPositiveY(int isPositive)
{
	mIsPositiveY = isPositive;
}

bool EntityMovable::IsHarmful() const
{
	return (mIsHarmful != 0);
}

void EntityMovable::SetIsHarmful(int isHarmful)
{
	mIsHarmful = isHarmful;
}

void EntityMovable::ChangeAnimation(const std::string& state)
{
	mSprite.SetAnimationState(state);
}


