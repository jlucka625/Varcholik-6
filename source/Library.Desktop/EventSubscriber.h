#pragma once

namespace LibraryDesktop
{
	class EventSubscriber
	{
	public:
		/** @brief					Default constructor for the EventSubscriber abstract base class.
		*   @return                 Instance of the EventSubscriber abstract base class.
		*/
		EventSubscriber() = default;

		EventSubscriber(const EventSubscriber& otherSubscriber) = delete;

		EventSubscriber& operator=(const EventSubscriber& otherSubscriber) = delete;

		/** @brief					Default destructor for the EventSubscriber abstract base class.
		*
		*/
		virtual ~EventSubscriber() = default;

		/** @brief					Processes a received payload from an event publisher if that publisher is of an event type this subscriber is subscribed to.
		*   @param event			Publisher that's delivering the payload.
		*/
		virtual void Notify(const class EventPublisher& event) = 0;
	};
}

