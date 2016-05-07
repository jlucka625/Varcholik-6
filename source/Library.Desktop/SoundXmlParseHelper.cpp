#include "pch.h"
#include "SoundXmlParseHelper.h"
#include "SoundSharedData.h"
#include <cstdlib>

using namespace LibraryDesktop;

const std::string SoundXmlParseHelper::BGM_TAG = "BGM";
const std::string SoundXmlParseHelper::SFX_TAG = "SFX";
const std::string SoundXmlParseHelper::BGMTRACKS_TAG = "BGMTracks";
const std::string SoundXmlParseHelper::SFXTRACKS_TAG = "SFXTracks";
const std::string SoundXmlParseHelper::NAME_ATTRIBUTE = "name";
const std::string SoundXmlParseHelper::TRACK_ATTRIBUTE = "track";
const std::string SoundXmlParseHelper::PATH_ATTRIBUTE = "path";


SoundXmlParseHelper::SoundXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingTile)
{
}

void LibraryDesktop::SoundXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingTile;
}

bool SoundXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	SoundSharedData* soundSharedData = sharedData.As<SoundSharedData>();

	if ((soundSharedData == nullptr) || (name != BGM_TAG && name != SFX_TAG && name != BGMTRACKS_TAG && name != SFXTRACKS_TAG) || (parsingStatus != ParsingStatus::NotParsingTile))
	{
		return false;
	}

	if (name == BGMTRACKS_TAG || name == SFXTRACKS_TAG)
	{
		int numTracks = stoi(GetRequiredAttribute("value", attributes));
		soundSharedData->SetTrackCount(name, numTracks);
	}
	else
	{
		//HashMap<char, std::string>& tilePaths = symbolSharedData->GetTilePaths();
		std::string trackName = GetRequiredAttribute(NAME_ATTRIBUTE, attributes);
		int trackNumber = stoi((GetRequiredAttribute(TRACK_ATTRIBUTE, attributes)));
		std::string pathName = GetRequiredAttribute(PATH_ATTRIBUTE, attributes).c_str();

		std::pair<int, std::string> info(trackNumber, pathName);

		soundSharedData->AddPath(trackName, info);
		//std::string path = symbolSharedData->GetTilePaths()[symbolName.at(0)];
		//tilePaths.Insert(std::make_pair(symbolName.at(0), pathName));
	}
	return true;
}

bool SoundXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	SoundSharedData* symbolSharedData = sharedData.As<SoundSharedData>();

	if ((symbolSharedData == nullptr) || (name != BGM_TAG && name != SFX_TAG && name != BGMTRACKS_TAG && name != SFXTRACKS_TAG) || (parsingStatus != ParsingStatus::ParsingTile))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingTile;

	return true;
}

IXmlParseHelper * SoundXmlParseHelper::Clone() const
{
	return new SoundXmlParseHelper();
}
