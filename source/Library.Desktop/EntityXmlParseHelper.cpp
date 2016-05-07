#include "pch.h"

using namespace LibraryDesktop;

const std::string EntityXmlParseHelper::ENTITY_TAG = "Entity";

EntityXmlParseHelper::EntityXmlParseHelper() : IXmlParseHelper(), mNumEntitiesParsed(0)
{
}

bool EntityXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != ENTITY_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(Sector::TypeIdClass()));

	std::string instanceName = GetOptionalAttribute("name", attributes, "");
	std::string className = GetRequiredAttribute("class", attributes);

	tableSharedData->mScope = static_cast<Sector*>(tableSharedData->mScope)->CreateEntity(instanceName, className);

	return true;
}

bool EntityXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != ENTITY_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(Entity::TypeIdClass()));

	Entity* entity = static_cast<Entity*>(tableSharedData->mScope);

	if (entity->Name() == "")
	{
		entity->SetName(ENTITY_TAG + std::to_string(mNumEntitiesParsed++));
	}

	if (tableSharedData->mScope->GetParent() != nullptr)
	{
		tableSharedData->mScope = tableSharedData->mScope->GetParent();
	}

	return true;
}

IXmlParseHelper* EntityXmlParseHelper::Clone() const
{
	return new EntityXmlParseHelper();
}
