#include "pch.h"
#include "EntityPlayer.h"

RTTI_DEFINITIONS(EntityPlayer);

const std::string EntityPlayer::WIDTH_ATTRIBUTE = "Width";
const std::string EntityPlayer::HEIGHT_ATTRIBUTE = "Height";
const std::string EntityPlayer::TEXTURE_NAME_ATTRIBUTE = "TextureName";
const std::string EntityPlayer::MOVE_SPEED_ATTRIBUTE = "MoveSpeed";
const std::string EntityPlayer::FALL_SPEED_ATTRIBUTE = "FallSpeed";

//Used to account for DirectX speed change
const float SPEED_ADJUSTOR = 7.0f;


EntityPlayer::EntityPlayer() : mHeight(), mWidth(), mTextureName(), mSprite(), mCanFlip(false), mCurrentSpeed(0), mCurrentFallSpeed(0), extraPush(10)
{
	AddExternalAttribute(WIDTH_ATTRIBUTE, 1, &mWidth);
	AddExternalAttribute(HEIGHT_ATTRIBUTE, 1, &mHeight);
	AddExternalAttribute(TEXTURE_NAME_ATTRIBUTE, 1, &mTextureName);
	AddExternalAttribute(MOVE_SPEED_ATTRIBUTE, 1, &mMoveSpeed);
	AddExternalAttribute(FALL_SPEED_ATTRIBUTE, 1, &mFallSpeed);
}

float EntityPlayer::Width() const
{
	return mWidth;
}

void EntityPlayer::SetWidth(float width)
{
	mWidth = width;
	mSprite.SetSize(glm::vec2(mWidth, mHeight));
}

float EntityPlayer::Height() const
{
	return mHeight;
}

void EntityPlayer::SetHeight(float height)
{
	mHeight = height;
	mSprite.SetSize(glm::vec2(mWidth, mHeight));
}

float EntityPlayer::MoveSpeed() const
{
	return mMoveSpeed;
}

void EntityPlayer::SetMoveSpeed(float speed)
{
	mMoveSpeed = speed;
}

float EntityPlayer::GetCurrentSpeed()
{
	return mCurrentSpeed;
}

float EntityPlayer::FallSpeed() const
{
	return mFallSpeed;
}

void EntityPlayer::SetFallSpeed(float speed)
{
	mFallSpeed = speed;
	mCurrentFallSpeed = speed;
}

glm::vec2 EntityPlayer::RespawnPosition() const
{
	return mRespawnPosition;
}

void EntityPlayer::SetRespawnPosition(glm::vec2 position)
{
	mRespawnPosition = position;
}

const std::string & EntityPlayer::TextureName() const
{
	return mTextureName;
}

void EntityPlayer::SetTextureName(const std::string & name)
{
	mTextureName = name;
	mSprite.SetTextureName(std::wstring(name.begin(), name.end()));
}

Sprite & EntityPlayer::GetSprite() const
{
	return const_cast<Sprite&>(mSprite);
}

void EntityPlayer::Draw()
{
	mSprite.Draw();
}

void EntityPlayer::Respawn()
{
	mSprite.SetPosition(mRespawnPosition);
}

void EntityPlayer::CreateSprite(const std::string & textureName, float u, float v, float width, float height)
{
	SetTextureName(textureName);
	mSprite.SetFramePosition(glm::vec2(u, v));
	mSprite.SetSize(glm::vec2(width, height));
	mSprite.Initialize();
}

void EntityPlayer::ChangeAnimation(const std::string& state)
{
	mSprite.SetAnimationState(state);
}

void EntityPlayer::MovePlayer(std::int8_t direction)
{
	mCurrentSpeed += mMoveSpeed * direction;
	if (mCurrentSpeed > 0)
	{
		if (direction > 0)
		{
			mSprite.SetIsXFlipped(false);
		}
		else if (direction < 0)
		{
			mSprite.SetIsXFlipped(true);
		}
	}
	if (abs(mCurrentSpeed) <= 0.01f)
	{
		ChangeAnimation("Idle");
	}
	else
	{
		ChangeAnimation("Walk");
	}
}

void EntityPlayer::StopPlayer()
{
	//do stop player stuff
}

void EntityPlayer::FlipPlayer()
{
	if (mCanFlip)
	{
		mMovable = nullptr;
		mCanFlip = false;
		mFallSpeed = -mFallSpeed;
		extraPush = -extraPush;
		mCurrentFallSpeed = mFallSpeed;
		mSprite.SetPosition(glm::vec2(mSprite.GetPosition().x, mSprite.GetPosition().y + extraPush));
		mSprite.SetIsYFlipped(!mSprite.IsYFlipped());
		GameDesktopShared::SoundManager::GetInstance()->Play("SFX", "Flip");
		GameDesktopShared::SoundManager::GetInstance()->SetVolume("SFX", .5f);
	}
}

void EntityPlayer::Update(WorldState & worldState)
{
	Entity::Update(worldState);

	glm::vec2 pos = mSprite.GetPosition();
	pos.x += mCurrentSpeed * worldState.deltaTime;
	pos.y += mCurrentFallSpeed * worldState.deltaTime;
	mSprite.SetPosition(pos);

	//mSprite.SetPosition(glm::vec2(ConvertMeterToPixel(mBody->GetPosition().x) - (mWidth / 2.0f), ConvertMeterToPixel(mBody->GetPosition().y) - (mHeight / 2.0f)));
	//mBody->SetTransform(b2Vec2(ConvertPixelToMeter(mSprite.GetPosition().x), ConvertPixelToMeter(mSprite.GetPosition().y)), 0.0f);
	bool collided = false;
	bool movablecollided = false;
	bool flipCollided = false;
	Sector& level = *worldState.mSector;
	Datum& entities = *level.Find("Entities");
	for (std::uint32_t i = 0; i < entities.Size(); ++i)
	{
		EntityTile* tile = entities.Get<Scope*>(i)->As<EntityTile>();
		EntityMovable* movable = entities.Get<Scope*>(i)->As<EntityMovable>();
		//bool collided = false;

		if (tile)
		{
			if (mSprite.IsColliding(tile->GetSprite()))
			{
				collided = true;

				std::string tileName = tile->Name();

				if (tileName != "Barrier")
				{
					if (tileName == "Exit")
					{
						worldState.SetUnloadSector(true);
					}
					else if (tile->IsHarmful())
					{
						ChangeAnimation("Death");
						GameDesktopShared::SoundManager::GetInstance()->Play("SFX", "Death");
						GameDesktopShared::SoundManager::GetInstance()->SetVolume("SFX", .5f);
						worldState.SetReloadSector(true);
					}
					else if (tileName == "Flip")
					{
						flipCollided = true;
						if (mFlip == nullptr)
						{
							mFlip = tile;
							mCanFlip = true;
							FlipPlayer();
						}
					}
					else
					{
						//if (!(mSprite.GetPosition().y <= (tile->GetSprite().GetPosition().y + tile->GetSprite().GetSize().y)))
						//if ((mSprite.GetPosition().y < tile->GetSprite().GetPosition().y) && ((mSprite.GetPosition().y + mSprite.GetSize().y) >(tile->GetSprite().GetPosition().y + tile->GetSprite().GetSize().y)))
						//{
						//	//check left
						//	if (mSprite.GetPosition().x < (tile->GetSprite().GetPosition().x + tile->GetSprite().GetSize().x) && (mSprite.GetPosition().x > tile->GetSprite().GetPosition().x))
						//	{
						//		pos = mSprite.GetPosition();
						//		pos.x = (tile->GetSprite().GetPosition().x + tile->GetSprite().GetSize().x);
						//	}
						//	//check right
						//	if ((mSprite.GetPosition().x + mSprite.GetSize().x) >(tile->GetSprite().GetPosition().x) && ((mSprite.GetPosition().x + mSprite.GetSize().x) < (tile->GetSprite().GetPosition().x + tile->GetSprite().GetSize().x)))
						//	{
						//		pos = mSprite.GetPosition();
						//		pos.x = (tile->GetSprite().GetPosition().x) - mSprite.GetSize().x;
						//	}
						//	mSprite.SetPosition(pos);
						//}
						//else
						//{
						//	mCurrentFallSpeed = 0;
						//	mCanFlip = true;
						//}

						mCurrentFallSpeed = 0;
						mCanFlip = true;
					}


					//break;
				}
			}
		}
		else if (movable)
		{
			if (mSprite.IsColliding(movable->GetSprite()))
			{
				if (movable->IsHarmful())
				{
					ChangeAnimation("Death");
					GameDesktopShared::SoundManager::GetInstance()->Play("SFX", "Death");
					GameDesktopShared::SoundManager::GetInstance()->SetVolume("SFX", .5f);
					worldState.SetReloadSector(true);
				}
				collided = true;
				movablecollided = true;
				mMovable = movable;
				mCurrentFallSpeed = 0;
				mCanFlip = true;

				pos = mSprite.GetPosition();



				if (mFallSpeed / abs(mFallSpeed) < 0)
				{
					pos.y = mMovable->GetSprite().GetPosition().y + mMovable->GetSprite().GetSize().y;
				}
				else
				{
					pos.y = mMovable->GetSprite().GetPosition().y - mSprite.GetSize().y;
				}

				mSprite.SetPosition(pos);
			}
			//mSprite.SetPosition(glm::vec2(movable->GetSprite().GetPosition().x, movable->GetSprite().GetPosition().y + movable->GetSprite().GetSize().x
		}
	}

	if (!flipCollided)
	{
		mFlip = nullptr;
	}

	if (!movablecollided && mMovable != nullptr)
	{
		glm::vec2 posPrev = mSprite.GetPosition();
		pos = posPrev;
		if (mMovable->IsPositiveY())
		{
			pos.y += ((mMovable->Speed().y * 4.0f) * worldState.deltaTime);
		}
		else
		{
			pos.y -= ((mMovable->Speed().y*4.0f)* worldState.deltaTime);
		}
		mSprite.SetPosition(pos);

		if (!mSprite.IsColliding(mMovable->GetSprite()))
		{
			mCurrentFallSpeed = (mFallSpeed / abs(mFallSpeed))* mMovable->Speed().y;
			mSprite.SetPosition(posPrev);
			mMovable = nullptr;
			mCanFlip = false;
		}
		else
		{
			pos = posPrev;

			if (mFallSpeed / abs(mFallSpeed) < 0)
			{
				pos.y = mMovable->GetSprite().GetPosition().y + mMovable->GetSprite().GetSize().y;
			}
			else
			{
				pos.y = mMovable->GetSprite().GetPosition().y - mSprite.GetSize().y;
			}

			/*if (mMovable->IsPositiveY())
			{
			pos.y += ((mMovable->Speed().y) * worldState.deltaTime);
			}
			else
			{
			pos.y -= ((mMovable->Speed().y)* worldState.deltaTime);
			}*/
			mSprite.SetPosition(pos);
		}

	}
	else if (!collided)
	{
		mCurrentFallSpeed = mFallSpeed;
		mCanFlip = false;
	}
}

bool EntityPlayer::GetRightPressed()
{
	return mRightPressed;
}

void EntityPlayer::SetRightPressed(bool pressed)
{
	mRightPressed = pressed;
}

bool EntityPlayer::GetLeftPressed()
{
	return mLeftPressed;
}

void EntityPlayer::SetLeftPressed(bool pressed)
{
	mLeftPressed = pressed;
}
