#pragma once
#include "Entity.h"
#include "Sprite.h"

using namespace LibraryDesktop;
using namespace Rendering;

class EntityPlayer : public Entity
{
	RTTI_DECLARATIONS(EntityPlayer, Entity);

public:
	EntityPlayer();
	
	/** @brief					Gets the width of the entity.
	*   @return                 The entity's width.
	*/
	float Width() const;

	/** @brief					Sets the width of the entity.
	*   @param width            The entity's new width.
	*/
	void SetWidth(float width);

	/** @brief					Gets the height of the entity.
	*   @return                 The entity's height.
	*/
	float Height() const;

	/** @brief					Sets the height of the entity.
	*   @param height             The entity's new name.
	*/
	void SetHeight(float height);

	/** @brief					Gets the move speed of the entity.
	*   @return                 The entity's move speed.
	*/
	float MoveSpeed() const;

	/** @brief					Sets the move speed of the entity.
	*   @param speed           The entity's new move speed.
	*/
	void SetMoveSpeed(float speed);

	/** @brief					Returns the current movespeed of the player
	*	@return					The player's movement speed
	*/
	float GetCurrentSpeed();

	/** @brief					Gets the fall speed of the entity.
	*   @return                 The entity's fall speed.
	*/
	float FallSpeed() const;

	/** @brief					Sets the fall speed of the entity.
	*   @param speed           The entity's new fall speed.
	*/
	void SetFallSpeed(float speed);

	/** @brief					Gets the respawn position of the entity.
	*   @return                 The entity's respawn position.
	*/
	glm::vec2 RespawnPosition() const;

	/** @brief					Sets the respawn position of the entity.
	*   @param position           The entity's new respawn position.
	*/
	void SetRespawnPosition(glm::vec2 position);

	/** @brief					Gets the texture name of the entity.
	*   @return                 The entity's texture name.
	*/
	const std::string& TextureName() const;

	/** @brief					Sets the texture name of the entity.
	*   @param name             The entity's new texture name.
	*/
	void SetTextureName(const std::string& name);

	/** @brief					Gets the sprite of the entity.
	*   @return                 The entity's sprite.
	*/
	Sprite& GetSprite() const;

	///Draw the sprite
	void Draw();

	///Respawn the player
	void Respawn();

	/** @brief					Creates a sprite from these parameters
	*   @param textureName      The sprite's texture name
	*   @param u				The sprite's horizontal frame position
	*   @param v				The sprite's vertical frame position
	*   @param width            The sprite's width
	*   @param height           The sprite's height
	*/
	void CreateSprite(const std::string& textureName, float u, float v, float width, float height);

	void ChangeAnimation(const std::string& state);

	void MovePlayer(std::int8_t direction);

	void StopPlayer();

	void FlipPlayer();

	void Update(class WorldState& worldState);

	/** @brief Returns if the player has logged right as pressed
	*/
	bool GetRightPressed();
	/** @brief Sets if the player has logged right as pressed
	*/
	void SetRightPressed(bool pressed);
	/** @brief Returns if the player has logged left as pressed
	*/
	bool GetLeftPressed();
	/** @brief Sets if the player has logged left as pressed
	*/
	void SetLeftPressed(bool pressed);

private:

	//The sprite to use
	Sprite mSprite;
	
	//The sprite renderer to use
	//SpriteRenderer mSpriteRenderer;

protected:

	//Can the player actually flip?
	bool mCanFlip;

	//The width of the sprite
	float mWidth;

	//The height of the sprite
	float mHeight;

	//The name of the texture
	std::string mTextureName;

	//How quickly to move left/right
	float mMoveSpeed;

	//the current velocity to add to the position each update
	float mCurrentSpeed;

	//How fast to fall down/up
	float mFallSpeed;

	// if left key is recorded as pressed
	bool mLeftPressed;

	// if right key is recorded as pressed
	bool mRightPressed;

	//Where to go if the player dies
	glm::vec2 mRespawnPosition;

	//The current fall speed of the player
	float mCurrentFallSpeed;

	//A little extra push for flipping
	float extraPush;

	//Last movable platform the player interacted with.
	class EntityMovable* mMovable;

	//Current flip wire the player interacted with.
	class EntityTile* mFlip;

	///The name of the width attribute
	static const std::string WIDTH_ATTRIBUTE;

	///The name of the height attribute
	static const std::string HEIGHT_ATTRIBUTE;

	///The name of the texture name attribute 
	static const std::string TEXTURE_NAME_ATTRIBUTE;

	///The name of the move speed attribute
	static const std::string MOVE_SPEED_ATTRIBUTE;

	///The name of the fall speed attribute
	static const std::string FALL_SPEED_ATTRIBUTE;
};

ConcreteFactory(RTTI, EntityPlayer);

