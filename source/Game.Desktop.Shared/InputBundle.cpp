#include "pch.h"
#include "InputBundle.h"


InputBundle::InputBundle()
{
}


InputBundle::~InputBundle()
{
}

void InputBundle::SetKeys(const bool leftKeyPressed, const bool rightKeyPressed, const bool gravityFlippedPressed)
{
	mLeftPressed = leftKeyPressed;
	mRightPressed = rightKeyPressed;
	mGravityFlipPressed = gravityFlippedPressed;
}

const bool InputBundle::LeftPressed() const
{
	return mLeftPressed;
}

const bool InputBundle::RightPressed() const
{
	return mRightPressed;
}

const bool InputBundle::GravityFlipPressed() const
{
	return mGravityFlipPressed;
}
