#include "pch.h"

void ContactListener::BeginContact(b2Contact * contact)
{
	if (contact->IsTouching())
	{
		b2Fixture *fixA = contact->GetFixtureA();
		b2Fixture *fixB = contact->GetFixtureB();
		b2Body *bodyA = fixA->GetBody();
		b2Body *bodyB = fixB->GetBody();
		void *actorA = bodyA->GetUserData();
		void *actorB = bodyB->GetUserData();
		std::uint16_t actorABits = fixA->GetFilterData().categoryBits;
		std::uint16_t actorBBits = fixB->GetFilterData().categoryBits;

		std::uint16_t barrierCategory = static_cast<std::uint16_t>(BodyTypes::Barrier);

		if (actorABits == barrierCategory || actorBBits == barrierCategory)
		{
			EntityMovable* movable = static_cast<EntityMovable*>((actorABits == barrierCategory) ? actorB : actorA);

			if (movable != nullptr)
			{
				EntityTile* barrier = static_cast<EntityTile*>((actorABits == barrierCategory) ? actorA : actorB);
				barrier->ToggleStillness();
				movable->SetSpeed(movable->Speed().x * -1, movable->Speed().y * -1);
			}
		}
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	b2Fixture *fixA = contact->GetFixtureA();
	b2Fixture *fixB = contact->GetFixtureB();
	b2Body *bodyA = fixA->GetBody();
	b2Body *bodyB = fixB->GetBody();
	void *actorA = bodyA->GetUserData();
	void *actorB = bodyB->GetUserData();
	std::uint16_t actorABits = fixA->GetFilterData().categoryBits;
	std::uint16_t actorBBits = fixB->GetFilterData().categoryBits;

	std::uint16_t barrierCategory = static_cast<std::uint16_t>(BodyTypes::Barrier);

	if (actorABits == barrierCategory || actorBBits == barrierCategory)
	{
		EntityTile* barrier = static_cast<EntityTile*>((actorABits == barrierCategory) ? actorA : actorB);
		barrier->ToggleStillness();
	}
}
