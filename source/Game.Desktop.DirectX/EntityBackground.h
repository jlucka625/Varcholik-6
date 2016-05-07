#pragma once
#include "Entity.h"
#include "Sprite.h"

using namespace Rendering;

///An entity that functions as a background. Has a translation speed for moving its texture for scrolling
class EntityBackground final : public Entity
{
	RTTI_DECLARATIONS(EntityBackground, Entity)
public:
	///The default constructor for this object
	EntityBackground();


	/** @brief					Gets the sprite of this entity
	*   @return                 The sprite of this entity
	*/
	Sprite& GetSprite();

	/** @brief					Creates a sprite from these parameters
	*   @param textureName      The sprite's texture name
	*   @param u				The sprite's horizontal frame position
	*   @param v				The sprite's vertical frame position
	*   @param width            The sprite's width
	*   @param height           The sprite's height
	*/
	void CreateSprite(const std::string& textureName, float u, float v, float width, float height);

	/** @brief					Updates the object
	*   @param worldState		The world state to use for updating
	*/
	virtual void Update(class WorldState& worldState) override;

	///Draw the sprite
	void Draw();


	/** @brief					Gets the scrolling speed of the background
	*   @return                 The scrolling speed of the background
	*/
	glm::vec2 ScrollSpeed() const;

	/** @brief					Sets the background's scroll speed
	*   @param x				The horizontal scroll speed
	*   @param y				The vertical scroll speed
	*/
	void SetScrollSpeed(float x, float y);

private:
	//The sprite of the background
	Sprite mSprite;

	//The sprite renderer to use
	//SpriteRenderer mSpriteRenderer;

	//The horizontal scrolling speed of the background
	float mScrollSpeedX;

	//The vertical scrolling speed of the background
	float mScrollSpeedY;	

	///The name of the scroll speed (x) attribute
	static const std::string SCROLL_X_ATTRIBUTE;

	///The name of the scroll speed (y) attribute
	static const std::string SCROLL_Y_ATTRIBUTE;
};
ConcreteFactory(RTTI, EntityBackground)


