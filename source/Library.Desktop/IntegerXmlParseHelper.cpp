#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string IntegerXmlParseHelper::INTEGER_TAG = "Integer";

IntegerXmlParseHelper::IntegerXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingInteger)
{
}

void IntegerXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingInteger;
}

bool IntegerXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != INTEGER_TAG) || (parsingStatus != ParsingStatus::NotParsingInteger))
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

	parsedDatum.SetFromString(instanceValue, Datum::DatumType::Integer, index);

	parsingStatus = ParsingStatus::ParsingInteger;

	return true;
}

bool IntegerXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != INTEGER_TAG) || (parsingStatus != ParsingStatus::ParsingInteger))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingInteger;

	return true;
}

IXmlParseHelper* IntegerXmlParseHelper::Clone() const
{
	return new IntegerXmlParseHelper();
}

