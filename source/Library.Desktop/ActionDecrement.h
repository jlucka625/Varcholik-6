#pragma once

#include "Action.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class ActionDecrement final : public Action
	{
		RTTI_DECLARATIONS(ActionDecrement, Action)
	public:
		/** @brief					Parameterless constructor for the ActionDecrement derived class.
		*   @return                 Instance of the ActionDecrement derived class.
		*/
		explicit ActionDecrement();

		ActionDecrement(const ActionDecrement& otherActionDecrement) = delete;

		ActionDecrement& operator=(const ActionDecrement& otherActionDecrement) = delete;

		/** @brief					Default destructor for the ActionDecrement derived class.
		*
		*/
		virtual ~ActionDecrement() = default;

		/** @brief					This action represents a scripted variation of a C++ decrement operation. Using the prescribed target attribute, this Update call will
		*							search up the elemental hierarchy for the desired element and then subtract 1 to its value. For reference, here is the grammar that should be used:
		*
		*							<Action class=\"ActionDecrement\">
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

