#pragma once

#include "IXmlParseHelper.h"

namespace LibraryDesktop
{
	class EntityXmlParseHelper : public IXmlParseHelper
	{
	public:
		/** @brief					Default constructor for the EntityXmlParseHelper derived class.
		*   @return                 Instance of the EntityXmlParseHelper derived class.
		*/
		explicit EntityXmlParseHelper();

		EntityXmlParseHelper(const EntityXmlParseHelper& otherSharedData) = delete;

		EntityXmlParseHelper& operator=(const EntityXmlParseHelper& otherSharedData) = delete;

		/** @brief					Default destructor for the EntityXmlParseHelper derived class.
		*
		*/
		virtual ~EntityXmlParseHelper() = default;

		/** @brief					Initializes any member data this helper may have.
		*
		*/
		virtual void Initialize() override {}

		/** @brief					Pure virtual reporter for starting XML tags that dictates whether or not this helper is designed to handle said tag.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the starting XML tag.
		*	@param attributes		List of strings representing attributes within the start tag. (This is null if there are no attributes.)
		*	@return					True if this helper handles this, and false otherwise.
		*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name, HashMap<std::string, std::string>& attributes) override;

		/** @brief					Pure virtual reporter for ending XML tags that dictates whether or not this helper is designed to handle said tag.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the starting XML tag.
		*	@return					True if this helper handles this, and false otherwise.
		*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name) override;

		/** @brief					Produces a new instance of the EntityXmlParseHelper class.
		*   @return					Heap-allocated instance of the EntityXmlParseHelper class.
		*/
		virtual IXmlParseHelper* Clone() const override;
	private:
		/** @brief					Name of the Entity element tag.
		*
		*/
		static const std::string ENTITY_TAG;

		/** @brief					Counter of how many Entity objects were processed from start to finish (used in automatic name generation.)
		*
		*/
		std::uint32_t mNumEntitiesParsed;
	};
}

