#pragma once

#include "GameComponent.h"
#include "WorldState.h"

namespace Library
{
	class Camera;

	class GameTime;

	class DrawableGameComponent : public GameComponent
	{
		RTTI_DECLARATIONS(DrawableGameComponent, GameComponent)

	public:
		DrawableGameComponent();
		DrawableGameComponent(Game& game);
		DrawableGameComponent(Game& game, Camera& camera);
		virtual ~DrawableGameComponent();

		DrawableGameComponent(const DrawableGameComponent& rhs) = delete;
		DrawableGameComponent& operator=(const DrawableGameComponent& rhs) = delete;

		/*bool Visible() const;
		void SetVisible(bool visible);*/

		Camera* GetCamera();
		void SetCamera(Camera* camera);

		virtual void Draw() = 0;

	protected:
		//bool mVisible;
		Camera* mCamera;
	};
}