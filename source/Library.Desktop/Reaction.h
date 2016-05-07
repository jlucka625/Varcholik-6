#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace LibraryDesktop
{
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)
	public:
		/** @brief					Default constructor for the Reaction abstract base class.
		*   @return                 Instance of the Reaction abstract base class.
		*/
		Reaction() = default;

		Reaction(const Reaction& otherReaction) = delete;

		Reaction& operator=(const Reaction& otherReaction) = delete;

		/** @brief					Default destructor for the Reaction abstract base class.
		*
		*/
		virtual ~Reaction() = default;

		/** @brief					Processes a received payload from an event publisher if that publisher is of an event type this subscriber is subscribed to.
		*   @param event			Publisher that's delivering the payload.
		*/
		virtual void Notify(const class EventPublisher& event) = 0;

		/** @brief					Fires a series of actions within its ActionList as a result of an event.
		*   @param worldState       Current state of the game's affairs.
		*/
		virtual void Update(class WorldState& worldState) override = 0;
	};

	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)
	public:
		/** @brief					Default constructor for the ReactionAttributed base class.
		*   @return                 Instance of the ReactionAttributed class.
		*/
		ReactionAttributed();

		ReactionAttributed(const ReactionAttributed& otherAReaction) = delete;

		ReactionAttributed& operator=(const ReactionAttributed& otherAReaction) = delete;

		/** @brief					Default destructor for the ReactionAttributed class.
		*
		*/
		virtual ~ReactionAttributed() = default;

		/** @brief					Processes a received payload from an event of type EventMessageAttributed (i.e. extracts the stored
		*							auxiliary attributes) and updates the enclosed ActionList.
		*   @param event			Publisher that's delivering the payload.
		*/
		virtual void Notify(const class EventPublisher& event) override;

		/** @brief					Fires a series of actions within its ActionList as a result of an event.
		*   @param worldState       Current state of the game's affairs.
		*/
		virtual void Update(class WorldState& worldState) override;
	private:
		std::string mSubtype;
	};
	ConcreteFactory(RTTI, ReactionAttributed)

	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)	
	public:
		/** @brief					Getter for mSubtype.
		*   @return                 The value of mSubtype.
		*/
		const std::string& GetSubtype() const;

		/** @brief					Setter for mSubtype.
		*
		*/
		void SetSubtype(const std::string& subtype);

		/** @brief					Getter for mWorld.
		*   @return                 The value of mWorld.
		*/
		class World& GetWorld() const;

		/** @brief					Setter for mWorld.
		*
		*/
		void SetWorld(class World& worldState);
	private:
		/** @brief					Subtype of event (i.e. Jump, Shoot, etc.) this event corresponds to.
		*
		*/
		std::string mSubtype;

		/** @brief					World whose event queue this event will be enqueued in.
		*
		*/
		class World* mWorld;
	};

	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)
	public:
		/** @brief					Default constructor for the ActionEvent class.
		*   @return                 Instance of the ActionEvent class.
		*/
		ActionEvent();

		ActionEvent(const ActionEvent& otherAReaction) = delete;

		ActionEvent& operator=(const ActionEvent& otherAReaction) = delete;

		/** @brief					Default destructor for the ActionEvent class.
		*
		*/
		virtual ~ActionEvent() = default;

		/** @brief					Creates an event of type EventMessageAttributed, stores all desired attributes within, and enqueues it in the 
		*							world's event queue.
		*   @param worldState       Current state of the game's affairs.
		*/
		virtual void Update(class WorldState& worldState) override;
	private:
		/** @brief					Subtype of event (i.e. Jump, Shoot, etc.) to create an event for.
		*
		*/
		std::string mSubtype;

		/** @brief					Lifetime of the event-to-create.
		*
		*/
		int mDelay;
	};
}
