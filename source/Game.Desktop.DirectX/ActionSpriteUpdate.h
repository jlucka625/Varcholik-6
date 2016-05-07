#pragma once
#include "Action.h"
#include "Factory.h"

using namespace LibraryDesktop;

class ActionSpriteUpdate final : public Action
{
public:
	///Default constructor for this object
	ActionSpriteUpdate();

	ActionSpriteUpdate(const ActionSpriteUpdate& rhs) = delete;

	ActionSpriteUpdate& operator=(const ActionSpriteUpdate& rhs) = delete;

	///Default destructor for this object
	virtual ~ActionSpriteUpdate() = default;

	/** @brief					This action updates and draws a sprite, using the entity that this action is contained in
	*   @param worldState       Current state of the game's affairs.
	*/
	virtual void Update(class WorldState& worldState) override;

	RTTI_DECLARATIONS(ActionSpriteUpdate, Action)
};

ConcreteFactory(RTTI, ActionSpriteUpdate);

