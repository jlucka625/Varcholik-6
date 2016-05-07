#pragma once

#include "ActionContainer.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class Action : public ActionContainer
	{
		RTTI_DECLARATIONS(Action, ActionContainer)
	public:
		/** @brief					Constructor for the Action abstract base class.
		*   @return                 Instance of the Action abstract base class.
		*/
		explicit Action();

		Action(const Action& otherAction) = delete;

		Action& operator=(const Action& otherAction) = delete;

		/** @brief					Default destructor for the Action abstract base class.
		*
		*/
		virtual ~Action() = default;

		/** @brief					Gets the name of the action.
		*   @return                 The action's name.
		*/
		const std::string& Name() const;

		/** @brief					Sets the name of the action.
		*   @param name             The action's new name.
		*/
		void SetName(const std::string& name);

		/** @brief					Updates the internal state of this action. (Since this is an abstract base class, derived classes must provide implementation of this.)
		*   @param worldState       Current state of the game's affairs.
		*/
		virtual void Update(class WorldState& world) = 0;

		virtual class Action* CreateAction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		virtual class Reaction* CreateReaction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

	protected:
		/** @brief					Name of this action.
		*
		*/
		std::string mName;

		/** @brief					Name of the prescribed name attribute.
		*
		*/
		static const std::string NAME_ATTRIBUTE;
	};
}