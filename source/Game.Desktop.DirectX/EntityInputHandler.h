#pragma once
#include "Entity.h"
#include "Factory.h"

///A class for handling input keys
class EntityInputHandler final : public Entity
{
public:
	typedef void(EntityInputHandler::*KeyFunctionPointer)(int mode);

	///The default constructor for this object
	EntityInputHandler();

	virtual ~EntityInputHandler() = default;

	/** @brief					Gets the left key of the entity.
	*   @return                 The entity's left key.
	*/
	int LeftKey() const;

	/** @brief					Sets the left key of the entity.
	*   @param width            The entity's new left key.
	*/
	void SetLeftKey(int key);

	/** @brief					Gets the right key of the entity.
	*   @return                 The entity's right key.
	*/
	int RightKey() const;

	/** @brief					Sets the right key of the entity.
	*   @param width            The entity's new right key.
	*/
	void SetRightKey(int key);

	/** @brief					Gets the Switch Gravity key of the entity.
	*   @return                 The entity's Switch Gravity key.
	*/
	int SwitchGravityKey() const;

	/** @brief					Sets the Switch Gravity key of the entity.
	*   @param width            The entity's new Switch Gravity key.
	*/
	void SetSwitchGravityKey(int key);

	/** \brief Gets the world state
	*   \return The world state
	*/
	const WorldState& GetWorldState() const;

	/** \brief Sets the world state
	*   \param world The world state
	*/
	void SetWorldState(const WorldState& world);

	///Creates handlers based on current key values
	void GenerateHandlers();

	/** @brief				  Handles a key stroke
	*   @param key            The key to handle
	*   @param mode           The current mode of key handling
	*/
	void HandleKey(int key, int mode);

	RTTI_DECLARATIONS(EntityInputHandler, Entity)

private:
	//helper function that the handler key methods use to dispatch events.
	void EntityInputHandler::SendInputMessage(std::string subtype);

	//The functions for the handler to use when handling keys
	HashMap<int, KeyFunctionPointer> mKeyHandlers;

	//Handles the left key press
	void HandleLeftKey(int action);

	//Handles the right key press
	void HandleRightKey(int action);

	//Handles the switch gravity key press
	void HandleSwitchGravityKey(int action);

	//GLFW key for moving left
	int mLeftKey;

	//GLFW key for moving right
	int mRightKey;

	//GLFW key for switching gravity
	int mSwitchGravityKey;

	//The world state
	WorldState* mWorldState;

	///The name of the left key attribute
	static const std::string LEFT_KEY_ATTRIBUTE;

	///The name of the right key attribute
	static const std::string RIGHT_KEY_ATTRIBUTE;

	///The name of the switch gravity attribute
	static const std::string SWITCH_GRAVITY_KEY_ATTRIBUTE;

	///The name of the world state attribute
	static const std::string WORLD_STATE_ATTRIBUTE;
};

ConcreteFactory(RTTI, EntityInputHandler)

