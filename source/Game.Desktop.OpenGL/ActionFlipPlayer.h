#pragma once
#include "Action.h"
#include "RTTI.h"


	class ActionFlipPlayer: public Action
	{
	public:
		explicit ActionFlipPlayer();
		
		ActionFlipPlayer(const ActionFlipPlayer& rhs) = delete;

		virtual ~ActionFlipPlayer() = default;

		ActionFlipPlayer& operator=(const ActionFlipPlayer& rhs) = delete;

		virtual void Update(class WorldState& worldState) override;

	private:
		//The name of the player target to invoke the FlipPlayer script on
		std::string mTarget;

		static const std::string TARGET_ATTRIBUTE_NAME;

	public:
		RTTI_DECLARATIONS(ActionFlipPlayer,Action)
	};
	ConcreteFactory(RTTI, ActionFlipPlayer)
