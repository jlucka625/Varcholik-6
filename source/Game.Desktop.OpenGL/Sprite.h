#pragma once
#include "Texture2D.h"
#include <unordered_map>

class Sprite
{
public:

	///The default constructor for this object
	Sprite();

	/** \brief Gets the texture of this sprite
	*   \return The texture of this sprite
	*/
	Texture2D& GetTexture() const;

	/** \brief Sets the texture of this sprite
	*   \param texture The texture of this sprite
	*/
	void SetTexture(const Texture2D& texture);

	/** \brief Gets the position of this sprite
	*   \return The position of this sprite
	*/
	glm::vec2 GetPosition() const;

	/** \brief Sets the position of this sprite
	*   \param position The position of this sprite
	*/
	void SetPosition(glm::vec2 position);

	/** \brief Gets the size of this sprite
	*   \return The size of this sprite
	*/
	glm::vec2 GetSize() const;

	/** \brief Sets the size of this sprite
	*   \param size The size of this sprite
	*/
	void SetSize(glm::vec2 size);

	/** \brief Gets the rotation of this sprite
	*   \return The rotation of this sprite
	*/
	float GetRotation() const;

	/** \brief Sets the rotation of this sprite
	*   \param rotation The rotation of this sprite
	*/
	void SetRotation(float rotation);

	/** \brief Gets the frame position of this sprite
	*   \return The frame position of this sprite
	*/
	glm::vec2 GetFramePosition() const;

	/** \brief Sets the frame position of this sprite
	*   \param position The frame position of this sprite
	*/
	void SetFramePosition(glm::vec2 position);

	/** \brief Gets the translation of this sprite
	*   \return The translation of this sprite
	*/
	glm::vec2 GetTranslation() const;

	/** \brief Sets the translation of this sprite
	*   \param translation The translation of this sprite
	*/
	void SetTranslation(glm::vec2 translation);

	/** \brief Gets the rotation speed of this sprite
	*   \return The rotation speed of this sprite
	*/
	float GetRotationSpeed() const;

	/** \brief Sets the rotation speed of this sprite
	*   \param speed The rotation speed of this sprite
	*/
	void SetRotationSpeed(float speed);

	/** \brief Gets the time to animate for this sprite
	*   \return The time to animate for this sprite
	*/
	float GetAnimationTime() const;

	/** \brief Sets the time to animate for this sprite
	*   \param time The time to animate for this sprite
	*/
	void SetAnimationTime(float time);

	/** \brief Gets the translation speed of this sprite
	*   \return The translation speed of this sprite
	*/
	glm::vec2 GetTranslationSpeed() const;

	/** \brief Sets the translation speed of this sprite
	*   \param speed The translation speed of this sprite
	*/
	void SetTranslationSpeed(glm::vec2 speed);

	/** \brief Gets the color of this sprite
	*   \return The color of this sprite
	*/
	glm::vec3 GetColor() const;

	/** \brief Sets the color of this sprite
	*   \param color The color of this sprite
	*/
	void SetColor(const glm::vec3 color);

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

	/** \brief Updates the sprite
	*   \param deltaSeconds The amount of seconds that have passed
	*/
	void Update(float deltaSeconds);

	void AddAnimation(const std::string& state, const Animation& animation);

	void SetAnimationState(const std::string& state);

	/** \brief Checks if a sprite is colliding with another sprite - box-bounded collision
	*   \param sprite The other sprite to check with
	*/
	bool IsColliding(const Sprite& other);

	bool IsVisible();

	void SetVisible(bool visible);
	
private:
	bool mIsVisible;

	//The texture reference by this sprite
	Texture2D* mTexture;

	//The position of this sprite
	glm::vec2 mPosition;
	
	//The size of this sprite
	glm::vec2 mSize;

	//The rotation of this sprite
	float mRotation;

	//The current frame of this sprite
	glm::vec2 mFramePosition;

	//The translation of this sprite
	glm::vec2 mTranslation;

	//The speed of translation of this sprite
	glm::vec2 mTranslationSpeed;

	//The speed of rotation of this sprite
	float mRotationSpeed;

	//The color of this sprite
	glm::vec3 mColor;

	//The current time until an animation is reached
	float mCurrentTimeToChangeAnimation;

	//The amount of time necessary to change animations
	float mTimeToChangeAnimation;

	//Is the sprite flipped in the x-axis?
	bool mIsXFlipped;

	//Is the sprite flipped in the y-axis?
	bool mIsYFlipped;

	//The default sizes for sprites if not specified - make this the basic tile size unit
	static float defaultXSize;
	static float defaultYSize;

	std::unordered_map<std::string, Animation> mAnimations;
	std::string mCurrentAnimationState;
};

