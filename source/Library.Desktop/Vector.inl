#pragma once

#include "DefaultIncrementFunctor.h"

using namespace LibraryDesktop;

//Vector definitions
template<typename T> inline Vector<T>::Vector() : mSize(0), mCapacity(0), mDataArray(nullptr), mReserveFunctor(DefaultIncrementFunctor())
{
}

template<typename T> inline Vector<T>::Vector(const Vector& otherVector) : mSize(0), mCapacity(0), mDataArray(nullptr), mReserveFunctor(nullptr)
{
	operator=(otherVector);
}

template<typename T> inline Vector<T>::Vector(Vector&& otherVector) : mSize(otherVector.mSize), mCapacity(otherVector.mCapacity), mDataArray(otherVector.mDataArray), mReserveFunctor(otherVector.mReserveFunctor)
{
	otherVector.mSize = 0;
	otherVector.mCapacity = 0;
	otherVector.mDataArray = nullptr;
	otherVector.mReserveFunctor = nullptr;
}

template<typename T> inline Vector<T>& Vector<T>::operator=(const Vector& otherVector)
{
	if (this != &otherVector)
	{
		Clear();
		Reserve(otherVector.mCapacity);

		for (Vector<T>::Iterator it = otherVector.begin(); it != otherVector.end(); ++it)
		{
			PushBack(*it);
		}

		mReserveFunctor = otherVector.mReserveFunctor;
	}

	return *this;
}

template<typename T> inline Vector<T>& Vector<T>::operator=(Vector&& otherVector)
{
	if (this != &otherVector)
	{
		Clear();

		mCapacity = otherVector.mCapacity;
		mSize = otherVector.mSize;
		mDataArray = otherVector.mDataArray;
		mReserveFunctor = otherVector.mReserveFunctor;

		otherVector.mCapacity = 0;
		otherVector.mSize = 0;
		otherVector.mDataArray = nullptr;
		otherVector.mReserveFunctor = nullptr;
	}

	return *this;
}

template<typename T> inline bool Vector<T>::operator==(const Vector& otherVector) const
{
	bool vectorsAreEqual = true;

	if (mSize != otherVector.mSize)
	{
		vectorsAreEqual = false;
	}
	else
	{
		Vector<T>::Iterator thisIterator = begin();
		Vector<T>::Iterator otherIterator = otherVector.begin();

		for (; (thisIterator != end()) && (otherIterator != otherVector.end()) && vectorsAreEqual; ++thisIterator, ++otherIterator)
		{
			vectorsAreEqual = ((*thisIterator) == (*otherIterator));
		}
	}

	return vectorsAreEqual;
}

template<typename T> inline bool Vector<T>::operator!=(const Vector& otherVector) const
{
	return !(operator==(otherVector));
}

template<typename T> inline T& Vector<T>::operator[](const std::uint32_t& index)
{
	if (index >= mSize)
	{
		throw std::exception("Error: index is out of range");
	}

	return mDataArray[index];
}

template<typename T> inline const T& Vector<T>::operator[](const std::uint32_t& index) const
{
	if (index >= mSize)
	{
		throw std::exception("Error: index is out of range");
	}

	return mDataArray[index];
}

template<typename T> inline void Vector<T>::PopBack()
{
	if (IsEmpty())
	{
		throw std::exception("Error: cannot pop from an empty vector");
	}

	mDataArray[--mSize].~T();
}

template<typename T> inline bool Vector<T>::IsEmpty() const
{
	return (mSize == 0);
}

template<typename T> inline const T& Vector<T>::Front() const
{
	return operator[](0);
}

template<typename T> inline T& Vector<T>::Front()
{
	return operator[](0);
}

template<typename T> inline const T& Vector<T>::Back() const
{
	return operator[](mSize - 1);
}

template<typename T> inline T& Vector<T>::Back()
{
	return operator[](mSize - 1);
}

template<typename T> inline std::uint32_t Vector<T>::Size() const
{
	return mSize;
}

template<typename T> inline std::uint32_t Vector<T>::Capacity() const
{
	return mCapacity;
}

template<typename T> inline typename Vector<T>::Iterator Vector<T>::begin() const
{
	return Iterator(this, 0);
}

template<typename T> inline typename Vector<T>::Iterator Vector<T>::end() const
{
	return Iterator(this, mSize);
}

template<typename T> inline typename Vector<T>::Iterator Vector<T>::PushBack(const T& data)
{
	if ((mSize + 1) > mCapacity)
	{
		std::uint32_t newCapacity = mReserveFunctor(mSize, mCapacity);

		if (newCapacity <= mCapacity)
		{
			newCapacity = (mCapacity + 1);
		}

		Reserve(newCapacity);
	}

	new (&mDataArray[mSize]) T(data);
	return Iterator(this, mSize++);
}

template<typename T> inline T& Vector<T>::At(const std::uint32_t & index)
{
	return operator[](index);
}

template<typename T> inline void Vector<T>::Reserve(const std::uint32_t & capacity, bool isFixedSize)
{
	if (capacity > mCapacity)
	{
		mCapacity = capacity;
		mDataArray = (T*)realloc(mDataArray, (mCapacity * sizeof(T)));

		if (isFixedSize)
		{
			for (std::uint32_t i = 0; i < mCapacity; i++)
			{
				new (&mDataArray[i]) T();
				++mSize;
			}
		}
	}
}

template<typename T> inline void Vector<T>::SetReserveFunctor(const IncrementFunctor& functor)
{
	mReserveFunctor = functor;
}

template<typename T> inline typename Vector<T>::Iterator Vector<T>::Find(const T& value) const
{
	auto it = begin();

	for (; it != end(); ++it)
	{
		if ((*it) == value)
		{
			break;
		}
	}

	return it;
}

template<typename T> inline void Vector<T>::Remove(const T& value)
{
	Iterator found = Find(value);

	if (found != end())
	{
		std::uint32_t foundIndex = found.currentNode;
		mDataArray[foundIndex].~T();
		--mSize;

		auto size = ((mSize - foundIndex) * sizeof(T));

		if (size > 0)
		{
			memmove_s(&mDataArray[foundIndex], size, &mDataArray[foundIndex + 1], size);
		}
	}
}

template<typename T> inline void Vector<T>::Remove(const Iterator& begin, const Iterator& end)
{
	if (begin.targetVector != this || end.targetVector != this)
	{
		throw std::exception("Error: use of invalid iterator");
	}

	std::uint32_t distanceFromBeginToEnd = (end.currentNode - begin.currentNode);

	Iterator it;

	for (it = begin; it != end; ++it)
	{
		(*it).~T();
	}

	auto size = ((mSize - it.currentNode) * sizeof(T));

	if (size > 0)
	{
		memmove_s(&mDataArray[it.currentNode - distanceFromBeginToEnd], size, &mDataArray[it.currentNode], size);
	}

	mSize -= distanceFromBeginToEnd;
}

template<typename T> inline Vector<T>::~Vector()
{
	Clear();
}

template<typename T> inline void Vector<T>::Clear()
{
	for (std::uint32_t i = 0; i < mSize; i++)
	{
		mDataArray[i].~T();
	}

	free(mDataArray);
	mDataArray = nullptr;

	mSize = mCapacity = 0;
}

//Iterator definitions
template<typename T> inline Vector<T>::Iterator::Iterator() : targetVector(nullptr), currentNode(0)
{
}

template<typename T> inline Vector<T>::Iterator::Iterator(const Vector* vector, const std::uint32_t& index) : targetVector(vector), currentNode(index)
{
}

template<typename T> inline Vector<T>::Iterator::Iterator(const typename Vector<T>::Iterator& otherIterator) : targetVector(nullptr), currentNode(0)
{
	operator=(otherIterator);
}

template<typename T> inline Vector<T>::Iterator::Iterator(typename Vector<T>::Iterator&& otherIterator) : targetVector(otherIterator.targetVector), currentNode(otherIterator.currentNode)
{
	otherIterator.targetVector = nullptr;
	otherIterator.currentNode = 0;
}

template<typename T> inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const typename Vector<T>::Iterator& otherIterator)
{
	if (this != &otherIterator)
	{
		targetVector = otherIterator.targetVector;
		currentNode = otherIterator.currentNode;
	}

	return *this;
}

template<typename T> inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(Iterator && otherIterator)
{
	if (this != &otherIterator)
	{
		targetVector = otherIterator.targetVector;
		currentNode = otherIterator.currentNode;

		otherIterator.targetVector = nullptr;
		otherIterator.currentNode = 0;
	}

	return *this;
}

template<typename T> inline bool typename Vector<T>::Iterator::operator==(const typename Vector<T>::Iterator& otherIterator) const
{
	return ((targetVector == otherIterator.targetVector) && (currentNode == otherIterator.currentNode));
}

template<typename T> inline bool typename Vector<T>::Iterator::operator!=(const typename Vector<T>::Iterator& otherIterator) const
{
	return !(operator==(otherIterator));
}

template<typename T> inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
{
	if ((targetVector == nullptr) || (currentNode >= targetVector->Size()))
	{
		throw std::exception("Error: cannot increment unaligned or end iterator");
	}

	++currentNode;
	return *this;
}

template<typename T> inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
{
	Vector<T>::Iterator tempIterator = *this;
	++(*this);
	return tempIterator;
}

template<typename T> inline typename T& Vector<T>::Iterator::operator*() const
{
	if ((targetVector == nullptr) || (currentNode >= targetVector->Size()))
	{
		throw std::exception("Error: trying to dereference unaligned or end iterator");
	}

	return const_cast<T&>(targetVector->operator[](currentNode));
}