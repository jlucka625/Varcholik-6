#pragma once
#include "Action.h"
#include "RTTI.h"

class ActionMovePlayer : public Action
{
public:
	explicit ActionMovePlayer();

	ActionMovePlayer(const ActionMovePlayer& rhs) = delete;

	virtual ~ActionMovePlayer() = default;

	ActionMovePlayer& operator=(const ActionMovePlayer& rhs) = delete;

	virtual void Update(class WorldState& worldState) override;

private:
	std::string mDirection;
	std::string mTarget;


	static const std::string TARGET_ATTRIBUTE_NAME;
	static const std::string DIRECTION_ATTRIBUTE_NAME;


public:
	RTTI_DECLARATIONS(ActionMovePlayer, Action)
};
ConcreteFactory(RTTI, ActionMovePlayer)

