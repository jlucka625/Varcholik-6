#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string VectorXmlParseHelper::VECTOR_TAG = "Vector";

VectorXmlParseHelper::VectorXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingVector)
{
}

void VectorXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingVector;
}

bool VectorXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != VECTOR_TAG) || (parsingStatus != ParsingStatus::NotParsingVector))
	{
		return false;
	}

	Scope& currentScope = tableSharedData->GetCurrentScope();

	std::string instanceName = GetRequiredAttribute("name", attributes);
	std::string instanceValue = GetRequiredAttribute("value", attributes);

	Datum& parsedDatum = currentScope.Append(instanceName);

	std::uint32_t index;

	//if (currentScope.Is(ActionContainer::TypeIdClass()))
	if(parsedDatum.IsExternal())
	{
		index = 0;
	}
	else
	{
		index = parsedDatum.Size();
	}

	parsedDatum.SetFromString(instanceValue, Datum::DatumType::Vector, index);

	parsingStatus = ParsingStatus::ParsingVector;

	return true;
}

bool VectorXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != VECTOR_TAG) || (parsingStatus != ParsingStatus::ParsingVector))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingVector;

	return true;
}

IXmlParseHelper* VectorXmlParseHelper::Clone() const
{
	return new VectorXmlParseHelper();
}

