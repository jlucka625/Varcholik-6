#pragma once

#include "Action.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class ActionIf final : public Action
	{
		RTTI_DECLARATIONS(ActionIf, Action)
	public:
		/** @brief					Parameterless constructor for the ActionIf derived class.
		*   @return                 Instance of the ActionIf derived class.
		*/
		explicit ActionIf();

		ActionIf(const ActionIf& otherActionIf) = delete;

		ActionIf& operator=(const ActionIf& otherActionIf) = delete;

		/** @brief					Default for the ActionIf derived class.
		*
		*/
		virtual ~ActionIf() = default;

		/** @brief					This action represents a scripted variation of a C++ if statement. Using the prescribed condition attribute, this Update call will
		*							determine which of two inner Action blocks get executed. If the condition is true, the then block will execute. Otherwise, the else block (if
		*							defined in XML; otherwise, nothing will happen) will be executed. For reference, here is the grammar that should be used:
		*
		*							<Action class=\"ActionIf\">
		*								<Integer name=\"Condition\" value=\"[insert number here]\"></Integer>
		*								<Action class=\"ActionList parentName=\"Then\">
		*									.
		*									.			(any nested Actions go here)
		*									.
		*								</Action>
		*								<Action class=\"ActionList parentName=\"Else\">
		*									.
		*									.			(any nested Actions go here)
		*									.
		*								</Action>
		*							</Action>
		*
		*							Note that the else block is completely optional, but there must be a then block defined somewhere in the XML for this if block.
		*
		*   @param worldState       Current state of the game's affairs.
		*	@exception				An assertion fail will occur if a then block wasn't defined in XML. (While "then" is a prescribed attribute, it is an emmpty container, and in
		*							compliance with syntax rules of the grammar, you must at least define a then block within the if block.)
		*/
		virtual void Update(class WorldState& worldState) override;
	private:
		/** @brief					Brancing condition (0 = false, and anything else = true.)
		*
		*/
		int mCondition;

		/** @brief					Name of the prescribed condition attribute.
		*
		*/
		static const std::string CONDITION_ATTRIBUTE_NAME;

		/** @brief					Name of the prescribed then attribute.
		*
		*/
		static const std::string THEN_ATTRIBUTE_NAME;

		/** @brief					Name of the prescribed else attribute.
		*
		*/
		static const std::string ELSE_ATTRIBUTE_NAME;
	};
}

