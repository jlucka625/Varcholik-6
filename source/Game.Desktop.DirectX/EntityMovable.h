#pragma once
#include "Entity.h"
#include "Sprite.h"

#include <DirectXMath.h>
using namespace DirectX;
using namespace Rendering;

///An entity that moves back and forth. Can be harmful or can be a platform.
class EntityMovable final : public Entity
{
public:
	///The default constructor for this object
	EntityMovable();

	virtual ~EntityMovable() = default;

	/** @brief					Gets the sprite of the entity.
	*   @return                 The entity's sprite.
	*/
	Sprite& GetSprite() const;

	/** @brief					Updates the object
	*   @param worldState		The world state to use for updating
	*/
	virtual void Update(class WorldState& worldState) override;

	///Draw the sprite
	void Draw();

	/** @brief					Creates a sprite from these parameters
	*   @param textureName      The sprite's texture name
	*   @param u				The sprite's horizontal frame position
	*   @param v				The sprite's vertical frame position
	*   @param width            The sprite's width
	*   @param height           The sprite's height
	*/
	void CreateSprite(const std::string& textureName, float u, float v, float width, float height);

	/** @brief					Gets the speed of the entity.
	*   @return                 The entity's speed.
	*/
	glm::vec2 Speed() const;


	/** @brief					Sets the speed of the entity.
	*   @param speedX			The entity's new speed (x)
	*   @param speedY			The entity's new speed (y)
	*/
	void SetSpeed(float speedX, float speedY);

	/** @brief					Gets the min-max data of the entity.
	*   @return                 The entity's min-max data.
	*/
	glm::vec4 MinMaxData() const;

	/** @brief					Sets the min-max data of the entity.
	*   @param minX				The entity's min (x)
	*   @param maxX				The entity's max (x)
	*   @param minY				The entity's min (y)
	*   @param minY				The entity's max (y)
	*/
	void SetMinMaxData(float minX, float maxX, float minY, float maxY);

	/** @brief					Is the x direction positive?
	*   @return                 Whether the x direction is positive or not
	*/
	bool IsPositiveX() const;

	/** @brief					Sets if the y direction is positive or not
	*   @param isPositive       Whether the y direction is positive or not
	*/
	void SetIsPositiveX(int isPositive);

	/** @brief					Is the y direction positive?
	*   @return                 Whether the y direction is positive or not
	*/
	bool IsPositiveY() const;

	/** @brief					Sets if the y direction is positive or not
	*   @param isPositive       Whether the y direction is positive or not
	*/
	void SetIsPositiveY(int isPositive);

	/** @brief					Is the tile harmful?
	*   @return                 Whether the tile is harmful or not
	*/
	bool IsHarmful() const;

	/** @brief					Sets if the tile is harmful
	*   @param isHarmful           Whether the tile is harmful or not
	*/
	void SetIsHarmful(int isHarmful);

	/** @brief					Changes the current animation
	*   @param state			The animation state to change to
	*/
	void ChangeAnimation(const std::string& state);

	RTTI_DECLARATIONS(EntityMovable, Entity);

private:

	//The sprite to use
	Sprite mSprite;

	//The width of the sprite
	float mWidth;

	//The height of the sprite
	float mHeight;

	//How quickly the entity moves in the x direction
	float mSpeedX;

	//How quickly the entity moves in the y direction
	float mSpeedY;

	//Min X, Max X, Min Y, and Max Y for 2D movement of the object. Object moves within these bounds
	glm::vec4 mMinMaxData;

	//Is the entity moving foward in the x direction?
	int mIsPositiveX;

	//Is the entity moving forward in the y direction?
	int mIsPositiveY;

	//Does the object kill the player when hit?
	int mIsHarmful;

	//The name of the IsHarmful Attribute
	static const std::string IS_HARMFUL_ATTRIBUTE;

	//The name of the speedX attribute
	static const std::string SPEED_X_ATTRIBUTE;

	//The name of the speedY attribute
	static const std::string SPEED_Y_ATTRIBUTE;

	//The name of the isPositiveX attribute
	static const std::string IS_POSITIVE_X_ATTRIBUTE;

	//The name of the isPositiveY attribute
	static const std::string IS_POSITIVE_Y_ATTRIBUTE;

	//The name of the minMax data attribute
	static const std::string MIN_MAX_ATTRIBUTE;
};

ConcreteFactory(RTTI, EntityMovable)

