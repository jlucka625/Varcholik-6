#pragma once

#include <chrono>
#include <memory>

#include "RTTI.h"
#include "Vector.h"

namespace LibraryDesktop
{
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		/** @brief					Constructor for the EventPublisher class.
		*	@param subscribers		Reference to a static list of subscribers from a specific Event class variation.
		*   @return                 Instance of the EventPublisher class.
		*/
		explicit EventPublisher(Vector<class EventSubscriber*>& subscribers);

		/** @brief					Copy constructor for the EventPublisher class.
		*   @param	otherPublisher	Constant reference to another EventPublisher instance.
		*	@return					Instance of the EventPublisher class from the deep copy of another.
		*/
		EventPublisher(const EventPublisher& otherPublisher);

		/** @brief					Move constructor for the EventPublisher class.
		*   @param	otherPublisher	Right-hand reference to another EventPublisher instance.
		*	@return					Instance of the EventPublisher class from the transferred ownership of another.
		*/
		EventPublisher(EventPublisher&& otherPublisher);

		/** @brief					Copy assignment operator for the EventPublisher class.
		*   @param	otherPublisher	Constant reference to another EventPublisher instance.
		*	@return					Instance of the EventPublisher class from the deep copy of another.
		*/
		EventPublisher& operator=(const EventPublisher& otherPublisher);

		/** @brief					Move assignment operator for the EventPublisher class.
		*   @param	otherPublisher	Right-hand reference to another EventPublisher instance.
		*	@return					Instance of the EventPublisher class from the transferred ownership of another.
		*/
		EventPublisher& operator=(EventPublisher&& otherPublisher);

		/** @brief					Equality operator for the EventPublisher class. (For two EventPublishers to be equal, the enqueue times and delays must be equal.)
		*   @param	otherPublisher	Constant reference to another EventPublisher instance.
		*	@return					True if this and another EventPublisher instance are equal, and false otherwise.
		*/
		bool operator==(const EventPublisher& otherPublisher) const;

		/** @brief					Inequality operator for the EventPublisher class.
		*   @param	otherPublisher	Constant reference to another EventPublisher instance.
		*	@return					False if this and another EventPublisher instance are equal, and true otherwise.
		*/
		bool operator!=(const EventPublisher& otherPublisher) const;

		/** @brief					Default destructor for the EventPublisher class.
		*
		*/
		~EventPublisher() = default;

		/** @brief					Sets the time members of this event publisher.
		*	@param timeEnqueued		The time in which this event publisher was enqueued.
		*	@param delay			Lifetime of the event publisher.
		*/
		void SetTime(const std::chrono::high_resolution_clock::time_point& timeEnqueued, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());

		/** @brief					Gets the time in which this was enqueued.
		*   @return                 The time in which the event publisher was enqueued.
		*/
		const std::chrono::high_resolution_clock::time_point& TimeEnqueued() const;

		/** @brief					Gets however long it takes fr the event publisher to expire.
		*   @return                 The lifetime of the event publisher.
		*/
		const std::chrono::milliseconds& Delay() const;

		/** @brief					Dictates whether or not the event has expired (the current time is greater than or equal to the time enqueued plus the delay.)
		*   @return                 True if the event publisher has expired, and false otherwise.
		*/
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		/** @brief					Distributes a payload to all subscribers.
		*
		*/
		void Deliver() const;
	private:
		/** @brief					Address of a list of subscribers that all events of a specified type share.
		*
		*/
		Vector<class EventSubscriber*>* mSubscribers;

		/** @brief					Point in time in which this event was enqueued.
		*
		*/
		std::chrono::high_resolution_clock::time_point mTimeEnqueued;

		/** @brief					How long it takes for this event publisher to expire.
		*
		*/
		std::chrono::milliseconds mDelay;
	};
}

