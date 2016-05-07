#pragma once
#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>

namespace LibraryDesktop
{
	typedef std::function<std::uint32_t(std::uint32_t, std::uint32_t)> IncrementFunctor;

	template<typename T> class Vector
	{
	public:
		class Iterator
		{
			friend class Vector<T>;
		public:
			/** @brief					Parameterless constructor for the Iterator class.
			*	@return					Instance of the Iterator class.
			*/
			Iterator();

			/** @brief					Copy constructor for the Iterator class.
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					Instance of the Iterator class from the deep copy of another.
			*/
			Iterator(const Iterator& otherIterator);

			/** @brief					Move constructor for the Iterator class.
			*  @param otherIterator		right-hand reference to another instance of the Iterator.
			*  @return					Instance of the Iterator class from the transferred ownership of the other.
			*/
			Iterator(Iterator&& otherIterator);

			/** @brief					Assignment operator for the Iterator class.
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					Instance of the Iterator class from the deep copy of another.
			*/
			Iterator& operator=(const Iterator& otherIterator);

			/** @brief					Move assignment operator for the Iterator class.
			*  @param otherIterator		right-hand reference to another instance of the Iterator.
			*  @return					Instance of the Iterator class from the transferred ownership of the other.
			*/
			Iterator& operator=(Iterator&& otherIterator);

			/** @brief					Equality operator for the Iterator class. (To be equal, both iterators must point to the same location of the same vector.)
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					True if this and another instance of the iterator class are equal, and false otherwise.
			*/
			bool operator==(const Iterator& otherIterator) const;

			/** @brief					Inequality operator for the Iterator class.
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					False if this and another instance of the iterator class are equal (pointing to the same vector and location), and true otherwise.
			*/
			bool operator!=(const Iterator& otherIterator) const;

			/** @brief					Prefix increment operator for the Iterator class.
			*  @return					A reference to the iterator at the next spot in the vector.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a vector, or pointing to an empty vector.)
			*/
			Iterator& operator++();

			/** @brief					Postfix increment operator for the Iterator class.
			*  @return					A copy of the iterator at the current location, while an increment to the next spot occurs in the background.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a vector, or pointing to an empty vector.)
			*/
			Iterator operator++(int);

			/** @brief					Dereference operator for the Iterator class.
			*  @return					Value of the node being pointed to.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a vector, or pointing to an empty vector.)
			*/
			T& operator*() const;
		private:
			/** @brief					Places a new node inside the vector after the node being pointed to by a specific iterator.
			*  @param owner				address of the vector being pointed to.
			*  @param index				current location being pointed to in the vector.
			*  @return					Instance of the Iterator class.
			*/
			Iterator(const Vector* owner, const std::uint32_t& index);

			/** @brief					vector the iterator is pointing to
			*
			*/
			const Vector* targetVector;

			/** @brief					Current location being pointed to in the vector.
			*
			*/
			std::uint32_t currentNode;
		};

		/** @brief					Parameterless constructor for the Vector template class.
		*	@return					Instance of the Vector template class of a specific type.
		*/
		Vector();

		/** @brief					Copy constructor for the Vector template class.
		*  @param otherVector		constant reference to another instance of the Vector template class of the same type.
		*  @return					Instance of the Vector template class from the deep copy of another.
		*/
		Vector(const Vector& otherVector);

		/** @brief					Move constructor for the Vector template class.
		*  @param otherVector		right-hand reference to another instance of the Vector template class of the same type.
		*  @return					Instance of the Vector template class from the transfered ownership of the other.
		*/
		Vector(Vector&& otherVector);

		/** @brief					Assignment operator for the Vector template class.
		*  @param otherVector		constant reference to another instance of the Vector template class of the same type.
		*  @return					Instance of the Vector template class from the deep copy of another.
		*/
		Vector& operator=(const Vector& otherVector);

		/** @brief					Move assignment operator for the Vector template class.
		*  @param otherVector		right-hand reference to another instance of the Vector template class of the same type.
		*  @return					Instance of the Vector template class from the transfered ownership of the other.
		*/
		Vector& operator=(Vector&& otherVector);

		/** @brief					Equality operator for the Vector template class. (To be equal, the vectors must be of the same size, and their nodes must have the same value.)
		*  @param otherVector		constant reference to another instance of the Vector template class of the same type.
		*  @return					True if this and another Vector instance are equal, and false otherwise.
		*/
		bool operator==(const Vector& otherVector) const;

		/** @brief					Inequality operator for the Vector template class.
		*  @param otherVector		constant reference to another instance of the Vector template class of the same type.
		*  @return					False if this and another Vector instance are equal, and true otherwise.
		*/
		bool operator!=(const Vector& otherVector) const;

		/** @brief					Non-const version of the subscript operator for the Vector template class.
		*  @param index				constant unsigned integer representing the desired spot in the vector.
		*  @return					A non-constant reference to the item in the vector at the desired spot.
		*  @exception				An std::exception is thrown if the index is greater than or equal to the capacity of the vector.
		*/
		T& operator[](const std::uint32_t& index);

		/** @brief					Const version of the subscript operator for the Vector template class.
		*  @param index				constant unsigned integer representing the desired spot in the vector.
		*  @return					A constance reference to the item in the vector at the desired spot.
		*  @exception				An std::exception is thrown if the index is greater than or equal to the capacity of the vector.
		*/
		const T& operator[](const std::uint32_t& index) const;

		/** @brief					Pops, or removes, the item at the back of the vector.
		*	@exception				An std::exception is thrown if the vector is empty.
		*/
		void PopBack();

		/** @brief					States whether or not there are items in the vector.
		*	@return					True if the number of items in the vector is 0, and false otherwise.
		*/
		bool IsEmpty() const;

		/** @brief					Gets the item at the front of the vector.
		*	@return					A NON-CONSTANT reference to whatever data (of a specific type) is at the front of the vector.
		*	@exception				An std::exception is thrown if the vector is empty.
		*/
		T& Front();

		/** @brief					Gets the item at the front of the vector.
		*	@return					A CONSTANT reference to whatever data (of a specific type) is at the front of the vector.
		*	@exception				An std::exception is thrown if the vector is empty.
		*/
		const T& Front() const;

		/** @brief					Gets the item at the back of the vector.
		*	@return					A NON-CONSTANT reference to whatever data (of a specific type) is at the back of the vector.
		*	@exception				An std::exception is thrown if the vector is empty.
		*/
		T& Back();

		/** @brief					Gets the item at the front of the vector.
		*	@return					A NON-CONSTANT reference to whatever data (of a specific type) is at the back of the vector.
		*	@exception				An std::exception is thrown if the vector is empty.
		*/
		const T& Back() const;

		/** @brief					Returns the current number of items in the vector.
		*	@return					The size of the vector.
		*/
		std::uint32_t Size() const;

		/** @brief					Returns the current number of items in the vector.
		*	@return					The size of the vector.
		*/
		std::uint32_t Capacity() const;

		/** @brief					Gets an iterator to the beginning of the vector.
		*   @return					an iterator pointing to the beginning of the vector, or the end if the vector is empty.
		*/
		Iterator begin() const;

		/** @brief					Gets an iterator to the end of the vector (the spot after the official last node of the vector.) Please note that this node value will ALWAYS be nullptr.
		*   @return					an iterator pointing to the spot after the last actual node of the vector.
		*/
		Iterator end() const;

		/** @brief					Push data of a specific type to the back of the vector. If the size of the vector after the push will exceed the capacity of the vector, a new vector is created
		*	@param data				value to be pushed.
		*   @return					An iterator to the node that was just added to the vector.
		*/
		Iterator PushBack(const T& data);

		/** @brief					Retrieve the data from the vector at the specified index.
		*	@param index			spot in the vector.
		*   @return					a reference to the data at the given index.
		*	@exception				An std::exception is thrown if the index exceeds the size of the vector.
		*/
		T& At(const std::uint32_t& index);

		/** @brief					Allocates a certain amount of memory to accomodate the value passed in. (Nothing will happen if said value is less than or equal to the current capacity.) Also, if the vector has previously-allocated memory, this memory will be preserved alongside the new memory.
		*	@param capacity			the new capacity of the vector.
		*/
		void Reserve(const std::uint32_t& capacity, bool isFixedSize = false);

		/** @brief					Sets the vector's increment functor to the one specified by the user, which will be called only when more memory is required to accomodate new entries to the vector. (By default, vectors use an increment functor that simply increases the capacity by 1.)
		*	@param functor			the increment functor to use instead of the default.
		*/
		void SetReserveFunctor(const IncrementFunctor& functor);

		/** @brief					Searches for the first instance of the desired value and returns an iterator to it. If the value isn't in the vector, an end iterator is returned.
		*   @param value			value (of a specific type) to search for.
		*/
		Iterator Find(const T& value) const;

		/** @brief					Destructor for the Vector template class.
		*
		*/
		~Vector();

		/** @brief					Searches for and removes the first instance of the desired value. If the value isn't in the vector, nothing happens.
		*	@param value			value (of a specific type) to remove.
		*/
		void Remove(const T& value);

		/** @brief					Removes a range of elements specified by the iterators passed in. The size of the vector after the operation will be one less than the number of nodes in between begin and end, as end is excluded from the removal.
		*	@param begin			starting point (inclusive) of the ranged removal.
		*	@param end				ending point (exclusive) of the ranged removal.
		*	@exception				An std::exception is thrown if either of the iterators are invalid (i.e. pointing to different vectors than the one to remove from), or if the index of begin is higher than that of end.
		*/
		void Remove(const Iterator& begin, const Iterator& end);

		/** @brief					Empties the vector.
		*
		*/
		void Clear();
	private:
		/** @brief					Capacity of the vector (a.k.a. the maximum number of elements it can contain.)
		*
		*/
		std::uint32_t mSize;

		/** @brief					Current number of items in the vector.
		*
		*/
		std::uint32_t mCapacity;

		/** @brief					Container for however many nodes the vector allows.
		*
		*/
		T* mDataArray;

		/** @brief					Increment functor used to specify what the new capacity of the vector should be.
		*
		*/
		IncrementFunctor mReserveFunctor;
	};
}

#include "Vector.inl"