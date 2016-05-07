#pragma once

using namespace LibraryDesktop;

//SList definitions
template<typename T> inline SList<T>::SList() : mSize(0), mFront(nullptr), mBack(nullptr)
{
}

template<typename T> inline SList<T>& SList<T>::operator=(const SList<T>& otherSList)
{
	if (this != &otherSList)
	{
		Clear();

		for (SList<T>::Iterator listIterator = otherSList.begin(); listIterator != otherSList.end(); ++listIterator)
		{
			PushBack((*listIterator));
		}

		mSize = otherSList.mSize;
	}

	return *this;
}

template<typename T> inline bool SList<T>::operator==(const SList<T>& otherSList)
{
	bool listsAreEqual = true;

	if (mSize != otherSList.mSize)
	{
		listsAreEqual = false;
	}
	else
	{
		SList<T>::Iterator thisIterator = begin();
		SList<T>::Iterator otherIterator = otherSList.begin();

		for (; (thisIterator != end()) && (otherIterator != otherSList.end()) && listsAreEqual; ++thisIterator, ++otherIterator)
		{
			listsAreEqual = ((*thisIterator) == (*otherIterator));
		}
	}

	return listsAreEqual;
}

template<typename T> inline bool SList<T>::operator!=(const SList<T>& otherSList)
{
	return !(operator==(otherSList));
}

template<typename T> inline SList<T>::SList(const SList<T>& otherSList) : mSize(0), mFront(nullptr), mBack(nullptr)
{
	operator=(otherSList);
}

template<typename T> inline SList<T>::SList(SList<T>&& otherSList) : mSize(otherSList.mSize), mFront(otherSList.mFront), mBack(otherSList.mBack)
{
	otherSList.mFront = nullptr;
	otherSList.mBack = nullptr;
}

template<typename T> inline SList<T>::~SList()
{
	Clear();
}

template<typename T> inline typename SList<T>::Iterator SList<T>::PushFront(const T& data)
{
	mFront = new Node(data, mFront);
	++mSize;

	if (mSize == 1)
	{
		mBack = mFront;
	}

	return Iterator(this, mFront);
}

template<typename T> inline void SList<T>::PopFront()
{
	if (IsEmpty())
	{
		throw std::exception("Error: cannot pop from an empty list");
	}

	Node* nodeToDelete = mFront;
	mFront = mFront->mNext;
	delete nodeToDelete;
	--mSize;

	if (IsEmpty())
	{
		mBack = nullptr;
	}
}

template<typename T> inline typename SList<T>::Iterator SList<T>::PushBack(const T& data)
{
	if (IsEmpty())
	{
		return PushFront(data);
	}
	else
	{
		Node *oldBack = mBack;
		mBack = mBack->mNext;
		mBack = new Node(data, nullptr);
		oldBack->mNext = mBack;
		++mSize;

		return Iterator(this, mBack);
	}
}

template<typename T> inline void SList<T>::InsertAfter(const T& value, const typename SList<T>::Iterator& iterator)
{
	if (iterator.targetList == nullptr)
	{
		throw std::exception("Error: iterator is not pointing to a list");
	}
	else if (this != iterator.targetList)
	{
		throw std::exception("Error: use of iterator of a different list");
	}

	if (IsEmpty())
	{
		PushFront(value);
	}
	else
	{
		if (iterator.currentNode == nullptr)
		{
			PushBack(value);
		}
		else
		{
			Node *nodeToInsertAfter = iterator.currentNode;
			nodeToInsertAfter->mNext = new Node(value, iterator.currentNode->mNext);
			++mSize;
		}
	}
}

template<typename T> inline bool SList<T>::IsEmpty() const
{
	return (mSize == 0);
}

template<typename T> inline T& SList<T>::Front()
{
	if (IsEmpty())
	{
		throw std::exception("Error: cannot get the back of an empty list");
	}
	
	return (mFront->mData);
}

template<typename T> inline typename SList<T>::Iterator SList<T>::begin() const
{
	Iterator listIterator(this, mFront);
	return listIterator;
}

template<typename T> inline T& SList<T>::Back()
{
	if (IsEmpty())
	{
		throw std::exception("Error: cannot get the back of an empty list");
	}

	return (mBack->mData);
}

template<typename T> inline typename SList<T>::Iterator SList<T>::end() const
{
	Iterator listIterator(this, nullptr);
	return listIterator;
}

template<typename T> inline std::uint32_t SList<T>::Size() const
{
	return mSize;
}

template<typename T> inline typename SList<T>::Iterator SList<T>::Find(const T& value) const
{
	bool foundValue = false;

	SList<T>::Iterator targetIterator = end();

	for (auto listIterator = begin(); listIterator != end() && !foundValue; ++listIterator)
	{
		if ((*listIterator) == value)
		{
			targetIterator = listIterator;
			foundValue = true;
		}
	}

	return targetIterator;
}

template<typename T> inline typename SList<T>::Iterator SList<T>::FindPreviousNode(const T& value) const
{
	Iterator previousNodeIterator = end();
	Iterator listIterator = begin();

	if ((*listIterator) == value)
	{
		previousNodeIterator = listIterator;
	}
	else
	{
		bool foundValue = false;

		for (; listIterator != end() && !foundValue; ++listIterator)
		{
			if ((listIterator.currentNode->mNext != nullptr) && (listIterator.currentNode->mNext->mData == value))
			{
				previousNodeIterator = listIterator;
				foundValue = true;
			}
		}
	}

	return previousNodeIterator;
}

template<typename T> inline void SList<T>::Remove(const T& value)
{
	Iterator beforeTargetNode = FindPreviousNode(value);

	if (beforeTargetNode.currentNode != nullptr)
	{
		if (beforeTargetNode.currentNode->mData == value)
		{
			PopFront();
		}
		else
		{
			Node *nodeToDelete = beforeTargetNode.currentNode->mNext;
			beforeTargetNode.currentNode->mNext = beforeTargetNode.currentNode->mNext->mNext;
			delete nodeToDelete;

			--mSize;

			if (mSize == 1)
			{
				mBack = mFront;
			}
		}
	}
}

template<typename T> inline void SList<T>::Clear()
{
	while (!IsEmpty())
	{
		PopFront();
	}

	mFront = mBack = nullptr;
}

//SList<T>::Iterator definitions
template<typename T> inline SList<T>::Iterator::Iterator() : targetList(nullptr), currentNode(nullptr)
{
}

template<typename T> inline SList<T>::Iterator::Iterator(const SList<T>* sList, typename SList<T>::Node *node) : targetList(sList), currentNode(node)
{
}

template<typename T> inline SList<T>::Iterator::Iterator(const typename SList<T>::Iterator& otherIterator) : targetList(nullptr), currentNode(nullptr)
{
	operator=(otherIterator);
}

template<typename T> inline SList<T>::Iterator::Iterator(typename SList<T>::Iterator&& otherIterator) : targetList(otherIterator.targetList), currentNode(otherIterator.currentNode)
{
	otherIterator.targetList = nullptr;
	otherIterator.currentNode = nullptr;
}

template<typename T> inline typename SList<T>::Iterator& SList<T>::Iterator::operator=(const typename SList<T>::Iterator& otherIterator)
{
	if (this != &otherIterator)
	{
		targetList = otherIterator.targetList;
		currentNode = otherIterator.currentNode;
	}

	return *this;
}

template<typename T> inline bool typename SList<T>::Iterator::operator==(const typename SList<T>::Iterator& otherIterator) const
{
	return ((targetList == otherIterator.targetList) && (currentNode == otherIterator.currentNode));
}

template<typename T> inline bool typename SList<T>::Iterator::operator!=(const typename SList<T>::Iterator& otherIterator) const
{
	return !(operator==(otherIterator));
}

template<typename T> inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
{
	if (targetList == nullptr)
	{
		throw std::exception("Error: invalid operation on unaligned list");
	}
	else if (currentNode == nullptr)
	{
		throw std::exception("Error: reached end of list");
	}

	currentNode = currentNode->mNext;
	return *this;
}

template<typename T> inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
{
	SList<T>::Iterator tempIterator = *this;
	++(*this);
	return tempIterator;
}

template<typename T> inline typename T& SList<T>::Iterator::operator*()
{
	if (currentNode == nullptr)
	{
		throw std::exception("Error: trying to dereference unaligned or end iterator");
	}

	return currentNode->mData;
}

//Node definitions
template<typename T> inline SList<T>::Node::Node(const T& data) : mData(data), mNext(nullptr)
{
}

template<typename T> inline SList<T>::Node::Node(const T & data, Node* otherNode) : mData(data), mNext(otherNode)
{
}

template<typename T> inline bool SList<T>::Node::operator==(const Node& otherNode)
{
	return (mData == otherNode.mData);
}

template<typename T> inline bool SList<T>::Node::operator!=(const Node & otherNode)
{
	return !(operator==(otherNode));
}
