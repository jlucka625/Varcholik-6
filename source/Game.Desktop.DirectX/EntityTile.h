#pragma once
//#include "SpriteRenderer.h"
#include "Entity.h"
#include "Factory.h"
#include "Sprite.h"
using namespace LibraryDesktop;
using namespace Rendering;

///The entity that represents a static tile/spike
class EntityTile final : public Entity
{
	RTTI_DECLARATIONS(EntityTile, Entity)
public:
	///The default constructor for this object
	EntityTile();

	///The default destructor for this object
	virtual ~EntityTile() = default;

	/** @brief					Gets the sprite of the entity.
	*   @return                 The entity's sprite.
	*/
	Sprite& GetSprite() const;

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

	/** @brief					Is the tile harmful?
	*   @return                 Whether the tile is harmful or not
	*/
	bool IsHarmful() const;

	/** @brief					Sets if the tile is harmful
	*   @param isHarmful           Whether the tile is harmful or not
	*/
	void SetIsHarmful(int isHarmful);

	/** @brief					Does the tile set the player's respawn point?
	*   @return                 Whether the tile is a checkpoint or not
	*/
	bool IsCheckpoint() const;

	/** @brief					Sets if the tile is a checkpoint
	*   @param isHarmful           Whether the tile is a checkpoint or not
	*/
	void SetIsCheckpoint(int isCheckpoint);

	/** @brief					Does the tile flip gravity?
	*   @return                 Whether the tile flips gravity or not
	*/
	bool IsGravityFlip() const;

	/** @brief					Sets if the tile flips gravity
	*   @param isHarmful           Whether the tile flips gravity or not
	*/
	void SetIsGravityFlip(int isGravityFlip);

	/** @brief					Is this tile an exit
	*   @return                 Whether this tile is an exit or not
	*/
	bool IsExit();

	/** @brief					Sets if the this tile teleports to new level
	*   @param isExit           Whether the tile is an exit or not
	*/
	void SetExit(bool isExit);
	
private:
	Sprite mSprite;

	//The width of the sprite
	float mWidth;

	//The height of the sprite
	float mHeight;

	//Does the tile kill the player when hit?
	int mIsHarmful;

	//Does the tile set the player's respawn point when hit?
	int mIsCheckpoint;
	
	//Does the tile flip the player's gravity when hit?
	int mIsGravityFlip;

	//Is this tile an exit?
	bool mIsExit;

	//The sprite renderer to use
	//SpriteRenderer mSpriteRenderer;

	///The name of the IsHarmful Attribute
	static const std::string IS_HARMFUL_ATTRIBUTE;

	///The name of the IsCheckpoint Attribute
	static const std::string IS_CHECKPOINT_ATTRIBUTE;

	///The name of the IsGravityFlip Attribute
	static const std::string IS_GRAVITY_FLIP_ATTRIBUTE;
};
ConcreteFactory(RTTI, EntityTile)

