#include "pch.h"

using namespace LibraryDesktop;

const std::string ActionXmlParseHelper::ACTION_TAG = "Action";

ActionXmlParseHelper::ActionXmlParseHelper() : IXmlParseHelper(), mNumActionsParsed(0)
{
}

ActionXmlParseHelper::~ActionXmlParseHelper()
{
}

bool ActionXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != ACTION_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(ActionContainer::TypeIdClass()));

	std::string instanceName = GetOptionalAttribute("name", attributes, "");
	std::string className = GetRequiredAttribute("class", attributes);
	std::string parentName = GetOptionalAttribute("parentName", attributes, "Actions");

	tableSharedData->mScope = (static_cast<ActionContainer*>(tableSharedData->mScope))->CreateAction(instanceName, className, parentName);

	return true;
}

bool ActionXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	TableSharedData* tableSharedData = sharedData.As<TableSharedData>();

	if (tableSharedData == nullptr || name != ACTION_TAG)
	{
		return false;
	}

	assert(tableSharedData->mScope != nullptr);
	assert(tableSharedData->mScope->Is(Action::TypeIdClass()));

	Action* action = static_cast<Action*>(tableSharedData->mScope);

	if (action->Name() == "")
	{
		action->SetName(ACTION_TAG + std::to_string(mNumActionsParsed++));
	}

	if (tableSharedData->mScope->GetParent() != nullptr)
	{
		tableSharedData->mScope = tableSharedData->mScope->GetParent();
	}

	return true;
}

IXmlParseHelper* ActionXmlParseHelper::Clone() const
{
	return new ActionXmlParseHelper();
}
