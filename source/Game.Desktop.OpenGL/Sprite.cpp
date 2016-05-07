#include "pch.h"
#include "Sprite.h"

float Sprite::defaultXSize = 10.0f;
float Sprite::defaultYSize = 10.0f;

Sprite::Sprite() : mTexture(nullptr), mPosition(), mSize(defaultXSize, defaultYSize), mRotation(0.0f),
mFramePosition(), mTranslation(), mColor(1.0f), mTranslationSpeed(), mRotationSpeed(0.0f), mTimeToChangeAnimation(0.0f), mCurrentTimeToChangeAnimation(0.0f), mIsXFlipped(false), mIsYFlipped(false), mCurrentAnimationState(""), mAnimations()
{
}

Texture2D & Sprite::GetTexture() const
{
	if (mTexture == nullptr)
	{
		throw std::exception("Texture not set!");
	}
	return *mTexture;
}

void Sprite::SetTexture(const Texture2D & texture)
{
	mTexture = &const_cast<Texture2D&>(texture);
}

glm::vec2 Sprite::GetPosition() const
{
	return mPosition;
}

void Sprite::SetPosition(glm::vec2 position)
{
	mPosition = position;
}

glm::vec2 Sprite::GetSize() const
{
	return mSize;
}

void Sprite::SetSize(glm::vec2 size)
{
	mSize = size;
}

float Sprite::GetRotation() const
{
	return mRotation;
}

void Sprite::SetRotation(float rotation)
{
	mRotation = rotation;
}

glm::vec2 Sprite::GetFramePosition() const
{
	return mFramePosition;
}

void Sprite::SetFramePosition(glm::vec2 position)
{
	mFramePosition = position;
}

glm::vec2 Sprite::GetTranslation() const
{
	return mTranslation;
}

void Sprite::SetTranslation(glm::vec2 translation)
{
	mTranslation = translation;
}

float Sprite::GetRotationSpeed() const
{
	return mRotationSpeed;
}

void Sprite::SetRotationSpeed(float speed)
{
	mRotationSpeed = speed;
}

float Sprite::GetAnimationTime() const
{
	return mTimeToChangeAnimation;
}

void Sprite::SetAnimationTime(float time)
{
	mTimeToChangeAnimation = time;
}

glm::vec2 Sprite::GetTranslationSpeed() const
{
	return mTranslationSpeed;
}

void Sprite::SetTranslationSpeed(glm::vec2 speed)
{
	mTranslationSpeed = speed;
}

glm::vec3 Sprite::GetColor() const
{
	return mColor;
}

void Sprite::SetColor(glm::vec3 color)
{
	color = mColor;
}

bool Sprite::IsXFlipped() const
{
	return mIsXFlipped;
}

void Sprite::SetIsXFlipped(bool isFlipped)
{
	mIsXFlipped = isFlipped;
}

bool Sprite::IsYFlipped() const
{
	return mIsYFlipped;
}

void Sprite::SetIsYFlipped(bool isFlipped)
{
	mIsYFlipped = isFlipped;
}

void Sprite::Update(float deltaSeconds)
{
	auto it = mAnimations.find(mCurrentAnimationState);
	if (it == mAnimations.end())
		return;

	Animation& currentAnimation = (*it).second;

	if (mTimeToChangeAnimation > 0.0f)
	{
		mCurrentTimeToChangeAnimation += deltaSeconds;
		if (mCurrentTimeToChangeAnimation >= mTimeToChangeAnimation)
		{	
			currentAnimation.UpdateAnimation();
			mFramePosition = currentAnimation.GetFrame();
			mCurrentTimeToChangeAnimation = 0.0f;
		}
	}
}

void Sprite::AddAnimation(const std::string& state, const Animation& animation)
{
	mAnimations.insert(std::make_pair(state, animation));
}

void Sprite::SetAnimationState(const std::string& state)
{
	mCurrentAnimationState = state;
	mAnimations[mCurrentAnimationState].ResetAnimation();
}

bool Sprite::IsColliding(const Sprite & other)
{
	float leftA, leftB, rightA, rightB;
	float topA, topB, bottomA, bottomB;

	leftA = mPosition.x;
	rightA = mPosition.x + mSize.x;
	topA = mPosition.y + mSize.y;
	bottomA = mPosition.y;

	leftB = other.mPosition.x;
	rightB = other.mPosition.x + other.mSize.x;
	topB = other.mPosition.y + other.mSize.y;
	bottomB = other.mPosition.y;

	if (leftA <= rightB &&
		rightA >= leftB &&
		bottomA <= topB &&
		topA >= bottomB) {
		return true;
	}

	/*if (bottomA > topB)
	{
		return false;
	}

	if (leftA > rightB)
	{
		return false;
	}

	if (rightA < leftB)
	{
		return false;
	}

	if (topA < bottomB)
	{
		return false;
	}*/

	/*if (bottomA <= topB && (leftA >= rightB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}*/
	return false;
}

bool Sprite::IsVisible()
{
	return mIsVisible;
}

void Sprite::SetVisible(bool visible)
{
	mIsVisible = visible;
}
