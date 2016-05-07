#pragma once

#include "ActionList.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class ActionListWhile final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListWhile, ActionList)
	public:
		/** @brief					Parameterless constructor for the ActionListWhile derived class.
		*   @return                 Instance of the ActionListWhile derived class.
		*/
		explicit ActionListWhile();

		ActionListWhile(const ActionListWhile& otherAListWhile) = delete;

		ActionListWhile& operator=(const ActionListWhile& otherAListWhile) = delete;

		/** @brief					Default destructor for the ActionListWhile derived class.
		*
		*/
		virtual ~ActionListWhile() = default;

		/** @brief					This action represents a scripted variation of a C++ while loop. While the prescribed condition is true, whatever actions are stored inside the
		*							ActionList will be executed. (To avoid an infinite loop, it is highly recommended to use  a decrement action to bring the condition down to a 
		*							false state.) For reference, here is the grammar that should be used:
		*
		*							<Action class=\"ActionListWhile\">
		*								<Integer name=\"Condition\" value=\"[insert number here]\"></Integer>
		*								.
		*								.		(any nested Actions go here)
		*								.
		*							</Action>
		*
		*
		*   @param worldState       Current state of the game's affairs.
		*/
		virtual void Update(class WorldState& worldState) override;
	private:
		/** @brief					Default constructor for the TableSharedData derived class.
		*
		*/
		int mCondition;

		/** @brief					Name of the prescribed condition attribute.
		*
		*/
		static const std::string CONDITION_ATTRIBUTE_NAME;
	};
}

