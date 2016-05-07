#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

		DrawableGameComponent::DrawableGameComponent()
		: GameComponent(),  mCamera(nullptr)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game)
		: GameComponent(game),  mCamera(nullptr)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game, Camera& camera)
		: GameComponent(game),  mCamera(&camera)
	{
	}

	DrawableGameComponent::~DrawableGameComponent()
	{
	}

	/*bool DrawableGameComponent::Visible() const
	{
		return mVisible;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		mVisible = visible;
	}*/

	Camera* DrawableGameComponent::GetCamera()
	{
		return mCamera;
	}

	void DrawableGameComponent::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

}