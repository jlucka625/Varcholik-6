#include "pch.h"

using namespace UnitTestLibraryDesktop;
using namespace LibraryDesktop;

RTTI_DEFINITIONS(Enemy)

Enemy::Enemy() : mHealth(100), mPosition(glm::vec4(0, 0, 0, 0)), mDamageModifier(1.0f), Entity()
{
	AddExternalAttribute("Health", 1, &mHealth);
	AddExternalAttribute("Position", 1, &mPosition);
	AddExternalAttribute("DamageModifier", 1, &mDamageModifier);
}

Enemy::~Enemy()
{
}

void Enemy::Update(WorldState& worldState)
{
	worldState.mEntity = this;

	Datum& actionsDatum = Actions();

	for (std::uint32_t i = 0; i < actionsDatum.Size(); i++)
	{
		Action* action = actionsDatum[i].As<Action>();
		assert(action != nullptr);
		action->Update(worldState);
	}
}

int Enemy::Health() const
{
	return mHealth;
}

const glm::vec4 & Enemy::Position() const
{
	return mPosition;
}

float Enemy::DamageModifier() const
{
	return mDamageModifier;
}
