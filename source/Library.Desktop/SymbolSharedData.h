#pragma once

#include "XmlParseMaster.h"
#include <unordered_map>

namespace LibraryDesktop
{
	class SymbolSharedData : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(SymbolSharedData, XmlParseMaster::SharedData)

		friend class XmlParseMaster;
		friend class TileXmlParseHelper;

	public:
		SymbolSharedData();

		SymbolSharedData(const SymbolSharedData& otherSharedData) = delete;
		SymbolSharedData& operator=(const SymbolSharedData& otherSharedData) = delete;

		virtual ~SymbolSharedData();

		virtual XmlParseMaster::SharedData* Clone() const override;

		std::unordered_map<char, std::string>& GetTilePaths();
		void AddPath(const char& symbol, const std::string& path);

	private:
		std::unordered_map<char, std::string> mTilePaths;

		static const std::int32_t MAP_SIZE;
	};
}

