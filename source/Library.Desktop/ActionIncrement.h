#pragma once

#include "Action.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)
	public:
		/** @brief					Parameterless constructor for the ActionIncrement derived class.
		*   @return                 Instance of the ActionIncrement derived class.
		*/
		explicit ActionIncrement();

		ActionIncrement(const ActionIncrement& otherActionIncrement) = delete;

		ActionIncrement& operator=(const ActionIncrement& otherActionIncrement) = delete;

		/** @brief					Default destructor for the ActionIncrement derived class.
		*
		*/
		virtual ~ActionIncrement() = default;

		/** @brief					This action represents a scripted variation of a C++ increment operation. Using the prescribed target attribute, this Update call will
		*							search up the elemental hierarchy for the desired element and then add 1 to its value.  For reference, here is the grammar that should be used:
		*
		*							<Action class=\"ActionIncrement\">
		*								<String name="Target" value=[name of a Datum higher up the hierarchy]></String>
		*							</Action>
		*
		*   @param worldState       Current state of the game's affairs.
		*	@exception				An std::exception will occur if no name was ever provided for the target, no Datum with the name could be found, or the Datum to modify is not of type Integer.
		*/
		virtual void Update(class WorldState& worldState) override;
	private:
		/** @brief					Name of the Datum to increment.
		*
		*/
		std::string mTarget;

		/** @brief					Name of the prescribed target attribute.
		*
		*/
		static const std::string TARGET_ATTRIBUTE_NAME;
	};
}

