#pragma once

#include "Attributed.h"

using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed)
	public:
		/** @brief					Parameterless constructor for the AttributedFoo class.
		*	@return					Instance of the AttributedFoo class.
		*/
		AttributedFoo();

		/** @brief					Copy constructor for the AttributedFoo class.
		*	@param otherAFoo		Constant reference to another AttributedFoo instance.
		*	@return					Instance of the AttributedFoo class from the deep copy of another.
		*/
		AttributedFoo(const AttributedFoo& otherAFoo);

		/** @brief					Move constructor for the AttributedFoo class.
		*	@param otherAFoo		Right-hand reference to another AttributedFoo instance.
		*	@return					Instance of the AttributedFoo class from the transferred ownership of another.
		*/
		AttributedFoo(AttributedFoo&& otherAFoo);

		/** @brief					Destructor for the AttributedFoo class.
		*
		*/
		virtual ~AttributedFoo();

		/** @brief					Assignment operator for the AttributedFoo class.
		*	@param otherAFoo		Constant reference to another AttributedFoo instance.
		*	@return					Instance of the AttributedFoo class from the deep copy of another.
		*/
		AttributedFoo& operator=(const AttributedFoo& otherAFoo);

		/** @brief					Move assignment operator for the AttributedFoo class.
		*	@param otherAFoo		Right-hand reference to another AttributedFoo instance.
		*	@return					Instance of the AttributedFoo class from the transferred ownership of another.
		*/
		AttributedFoo& operator=(AttributedFoo&& otherAFoo);
	protected:
		/** @brief					Test internal data.
		*
		*/
		int health;
		float money;
		std::string identity;
		glm::vec4 position;
		glm::mat4 currentFieldGrid;
		Foo dummyFoo;

		RTTI* pointer;
	};
}

