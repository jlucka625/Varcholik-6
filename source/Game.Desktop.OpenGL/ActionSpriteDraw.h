#pragma once

#include "Action.h"

using namespace LibraryDesktop;

class ActionSpriteDraw final : public Action
{
public:
	///Default constructor for this object
	ActionSpriteDraw();

	ActionSpriteDraw(const ActionSpriteDraw& rhs) = delete;

	ActionSpriteDraw& operator=(const ActionSpriteDraw& rhs) = delete;

	///Default destructor for this object
	virtual ~ActionSpriteDraw() = default;

	/** @brief					This action updates and draws a sprite, using the entity that this action is contained in
	*   @param worldState       Current state of the game's affairs.
	*/
	virtual void Update(class WorldState& worldState) override;
	
	RTTI_DECLARATIONS(ActionSpriteDraw, Action)
};

ConcreteFactory(RTTI, ActionSpriteDraw);

