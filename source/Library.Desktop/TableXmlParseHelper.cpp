#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string TableXmlParseHelper::TABLE_TAG = "Table";

bool TableXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != TABLE_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);

	std::string instanceName = GetRequiredAttribute("name", attributes);

	tableSharedData->mScope = &(tableSharedData->mScope->AppendScope(instanceName));

	return true;
}

bool TableXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != TABLE_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);

	if (tableSharedData->mScope->GetParent() != nullptr)
	{
		tableSharedData->mScope = tableSharedData->mScope->GetParent();
	}

	return true;
}

IXmlParseHelper* TableXmlParseHelper::Clone() const
{
	return new TableXmlParseHelper();
}
