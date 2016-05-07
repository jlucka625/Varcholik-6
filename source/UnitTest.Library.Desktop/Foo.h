/** @file */

#pragma once

#include "RTTI.h"

using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		/** @brief					Parameterless constructor for the Foo class.
		*	@return					Instance of the Foo class.
		*/
		Foo();

		/** @brief					Parametered constructor for the Foo class.
		*  @param data				integer value to be stored in mData.
		*  @return					Instance of the Foo class.
		*/
		Foo(int data);

		/** @brief					Copy constructor for the Foo class.
		*  @param otherFoo			constant reference to another instance of the Foo class.
		*  @return					Instance of the Foo class from the deep copy of another.
		*/
		Foo(const Foo& otherFoo);

		/** @brief					Move constructor for the Foo class.
		*  @param otherFoo			right-hand reference to another instance of the Foo class.
		*  @return					Instance of the Foo class from the transferred ownership of another.
		*/
		Foo(Foo&& otherFoo);

		/** @brief					Assignment operator for the Foo class.
		*  @param otherFoo			constant reference to another instance of the Foo class.
		*  @return					Instance of the Foo class from the deep copy of another.
		*/
		Foo& operator=(const Foo& otherFoo);

		/** @brief					Move assignment operator for the Foo class.
		*  @param otherFoo			right-hand reference to another instance of the Foo class.
		*  @return					Instance of the Foo class from the transferred ownership of another.
		*/
		Foo & operator=(Foo&& otherFoo);

		/** @brief					Destructor for the Foo class.
		*
		*/
		~Foo();

		/** @brief					Equality operator for the Foo class. (To be equal, mData and the dereference of mPointerData must have the same value.)
		*  @param otherFoo			constant reference to another instance of the Foo class.
		*  @return					True if this and another Foo instance are equal, and false otherwise.
		*/
		bool operator==(const Foo& otherFoo) const;

		/** @brief					Inequality operator for the Foo class.
		*  @param otherFoo			constant reference to another instance of the Foo class.
		*  @return					False if this and another Foo instance are equal, and true otherwise.
		*/
		bool operator!=(const Foo& otherFoo) const;

		/** @brief					Public accessor for mData. (Instead of a copy, a reference to the int value is returned so it can be modified directly or indirectly if need be.)
		*  @return					Reference to mData.
		*/
		int &Data();

		/** @brief					Constant version of the public accessor for mData.
		*  @return					Constant copy of mData.
		*/
		const int Data() const;

		/** @brief					Public accessor for mPointerData.
		*  @return					Address of the thing mPointerData is pointing to.
		*/
		int* PointerData();

		/** @brief					Constant version of the public accessor for mPointerData.
		*  @return					Constant pointer to the address of the thing mPointerData is pointing to.
		*/
		const int* PointerData() const;

		/** @brief					Override of RTTI's ToString method for the Foo class.
		*  @return					String representation of the Foo instance ({mData: _____, mPointerData: _____}).
		*/
		virtual std::string ToString() const;
	private:
		/** @brief					Value of this particular instance.
		*
		*/
		int mData;

		/** @brief					Pointer used in deep copies of Foo objects.
		*
		*/
		int *mPointerData;
	};
}

