#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string MatrixXmlParseHelper::MATRIX_TAG = "Matrix";

MatrixXmlParseHelper::MatrixXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingMatrix)
{
}

void MatrixXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingMatrix;
}

bool MatrixXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != MATRIX_TAG) || (parsingStatus != ParsingStatus::NotParsingMatrix))
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

	parsedDatum.SetFromString(instanceValue, Datum::DatumType::Matrix, index);

	parsingStatus = ParsingStatus::ParsingMatrix;

	return true;
}

bool MatrixXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if ((tableSharedData == nullptr) || (name != MATRIX_TAG) || (parsingStatus != ParsingStatus::ParsingMatrix))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingMatrix;

	return true;
}

IXmlParseHelper* MatrixXmlParseHelper::Clone() const
{
	return new MatrixXmlParseHelper();
}

