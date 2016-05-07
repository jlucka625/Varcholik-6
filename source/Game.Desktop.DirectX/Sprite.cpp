#include "pch.h"
#include "Sprite.h"

namespace Rendering
{
	const float ACTUAL_SCALE_X = 0.0069f;
	const float ACTUAL_SCALE_Y = 0.0069f;
	const float COLLISION_Y_OFFSET = -30.0f;
	RTTI_DEFINITIONS(Sprite)

		Sprite::Sprite() : DrawableGameComponent(), mVertexShader(nullptr), mInputLayout(nullptr), mPixelShader(nullptr), mVertexBuffer(nullptr),
		mIndexBuffer(nullptr), mConstantBuffer(nullptr), mCBufferPerObject(XMFLOAT4X4(), 0.0f), mWorldMatrix(MatrixHelper::Identity), mColorTexture(nullptr),
		mColorSampler(nullptr), mBlendState(nullptr), mKeyboard(nullptr), mPositionX(0.0f), mPositionY(0.0f), mSize(0.32f, 0.64f), mFramePosition(0.0f, 0.0f), mTotalXFrames(8.0f), mTotalYFrames(2.0f), mVisible(true),
		mCurrentAnimationState(""), mAnimations(), mTimeToChangeAnimation(0.0f), mCurrentTimeToChangeAnimation(0.0f)
	{

	}
		Sprite::Sprite(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera), mVertexShader(nullptr), mInputLayout(nullptr), mPixelShader(nullptr), mVertexBuffer(nullptr),
		mIndexBuffer(nullptr), mConstantBuffer(nullptr), mCBufferPerObject(XMFLOAT4X4(), 0.0f), mWorldMatrix(MatrixHelper::Identity), mColorTexture(nullptr),
		mColorSampler(nullptr), mBlendState(nullptr), mKeyboard(nullptr), mPositionX(0.0f), mPositionY(0.0f), mSize(0.32f, 0.64f), mFramePosition(0.0f, 0.0f), mTotalXFrames(8.0f), mTotalYFrames(2.0f), mVisible(true),
		mCurrentAnimationState(""), mAnimations(), mTimeToChangeAnimation(0.0f), mCurrentTimeToChangeAnimation(0.0f)
	{
	}

	Sprite::~Sprite()
	{
		ReleaseObject(mConstantBuffer)
		ReleaseObject(mIndexBuffer)
		ReleaseObject(mPixelShader)
		ReleaseObject(mInputLayout)
		ReleaseObject(mVertexShader)
		ReleaseObject(mColorTexture)
		ReleaseObject(mColorSampler)
		ReleaseObject(mBlendState)
	}

	void Sprite::Initialize()
	{
		mKeyboard = (Keyboard*)mGame->Services().GetService(static_cast<UINT>(Keyboard::TypeIdClass()));
		assert(mKeyboard != nullptr);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load a compiled vertex shader
		std::vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"Content\\Effects\\VertexShader.cso", compiledVertexShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &mVertexShader), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		std::vector<char> compiledPixelShader;
		Utility::LoadBinaryFile(L"Content\\Effects\\PixelShader.cso", compiledPixelShader);
		ThrowIfFailed(mGame->Direct3DDevice()->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, &mPixelShader), "ID3D11Device::CreatedPixelShader() failed.");

		// Create an input layout
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		ThrowIfFailed(mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), &mInputLayout), "ID3D11Device::CreateInputLayout() failed.");

		// Create an index buffer
		UINT indices[] =
		{
			0, 1, 2,
			0, 2, 3,
		};

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData;
		ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
		indexSubResourceData.pSysMem = indices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &mIndexBuffer), "ID3D11Device::CreateBuffer() failed.");

		// Create a constant buffer
		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
		constantBufferDesc.ByteWidth = 80U;
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer), "ID3D11Device::CreateBuffer() failed.");
		
		// Create a blend state - for alpha channel to be rendered
		D3D11_BLEND_DESC1 BlendState;
		ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC1));
		BlendState.RenderTarget[0].BlendEnable = TRUE;
		BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		ThrowIfFailed(mGame->Direct3DDevice()->CreateBlendState1(&BlendState, &mBlendState), "ID3D11Device::CreateBlendState1() failed.");

		// Load a texture
		std::string charTextureName = std::string(mTextureName.begin(), mTextureName.end());
		mTotalXFrames = Game::textureFrameLookups.Find(charTextureName)->second.x;
		mTotalYFrames = Game::textureFrameLookups.Find(charTextureName)->second.y;

		auto actualTextureLocation = Game::textureLookups.Find(charTextureName)->second;
		std::wstring wTextureLocation = std::wstring(actualTextureLocation.begin(), actualTextureLocation.end());
		ThrowIfFailed(DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), wTextureLocation.c_str(), nullptr, &mColorTexture));

		// Create a texture sampler
		// Address mode must be set to clamp to render cleanly
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		ThrowIfFailed(mGame->Direct3DDevice()->CreateSamplerState(&samplerDesc, &mColorSampler), "ID3D11Device::CreateSamplerState() failed.");
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
				mCBufferPerObject.XAnimationFrame = mFramePosition.x;
				mCurrentTimeToChangeAnimation = 0.0f;
			}
		}
	}

	void Sprite::Draw()
	{
		if (!mVisible)
		{
			return;
		}

		// Create a vertex buffer here for easy animation
		VertexPositionTexture vertices[] =
		{
			VertexPositionTexture(XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(mFramePosition.x * (1.0f / mTotalXFrames), (mFramePosition.y + 1.0f) * (1.0f / mTotalYFrames))),
			VertexPositionTexture(XMFLOAT4(0.0f, mSize.y, 0.0f, 1.0f), XMFLOAT2(mFramePosition.x * (1.0f / mTotalXFrames), (mFramePosition.y) * (1.0f / mTotalYFrames))),
			VertexPositionTexture(XMFLOAT4(mSize.x, mSize.y, 0.0f, 1.0f), XMFLOAT2((mFramePosition.x + 1.0f) * (1.0f / mTotalXFrames), (mFramePosition.y) * (1.0f / mTotalYFrames))),
			VertexPositionTexture(XMFLOAT4(mSize.x, 0.0f, 0.0f, 1.0f), XMFLOAT2((mFramePosition.x + 1.0f) * (1.0f / mTotalXFrames), (mFramePosition.y + 1.0f) * (1.0f / mTotalYFrames)))
		};

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer), "ID3D11Device::CreateBuffer() failed.");

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout);

		UINT stride = sizeof(VertexPositionTexture);
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		direct3DDeviceContext->VSSetShader(mVertexShader, nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader, nullptr, 0);

		XMStoreFloat4x4(&mWorldMatrix, XMMatrixIdentity() * XMMatrixScaling(ACTUAL_SCALE_X, ACTUAL_SCALE_Y, 0.0f));
		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixTranslation((mPositionX - (800.0f / 2.0f)) *ACTUAL_SCALE_X, (mPositionY - (600.0f / 2.0f)) * -ACTUAL_SCALE_Y, 0.0f));
		XMMATRIX wvp = worldMatrix * mCamera->ViewProjectionMatrix();
		wvp = XMMatrixTranspose(wvp);
		XMStoreFloat4x4(&mCBufferPerObject.WorldViewProjection, wvp);

		direct3DDeviceContext->UpdateSubresource(mConstantBuffer, 0, nullptr, &mCBufferPerObject, 0, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, &mConstantBuffer);

		// Bind the blend state to the OM stage
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT sampleMask = 0xffffffff;
		direct3DDeviceContext->OMSetBlendState(mBlendState, blendFactor, sampleMask);

		direct3DDeviceContext->PSSetShaderResources(0, 1, &mColorTexture);
		direct3DDeviceContext->PSSetSamplers(0, 1, &mColorSampler);

		direct3DDeviceContext->DrawIndexed(6, 0, 0);

		ReleaseObject(mVertexBuffer)
	}

	const std::wstring & Sprite::GetTextureName() const
	{
		return mTextureName;
	}

	void Sprite::SetTextureName(const std::wstring & texture)
	{
		mTextureName = texture;
	}

	glm::vec2 Sprite::GetSize() const
	{
		return mSize;
	}

	void Sprite::SetSize(glm::vec2 size)
	{
		mSize = size;
	}

	glm::vec2 Sprite::GetFramePosition() const
	{
		return mFramePosition;
	}

	void Sprite::SetFramePosition(glm::vec2 position)
	{
		mFramePosition = position;
	}

	glm::vec2 Sprite::GetPosition() const
	{
		return glm::vec2(mPositionX, mPositionY);
	}

	void Sprite::SetPosition(glm::vec2 position)
	{
		mPositionX = position.x;
		mPositionY = position.y;
	}

	float Sprite::GetTotalXFrames() const
	{
		return mTotalXFrames;
	}

	void Sprite::SetTotalXFrames(float frames)
	{
		mTotalXFrames = frames;
	}

	float Sprite::GetTotalYFrames() const
	{
		return mTotalYFrames;
	}

	void Sprite::SetTotalYFrames(float frames)
	{
		mTotalYFrames = frames;
	}

	bool Sprite::IsXFlipped() const
	{
		return (mCBufferPerObject.FlipX != 0);
	}

	void Sprite::SetIsXFlipped(bool isFlipped)
	{
		mCBufferPerObject.FlipX = (isFlipped ? 1 : 0);
	}

	bool Sprite::IsYFlipped() const
	{
		return (mCBufferPerObject.FlipY != 0);
	}

	void Sprite::SetIsYFlipped(bool isFlipped)
	{
		mCBufferPerObject.FlipY = (isFlipped ? 1 : 0);
	}

	bool Sprite::IsVisible() const
	{
		return mVisible;
	}

	void Sprite::SetVisbility(bool visible)
	{
		mVisible = visible;
	}

	float Sprite::GetAnimationTime() const
	{
		return mTimeToChangeAnimation;
	}

	void Sprite::SetAnimationTime(float time)
	{
		mTimeToChangeAnimation = time;
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

		leftA = mPositionX;
		rightA = mPositionX + mSize.x;
		topA = mPositionY + mSize.y + COLLISION_Y_OFFSET;
		bottomA = mPositionY + COLLISION_Y_OFFSET;

		leftB = other.mPositionX;
		rightB = other.mPositionX + other.mSize.x;
		topB = other.mPositionY + other.mSize.y;
		bottomB = other.mPositionY;

		if (leftA <= rightB &&
			rightA >= leftB &&
			bottomA <= topB &&
			topA >= bottomB) 
		{
			return true;
		}
		return false;
	}
}
