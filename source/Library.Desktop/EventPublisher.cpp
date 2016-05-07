#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(EventPublisher)

EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers) : mSubscribers(&subscribers), mTimeEnqueued(), mDelay(std::chrono::milliseconds::zero())
{
}

EventPublisher::EventPublisher(const EventPublisher& otherPublisher) : mTimeEnqueued(otherPublisher.mTimeEnqueued), mDelay(otherPublisher.mDelay)
{
}

EventPublisher::EventPublisher(EventPublisher&& otherPublisher) : mTimeEnqueued(std::move(otherPublisher.mTimeEnqueued)), mDelay(std::move(otherPublisher.mDelay))
{
	otherPublisher.mTimeEnqueued = std::chrono::high_resolution_clock::time_point();
	otherPublisher.mDelay = std::chrono::milliseconds();
}

EventPublisher& EventPublisher::operator=(const EventPublisher& otherPublisher)
{
	if (this != &otherPublisher)
	{
		mTimeEnqueued = otherPublisher.mTimeEnqueued;
		mDelay = otherPublisher.mDelay;
	}

	return *this;
}

EventPublisher& EventPublisher::operator=(EventPublisher && otherPublisher)
{
	if (this != &otherPublisher)
	{
		mTimeEnqueued = std::move(otherPublisher.mTimeEnqueued);
		mDelay = std::move(otherPublisher.mDelay);

		otherPublisher.mTimeEnqueued = std::chrono::high_resolution_clock::time_point();
		otherPublisher.mDelay = std::chrono::milliseconds();
	}

	return *this;
}

bool EventPublisher::operator==(const EventPublisher& otherPublisher) const
{
	return ((mTimeEnqueued == otherPublisher.mTimeEnqueued) && (mDelay == otherPublisher.mDelay));
}

bool EventPublisher::operator!=(const EventPublisher& otherPublisher) const
{
	return !(operator==(otherPublisher));
}

void EventPublisher::SetTime(const std::chrono::high_resolution_clock::time_point& timeEnqueued, const std::chrono::milliseconds& delay)
{
	mTimeEnqueued = timeEnqueued;
	mDelay = delay;
}

const std::chrono::high_resolution_clock::time_point& EventPublisher::TimeEnqueued() const
{
	return mTimeEnqueued;
}

const std::chrono::milliseconds& EventPublisher::Delay() const
{
	return mDelay;
}

bool EventPublisher::IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const
{
	return (currentTime >= (mTimeEnqueued + mDelay));
}

void EventPublisher::Deliver() const
{
	for (const auto& subscriber : *mSubscribers)
	{
		subscriber->Notify(*this);
	}
}
