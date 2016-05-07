#pragma once

#include "Common.h"
#include "Game.h"
#include "Keyboard.h"

using namespace Library;

namespace Library
{

	class Keyboard;
	class Camera;
}

namespace Rendering
{
	class Sprite;

	class RenderingGame : public Game
	{
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);

		virtual void Initialize() override;
		virtual void Update(float deltaSeconds) override;
		virtual void Draw() override;

	protected:
		virtual void Shutdown() override;

	private:
		LPDIRECTINPUT8 mDirectInput;
		std::unique_ptr<Keyboard> mKeyboard;

		std::unique_ptr<Sprite> mSprite;
	};
}