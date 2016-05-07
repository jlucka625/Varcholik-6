#pragma once

#include "XmlParseMaster.h"
#include <unordered_map>

namespace LibraryDesktop
{
	class SoundSharedData : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(SoundSharedData, XmlParseMaster::SharedData)

			friend class XmlParseMaster;
		friend class SoundXmlParseHelper;

	public:
		SoundSharedData();

		SoundSharedData(const SoundSharedData& otherSharedData) = delete;
		SoundSharedData& operator=(const SoundSharedData& otherSharedData) = delete;

		virtual ~SoundSharedData();

		virtual XmlParseMaster::SharedData* Clone() const override;

		std::unordered_map<std::string, std::pair<int, std::string>>& GetTrackPaths();
		std::pair<int,int> GetTrackCount();
		//HashMap<char, std::string>& GetTilePaths();
		void AddPath(const std::string track, std::pair<int, std::string>& info);
		void SetTrackCount(std::string trackCategory, int numTracks);

	private:
		std::unordered_map<std::string, std::pair<int, std::string>> mTrackInfo;
		int mBgmCount;
		int mSfxCount;
		//HashMap<char, std::string> mTilePaths;

		static const std::int32_t MAP_SIZE;
	};
}

