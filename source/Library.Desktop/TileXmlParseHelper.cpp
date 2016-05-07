#include "pch.h"

#include <cstdlib>

using namespace LibraryDesktop;

const std::string TileXmlParseHelper::TILE_TAG = "Tile";
const std::string TileXmlParseHelper::SYMBOL_ATTRIBUTE = "symbol";
const std::string TileXmlParseHelper::PATH_ATTRIBUTE = "path";


TileXmlParseHelper::TileXmlParseHelper() : parsingStatus(ParsingStatus::NotParsingTile)
{
}

void LibraryDesktop::TileXmlParseHelper::Initialize()
{
	parsingStatus = ParsingStatus::NotParsingTile;
}

bool TileXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name, HashMap<std::string, std::string>& attributes)
{
	SymbolSharedData* symbolSharedData = sharedData.As<SymbolSharedData>();

	if ((symbolSharedData == nullptr) || (name != TILE_TAG) || (parsingStatus != ParsingStatus::NotParsingTile))
	{
		return false;
	}

	//HashMap<char, std::string>& tilePaths = symbolSharedData->GetTilePaths();

	std::string symbolName = GetRequiredAttribute(SYMBOL_ATTRIBUTE, attributes);
	std::string pathName = GetRequiredAttribute(PATH_ATTRIBUTE, attributes);

	symbolSharedData->AddPath(symbolName.at(0), pathName);
	//std::string path = symbolSharedData->GetTilePaths()[symbolName.at(0)];
	//tilePaths.Insert(std::make_pair(symbolName.at(0), pathName));

	return true;
}

bool TileXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & name)
{
	SymbolSharedData* symbolSharedData = sharedData.As<SymbolSharedData>();

	if ((symbolSharedData == nullptr) || (name != TILE_TAG) || (parsingStatus != ParsingStatus::ParsingTile))
	{
		return false;
	}

	parsingStatus = ParsingStatus::NotParsingTile;

	return true;
}

IXmlParseHelper * TileXmlParseHelper::Clone() const
{
	return new TileXmlParseHelper();
}
