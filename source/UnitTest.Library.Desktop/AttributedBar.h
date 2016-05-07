#pragma once

#include "AttributedFoo.h"

using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	class AttributedBar : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, AttributedFoo)
	public:
		/** @brief					Parameterless constructor for the AttributedBar class.
		*	@return					Instance of the AttributedBar class.
		*/
		AttributedBar();

		/** @brief					Destructor for the AttributedBar class.
		*
		*/
		virtual ~AttributedBar();
	private:
		/** @brief					Test internal data.
		*
		*/
		int classID;
		std::string description;
	};
}

