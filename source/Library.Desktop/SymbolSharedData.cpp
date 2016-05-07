#include "pch.h"

using namespace LibraryDesktop;

RTTI_DEFINITIONS(SymbolSharedData)

const std::int32_t SymbolSharedData::MAP_SIZE = 13;

SymbolSharedData::SymbolSharedData() : XmlParseMaster::SharedData(), mTilePaths(MAP_SIZE)
{
}

SymbolSharedData::~SymbolSharedData()
{
	mTilePaths.clear();
}

XmlParseMaster::SharedData * LibraryDesktop::SymbolSharedData::Clone() const
{
	return new SymbolSharedData();
}

std::unordered_map<char, std::string>& LibraryDesktop::SymbolSharedData::GetTilePaths()
{
	return mTilePaths;
}

void LibraryDesktop::SymbolSharedData::AddPath(const char & symbol, const std::string & path)
{
	mTilePaths.insert(std::make_pair(symbol, path));
}


