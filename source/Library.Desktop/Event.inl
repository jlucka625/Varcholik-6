#pragma once

#include "pch.h"
#include "Event.h"

using namespace LibraryDesktop;

template<class T>
RTTI_DEFINITIONS(Event<T>)

template<class T> Vector<EventSubscriber*> Event<T>::mSubscribers;

template<class T> inline Event<T>::Event(const T& payload) : EventPublisher(mSubscribers), mMessage(payload)
{
}

template<class T> inline Event<T>::Event(const Event & otherEvent) : EventPublisher(otherEvent), mMessage(otherEvent.mMessage)
{
}

template<class T> inline Event<T>::Event(Event&& otherEvent) : EventPublisher(std::move(otherEvent)), mMessage(std::move(otherEvent.mMessage))
{
}

template<class T> inline Event<T>& Event<T>::operator=(const Event& otherEvent)
{
	if (this != &otherEvent)
	{
		EventPublisher::operator=(otherEvent);
		mMessage = otherEvent.mMessage;
	}

	return *this;
}

template<class T> inline Event<T>& Event<T>::operator=(Event&& otherEvent)
{
	if (this != &otherEvent)
	{
		EventPublisher::operator=(std::move(otherEvent));
		mMessage = std::move(otherEvent.mMessage);
	}

	return *this;
}

template<class T> inline bool Event<T>::operator==(const Event& otherEvent) const
{
	return (!(EventPublisher::operator!=(otherEvent)) && (mMessage == otherEvent.mMessage));
}

template<class T> inline bool Event<T>::operator!=(const Event& otherEvent) const
{
	return !(operator==(otherEvent));
}

template<class T> inline void Event<T>::Subscribe(EventSubscriber& subscriber)
{
	if (mSubscribers.Find(&subscriber) != mSubscribers.end())
	{
		throw std::exception("Error: this subscriber has already subscribed to this event");
	}

	mSubscribers.PushBack(&subscriber);
}

template<class T> inline void Event<T>::Unsubscribe(EventSubscriber& subscriber)
{
	mSubscribers.Remove(&subscriber);
}

template<class T> inline void Event<T>::UnsubscribeAll()
{
	mSubscribers.Clear();
}

template<class T> inline Vector<class EventSubscriber*>& Event<T>::Subscribers()
{
	return mSubscribers;
}

template<class T> inline const T& Event<T>::Message() const
{
	return mMessage;
}
