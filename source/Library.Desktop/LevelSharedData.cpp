#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(LevelSharedData)

LevelSharedData::LevelSharedData() : XmlParseMaster::SharedData()
{
}

LevelSharedData::~LevelSharedData()
{
	mLevelPaths.Clear();
}

XmlParseMaster::SharedData * LevelSharedData::Clone() const
{
	return new LevelSharedData();
}

Vector<std::string>& LevelSharedData::GetLevelPaths()
{
	return mLevelPaths;
}

void LevelSharedData::AddPath(const std::string & path)
{
	mLevelPaths.PushBack(path);
}