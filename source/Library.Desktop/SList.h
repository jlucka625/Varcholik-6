#pragma once
#include <cstdint>
#include <memory>

namespace LibraryDesktop
{
	template <typename T> class SList
	{
	public:
		class Iterator
		{
			friend class SList<T>;
		public:
			/** @brief					Parameterless constructor for the Iterator class.
			*	@return					Instance of the Iterator class.
			*/
			Iterator();

			/** @brief					Copy constructor for the Iterator class.
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					Instance of the Iterator class from the deep copy of another.
			*/
			Iterator(const typename SList<T>::Iterator& otherIterator);

			/** @brief					Move constructor for the Iterator class.
			*  @param otherSList		right-hand reference to another instance of the Iterator.
			*  @return					Instance of the SList template class from the transferred ownership of the other.
			*/
			Iterator(typename SList<T>::Iterator&& otherIterator);

			/** @brief					Assignment operator for the Iterator class.
			*  @param otherSList		constant reference to another instance of the Iterator class.
			*  @return					Instance of the Iterator class from the deep copy of another.
			*/
			typename SList<T>::Iterator& operator=(const typename SList<T>::Iterator& otherIterator);

			/** @brief					Equality operator for the Iterator class. (To be equal, both iterators must point to the same location of the same list.)
			*  @param otherSList		constant reference to another instance of the Iterator class.
			*  @return					True if this and another instance of the iterator class are equal, and false otherwise.
			*/
			bool operator==(const typename SList<T>::Iterator& otherIterator) const;

			/** @brief					Inequality operator for the Iterator class.
			*  @param otherSList		constant reference to another instance of the Iterator class.
			*  @return					False if this and another instance of the iterator class are equal (pointing to the same list and location), and true otherwise.
			*/
			bool operator!=(const typename SList<T>::Iterator& otherIterator) const;

			/** @brief					Prefix increment operator for the Iterator class.
			*  @return					False if this and another instance of the iterator class are equal (pointing to the same list and location), and true otherwise.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a list, or pointing to an empty list.)
			*/
			typename SList<T>::Iterator& operator++();

			/** @brief					Postfix increment operator for the Iterator class.
			*  @return					False if this and another instance of the iterator class are equal (pointing to the same list and location), and true otherwise.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a list, or pointing to an empty list.)
			*/
			typename SList<T>::Iterator operator++(int);

			/** @brief					Dereference operator for the Iterator class.
			*  @return					Value of the node being pointed to.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a list, or pointing to an empty list.)
			*/
			T& operator*();
		private:
			/** @brief					Places a new node inside the list after the node being pointed to by a specific iterator.
			*  @param sList				address of the list being pointed to.
			*  @param node				current location being pointed to in the list.
			*  @return					Instance of the Iterator class.
			*/
			Iterator(const SList<T>* sList, typename SList<T>::Node *node);

			/** @brief					List the iterator is pointing to
			*
			*/
			const SList<T>* targetList;

			/** @brief					Current location being pointed to in the list.
			*
			*/
			typename SList<T>::Node *currentNode;
		};

		/** @brief					Parameterless constructor for the SList template class.
		*	@return					Instance of the SList template class of a specific type.
		*/
		SList();

		/** @brief					Copy constructor for the SList template class.
		*  @param otherSList		constant reference to another instance of the SList template class of the same type.
		*  @return					Instance of the SList template class from the deep copy of another.
		*/
		SList(const SList<T>& otherSList);

		/** @brief					Move constructor for the SList template class.
		*  @param otherSList		right-hand reference to another instance of the SList template class of the same type.
		*  @return					Instance of the SList template class from the transfered ownership of the other.
		*/
		SList(SList<T>&& otherSList);

		/** @brief					Assignment operator for the SList template class.
		*  @param otherSList		constant reference to another instance of the SList template class of the same type.
		*  @return					Instance of the SList template class from the deep copy of another.
		*/
		SList<T>& operator=(const SList<T>& otherSList);

		/** @brief					Equality operator for the SList template class. (To be equal, the lists must be of the same size, and their nodes must have the same value.)
		*  @param otherSList		constant reference to another instance of the SList template class of the same type.
		*  @return					True if this and another SList instance are equal, and false otherwise.
		*/
		bool operator==(const SList<T>& otherSList);

		/** @brief					Inequality operator for the SList template class.
		*  @param otherSList		constant reference to another instance of the SList template class of the same type.
		*  @return					False if this and another SList instance are equal, and true otherwise.
		*/
		bool operator!=(const SList<T>& otherSList);

		/** @brief					Destructor for the SList template class.
		*
		*/
		~SList();

		/** @brief					Push data of a specific type to the front of the list.
		*	@param data				value to be pushed.
		*/
		typename SList<T>::Iterator PushFront(const T& data);

		/** @brief					Pops, or removes, the item at the front of the list.
		*	@exception				An std::exception is thrown if the list is empty.
		*/
		void PopFront();

		/** @brief					Push data of a specific type to the back of the list.
		*	@param data				value to be pushed.
		*/
		typename SList<T>::Iterator PushBack(const T& data);

		/** @brief					States whether or not there are items in the list.
		*	@return					True if the size of the list is 0, and false otherwise.
		*/
		bool IsEmpty() const;

		/** @brief					Gets the item at the front of the list.
		*	@return					Whatever data (of a specific type) is at the front of the list.
		*	@exception				An std::exception is thrown if the list is empty.
		*/
		T& Front();

		/** @brief					Gets the item at the back of the list.
		*	@return					Whatever data (of a specific type) is at the back of the list.
		*	@exception				An std::exception is thrown if the list is empty.
		*/
		T& Back();

		/** @brief					Returns the current number of items in the list.
		*	@return					The size of the list.
		*/
		std::uint32_t Size() const;

		/** @brief					Empties the list.
		*
		*/
		void Clear();

		/** @brief					Gets an iterator to the beginning of the list.
		*   @return					an iterator pointing to the beginning of the list, or the end if the list is empty.
		*/
		typename SList<T>::Iterator begin() const;

		/** @brief					Gets an iterator to the end of the list (the spot after the official last node of the list.) Please note that this node value will ALWAYS be nullptr.
		*   @return					an iterator pointing to the spot after the last actual node of the list.
		*/
		typename SList<T>::Iterator end() const;

		/** @brief					Places a new node inside the list after the node being pointed to by a specific iterator.
		*  @param value				value (of a specific type) to insert.
		*  @param iterator			iterator pointing to a specific list location.
		*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a list, or pointing to a different list than the one to add to.)
		*/
		void InsertAfter(const T& value, const typename SList<T>::Iterator& iterator);

		/** @brief					Searches for and removes the first instance of the desired value. If the value isn't in the list, nothing happens.
		*   @param value			value (of a specific type) to search for.
		*/
		typename SList<T>::Iterator Find(const T& value) const;

		/** @brief					Searches for and removes the first instance of the desired value. If the value isn't in the list, nothing happens.
		*  @param value				value (of a specific type) to remove.
		*/
		void Remove(const T& value);
	private:
		typename SList<T>::Iterator FindPreviousNode(const T& value) const;

		class Node
		{
			friend class SList<T>::Iterator;
		public:
			Node() {}

			/** @brief					Parametered constructor for the Node template class.
			*  @param data				value (of a specific type) to be stored.
			*  @return					Instance of the Node template class for a specific type.
			*/
			Node(const T& data);

			/** @brief					Parametered constructor for the Node template class.
			*  @param data				value (of a specific type) to be stored.
			*  @return					Instance of the Node template class for a specific type.
			*/
			Node(const T& data, Node* otherNode);

			Node(Node& otherNode) = delete;

			Node operator=(Node& otherNode) = delete;

			/** @brief					Equality operator for the Node class. (To be equal, both nodes must have the same data.)
			*  @param otherNode			constant reference to another instance of the Node class.
			*  @return					True if this and another Node instance are equal, and false otherwise.
			*/
			bool operator==(const Node& otherNode);

			/** @brief					Inequality operator for the Node class.
			*  @param otherNode			constant reference to another instance of the Node class.
			*  @return					False if this and another Node instance are equal, and true otherwise.
			*/
			bool operator!=(const Node& otherNode);

			/** @brief					Value stored in this node.
			*
			*/
			T mData;

			/** @brief					Succeeding entry in the list.
			*
			*/
			Node *mNext;
		};

		/** @brief					First entry in the list.
		*
		*/
		Node *mFront;

		/** @brief					Last entry in the list.
		*
		*/
		Node *mBack;

		/** @brief					Number of elements in the list.
		*
		*/
		std::uint32_t mSize;
	};
}

#include "SList.inl"