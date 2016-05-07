#include "pch.h"
#include "SoundParser.h"

SoundParser::SoundParser()
{
	PopulateSoundTable();
}


SoundParser::~SoundParser()
{
}

std::unordered_map<std::string, std::pair<int, std::string>>& SoundParser::GetSoundTable()
{
	return mSoundTable;
}

void SoundParser::PopulateSoundTable()
{
	SoundSharedData sharedData;
	XmlParseMaster master(sharedData);
	SoundXmlParseHelper helper;

	master.AddHelper(helper);

	master.ParseFromFile("Content\\SoundLibrary.xml");

	std::pair<int,int> trackCounts = sharedData.GetTrackCount();
	mSoundTable.rehash(trackCounts.first + trackCounts.second);
	mSoundTable = sharedData.GetTrackPaths();
}