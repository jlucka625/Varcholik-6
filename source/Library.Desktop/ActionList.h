#pragma once

#include "Action.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	/*For reference, here is the grammar that should be used :
		*
		*							<Action class = \"ActionList\">
		*								.
		*								.		(any nested Actions go here)
		*								.
		*							< / Action>*/
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)
	public:
		/** @brief					Parameterless constructor for the ActionList base class.
		*   @return                 Instance of the ActionList base class.
		*/
		explicit ActionList();

		ActionList(const ActionList& otherActionList) = delete;

		ActionList& operator=(const Action& otherActionList) = delete;

		/** @brief					Default destructor for the ActionList base class.
		*
		*/
		virtual ~ActionList() = default;

		/** @brief					Updates the internal state of each action within this ActionList.
		*   @param worldState       Current state of the game's affairs.
		*	@exception				An assertion fail will occur if an entry enclosed within the ActionList isn't an Action of some sort.
		*/
		virtual void Update(class WorldState& world) override;
	};
	ConcreteFactory(RTTI, ActionList)
}

