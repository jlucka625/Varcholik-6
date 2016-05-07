#include "pch.h"

using namespace std;

namespace Rendering
{
	//Number of times to re-present the screen so frames no longer slow to a crawl

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mDirectInput(nullptr), mKeyboard(nullptr), mSprite(nullptr)
	{
	}

	void RenderingGame::Initialize()
	{
		ThrowIfFailed(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr), "DirectInput8Create() failed");

		mKeyboard = make_unique<Keyboard>(*this, mDirectInput);
		mComponents.push_back(mKeyboard.get());
		mServices.AddService(static_cast<UINT>(Keyboard::TypeIdClass()), mKeyboard.get());

		mCamera = make_unique<Camera>(*this);
		mComponents.push_back(mCamera.get());
		mServices.AddService(static_cast<UINT>(Camera::TypeIdClass()), mCamera.get());

		Game::Initialize();

		mCamera->SetPosition(0.0f, 0.0f, 5.0f);
	}

	void RenderingGame::Shutdown()
	{
		ReleaseObject(mDirectInput);

		Game::Shutdown();
	}

	void RenderingGame::Update(float deltaSeconds)
	{
		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}

		Game::Update(deltaSeconds);
	}

	void RenderingGame::Draw()
	{

		Game::Draw();
		
		//Apparently if this isn't presented enough times, the updates slow down to a crawl
		/*for (std::uint32_t i = 0U; i < TIMES_TO_PRESENT; ++i)
		{
			ThrowIfFailed(mSwapChain->Present(0, 0), "IDXGISwapChain::Present() failed.");
		}*/
	}
}