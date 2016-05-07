#include "pch.h"
#include "FooEventSubscriber.h"

using namespace UnitTestLibraryDesktop;

IntEventSubscriber::IntEventSubscriber() : EventSubscriber(), mNumber(0)
{
}

void IntEventSubscriber::Notify(const EventPublisher& event)
{
	if (event.Is(Event<std::uint32_t>::TypeIdClass()))
	{
		Event<std::uint32_t>* intEvent = event.As<Event<std::uint32_t>>();
		mNumber = intEvent->Message();
	}
}

std::uint32_t IntEventSubscriber::Number() const
{
	return mNumber;
}
