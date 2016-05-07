#pragma once

#include "ActionContainer.h"
#include "EventQueue.h"
#include "WorldState.h"
#include "RTTI.h"

#include <Box2D\Box2D.h>

namespace LibraryDesktop
{
	class World final : public ActionContainer, public b2ContactListener
	{
		RTTI_DECLARATIONS(World, ActionContainer)
	public:
		/** @brief					Constructor for the World class.
		*   @return                 Instance of the World class.
		*/
		explicit World(const b2Vec2& gravity);

		World(const World& otherWorld) = delete;

		World& operator=(const World& otherWorld) = delete;

		/** @brief					Default destructor for the World class.
		*
		*/
		~World();

		/** @brief					Gets the name of the world.
		*   @return                 The world's name.
		*/
		const std::string& Name() const;

		/** @brief					Sets the name of the world.
		*   @param name             The world's new name.
		*/
		void SetName(const std::string& name);

		/** @brief					Gets the state of this world.
		*   @return                 The state of this world.
		*/
		WorldState& GetWorldState();

		EventQueue& GetEventQueue();

		class b2World* GetB2DWorld() const;

		void FlipGravity();

		/** @brief					Gets the Datum containing all of the sectors within this world.
		*   @return                 The Datum containing all of the sectors within this world.
		*/
		Datum& Sectors();

		virtual class Action* CreateAction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		virtual class Reaction* CreateReaction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		/** @brief					Creates a sector and appends it to this world.
		*	@param mame				Name this sector will have.
		*   @return                 The address of the newly-created entity, which resides in this sector.
		*	@exception				An assertion fail will occur if the sector is attempted to be appended to something other than a world.
		*/
		class Sector* CreateSector(const std::string& name);

		/** @brief					Updates the internal state of this world.
		*   @param gameClock		Game's internal clock.
		*/
		void Update(class GameClock& gameClock);
	private:
		/** @brief					Name of this world.
		*
		*/
		std::string mName;

		class b2World *b2DWorld;

		/** @brief					Name of the prescribed name attribute.
		*
		*/
		static const std::string NAME_ATTRIBUTE;

		static const std::string LEVEL_ATTRIBUTE_NAME;

		/** @brief					Observer of the conditions of the game's current time and which components of the game are currently being updated.
		*
		*/
		WorldState mWorldState;

		int mLevel;

		EventQueue mEventQueue;
	};
}

