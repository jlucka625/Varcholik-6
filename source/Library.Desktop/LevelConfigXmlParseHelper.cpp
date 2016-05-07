#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string LevelConfigXmlParseHelper::LEVEL_TAG = "Level";
const std::string LevelConfigXmlParseHelper::PATH_ATTRIBUTE = "path";


LevelConfigXmlParseHelper::LevelConfigXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingLevel)
{
}

void LibraryDesktop::LevelConfigXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingLevel;
}

bool LevelConfigXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	LevelSharedData* levelSharedData = sharedData.As<LevelSharedData>();

	if ((levelSharedData == nullptr) || (name != LEVEL_TAG) || (parsingStatus != ParsingStatus::NotParsingLevel))
	{
		return false;
	}

	std::string pathName = GetRequiredAttribute(PATH_ATTRIBUTE, attributes);
	levelSharedData->AddPath(pathName);

	return true;
}

bool LevelConfigXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	LevelSharedData* levelSharedData = sharedData.As<LevelSharedData>();

	if ((levelSharedData == nullptr) || (name != LEVEL_TAG) || (parsingStatus != ParsingStatus::ParsingLevel))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingLevel;

	return true;
}

IXmlParseHelper * LevelConfigXmlParseHelper::Clone() const
{
	return new LevelConfigXmlParseHelper();
}