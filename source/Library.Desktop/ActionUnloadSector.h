#pragma once

#include "Action.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class ActionUnloadSector final : public Action
	{
		RTTI_DECLARATIONS(ActionUnloadSector, Action)
	public:
		/** @brief					Parameterless constructor for the ActionIf derived class.
		*   @return                 Instance of the ActionIf derived class.
		*/
		ActionUnloadSector() = default;

		ActionUnloadSector(const ActionUnloadSector& otherAUnloadSector) = delete;

		ActionUnloadSector& operator=(const ActionUnloadSector& otherAUnloadSector) = delete;

		/** @brief					Default for the ActionIf derived class.
		*
		*/
		virtual ~ActionUnloadSector() = default;

		/** @brief					This action "unloads" the current sector (clears out any heap-allocated contents, takes care of imminent graphics
		*							changes, etc.) and sets the Sector pointer of the world's WorldState variable to null so that a new one can be 
		*							loaded. Note that by using this action, you guarantee that a sector has already been loaded, and an ActionSectorLoad
		*							will be done immediately after so that the game can continue on without problems.
		*							For reference, here is the grammar:
		*
		*							<Action class=\"ActionUnloadSector\"/>
		*
		*   @param worldState       Current state of the game's affairs.
		*	@exception				An assertion fail occurs if the current sector is null.
		*/
		virtual void Update(class WorldState& worldState) override;
	};
}

