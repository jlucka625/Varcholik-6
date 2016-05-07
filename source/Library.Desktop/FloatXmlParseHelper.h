#pragma once

#include "IXmlParseHelper.h"

namespace LibraryDesktop
{
	class FloatXmlParseHelper : public IXmlParseHelper
	{
	public:
		/** @brief					Default constructor for the FloatXmlParseHelper derived class.
		*   @return                 Instance of the FloatXmlParseHelper derived class.
		*/
		explicit FloatXmlParseHelper();

		FloatXmlParseHelper(const FloatXmlParseHelper& otherSharedData) = delete;

		FloatXmlParseHelper& operator=(const FloatXmlParseHelper& otherSharedData) = delete;

		/** @brief					Default destructor for the FloatXmlParseHelper derived class.
		*
		*/
		virtual ~FloatXmlParseHelper() = default;

		/** @brief					Initializes any member data this helper may have.
		*
		*/
		virtual void Initialize() override;

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

		/** @brief					Produces a new instance of the FloatXmlParseHelper class.
		*   @return					Heap-allocated instance of the FloatXmlParseHelper class.
		*/
		virtual IXmlParseHelper* Clone() const override;
	private:
		enum class ParsingStatus
		{
			NotParsingFloat,
			ParsingFloat
		};

		ParsingStatus parsingStatus;

		static const std::string FLOAT_TAG;
	};
}

