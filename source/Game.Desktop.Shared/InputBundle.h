#pragma once
class InputBundle
{
public:
	InputBundle();
	~InputBundle();

	void SetKeys(const bool leftKeyPressed, const bool rightKeyPressed, const bool gravityFlippedPressed);

	const bool LeftPressed() const;
	const bool RightPressed() const;
	const bool GravityFlipPressed() const;

private:
	bool mLeftPressed;
	bool mRightPressed;
	bool mGravityFlipPressed;
};

