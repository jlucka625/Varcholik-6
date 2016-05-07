#pragma once

#include <cstdint>
#include <chrono>
#include <memory>

#include "Vector.h"

namespace LibraryDesktop
{
	class EventQueue final
	{
	public:
		/** @brief					Constructor for the EventQueue class.
		*   @return                 Instance of the EventQueue class.
		*/
		EventQueue();

		EventQueue(const EventQueue& otherEventQueue) = delete;

		EventQueue& operator=(const EventQueue& otherEventQueue) = delete;

		/** @brief					Default destructor for the EventQueue class.
		*
		*/
		~EventQueue();

		/** @brief					Adds a subscriber to the list of subscribers.
		*	@param publisher		Publisher to enqueue.
		*	@param gameTime			Game's current time.
		*	@param delay			Time to wait until the event expires.
		*	@exception				An std::exception occurs if the publisher has already been enqueued.
		*/
		void Enqueue(const std::shared_ptr<class EventPublisher>& publisher, const class GameTime& gameTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());

		/** @brief					Immediately delivers a publisher's message to its subscribers.
		*	@param publisher		Publisher whose message needs to be sent.
		*/
		void Send(const std::shared_ptr<EventPublisher>& publisher);

		/** @brief					Iterates through the event queue and delivers the payload of any expired events.
		*	@param gameTime			Game's current time.
		*/
		void Update(const class GameTime& gameTime);

		/** @brief					Removes all enqueued events from the event queue.
		*
		*/
		void Clear();

		/** @brief					Dictates whether or not there are any enqueued events.
		*	@return					True if the event queue is empty, and false otherwise.
		*/
		bool IsEmpty() const;

		/** @brief					Gets the number of enqueued events.
		*	@return					The size of the event queue.
		*/
		std::uint32_t Size() const;
	private:
		/** @brief					Event queue.
		*
		*/
		Vector<std::shared_ptr<class EventPublisher>> mQueue;
	};
}

