#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(Reaction)
RTTI_DEFINITIONS(ReactionAttributed)
RTTI_DEFINITIONS(EventMessageAttributed)
RTTI_DEFINITIONS(ActionEvent)

#pragma region ReactionAttributed

ReactionAttributed::ReactionAttributed() : Reaction()
{
	AddExternalAttribute("Subtype", 1, &mSubtype);
}

void ReactionAttributed::Notify(const EventPublisher& event)
{
	if (event.Is(Event<EventMessageAttributed>::TypeIdClass()))
	{
		Event<EventMessageAttributed>* attributedEvent = event.As<Event<EventMessageAttributed>>();
		const EventMessageAttributed& message = attributedEvent->Message();

		if (message.GetSubtype() == mSubtype)
		{
			ReceiveAuxiliaryAttributes(const_cast<EventMessageAttributed&>(message));
			Update(message.GetWorld().GetWorldState());
		}
	}
}

void ReactionAttributed::Update(WorldState& worldState)
{
	ActionList::Update(worldState);
}

#pragma endregion ReactionAttributed

#pragma region EventMessageAttributed

const std::string& EventMessageAttributed::GetSubtype() const 
{
	return mSubtype;
}

void EventMessageAttributed::SetSubtype(const std::string& subtype)
{
	mSubtype = subtype;
}

World& EventMessageAttributed::GetWorld() const
{
	assert(mWorld != nullptr);
	return *mWorld;
}

void EventMessageAttributed::SetWorld(World& world)
{
	mWorld = &world;
}

#pragma endregion EventMessageAttributed

#pragma region ActionEvent

ActionEvent::ActionEvent() : Action()
{
	AddExternalAttribute("Subtype", 1, &mSubtype);
	AddExternalAttribute("Delay", 1, &mDelay);
	AddNestedScopeAttribute("Parameters", 1);
}

void ActionEvent::Update(WorldState& worldState)
{
	EventMessageAttributed attributedEvent;
	attributedEvent.SetSubtype(mSubtype);
	attributedEvent.SetWorld(*(worldState.mWorld));

	DistributeAuxiliaryAttributes(attributedEvent);

	worldState.mWorld->GetEventQueue().Enqueue(std::make_shared<Event<EventMessageAttributed>>(std::move(attributedEvent)), worldState.GetGameTime(), std::chrono::milliseconds(mDelay));
}

#pragma endregion ActionEvent
