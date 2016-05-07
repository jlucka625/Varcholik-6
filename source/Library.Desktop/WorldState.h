#pragma once

namespace LibraryDesktop
{
	class WorldState
	{
	public:
		/** @brief					Constructor for the WorldState class.
		*   @return                 Instance of the WorldState class.
		*/
		WorldState();

		WorldState(const WorldState& otherWorldState) = delete;

		WorldState& operator=(const WorldState& otherWorldState) = delete;

		/** @brief					Default destructor for the WorldState class.
		*
		*/
		~WorldState() = default;

		/** @brief					Gets the game's current time status.
		*   @return					The game's current time status.
		*/
		GameTime& GetGameTime();

		/** @brief					Sets the game's time status variable (typically done before the main Update loop.)
		*   @param gameTime			Reference to a time status variable to be used by this world state.
		*/
		void SetGameTime(GameTime& gameTime);

		/** @brief					Gets if the world needs to unload its current sector
		*   @return					Gets if the world needs to unload its current sector
		*/
		bool GetUnloadSector();

		/** @brief					Sets if the world needs to unload its current sector
		*   @param unload			whether or not the current level will be unloaded.
		*/
		void SetUnloadSector(bool unload);

		/** @brief					Gets if the world needs to reload its current sector
		*   @return					Gets if the world needs to reload its current sector
		*/
		bool GetReloadSector();

		/** @brief					Sets if the world needs to reload its current sector
		*   @param unload			whether or not the current level will be reloaded.
		*/
		void SetReloadSector(bool reload);

		/** @brief					Address of the World currently being processed.
		*
		*/
		class World* mWorld; 

		/** @brief					Address of the Sector currently being processed.
		*
		*/
		class Sector* mSector;

		/** @brief					Address of the Entity currently being processed.
		*
		*/
		class Entity* mEntity;

		/** @brief					Address of the Action currently being processed.
		*
		*/
		class Action* mAction;

		///The current change in time, in seconds per frame
		float deltaTime;			

	private:
		/** @brief					Time the game has been running.
		*
		*/
		class GameTime* mGameTime;

		bool mUnloadSector;

		bool mReloadSector;
	};
}

