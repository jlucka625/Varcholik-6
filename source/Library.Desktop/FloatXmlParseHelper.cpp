#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string FloatXmlParseHelper::FLOAT_TAG = "Float";

FloatXmlParseHelper::FloatXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingFloat)
{
}

void FloatXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingFloat;
}

bool FloatXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != FLOAT_TAG) || (parsingStatus != ParsingStatus::NotParsingFloat))
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

	parsedDatum.SetFromString(instanceValue, Datum::DatumType::Float, index);

	parsingStatus = ParsingStatus::ParsingFloat;

	return true;
}

bool FloatXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != FLOAT_TAG) || (parsingStatus != ParsingStatus::ParsingFloat))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingFloat;

	return true;
}

IXmlParseHelper* FloatXmlParseHelper::Clone() const
{
	return new FloatXmlParseHelper();
}

