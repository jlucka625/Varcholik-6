#pragma once
#include <cstdint>
#include <cstring>
#include <functional>
#include <memory>

#include "DefaultHashFunctor.h"

#include "SList.h"
#include "Vector.h"

namespace LibraryDesktop
{
	template <typename TKey, typename TData, typename HashFunctor = DefaultHashFunctor<TKey>> class HashMap
	{
	public:
		typedef std::pair<TKey, TData> PairType;

		class Iterator
		{
			friend class HashMap<TKey, TData, HashFunctor>;
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

			/** @brief					Destructor for the Iterator class.
			*
			*/
			~Iterator();

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

			/** @brief					Equality operator for the Iterator class. (To be equal, both iterators must point to the same HashMap, same bucket in that HashMap, and same node of the bucket's chain.)
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					True if this and another instance of the iterator class are equal, and false otherwise.
			*/
			bool operator==(const Iterator& otherIterator) const;

			/** @brief					Inequality operator for the Iterator class.
			*  @param otherIterator		constant reference to another instance of the Iterator class.
			*  @return					False if this and another instance of the iterator class are equal (pointing to the same HashMap, bucket, and chain node), and true otherwise.
			*/
			bool operator!=(const Iterator& otherIterator) const;

			/** @brief					Prefix increment operator for the Iterator class.
			*  @return					A reference to the iterator at the next spot in the vector.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a HashMap, or pointing to an empty HashMap.)
			*/
			Iterator& operator++();

			/** @brief					Postfix increment operator for the Iterator class.
			*  @return					A copy of the iterator at the current location, while an increment to the next spot occurs in the background.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a HashMap, or pointing to an empty HashMap.)
			*/
			Iterator operator++(int);

			/** @brief					Dereference (star) operator for the Iterator class.
			*  @return					Reference to the key-value pair of the node being pointed to.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a HashMap, or pointing to an empty HashMap.)
			*/
			PairType& operator*() const;

			/** @brief					Dereference (arrow) operator for the Iterator class.
			*  @return					Pointer to the key-value pair of the node being pointed to.
			*  @exception				An std::exception is thrown if the iterator is invalid (i.e. not pointing to a HashMap, or pointing to an empty HashMap.)
			*/
			PairType* operator->() const;
		private:
			/** @brief					Places a new node inside the vector after the node being pointed to by a specific iterator.
			*  @param owner				address of the HashMap being pointed to.
			*  @param index				current location being pointed to in the BucketType.
			*  @param chainNode			current location being pointed to in the ChainType.
			*  @return					Instance of the Iterator class.
			*/
			Iterator(const HashMap* owner, const std::uint32_t& index, const typename HashMap::ChainType::Iterator& chainNode);

			/** @brief					Index of the current bucket.
			*
			*/
			std::uint32_t mCurrentBucket;

			/** @brief					Iterator to the current node within the current bucket's chain.
			*
			*/
			typename HashMap::ChainType::Iterator mCurrentChainNode;

			/** @brief					Address of the HashMap being pointed to.
			*
			*/
			const HashMap *mTargetMap;
		};

		/** @brief					Parametered constructor for the HashMap template class.
		*   @param size				number of buckets in the HashMap.
		*	@return					Instance of the HashMap template class of a specific type.
		*/
		HashMap(std::uint32_t size = 11);

		/** @brief					Destructor for the HashMap template class.
		*
		*/
		~HashMap();

		/** @brief					Copy constructor for the HashMap template class.
		*   @param	otherMap		Constant reference to a HashMap of the same type.
		*	@return					Instance of the HashMap template class from the deep copy of another.
		*/
		HashMap(const HashMap& otherMap);

		/** @brief					Move constructor for the HashMap template class.
		*   @param	otherMap		Right-hand reference to a HashMap of the same type.
		*	@return					Instance of the HashMap template class from the transferred ownership of another.
		*/
		HashMap(HashMap&& otherMap);

		/** @brief					Assignment operator for the HashMap template class.
		*   @param	otherMap		Constant reference to a HashMap of the same type.
		*	@return					Instance of the HashMap template class from the deep copy of another.
		*/
		HashMap& operator=(const HashMap& otherMap);

		/** @brief					Move assignment operator for the HashMap template class.
		*   @param	otherMap		Right-hand reference to a HashMap of the same type.
		*	@return					Instance of the HashMap template class from the transferred ownership of another.
		*/
		HashMap& operator=(HashMap&& otherMap);

		/** @brief					Equality operator for the HashMap template class. (To be equal, both HashMaps must have the same structure.)
		*   @param	otherMap		Constant reference to a HashMap of the same type.
		*	@return					True if the HashMaps are equal, and false otherwise.
		*/
		bool operator==(const HashMap& otherMap) const;

		/** @brief					Inequality operator for the HashMap template class.
		*   @param	otherMap		Constant reference to a HashMap of the same type.
		*	@return					False if the HashMaps are equal, and true otherwise.
		*/
		bool operator!=(const HashMap& otherMap) const;

		/** @brief					Searches the HashMap for the specified key, and returns an iterator to the location of it. If the key wasn't found, an iterator to the end of the HashMap is returned instead.
		*   @param key				key of the pair to search for.
		*	@return					Iterator to the location of the key, or to the end of the HashMap if the key wasn't found.
		*/
		Iterator Find(const TKey& key) const;

		/** @brief					Inserts the specified key-value pair into the HashMap and returns an iterator to it. However, if the key of the pair matches a key already in the HashMap, an iterator to the HashMap's entry is returned instead.
		*   @param pair				pair to insert.
		*	@return					Iterator to the newly-inserted key-value pair, or the pair that currently exists in the HashMap dictated by the specified pair's key.
		*/
		Iterator Insert(const PairType& pair);

		/** @brief					Inserts the specified key-value pair into the HashMap and returns an iterator to it. However, if the key of the pair matches a key already in the HashMap, an iterator to the HashMap's entry is returned instead.
		*   @param pair				pair to insert.
		*   @param wasInserted		Output parameter that dictates whether or not the pair was successfully inserted into the HashMap. (Note that this flag gets set to false if the pair already exists.)
		*	@return					Iterator to the newly-inserted key-value pair, or the pair that currently exists in the HashMap dictated by the specified pair's key.
		*/
		Iterator Insert(const PairType& pair, bool& wasInserted);

		/** @brief					Subscript operator for the HashMap template class. By default, this simply returns whatever value was stored at the specified key's location. If, however, the key wasn't found, an insertion of a pair consisting of the key and a default value is performed.
		*   @param key				key of the pair to retrieve.
		*	@return					Value of the specified key's pair in the HashMap (default data if the key previously didn't exist at the time of the call.)
		*/
		TData& operator[](const TKey& key);

		/** @brief					Constant version of the subscript operator for the HashMap template class. This version will not add or modify the entry specified by the key; it just searches for and returns the data if found.
		*   @param key				key of the pair to retrieve.
		*	@return					Value of the specified key's pair in the HashMap.
		*   @exception				An std::exception is thrown if no entry with the given key exists.
		*/
		const TData& operator[](const TKey& key) const;

		/** @brief					Removes a key-value pair from the HashMap, as designated by the specified key, or does nothing if the HashMap doesn't contain the key.
		*   @param key				key of the pair to remove.
		*
		*/
		void Remove(const TKey& key);

		/** @brief					Eliminates all key-value pairs in the HashMap.
		*
		*/
		void Clear();

		/** @brief					Get the size of, or number of key-value pairs in, the HashMap.
		*	@return					Total number of key-values pairs of the HashMap.
		*/
		std::uint32_t Size() const;

		/** @brief					Get the maximum number of key-value pairs allowed in the HashMap.
		*	@return					Maximum number of key-value pairs allowed in the HashMap.
		*/
		std::uint32_t Capacity() const;

		/** @brief					Dictates whether or not the HashMap has any entries in it.
		*	@return					True if the HashMap has at least one key-value pair within it, and false otherwise.
		*/
		bool IsEmpty() const;

		/** @brief					Searches for the specified key in the HashMap and states whether or not it is a defined key.
		*   @param key				key to search for.
		*	@return					True if the key was found, and false otherwise.
		*/
		bool ContainsKey(const TKey& key) const;

		/** @brief					Gets an iterator to the beginning of the HashMap. (The beginning is defined as the first key-value pair in the first occupied bucket of the HashMap.)
		*	@return					An iterator to the beginning of the HashMap.
		*/
		Iterator begin() const;

		/** @brief					Gets an iterator to the beginning of the HashMap. (The end is defined as one entry past the last key-value pair in the last occupied (or overall) bucket of the HashMap.)
		*	@return					An iterator to the end of the HashMap.
		*/
		Iterator end() const;
	private:
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;

		/** @brief					Number of buckets in the HashMap.
		*
		*/
		std::uint32_t mBucketCount;

		/** @brief					Number of key-value pairs in the HashMap.
		*
		*/
		std::uint32_t mPairCount;

		/** @brief					Index of the first filled bucket.
		*
		*/
		std::uint32_t mBeginIndex;

		/** @brief					Index of the last filled bucket (or the end of the HashMap.)
		*
		*/
		std::uint32_t mEndIndex;

		/** @brief					Container for bucket entries.
		*
		*/
		BucketType mDataBuckets;

		/** @brief					Generic Equals function, which compares two keys via the equality (==) operator. (NOTE: user-defined data types require an overload of the equality operator for accurate results. Failure to do so will result in a comparison by address rather by contents.)
		*   @param tKey				key for comparison.
		*   @param otherTKey		key for comparison.
		*	@return					True if the keys are equal, and false otherwise.
		*/
		template<typename T> bool Equals(const T& tKey, const T& otherTKey) const
		{
			return (tKey == otherTKey);
		}

		/** @brief					C-style string (char*) specialization of Equals, which compares two C-style strings for equality.
		*   @param stringKey		String key for comparison.
		*   @param otherStringKey	String key for comparison.
		*	@return					True if the strings are equal, and false otherwise.
		*/
		template<> bool Equals<char*>(char* const& stringKey, char* const& otherStringKey) const
		{
			return (strcmp(stringKey, otherStringKey) == 0);
		}

		/** @brief					Searches the HashMap for the specified key, and returns an iterator to the location of it. If the key wasn't found, an iterator to the end of the HashMap is returned instead. At the same time, a hashIndex is calculated and sent back to the caller by reference.
		*   @param key				key of the pair to search for.
		*	@param hashIndex		pointer to an index value that gets modified within the function body and sent back after completion.
		*	@return					Iterator to the location of the key, or to the end of the HashMap if the key wasn't found.
		*/
		Iterator Find(const TKey& key, std::uint32_t* hashIndex) const;
	};
}

#include "HashMap.inl"