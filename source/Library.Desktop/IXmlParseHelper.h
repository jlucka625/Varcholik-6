#pragma once

#include "HashMap.h"
#include "XmlParseMaster.h"

namespace LibraryDesktop
{
	class IXmlParseHelper
	{
	public:
		/** @brief					Default constructor for the IXmlParseHelper base class. (However, since this is an abstract base class, it can only be called on the 
		*							initialization lines of derived class constructors.)
		*   @return                 Instance of the SharedData base class.
		*/
		IXmlParseHelper() = default;

		IXmlParseHelper(const IXmlParseHelper& otherSharedData) = delete;

		IXmlParseHelper& operator=(const IXmlParseHelper& otherSharedData) = delete;

		/** @brief					Default destructor for the SharedData base class.
		*
		*/
		virtual ~IXmlParseHelper() = default;

		/** @brief					Initializes any member data this helper may have.
		*
		*/
		virtual void Initialize() = 0;

		/** @brief					Pure virtual reporter for starting XML tags that dictates whether or not this helper is designed to handle said tag.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the starting XML tag.
		*	@param attributes		List of strings representing attributes within the start tag. (This is null if there are no attributes.)
		*	@return					True if this helper handles this, and false otherwise.
		*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name, HashMap<std::string, std::string>& attributes) = 0;

		/** @brief					Pure virtual reporter for ending XML tags that dictates whether or not this helper is designed to handle said tag.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the starting XML tag.
		*	@return					True if this helper handles this, and false otherwise.
		*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name) = 0;

		/** @brief					Pure virtual reporter for character data within XML tags that dictates whether or not this helper is designed to handle said data.
		*	@param userData			Shared data the parse master provides to its helpers.
		*	@param name				Name of the starting XML tag.
		*	@param length			List of strings representing attributes within the start tag. (This is null if there are no attributes.)
		*	@return					True if this helper handles this, and false otherwise.
		*/
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& name);

		/** @brief					Produces a new instance of the helper class.
		*   @return					Heap-allocated instance of the helper class.
		*/
		virtual IXmlParseHelper* Clone() const = 0;
	protected:
		const std::string& GetRequiredAttribute(const std::string& attribute, const HashMap<std::string, std::string>& attributes);

		const std::string& GetOptionalAttribute(const std::string& attribute, const HashMap<std::string, std::string>& attributes, const std::string& alternative);
	};
}

