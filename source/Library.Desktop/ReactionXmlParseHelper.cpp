#include "pch.h"

using namespace LibraryDesktop;

const std::string ReactionXmlParseHelper::REACTION_TAG = "Reaction";

ReactionXmlParseHelper::ReactionXmlParseHelper() : IXmlParseHelper(), mNumReactionsParsed(0)
{
}

bool ReactionXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != REACTION_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(ActionContainer::TypeIdClass()));

	std::string instanceName = GetOptionalAttribute("name", attributes, "");
	std::string className = GetRequiredAttribute("class", attributes);
	std::string parentName = GetOptionalAttribute("parentName", attributes, "Reactions");

	Reaction* reaction = (static_cast<ActionContainer*>(tableSharedData->mScope))->CreateReaction(instanceName, className, parentName);
	Event<EventMessageAttributed>::Subscribe(*reaction);

	tableSharedData->mScope = reaction;

	return true;
}

bool ReactionXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != REACTION_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(Reaction::TypeIdClass()));

	Reaction* reaction = static_cast<Reaction*>(tableSharedData->mScope);

	if (reaction->Name() == "")
	{
		reaction->SetName(REACTION_TAG + std::to_string(mNumReactionsParsed++));
	}

	if (tableSharedData->mScope->GetParent() != nullptr)
	{
		tableSharedData->mScope = tableSharedData->mScope->GetParent();
	}

	return true;
}

IXmlParseHelper* ReactionXmlParseHelper::Clone() const
{
	return new ReactionXmlParseHelper();
}
