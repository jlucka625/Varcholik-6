#include "pch.h"
#include "FooEventSubscriber.h"

using namespace UnitTestLibraryDesktop;

FooEventSubscriber::FooEventSubscriber() : EventSubscriber(), mData(0), mPointerValue(0)
{
}

void FooEventSubscriber::Notify(const EventPublisher& event)
{
	if (event.Is(Event<Foo>::TypeIdClass()))
	{
		Event<Foo>* fooEvent = event.As<Event<Foo>>();
		const Foo& foo = fooEvent->Message();

		mData = foo.Data();
		mPointerValue = *(foo.PointerData());
	}
}

int FooEventSubscriber::Data() const
{
	return mData;
}

int FooEventSubscriber::PointerData() const
{
	return mPointerValue;
}
