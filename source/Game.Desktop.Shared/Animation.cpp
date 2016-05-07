#include "pch.h"

RTTI_DEFINITIONS(Animation)

Animation::Animation() :
	mFrameIndex(0), mFrameCount(0), mFrames()
{
}

Animation::Animation(std::uint32_t frameCount) : 
	mFrameIndex(0), mFrameCount(frameCount), mFrames()
{
}

void Animation::AddFrame(const glm::vec4& frame)
{
	glm::vec2 vec(frame.x, frame.y);
	mFrames.PushBack(vec);
}
	
void Animation::UpdateAnimation()
{
	++mFrameIndex;
	if (mFrameIndex == mFrameCount)
		mFrameIndex = 0;
}

glm::vec2& Animation::GetFrame()
{
	return mFrames[mFrameIndex];
}

void Animation::ResetAnimation()
{
	mFrameIndex = 0;
}
