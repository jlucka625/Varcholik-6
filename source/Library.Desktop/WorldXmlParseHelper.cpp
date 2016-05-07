#include "pch.h"

using namespace LibraryDesktop;

const std::string WorldXmlParseHelper::WORLD_TAG = "World";

WorldXmlParseHelper::WorldXmlParseHelper() : IXmlParseHelper(), mNumWorldsParsed(0)
{
}

bool WorldXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != WORLD_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->GetParent() == nullptr);

	std::string instanceName = GetOptionalAttribute("name", attributes, "");

	World* world = new World(b2Vec2(0.0f, 9.8f));
	world->SetName(instanceName);

	tableSharedData->mScope->Adopt(*world, "Worlds");
	tableSharedData->mScope = world;

	return true;
}

bool WorldXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != WORLD_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(World::TypeIdClass()));

	World* world = static_cast<World*>(tableSharedData->mScope);

	if (world->Name() == "")
	{
		world->SetName(WORLD_TAG + std::to_string(mNumWorldsParsed++));
	}

	if (tableSharedData->mScope->GetParent() != nullptr)
	{
		tableSharedData->mScope = tableSharedData->mScope->GetParent();
	}

	return true;
}

IXmlParseHelper* WorldXmlParseHelper::Clone() const
{
	return new WorldXmlParseHelper();
}
