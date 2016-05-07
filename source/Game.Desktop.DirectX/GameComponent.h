#pragma once

#include "Common.h"

namespace Library
{
	class Game;
	class GameTime;

	class GameComponent : public RTTI
	{
		RTTI_DECLARATIONS(GameComponent, RTTI)

	public:
		GameComponent();
		GameComponent(Game& game);
		virtual ~GameComponent();

		GameComponent(const GameComponent& rhs) = delete;
		GameComponent& operator=(const GameComponent& rhs) = delete;

		Game* GetGame();
		void SetGame(Game& game);
		bool Enabled() const;
		void SetEnabled(bool enabled);

		virtual void Initialize();
		virtual void Update(float deltaSeconds) = 0;

	protected:
		Game* mGame;
		bool mEnabled;
	};
}