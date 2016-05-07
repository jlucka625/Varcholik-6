#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string StringXmlParseHelper::STRING_TAG = "String";

StringXmlParseHelper::StringXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingString)
{
}

void StringXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingString;
}

bool StringXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != STRING_TAG) || (parsingStatus != ParsingStatus::NotParsingString))
	{
		return false;
	}

	Scope& currentScope = tableSharedData->GetCurrentScope();

	std::string instanceName = GetRequiredAttribute("name", attributes);
	std::string instanceValue = GetRequiredAttribute("value", attributes);

	Datum& parsedDatum = currentScope.Append(instanceName);

	std::uint32_t index;

	if (currentScope.Is(ActionContainer::TypeIdClass()))
	{
		index = 0;
	}
	else
	{
		index = parsedDatum.Size();
	}

	parsedDatum.SetFromString(instanceValue, Datum::DatumType::String, index);

	parsingStatus = ParsingStatus::ParsingString;

	return true;
}

bool StringXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != STRING_TAG) || (parsingStatus != ParsingStatus::ParsingString))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingString;

	return true;
}

IXmlParseHelper* StringXmlParseHelper::Clone() const
{
	return new StringXmlParseHelper();
}

