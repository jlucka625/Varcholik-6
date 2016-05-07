#pragma once

#include "DrawableGameComponent.h"
#include "Animation.h"

using namespace Library;

namespace Library
{
	class Keyboard;
}

namespace Rendering
{
	class Sprite : public DrawableGameComponent
	{
		
		RTTI_DECLARATIONS(Sprite, DrawableGameComponent)

	public:
		Sprite();
		Sprite(Game& game, Camera& camera);
		~Sprite();

		Sprite(const Sprite& rhs) = delete;
		Sprite& operator=(const Sprite& rhs) = delete;

		virtual void Initialize() override;
		virtual void Update(float deltaSeconds) override;
		virtual void Draw() override;

		/** \brief Gets the texture of this sprite
		*   \return The texture of this sprite
		*/
		const std::wstring& GetTextureName() const;

		/** \brief Sets the texture of this sprite
		*   \param texture The texture of this sprite
		*/
		void SetTextureName(const std::wstring& texture);

		/** \brief Gets the size of this sprite
		*   \return The size of this sprite
		*/
		glm::vec2 GetSize() const;

		/** \brief Sets the size of this sprite
		*   \param size The size of this sprite
		*/
		void SetSize(glm::vec2 size);

		/** \brief Gets the frame position of this sprite
		*   \return The frame position of this sprite
		*/
		glm::vec2 GetFramePosition() const;

		/** \brief Sets the frame position of this sprite
		*   \param position The frame position of this sprite
		*/
		void SetFramePosition(glm::vec2 position);

		/** @brief					Gets the position of the entity.
		*   @return                 The entity's position.
		*/
		glm::vec2 GetPosition() const;

		/** @brief					Sets the position of the entity.
		*   @param position           The entity's new position.
		*/
		void SetPosition(glm::vec2 position);

		/** \brief Gets the total horizontal frames of the texture
		*   \return The total horizontal frames of the texture
		*/
		float GetTotalXFrames() const;

		/** \brief Sets the total horizontal frames of the texture
		*   \param frames The total horizontal frames of the texture
		*/
		void SetTotalXFrames(float frames);

		/** \brief Gets the total vertical frames of the texture
		*   \return The total vertical frames of the texture
		*/
		float GetTotalYFrames() const;

		/** \brief Sets the total vertical frames of the texture
		*   \param frames The total vertical frames of the texture
		*/
		void SetTotalYFrames(float frames);

		/** \brief Gets the total visibility of the texture
		*   \return The visibility of the texture
		*/
		bool IsVisible() const;

		/** \brief Sets the visibility of the texture
		*   \param visible The visibility of the texture
		*/
		void SetVisbility(bool visible);

		/** \brief Adds an animation to this sprite
		*   \param state The state of animation
		*   \param animation The animation to add
		*/
		void AddAnimation(const std::string& state, const Animation& animation);

		/** \brief Sets the animation state
		*   \param state The state of animation
		*/
		void SetAnimationState(const std::string& state);

		/** \brief Gets the time to animate for this sprite
		*   \return The time to animate for this sprite
		*/
		float GetAnimationTime() const;

		/** \brief Sets the time to animate for this sprite
		*   \param time The time to animate for this sprite
		*/
		void SetAnimationTime(float time);

		/** \brief Checks if a sprite is colliding with another sprite - box-bounded collision
		*   \param sprite The other sprite to check with
		*/
		bool IsColliding(const Sprite& other);

		/** \brief Gets the x-flip of this sprite
		*   \return The x-flip of this sprite
		*/
		bool IsXFlipped() const;

		/** \brief Sets the x-flip of this sprite
		*   \param isFlipped The x-flip of this sprite
		*/
		void SetIsXFlipped(bool isFlipped);

		/** \brief Gets the y-flip of this sprite
		*   \return The y-flip of this sprite
		*/
		bool IsYFlipped() const;

		/** \brief Sets the y-flip of this sprite
		*   \param isFlipped The y-flip of this sprite
		*/
		void SetIsYFlipped(bool isFlipped);

	private:
		struct CBufferPerObject
		{
			XMFLOAT4X4 WorldViewProjection;
			float XAnimationFrame;
			int FlipX;
			int FlipY;
			
			CBufferPerObject() : 
				WorldViewProjection(), XAnimationFrame(0.0f), FlipX(0), FlipY(0) { }
			CBufferPerObject(const XMFLOAT4X4& wvp, float xAnimationFrame = 0.0f) :
				WorldViewProjection(wvp), FlipX(0), FlipY(0), XAnimationFrame(xAnimationFrame) { }
		};

		ID3D11VertexShader* mVertexShader;
		ID3D11PixelShader* mPixelShader;
		ID3D11InputLayout* mInputLayout;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		ID3D11Buffer* mConstantBuffer;

		CBufferPerObject mCBufferPerObject;
		XMFLOAT4X4 mWorldMatrix;

		ID3D11ShaderResourceView* mColorTexture;
		ID3D11SamplerState* mColorSampler;
		ID3D11BlendState1* mBlendState;

		Keyboard* mKeyboard;

		//The name of the texture
		std::wstring mTextureName;

		//The size of the texture (width/height)
		glm::vec2 mSize;

		//The location in the texture where to draw the sprite
		glm::vec2 mFramePosition;

		//The total frames on the texture used
		float mTotalXFrames;
		float mTotalYFrames;

		//The position of the sprite
		float mPositionX;
		float mPositionY;

		//Is the sprite visible?
		bool mVisible;

		std::unordered_map<std::string, Animation> mAnimations;
		std::string mCurrentAnimationState;

		//The current time until an animation is reached
		float mCurrentTimeToChangeAnimation;

		//The amount of time necessary to change animations
		float mTimeToChangeAnimation;
	};
}
