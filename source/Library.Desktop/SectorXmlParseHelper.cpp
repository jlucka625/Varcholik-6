#include "pch.h"

using namespace LibraryDesktop;

const std::string SectorXmlParseHelper::SECTOR_TAG = "Sector";

SectorXmlParseHelper::SectorXmlParseHelper() : IXmlParseHelper(), mNumSectorsParsed(0)
{
}

bool SectorXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != SECTOR_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(World::TypeIdClass()));

	std::string instanceName = GetOptionalAttribute("name", attributes, "");

	tableSharedData->mScope = static_cast<World*>(tableSharedData->mScope)->CreateSector(instanceName);

	return true;
}

bool SectorXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != SECTOR_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(Sector::TypeIdClass()));

	Sector* sector = static_cast<Sector*>(tableSharedData->mScope);

	if (sector->Name() == "")
	{
		sector->SetName(SECTOR_TAG + std::to_string(mNumSectorsParsed++));
	}

	if (tableSharedData->mScope->GetParent() != nullptr)
	{
		tableSharedData->mScope = tableSharedData->mScope->GetParent();
	}

	return true;
}

IXmlParseHelper* SectorXmlParseHelper::Clone() const
{
	return new SectorXmlParseHelper();
}
