#pragma once

#include "Attributed.h"
#include "Entity.h"

namespace UnitTestLibraryDesktop
{
	class Enemy : public Entity
	{
		RTTI_DECLARATIONS(Enemy, Entity)
	public:
		Enemy();
		Enemy(const Enemy& otherEnemy) = delete;
		Enemy& operator=(const Enemy& otherEnemy) = delete;
		virtual ~Enemy();
		virtual void Update(class WorldState& worldState) override;

		int Health() const;
		const glm::vec4& Position() const;
		float DamageModifier() const;
	protected:
		int mHealth;
		glm::vec4 mPosition;
		float mDamageModifier;
	};
}

