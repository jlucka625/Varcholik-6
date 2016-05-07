#include "pch.h"

using namespace UnitTestLibraryDesktop;

AttributedFooEventSubscriber::AttributedFooEventSubscriber() : EventSubscriber(), health(0), money(0.0f), identity(), position(glm::vec4(0, 0, 0, 0)), currentFieldGrid(glm::mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
{
}

void AttributedFooEventSubscriber::Notify(const EventPublisher& publisher)
{
	if (publisher.Is(Event<AttributedFoo>::TypeIdClass()))
	{
		Event<AttributedFoo>* aFooEvent = publisher.As<Event<AttributedFoo>>();
		const AttributedFoo& aFoo = aFooEvent->Message();

		health = aFoo["Health"].Get<int>();
		money = aFoo["Money"].Get<float>();
		identity = aFoo["Name"].Get<std::string>();
		position = aFoo["Position"].Get<glm::vec4>();
		currentFieldGrid = aFoo["CurrentFieldGrid"].Get<glm::mat4>();
	}
}

int AttributedFooEventSubscriber::Health() const
{
	return health;
}

const glm::vec4& AttributedFooEventSubscriber::Position() const
{
	return position;
}

float AttributedFooEventSubscriber::Money() const
{
	return money;
}

const std::string& AttributedFooEventSubscriber::Identity() const
{
	return identity;
}

const glm::mat4& AttributedFooEventSubscriber::CurrentFieldGrid() const
{
	return currentFieldGrid;
}
