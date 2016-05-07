#pragma once

#include <cstdint>

#include "EventSubscriber.h"

using namespace LibraryDesktop;

namespace UnitTestLibraryDesktop
{
	class AttributedFooEventSubscriber : public EventSubscriber
	{
	public:
		/** @brief					Parameterless constructor for the AttributedFooEventSubscriber class.
		*   @return                 Instance of the AttributedFooEventSubscriber class.
		*/
		AttributedFooEventSubscriber();

		AttributedFooEventSubscriber(const AttributedFooEventSubscriber& otherAFooSubscriber) = delete;

		AttributedFooEventSubscriber& operator=(const AttributedFooEventSubscriber& otherAFooSubscriber) = delete;

		/** @brief					Default destructor for the AttributedFooEventSubscriber class.
		*
		*/
		virtual ~AttributedFooEventSubscriber() = default;

		/** @brief					Receives payloads of AttributedFoo events (objects of type Event<AttributedFoo>) and uses its data to initialize members.
		*   @parameter event		Event that's delivering a payload to this subscriber.
		*/
		virtual void Notify(const class EventPublisher& event) override;

		/** @brief					Getter for health.
		*   @return                 The value of health.
		*/
		int Health() const;

		/** @brief					Getter for position.
		*   @return                 The value of position.
		*/
		const glm::vec4& Position() const;

		/** @brief					Getter for money.
		*   @return                 The value of money.
		*/
		float Money() const;

		/** @brief					Getter for identity.
		*   @return                 The value of identity.
		*/
		const std::string& Identity() const;

		/** @brief					Getter for currentFieldGrid.
		*   @return                 The value of currentFieldGrid.
		*/
		const glm::mat4& CurrentFieldGrid() const;
	private:
		/** @brief					Test internal data.
		*
		*/
		int health;
		float money;
		std::string identity;
		glm::vec4 position;
		glm::mat4 currentFieldGrid;
	};
}

