#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class Animation : public RTTI
{
	RTTI_DECLARATIONS(Animation, RTTI)
public:
	Animation();
	explicit Animation(std::uint32_t frameCount);
	~Animation() = default;

	void AddFrame(const glm::vec4& frame);

	void UpdateAnimation();
	void ResetAnimation();

	glm::vec2& GetFrame();

private:
	std::uint32_t mFrameIndex;
	std::uint32_t mFrameCount;

	Vector<glm::vec2> mFrames;

};

enum class AnimationState
{
	INVALID,
	IDLE,
	WALK,
	DEATH,
	MAX
};

