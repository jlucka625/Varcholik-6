#pragma once

using namespace LibraryDesktop;

template<typename T> inline Stack<T>::Stack() : mDataVector(Vector<T>())
{
}

template<typename T> inline Stack<T>::Stack(const Stack& otherStack) : mDataVector(otherStack.mDataVector)
{
}

template<typename T> inline Stack<T>::Stack(Stack&& otherStack) : mDataVector(otherStack.mDataVector)
{
	otherStack.mDataVector.~Vector();
}

template<typename T> inline Stack<T>& Stack<T>::operator=(const Stack& otherStack)
{
	if (this != &otherStack)
	{
		mDataVector = otherStack.mDataVector;
	}

	return *this;
}

template<typename T> inline Stack<T>& Stack<T>::operator=(Stack&& otherStack)
{
	if (this != &otherStack)
	{
		mDataVector = otherStack.mDataVector;
		otherStack.mDataVector.~Vector();
	}

	return *this;
}

template<typename T> inline Stack<T>::~Stack()
{
	Clear();
}

template<typename T> inline bool Stack<T>::operator==(const Stack & otherStack) const
{
	return (mDataVector == otherStack.mDataVector);
}

template<typename T> inline bool Stack<T>::operator!=(const Stack & otherStack) const
{
	return !(operator==(otherStack));
}

template<typename T> inline void Stack<T>::Push(const T& item)
{
	mDataVector.PushBack(item);
}

template<typename T> inline void Stack<T>::Pop()
{
	mDataVector.PopBack();
}

template<typename T> inline T& Stack<T>::Top()
{
	return mDataVector.Back();
}

template<typename T> inline const T& Stack<T>::Top() const
{
	return mDataVector.Back();
}

template<typename T> inline unsigned int Stack<T>::Size() const
{
	return mDataVector.Size();
}

template<typename T> inline bool Stack<T>::IsEmpty() const
{
	return mDataVector.IsEmpty();
}

template<typename T> inline void Stack<T>::Clear()
{
	mDataVector.Clear();
}
