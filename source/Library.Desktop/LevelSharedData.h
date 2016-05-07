#pragma once

#include "XmlParseMaster.h"
#include <unordered_map>

namespace LibraryDesktop
{
	class LevelSharedData : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(LevelSharedData, XmlParseMaster::SharedData)

		friend class XmlParseMaster;
		friend class LevelConfigXmlParseHelper;

	public:
		LevelSharedData();

		LevelSharedData(const LevelSharedData& otherSharedData) = delete;
		LevelSharedData& operator=(const LevelSharedData& otherSharedData) = delete;

		virtual ~LevelSharedData();

		virtual XmlParseMaster::SharedData* Clone() const override;

		Vector<std::string>& GetLevelPaths();
		void AddPath(const std::string& path);

	private:
		Vector<std::string> mLevelPaths;
	};
}
