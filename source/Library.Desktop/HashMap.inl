#include "HashMap.h"
#pragma once

using namespace LibraryDesktop;

//Iterator definitions
template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::Iterator::Iterator() : mTargetMap(nullptr), mCurrentBucket(0), mCurrentChainNode(HashMap::ChainType::Iterator())
{
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::Iterator::Iterator(const HashMap* owner, const std::uint32_t& index, const typename HashMap::ChainType::Iterator& chainNode) : mTargetMap(owner), mCurrentBucket(index), mCurrentChainNode(chainNode)
{
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::Iterator::Iterator(const typename HashMap<TKey, TData, HashFunctor>::Iterator& otherIterator) : mTargetMap(nullptr), mCurrentBucket(0)
{
	operator=(otherIterator);
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::Iterator::Iterator(typename HashMap<TKey, TData, HashFunctor>::Iterator&& otherIterator) : mTargetMap(otherIterator.mTargetMap), mCurrentBucket(otherIterator.mCurrentBucket), mCurrentChainNode(otherIterator.mCurrentChainNode)
{
	otherIterator.mTargetMap = nullptr;
	otherIterator.mCurrentBucket = 0;
	otherIterator.mCurrentChainNode = HashMap::ChainType::Iterator();
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::Iterator::~Iterator()
{
	mTargetMap = nullptr;
	mCurrentBucket = 0;
	mCurrentChainNode = HashMap::ChainType::Iterator();
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator=(const typename HashMap<TKey, TData, HashFunctor>::Iterator& otherIterator)
{
	if (this != &otherIterator)
	{
		mTargetMap = otherIterator.mTargetMap;
		mCurrentBucket = otherIterator.mCurrentBucket;
		mCurrentChainNode = otherIterator.mCurrentChainNode;
	}

	return *this;
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator=(Iterator && otherIterator)
{
	if (this != &otherIterator)
	{
		mTargetMap = otherIterator.mTargetMap;
		mCurrentBucket = otherIterator.mCurrentBucket;
		mCurrentChainNode = otherIterator.mCurrentChainNode;

		otherIterator.mTargetMap = nullptr;
		otherIterator.mCurrentBucket = 0;
		otherIterator.mCurrentChainNode = HashMap::ChainType::Iterator();
	}

	return *this;
}

template<typename TKey, typename TData, typename HashFunctor> inline bool HashMap<TKey, TData, HashFunctor>::Iterator::operator==(const typename HashMap<TKey, TData, HashFunctor>::Iterator& otherIterator) const
{
	return ((mTargetMap == otherIterator.mTargetMap) && (mCurrentBucket == otherIterator.mCurrentBucket) && (mCurrentChainNode == otherIterator.mCurrentChainNode));
}

template<typename TKey, typename TData, typename HashFunctor> inline bool HashMap<TKey, TData, HashFunctor>::Iterator::operator!=(const typename HashMap<TKey, TData, HashFunctor>::Iterator& otherIterator) const
{
	return !(operator==(otherIterator));
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator++()
{
	if ((mTargetMap == nullptr) || (mCurrentBucket >= mTargetMap->mBucketCount))
	{
		throw std::exception("Error: cannot increment unaligned or end iterator");
	}

	++mCurrentChainNode;

	if ((mCurrentChainNode == mTargetMap->mDataBuckets[mCurrentBucket].end()) && (mCurrentBucket < (mTargetMap->mBucketCount - 1)))
	{
		for (mCurrentBucket += 1; mCurrentBucket < mTargetMap->mEndIndex; mCurrentBucket++)
		{
			if (!mTargetMap->mDataBuckets[mCurrentBucket].IsEmpty())
			{
				break;
			}
		}

		mCurrentChainNode = mTargetMap->mDataBuckets[mCurrentBucket].begin();
	}

	return *this;
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Iterator::operator++(int)
{
	Iterator tempIterator = *this;
	++(*this);
	return tempIterator;
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::PairType& HashMap<TKey, TData, HashFunctor>::Iterator::operator*() const
{
	if (mTargetMap == nullptr)
	{
		throw std::exception("Error: trying to dereference unaligned iterator");
	}

	return (const_cast<PairType&>(*(const_cast<ChainType::Iterator&>(mCurrentChainNode))));
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::PairType* HashMap<TKey, TData, HashFunctor>::Iterator::operator->() const
{
	return &(operator*());
}

//HashMap definitions
template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::HashMap(std::uint32_t size) : mBucketCount(size), mPairCount(0), mBeginIndex(size - 1), mEndIndex(size - 1)
{
	if (size == 0)
	{
		++mBucketCount;
		mBeginIndex = mEndIndex = 0;
	}

	mDataBuckets.Reserve(mBucketCount, true);
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::HashMap(const typename HashMap<TKey, TData, HashFunctor> & otherMap) :
	mBucketCount(otherMap.mBucketCount), mPairCount(otherMap.mPairCount), mBeginIndex(otherMap.mBeginIndex), mEndIndex(otherMap.mEndIndex),
	mDataBuckets(otherMap.mDataBuckets)
{
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::HashMap(typename HashMap<TKey, TData, HashFunctor> && otherMap) :
	mBucketCount(otherMap.mBucketCount), mPairCount(otherMap.mPairCount), mBeginIndex(otherMap.mBeginIndex), mEndIndex(otherMap.mEndIndex), mDataBuckets(std::move(otherMap.mDataBuckets))
{
	otherMap.mBucketCount = 0;
	otherMap.mPairCount = 0;
	otherMap.mBeginIndex = 0;
	otherMap.mEndIndex = 0;
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>& HashMap<TKey, TData, HashFunctor>::operator=(const HashMap & otherMap)
{
	if (this != &otherMap)
	{
		mBucketCount = otherMap.mBucketCount;
		mPairCount = otherMap.mPairCount;
		mBeginIndex = otherMap.mBeginIndex;
		mEndIndex = otherMap.mEndIndex;
		mDataBuckets = otherMap.mDataBuckets;
	}

	return *this;
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>& HashMap<TKey, TData, HashFunctor>::operator=(HashMap && otherMap)
{
	if (this != &otherMap)
	{
		mBucketCount = otherMap.mBucketCount;
		mPairCount = otherMap.mPairCount;
		mBeginIndex = otherMap.mBeginIndex;
		mEndIndex = otherMap.mEndIndex;
		mDataBuckets = std::move(otherMap.mDataBuckets);

		otherMap.mBucketCount = 0;
		otherMap.mPairCount = 0;
		otherMap.mBeginIndex = 0;
		otherMap.mEndIndex = 0;
	}

	return *this;
}

template<typename TKey, typename TData, typename HashFunctor> inline HashMap<TKey, TData, HashFunctor>::~HashMap()
{
	mPairCount = 0;
	mBeginIndex = mEndIndex = mBucketCount = 0;
}

template<typename TKey, typename TData, typename HashFunctor> inline bool HashMap<TKey, TData, HashFunctor>::operator==(const HashMap & otherMap) const
{
	return ((mBucketCount == otherMap.mBucketCount) && (mPairCount == otherMap.mPairCount) && (mBeginIndex == otherMap.mBeginIndex) && (mEndIndex == otherMap.mEndIndex) && (mDataBuckets == otherMap.mDataBuckets));
}

template<typename TKey, typename TData, typename HashFunctor> inline bool HashMap<TKey, TData, HashFunctor>::operator!=(const HashMap & otherMap) const
{
	return !(operator==(otherMap));
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Find(const TKey& key) const
{
	std::uint32_t dummyInt;
	return Find(key, &dummyInt);
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Insert(const PairType& pair)
{
	bool wasInserted;
	return Insert(pair, wasInserted);
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Find(const TKey & key, std::uint32_t* hashIndex) const
{
	HashFunctor hashFunctor;
	*hashIndex = (hashFunctor(key) % mBucketCount);

	std::uint32_t bucketIndex;
	ChainType::Iterator chainFound;

	if (!mDataBuckets[*hashIndex].IsEmpty())
	{
		for (chainFound = mDataBuckets[*hashIndex].begin(); chainFound != mDataBuckets[*hashIndex].end(); ++chainFound)
		{
			if (Equals<TKey>((*chainFound).first, key))
			{
				break;
			}
		}
	}

	if (mDataBuckets[*hashIndex].IsEmpty() || chainFound == mDataBuckets[*hashIndex].end())
	{
		bucketIndex = mEndIndex;
		chainFound = mDataBuckets[mEndIndex].end();
	}
	else
	{
		bucketIndex = *hashIndex;
	}

	return Iterator(this, bucketIndex, chainFound);
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Insert(const PairType& pair, bool& wasInserted)
{
	std::uint32_t hashIndex;

	Iterator found = Find(pair.first, &hashIndex);

	if (found != end())
	{
		wasInserted = false;
		return found;
	}
	else
	{
		ChainType::Iterator insertion = mDataBuckets[hashIndex].PushBack(pair);

		++mPairCount;

		if (((mBeginIndex == (mBucketCount - 1)) && (mPairCount == 1)) || (hashIndex < mBeginIndex))
		{
			mBeginIndex = hashIndex;
		}

		if ((hashIndex >= mEndIndex) || (mPairCount == 1))
		{
			mEndIndex = hashIndex;

			if (hashIndex < (mBucketCount - 1))
			{
				++mEndIndex;
			}
		}

		wasInserted = true;
		return Iterator(this, hashIndex, insertion);
	}
}

template<typename TKey, typename TData, typename HashFunctor> inline TData& HashMap<TKey, TData, HashFunctor>::operator[](const TKey& key)
{
	bool wasInserted;
	Iterator it = Insert(std::make_pair(key, TData()), wasInserted);
	return ((*it).second);
}

template<typename TKey, typename TData, typename HashFunctor> inline const TData& HashMap<TKey, TData, HashFunctor>::operator[](const TKey& key) const
{
	Iterator it = Find(key);

	if (it == end())
	{
		throw std::exception("Error: no entry with the given key exists");
	}

	return ((*it).second);
}

template<typename TKey, typename TData, typename HashFunctor> inline void HashMap<TKey, TData, HashFunctor>::Remove(const TKey& key)
{
	Iterator found = Find(key);

	if (found != end())
	{
		mDataBuckets[found.mCurrentBucket].Remove(*found);
		--mPairCount;

		if (mDataBuckets[found.mCurrentBucket].IsEmpty())
		{
			if (found.mCurrentBucket == mBeginIndex)
			{
				for (mBeginIndex = 0; mBeginIndex < mEndIndex; mBeginIndex++)
				{
					if (!mDataBuckets[found.mCurrentBucket].IsEmpty())
					{
						break;
					}
				}
			}
			else if (found.mCurrentBucket == mEndIndex)
			{
				for (mEndIndex -= 1; mEndIndex > mBeginIndex; mEndIndex--)
				{
					if (!mDataBuckets[found.mCurrentBucket].IsEmpty())
					{
						break;
					}
				}
			}
		}
	}
}

template<typename TKey, typename TData, typename HashFunctor> inline void HashMap<TKey, TData, HashFunctor>::Clear()
{
	for (auto &bucket : mDataBuckets)
	{
		bucket.Clear();
	}

	mPairCount = 0;
	mBeginIndex = mEndIndex = (mBucketCount - 1);
}

template<typename TKey, typename TData, typename HashFunctor> inline std::uint32_t HashMap<TKey, TData, HashFunctor>::Size() const
{
	return mPairCount;
}

template<typename TKey, typename TData, typename HashFunctor> inline std::uint32_t HashMap<TKey, TData, HashFunctor>::Capacity() const
{
	return mBucketCount;
}

template<typename TKey, typename TData, typename HashFunctor> inline bool HashMap<TKey, TData, HashFunctor>::IsEmpty() const
{
	return (mPairCount == 0);
}

template<typename TKey, typename TData, typename HashFunctor> inline bool HashMap<TKey, TData, HashFunctor>::ContainsKey(const TKey& key) const
{
	return (Find(key) != end());
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::begin() const
{
	return Iterator(this, mBeginIndex, mDataBuckets[mBeginIndex].begin());
}

template<typename TKey, typename TData, typename HashFunctor> inline typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::end() const
{
	return Iterator(this, mEndIndex, mDataBuckets[mEndIndex].end());
}

