#include <unordered_map>

class SoundParser
{
public:
	SoundParser();
	~SoundParser();

	std::unordered_map<std::string, std::pair<int, std::string>>& GetSoundTable();

private:
	void PopulateSoundTable();
	std::unordered_map<std::string, std::pair<int, std::string>> mSoundTable;
};
