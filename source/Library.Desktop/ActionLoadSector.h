#pragma once

#include "Action.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class ActionLoadSector final : public Action
	{
		RTTI_DECLARATIONS(ActionLoadSector, Action)
	public:
		/** @brief					Parameterless constructor for the ActionIf derived class.
		*   @return                 Instance of the ActionIf derived class.
		*/
		ActionLoadSector();

		ActionLoadSector(const ActionLoadSector& otherALoadSector) = delete;

		ActionLoadSector& operator=(const ActionLoadSector& otherALoadSector) = delete;

		/** @brief					Default for the ActionIf derived class.
		*
		*/
		virtual ~ActionLoadSector() = default;

		/** @brief					This action "loads" a sector (sets the Sector pointer of the world's WorldState variable to a particular sector, 
		*							loads any new graphics, creates heap-allocated objects, etc.) Note that by using this action, you guarantee that 
		*							the previous sector has already been unloaded, and that this is the last action in the set, as undefined behavior
		*							may occur if other actions exist that are intended for the previous sector, but operate in this new one. For 
		*							reference, here is the grammar:
		*
		*							<Action class=\"ActionLoadSector\"/>
		*								<Integer name=\"Level\" value=\"[insert Sector number (ranging from 0 to (n - 1), where n is the total number of sectors)]\"></String>
		*							</Action>
		*
		*   @param worldState       Current state of the game's affairs.
		*	@exception				An std::exception will occur if a level with the desired name is not found in the world's list of sectors.
		*/
		virtual void Update(class WorldState& worldState) override;
	private:
		/** @brief					Name of the prescribed condition attribute.
		*
		*/
		static const std::string LEVEL_ATTRIBUTE_NAME;
	};
}

