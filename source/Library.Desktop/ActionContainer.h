#pragma once

#include "Attributed.h"

namespace LibraryDesktop
{
	class ActionContainer : public Attributed
	{
		RTTI_DECLARATIONS(ActionContainer, Attributed)
	public:
		ActionContainer();
		virtual ~ActionContainer() = default;
		virtual class Action* CreateAction(const std::string& instanceName, const std::string& className, const std::string& parentName) = 0;
		virtual class Reaction* CreateReaction(const std::string& instanceName, const std::string& className, const std::string& parentName) = 0;
		class Datum& Actions();
		class Datum& Reactions();
	};
}

