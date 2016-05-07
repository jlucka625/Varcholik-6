#pragma once
#include <cstdint>
#include <memory>

#include "Vector.h"

namespace LibraryDesktop
{
	template <typename T> class Stack
	{
	public:
		/** @brief					Parameterless constructor for the Iterator class.
		*	@return					Instance of the Iterator class.
		*/
		Stack();

		/** @brief					Copy constructor for the Stack template class.
		*  @param otherStack		constant reference to another instance of the Stack template class of the same type.
		*  @return					Instance of the Stack template class from the deep copy of another.
		*/
		Stack(const Stack& otherStack);

		/** @brief					Move constructor for the Stack template class.
		*  @param otherStack		right-hand reference to another instance of the Stack template class of the same type.
		*  @return					Instance of the Stack template class from the transfered ownership of the other.
		*/
		Stack(Stack&& otherStack);

		/** @brief					Destructor for the Stack template class.
		*
		*/
		~Stack();

		/** @brief					Assignment operator for the Stack template class.
		*  @param otherStack		constant reference to another instance of the Stack template class of the same type.
		*  @return					Instance of the Stack template class from the deep copy of another.
		*/
		Stack& operator=(const Stack& otherStack);

		/** @brief					Move assignment operator for the Stack template class.
		*  @param otherStack		right-hand reference to another instance of the Stack template class of the same type.
		*  @return					Instance of the Stack template class from the transfered ownership of the other.
		*/
		Stack& operator=(Stack&& otherStack);

		/** @brief					Equality operator for the Stack template class. (To be equal, the stacks must have the same contents in the same order.)
		*  @param otherStack		constant reference to another instance of the Stack template class of the same type.
		*  @return					True if this and another Stack instance are equal, and false otherwise.
		*/
		bool operator==(const Stack& otherStack) const;

		/** @brief					Inequality operator for the Stack template class.
		*  @param otherStack		constant reference to another instance of the Stack template class of the same type.
		*  @return					False if this and another Stack instance are equal, and true otherwise.
		*/
		bool operator!=(const Stack& otherStack) const;

		/** @brief					Pushes an item onto the stack. This item will now become the top of the stack.
		*  @param item				value to be pushed onto the stack.
		*/
		void Push(const T& item);

		/** @brief					Pops, or removes, the item at the top of the stack.
		*	@exception				An std::exception is thrown if the stack is empty.
		*/
		void Pop();

		/** @brief					Gets the item at the top of the stack.
		*  @return					A NON-CONSTANT reference to the item at the top of the stack.
		*  @exception				An std::exception is thrown if the stack is empty.
		*/
		T& Top();

		/** @brief					Gets the item at the top of the stack.
		*  @return					A CONSTANT reference to the item at the top of the stack.
		*  @exception				An std::exception is thrown if the stack is empty.
		*/
		const T& Top() const;

		/** @brief					Returns the current number of items on the stack.
		*	@return					The size of the stack.
		*/
		unsigned int Size() const;

		/** @brief					States whether or not there are items on the stack.
		*	@return					True if the size of the stack is 0, and false otherwise.
		*/
		bool IsEmpty() const;

		/** @brief					Empties the stack.
		*
		*/
		void Clear();
	private:
		/** @brief					Back-end container for the items on the stack.
		*
		*/
		Vector<T> mDataVector;
	};
}

#include "Stack.inl"