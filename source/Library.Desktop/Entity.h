#pragma once

#include "ActionContainer.h"
#include "RTTI.h"

namespace LibraryDesktop
{
	class Entity : public ActionContainer
	{
		RTTI_DECLARATIONS(Entity, ActionContainer)
	public:
		/** @brief					Constructor for the Entity base class.
		*   @return                 Instance of the Entity base class.
		*/
		explicit Entity();

		Entity(const Entity& otherEntity) = delete;

		Entity& operator=(const Entity& otherEntity) = delete;

		/** @brief					Default destructor for the Entity base class.
		*
		*/
		virtual ~Entity() = default;

		/** @brief					Gets the name of the entity.
		*   @return                 The entity's name.
		*/
		const std::string& Name() const;

		/** @brief					Sets the name of the entity.
		*   @param name             The entity's new name.
		*/
		void SetName(const std::string& name);

		/** @brief					Gets the sector this entity resides in.
		*   @return					Address of the sector this entity resides in.
		*/
		const class Sector* GetSector() const;

		/** @brief					Sets the sector this entity will reside in.
		*   @param sector           Address of the new sector this entity will belong to.
		*/
		void SetSector(class Sector* sector);

		/** @brief					Creates an action of a specific type via the factory pattern and appends it to this entity.
		*	@param instanceName		Name this action will have.
		*	@param className		Type of the action (used to determine which factory is responsible for creating it.)
		*	@param parentName		Name of the parent that adopts the Action.
		*   @return                 The address of the newly-created action, which resides in this entity.
		*	@exception				An std::exception occurs if the factory for the action-to-create does not exist.
		*/
		virtual class Action* CreateAction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		virtual class Reaction* CreateReaction(const std::string& instanceName, const std::string& className, const std::string& parentName) override;

		/** @brief					Updates the internal state of this entity.
		*   @param worldState       Current state of the game's affairs.
		*	@exception				An assertion fail will occur if an entry enclosed within the actions container isn't an Action of some sort.
		*/
		virtual void Update(class WorldState& worldState);
	protected:
		/** @brief					Name of this entity.
		*
		*/
		std::string mName;

		/** @brief					Name of the prescribed name attribute.
		*
		*/
		static const std::string NAME_ATTRIBUTE;
	};
}

