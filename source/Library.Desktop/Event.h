#pragma once

#include "EventPublisher.h"
#include "Vector.h"

namespace LibraryDesktop
{
	template <class T> class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)
	public:
		/** @brief					Constructor for the Event templated class.
		*	@param payload			Contents (of a specific type) of this instance.
		*   @return                 Instance of the Event templated class.
		*/
		explicit Event(const T& payload);

		/** @brief					Copy constructor for the Event templated class.
		*   @param	otherEvent		Constant reference to another Event of a specific type.
		*	@return					Instance of Event class (of a specific type) from the deep copy of another.
		*/
		Event(const Event& otherEvent);

		/** @brief					Move constructor for the Event templated class.
		*   @param	otherEvent		Right-hand reference to another Event of a specific type.
		*	@return					Instance of Event class (of a specific type) from the transferred ownership of another.
		*/
		Event(Event&& otherEvent);

		/** @brief					Copy assignment operator for the Event templated class.
		*   @param	otherEvent		Constant reference to another Event of a specific type.
		*	@return					Instance of Event class (of a specific type) from the deep copy of another.
		*/
		Event& operator=(const Event& otherEvent);

		/** @brief					Move assignment operator for the Event templated class.
		*   @param	otherEvent		Right-hand reference to another Event of a specific type.
		*	@return					Instance of Event class (of a specific type) from the transferred ownership of another.
		*/
		Event& operator=(Event&& otherEvent);

		/** @brief					Equality operator for the Event templated class. (For two Events to be equal, their base EventPublishers must be equal, and their messages must be equal.)
		*   @param	otherEvent		Constant reference to another Event of a specific type.
		*	@return					True if this and another Event instance are equal, and false otherwise.
		*/
		bool operator==(const Event& otherEvent) const;

		/** @brief					Inequality operator for the Event templated class.
		*   @param	otherEvent		Constant reference to another Event of a specific type.
		*	@return					False if this and another Event instance are equal, and true otherwise.
		*/
		bool operator!=(const Event& otherEvent) const;

		/** @brief					Default destructor for the Event templated class.
		*
		*/
		~Event() = default;

		/** @brief					Adds a subscriber to the list of subscribers.
		*	@param subscriber		Subscriber to add.
		*	@exception				An std::exception occurs if the subscriber has already been added to the list. (Note that if multiple events of 
		*							a specific type have been enqueued, since all instances share the same static list of subscribers, you need not
		*							call this on the same set of subscribers that were added by a previous Event instance. Otherwise, the exception
		*							will occur.)
		*/
		static void Subscribe(class EventSubscriber& subscriber);

		/** @brief					Removes a subscriber from the list of subscribers. (Note that if multiple events of a specific type have been 
		*							enqueued, since all instances share the same static list of subscribers, you need only call this on one particular
		*							Event instance.)
		*	@param subscriber		Subscriber to remove.
		*/
		static void	Unsubscribe(class EventSubscriber& subscriber);

		/** @brief					Removes a subscriber from the list of subscribers. (Note that if multiple events of a specific type have been
		*							enqueued, since all instances share the same static list of subscribers, you need only call this on one particular
		*							Event instance.)
		*
		*/
		static void UnsubscribeAll();

		static Vector<class EventSubscriber*>& Subscribers();

		/** @brief					Gets the payload of this particular event.
		*   @return                 The payload of this particular event.
		*/
		const T& Message() const;
	private:
		/** @brief					List of subscribers that all events of a specified type will share.
		*
		*/
		static Vector<class EventSubscriber*> mSubscribers;

		/** @brief					Payload of the event.
		*
		*/
		T mMessage;
	};
}

#include "Event.inl"

