#include "pch.h"
#include "SoundSharedData.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(SoundSharedData)

const std::int32_t SoundSharedData::MAP_SIZE = 13;

SoundSharedData::SoundSharedData() : XmlParseMaster::SharedData(), mTrackInfo(MAP_SIZE)
{
}

SoundSharedData::~SoundSharedData()
{
	mTrackInfo.clear();
}

XmlParseMaster::SharedData * LibraryDesktop::SoundSharedData::Clone() const
{
	return new SoundSharedData();
}

/*HashMap<char, std::string>& LibraryDesktop::SymbolSharedData::GetTilePaths()
{
return mTilePaths;
}*/

std::unordered_map<std::string, std::pair<int, std::string>>& LibraryDesktop::SoundSharedData::GetTrackPaths()
{
	return mTrackInfo;
}

std::pair<int,int> LibraryDesktop::SoundSharedData::GetTrackCount()
{
	return std::make_pair(mBgmCount, mSfxCount);
}

void LibraryDesktop::SoundSharedData::AddPath(const std::string track, std::pair<int, std::string>& info)
{
	mTrackInfo.insert(std::make_pair(track, info));
}

void LibraryDesktop::SoundSharedData::SetTrackCount(const std::string trackCategory, int numTracks)
{
	if (trackCategory == "BGMTracks")
	{
		mBgmCount = numTracks;
	}
	else if (trackCategory == "SFXTracks")
	{
		mSfxCount = numTracks;
	}
}


