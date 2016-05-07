#pragma once

namespace GameDesktopShared
{
	class SoundManager
	{
	public:
		static SoundManager* CreateInstance();
		static SoundManager* GetInstance();
		~SoundManager();

		void Initialize();

		char errorString[256];
		FMOD_RESULT Play(std::string channel, std::string trackName, int fadeIn = 0);
		FMOD_RESULT Stop();
		FMOD_RESULT Update();
		FMOD_RESULT SetVolume(std::string channel, float volume);
		FMOD_RESULT Loop(std::string channel, int loopCount);

		FMOD_RESULT mResult;
		static SoundManager* sInstance;

	private:
		SoundManager();
		void FMODErrorCheck(FMOD_RESULT result);

		FMOD::System* system;
		FMOD::Sound* audio[100];
		std::unordered_map<std::string, std::pair<int, std::string>> mSoundTable;
		FMOD::Channel* sfxChannel;
		FMOD::Channel* bgmChannel;
		FMOD::ChannelGroup *channelMusic;
		FMOD::ChannelGroup *channelEffects;

		unsigned long long dspClock;
		int rate;

		int mNumDrivers;
	};
}