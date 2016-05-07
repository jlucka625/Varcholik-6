#include "pch.h"

namespace Library
{
	RTTI_DEFINITIONS(Game)

	const std::uint32_t TIMES_TO_PRESENT = 1U;
	const XMVECTORF32 Game::BackgroundColor = ColorHelper::Black;

	const UINT Game::DefaultScreenWidth = 800;
	const UINT Game::DefaultScreenHeight = 600;
	const UINT Game::DefaultFrameRate = 60;
	const UINT Game::DefaultMultiSamplingCount = 4;

	const std::uint32_t maxHashMapSize = 15U;
	HashMap<std::string, std::string> Game::textureLookups(maxHashMapSize);
	HashMap<std::string, glm::vec2> Game::textureFrameLookups(maxHashMapSize);

	const int IS_PRESSED = 1;
	const int IS_RELEASED = 0;

	Game::Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: RenderTarget(), mInstance(instance), mWindowClass(windowClass), mWindowTitle(windowTitle), mShowCommand(showCommand),
		mWindowHandle(), mWindow(),
		mScreenWidth(DefaultScreenWidth), mScreenHeight(DefaultScreenHeight),
		mGameClock(), mGameTime(),
		mFeatureLevel(D3D_FEATURE_LEVEL_9_1), mDirect3DDevice(nullptr), mDirect3DDeviceContext(nullptr), mSwapChain(nullptr),
		mFrameRate(DefaultFrameRate), mIsFullScreen(false),
		mDepthStencilBufferEnabled(true), mMultiSamplingEnabled(true), mMultiSamplingCount(DefaultMultiSamplingCount), mMultiSamplingQualityLevels(0),
		mDepthStencilBuffer(nullptr), mRenderTargetView(nullptr), mDepthStencilView(nullptr), mViewport(),
		mComponents(), mServices(), mInputHandler(nullptr), mKeyboard(nullptr), mCamera(nullptr)
	{
	}

	Game::~Game()
	{
	}

	HINSTANCE Game::Instance() const
	{
		return mInstance;
	}

	HWND Game::WindowHandle() const
	{
		return mWindowHandle;
	}

	const WNDCLASSEX& Game::Window() const
	{
		return mWindow;
	}

	const std::wstring& Game::WindowClass() const
	{
		return mWindowClass;
	}

	const std::wstring& Game::WindowTitle() const
	{
		return mWindowTitle;
	}

	int Game::ScreenWidth() const
	{
		return mScreenWidth;
	}

	int Game::ScreenHeight() const
	{
		return mScreenHeight;
	}

	ID3D11Device1* Game::Direct3DDevice() const
	{
		return mDirect3DDevice;
	}

	ID3D11DeviceContext1* Game::Direct3DDeviceContext() const
	{
		return mDirect3DDeviceContext;
	}

	bool Game::DepthStencilBufferEnabled() const
	{
		return mDepthStencilBufferEnabled;
	}

	ID3D11RenderTargetView* Game::RenderTargetView() const
	{
		return mRenderTargetView;
	}

	ID3D11DepthStencilView* Game::DepthStencilView() const
	{
		return mDepthStencilView;
	}

	float Game::AspectRatio() const
	{
		return static_cast<float>(mScreenWidth) / mScreenHeight;
	}

	bool Game::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	const D3D11_TEXTURE2D_DESC& Game::BackBufferDesc() const
	{
		return mBackBufferDesc;
	}

	const D3D11_VIEWPORT& Game::Viewport() const
	{
		return mViewport;
	}

	bool Game::MultiSamplingEnabled() const
	{
		return mMultiSamplingEnabled;
	}

	UINT Game::MultiSamplingCount() const
	{
		return mMultiSamplingCount;
	}

	UINT Game::MultiSamplingQualityLevels() const
	{
		return mMultiSamplingQualityLevels;
	}

	const std::vector<GameComponent*>& Game::Components() const
	{
		return mComponents;
	}

	const ServiceContainer& Game::Services() const
	{
		return mServices;
	}

	void Game::Run()
	{
		InitializeWindow();
		InitializeDirectX();
		Initialize();

		//creat factories for reactions
		ReactionAttributedFactory raf;
		ActionMovePlayerFactory ampf;
		ActionFlipPlayerFactory afpf;

		mInputHandler = new EntityInputHandler();
		mInputHandler->GenerateHandlers();

		MSG message;
		ZeroMemory(&message, sizeof(message));

		mGameClock.Reset();

		World world(b2Vec2(0.0f, 9.8f));

		LevelParser lp(*this, *mCamera);
		Sector* level = &lp.LoadLevel();

		world.Adopt(*level, "Sectors");
		auto currentTime = mGameTime.CurrentTime();
		world.GetWorldState().SetGameTime(mGameTime);
		mInputHandler->SetWorldState(world.GetWorldState());

		GameDesktopShared::SoundManager* Sound = GameDesktopShared::SoundManager::CreateInstance();
		Sound->Play("BGM", "PushingOnwards", 3);
		Sound->SetVolume("BGM", .2f);
		Sound->Loop("BGM", -1);
		

		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
				mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

				mGameClock.UpdateGameTime(mGameTime);
				float deltaSeconds = static_cast<float>(mGameTime.ElapsedGameTime().count()) / 1000.0f;
				
				world.GetWorldState().deltaTime = deltaSeconds;

				if (deltaSeconds <= 0.1f)
				{
					Update(deltaSeconds);
					world.Update(mGameClock);
				}

				Sound->Update();
				PollInput();
				//Apparently if this isn't presented enough times, the updates slow down to a crawl
				for (std::uint32_t i = 0U; i < TIMES_TO_PRESENT; ++i)
				{
					ThrowIfFailed(mSwapChain->Present(0, 0), "IDXGISwapChain::Present() failed.");
				}

				if (world.GetWorldState().GetUnloadSector() || world.GetWorldState().GetReloadSector())
				{
					level->Orphan();
					Event<EventMessageAttributed>::UnsubscribeAll();
					world.GetEventQueue().Clear();
					delete level;

					if (world.GetWorldState().GetUnloadSector())
						lp.NextLevel();

					level = &lp.LoadLevel();
					world.Adopt(*level, "Sectors");
					world.GetWorldState().SetUnloadSector(false);
					world.GetWorldState().SetReloadSector(false);
				}

			}
		}

		delete Sound;
		delete mInputHandler;
		Shutdown();
	}

	void Game::Exit()
	{
		PostQuitMessage(0);
	}

	void Game::Shutdown()
	{
		ReleaseObject(mRenderTargetView);
		ReleaseObject(mDepthStencilView);
		ReleaseObject(mSwapChain);
		ReleaseObject(mDepthStencilBuffer);

		if (mDirect3DDeviceContext != nullptr)
		{
			mDirect3DDeviceContext->ClearState();
		}

		ReleaseObject(mDirect3DDeviceContext);
		ReleaseObject(mDirect3DDevice);

		UnregisterClass(mWindowClass.c_str(), mWindow.hInstance);
	}

	void Game::Initialize()
	{
		mKeyboard = reinterpret_cast<Keyboard*>(mServices.GetService(static_cast<UINT>(Keyboard::TypeIdClass())));

		textureLookups.Insert(std::make_pair("Tiles", "Content/Tiles.png"));
		textureLookups.Insert(std::make_pair("characters", "Content/characters.png"));
		textureLookups.Insert(std::make_pair("Title", "Content/title.png"));
		textureFrameLookups.Insert(std::make_pair("Tiles", glm::vec2(8.0f, 4.0f)));
		textureFrameLookups.Insert(std::make_pair("characters", glm::vec2(8.0f, 2.0f)));
		textureFrameLookups.Insert(std::make_pair("Title", glm::vec2(1.0f, 1.0f)));

		for (GameComponent* component : mComponents)
		{
			component->Initialize();
		}
		
	}

	void Game::Update(float deltaSeconds)
	{
		for (GameComponent* component : mComponents)
		{
			if (component->Enabled())
			{
				component->Update(deltaSeconds);
			}
		}
	}

	void Game::Draw()
	{
		for (GameComponent* component : mComponents)
		{
			DrawableGameComponent* drawableGameComponent = component->As<DrawableGameComponent>();
			if (drawableGameComponent != nullptr)
			{
				drawableGameComponent->Draw();
			}
		}
	}

	void Game::ResetRenderTargets()
	{
		mDirect3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	}

	void Game::UnbindPixelShaderResources(UINT startSlot, UINT count)
	{
		static ID3D11ShaderResourceView* emptySRV = nullptr;

		mDirect3DDeviceContext->PSSetShaderResources(startSlot, count, &emptySRV);
	}

	void Game::PollInput()
	{
		if (mKeyboard->WasKeyPressedThisFrame(static_cast<Library::byte>(mInputHandler->LeftKey())))
		{
			mInputHandler->HandleKey(mInputHandler->LeftKey(), IS_PRESSED);
		}
		else if (mKeyboard->WasKeyReleasedThisFrame(static_cast<Library::byte>(mInputHandler->LeftKey())))
		{
			mInputHandler->HandleKey(mInputHandler->LeftKey(), IS_RELEASED);
		}

		if (mKeyboard->WasKeyPressedThisFrame(static_cast<Library::byte>(mInputHandler->RightKey())))
		{
			mInputHandler->HandleKey(mInputHandler->RightKey(), IS_PRESSED);
		}
		else if (mKeyboard->WasKeyReleasedThisFrame(static_cast<Library::byte>(mInputHandler->RightKey())))
		{
			mInputHandler->HandleKey(mInputHandler->RightKey(), IS_RELEASED);
		}

		if (mKeyboard->WasKeyPressedThisFrame(static_cast<Library::byte>(mInputHandler->SwitchGravityKey())))
		{
			mInputHandler->HandleKey(mInputHandler->SwitchGravityKey(), IS_PRESSED);
		}
		else if (mKeyboard->WasKeyReleasedThisFrame(static_cast<Library::byte>(mInputHandler->SwitchGravityKey())))
		{
			mInputHandler->HandleKey(mInputHandler->SwitchGravityKey(), IS_RELEASED);
		}
	}

	void Game::Begin()
	{
		RenderTarget::Begin(mDirect3DDeviceContext, 1, &mRenderTargetView, mDepthStencilView, mViewport);
	}

	void Game::End()
	{
		RenderTarget::End(mDirect3DDeviceContext);
	}

	void Game::InitializeWindow()
	{
		ZeroMemory(&mWindow, sizeof(mWindow));
		mWindow.cbSize = sizeof(WNDCLASSEX);
		mWindow.style = CS_CLASSDC;
		mWindow.lpfnWndProc = WndProc;
		mWindow.hInstance = mInstance;
		mWindow.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
		mWindow.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
		mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
		mWindow.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
		mWindow.lpszClassName = mWindowClass.c_str();

		RECT windowRectangle = { 0, 0, static_cast<LONG>(mScreenWidth), static_cast<LONG>(mScreenHeight) };
		AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

		RegisterClassEx(&mWindow);
		POINT center = CenterWindow(mScreenWidth, mScreenHeight);
		mWindowHandle = CreateWindow(mWindowClass.c_str(), mWindowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, mInstance, nullptr);

		ShowWindow(mWindowHandle, mShowCommand);
		UpdateWindow(mWindowHandle);
	}

	void Game::InitializeDirectX()
	{
		HRESULT hr;
		UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		ID3D11Device* direct3DDevice = nullptr;
		ID3D11DeviceContext* direct3DDeviceContext = nullptr;
		if (FAILED(hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &mFeatureLevel, &direct3DDeviceContext)))
		{
			throw GameException("D3D11CreateDevice() failed", hr);
		}

		if (FAILED(hr = direct3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mDirect3DDevice))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", hr);
		}

		if (FAILED(hr = direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mDirect3DDeviceContext))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", hr);
		}

		ReleaseObject(direct3DDevice);
		ReleaseObject(direct3DDeviceContext);

		mDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, mMultiSamplingCount, &mMultiSamplingQualityLevels);
		if (mMultiSamplingQualityLevels == 0)
		{
			throw GameException("Unsupported multi-sampling quality");
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.Width = mScreenWidth;
		swapChainDesc.Height = mScreenHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (mMultiSamplingEnabled)
		{
			swapChainDesc.SampleDesc.Count = mMultiSamplingCount;
			swapChainDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
		}
		else
		{
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		IDXGIDevice* dxgiDevice = nullptr;
		if (FAILED(hr = mDirect3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
		{
			throw GameException("ID3D11Device::QueryInterface() failed", hr);
		}

		IDXGIAdapter *dxgiAdapter = nullptr;
		if (FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
		{
			ReleaseObject(dxgiDevice);
			throw GameException("IDXGIDevice::GetParent() failed retrieving adapter.", hr);
		}

		IDXGIFactory2* dxgiFactory = nullptr;
		if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			throw GameException("IDXGIAdapter::GetParent() failed retrieving factory.", hr);
		}

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
		fullScreenDesc.RefreshRate.Numerator = mFrameRate;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = !mIsFullScreen;

		if (FAILED(hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, mWindowHandle, &swapChainDesc, &fullScreenDesc, nullptr, &mSwapChain)))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			ReleaseObject(dxgiFactory);
			throw GameException("IDXGIDevice::CreateSwapChainForHwnd() failed.", hr);
		}

		ReleaseObject(dxgiDevice);
		ReleaseObject(dxgiAdapter);
		ReleaseObject(dxgiFactory);

		ID3D11Texture2D* backBuffer;
		if (FAILED(hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
		{
			throw GameException("IDXGISwapChain::GetBuffer() failed.", hr);
		}

		backBuffer->GetDesc(&mBackBufferDesc);

		if (FAILED(hr = mDirect3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView)))
		{
			ReleaseObject(backBuffer);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", hr);
		}

		ReleaseObject(backBuffer);

		if (mDepthStencilBufferEnabled)
		{
			D3D11_TEXTURE2D_DESC depthStencilDesc;
			ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
			depthStencilDesc.Width = mScreenWidth;
			depthStencilDesc.Height = mScreenHeight;
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

			if (mMultiSamplingEnabled)
			{
				depthStencilDesc.SampleDesc.Count = mMultiSamplingCount;
				depthStencilDesc.SampleDesc.Quality = mMultiSamplingQualityLevels - 1;
			}
			else
			{
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
			}

			if (FAILED(hr = mDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer)))
			{
				throw GameException("IDXGIDevice::CreateTexture2D() failed.", hr);
			}

			if (FAILED(hr = mDirect3DDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView)))
			{
				throw GameException("IDXGIDevice::CreateDepthStencilView() failed.", hr);
			}
		}

		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.Width = static_cast<float>(mScreenWidth);
		mViewport.Height = static_cast<float>(mScreenHeight);
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		// Set render targets and viewport through render target stack	
		Begin();
	}

	LRESULT WINAPI Game::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(windowHandle, message, wParam, lParam);
	}

	POINT Game::CenterWindow(int windowWidth, int windowHeight)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		POINT center;
		center.x = (screenWidth - windowWidth) / 2;
		center.y = (screenHeight - windowHeight) / 2;

		return center;
	}
}