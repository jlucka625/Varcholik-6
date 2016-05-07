#pragma once

#include <cstdint>

#include "EventSubscriber.h"

using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	class FooEventSubscriber : public EventSubscriber
	{
	public:
		/** @brief					Parameterless constructor for the FooEventSubscriber abstract base class.
		*   @return                 Instance of the FooEventSubscriber abstract base class.
		*/
		FooEventSubscriber();

		FooEventSubscriber(const FooEventSubscriber& otherFooSubscriber) = delete;

		FooEventSubscriber& operator=(const FooEventSubscriber& otherFooSubscriber) = delete;

		/** @brief					Default destructor for the FooEventSubscriber abstract base class.
		*
		*/
		virtual ~FooEventSubscriber() = default;

		/** @brief					Receives payloads of Foo events (objects of type Event<Foo>) and uses its data to initialize members.
		*   @parameter event		Event that's delivering a payload to this subscriber.
		*/
		virtual void Notify(const class EventPublisher& event) override;

		/** @brief					Gets the data received from the Foo payload.
		*   @return                 Value of mData.
		*/
		int Data() const;

		/** @brief					Gets the pointer data received from the Foo payload.
		*   @return                 Value of mPointerValue.
		*/
		int PointerData() const;
	private:
		/** @brief					Test internal data.
		*
		*/
		int mData;
		int mPointerValue;
	};
}

