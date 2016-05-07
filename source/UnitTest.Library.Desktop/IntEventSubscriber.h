#pragma once

#include <cstdint>

#include "EventSubscriber.h"

using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	class IntEventSubscriber : public EventSubscriber
	{
	public:
		/** @brief					Parameterless constructor for the IntEventSubscriber abstract base class.
		*   @return                 Instance of the IntEventSubscriber abstract base class.
		*/
		IntEventSubscriber();

		IntEventSubscriber(const IntEventSubscriber& otherFooSubscriber) = delete;

		IntEventSubscriber& operator=(const IntEventSubscriber& otherFooSubscriber) = delete;

		/** @brief					Default destructor for the IntEventSubscriber abstract base class.
		*
		*/
		virtual ~IntEventSubscriber() = default;

		/** @brief					Receives payloads of std::uint32_t events (objects of type Event<std::uint32_t>) and uses its data to initialize members.
		*   @parameter event		Event that's delivering a payload to this subscriber.
		*/
		virtual void Notify(const class EventPublisher& event) override;

		/** @brief					Gets the data received from the std::uint32_t payload.
		*   @return                 Value of mNumber.
		*/
		std::uint32_t Number() const;
	private:
		/** @brief					Test internal data.
		*
		*/
		std::uint32_t mNumber;
	};
}

