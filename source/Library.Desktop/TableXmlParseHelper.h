#pragma once

#include "IXmlParseHelper.h"

namespace LibraryDesktop
{
	class TableXmlParseHelper : public IXmlParseHelper
	{
	public:
		/** @brief					Default constructor for the TableXmlParseHelper derived class.
		*   @return                 Instance of the TableXmlParseHelper derived class.
		*/
		explicit TableXmlParseHelper() = default;

		TableXmlParseHelper(const TableXmlParseHelper& otherSharedData) = delete;

		TableXmlParseHelper& operator=(const TableXmlParseHelper& otherSharedData) = delete;

		/** @brief					Default destructor for the TableXmlParseHelper derived class.
		*
		*/
		virtual ~TableXmlParseHelper() = default;

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

		/** @brief					Produces a new instance of the TableXmlParseHelper class.
		*   @return					Heap-allocated instance of the TableXmlParseHelper class.
		*/
		virtual IXmlParseHelper* Clone() const override;
	private:
		static const std::string TABLE_TAG;
	};
}

