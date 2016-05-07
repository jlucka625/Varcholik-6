#pragma once

#include "ActionContainer.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class Sector final : public ActionContainer
	{
		RTTI_DECLARATIONS(Sector, ActionContainer)
	public:
		/** @brief					Constructor for the Sector class.
		*   @return                 Instance of the Sector class.
		*/
		explicit Sector();

		Sector(const Sector& otherSector) = delete;

		Sector& operator=(const Sector& otherSector) = delete;

		/** @brief					Default destructor for the Sector class.
		*
		*/
		virtual ~Sector() = default;

		/** @brief					Gets the name of the sector.
		*   @return                 The sector's name.
		*/
		const std::string& Name() const;

		/** @brief					Sets the name of the sector.
		*   @param name             The sector's new name.
		*/
		void SetName(const std::string& name);

		/** @brief					Gets the Datum containing all of the entities within this sector.
		*   @return                 The Datum containing all of the entities within this sector.
		*/
		Datum& Entities();

		virtual class Action* CreateAction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		virtual class Reaction* CreateReaction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		/** @brief					Creates an entity of a specific type via the factory pattern and appends it to this sector.
		*	@param instanceName		Name this sector will have.
		*	@param className		Type of the entity (used to determine which factory is responsible for creating it.)
		*   @return                 The address of the newly-created entity, which resides in this sector.
		*	@exception				An std::exception occurs if the factory for the entity-to-create does not exist, and an assertion fail will occur if the entity is attempted to be 
		*							appended to something other than a sector.
		*/
		class Entity* CreateEntity(const std::string& instanceName, const std::string& className);

		/** @brief					Gets the world this sector resides in.
		*   @return					Address of the world this sector resides in.
		*/
		const class World* GetWorld() const;

		/** @brief					Sets the world this sector will reside in.
		*   @param world			Address of the new world this sector will belong to.
		*/
		void SetWorld(class World* world);

		/** @brief					Updates the internal state of this sector.
		*   @param worldState       Current state of the game's affairs.
		*/
		void Update(class WorldState& worldState);
	private:
		/** @brief					Name of this sector.
		*
		*/
		std::string mName;

		/** @brief					Name of the prescribed name attribute.
		*
		*/
		static const std::string NAME_ATTRIBUTE;
	};
}

