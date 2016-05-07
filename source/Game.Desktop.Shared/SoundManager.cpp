#include "pch.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "SoundManager.h"
#include "SoundParser.h"

namespace GameDesktopShared
{
	SoundManager* SoundManager::sInstance = nullptr;

	SoundManager::SoundManager()
	{
		Initialize();
	}

	SoundManager::~SoundManager()
	{
	}

	SoundManager* SoundManager::CreateInstance()
	{
		if (sInstance != nullptr)
		{
			return sInstance;
		}
		else
		{
			sInstance = new SoundManager();
		}
		return sInstance;
	}

	SoundManager * SoundManager::GetInstance()
	{
		return sInstance;
	}

	void SoundManager::Initialize()
	{
		OutputDebugStringA("initializing SOUND!!!");

		SoundParser sp;
		mSoundTable = sp.GetSoundTable();

		// Create FMOD interface object
		mResult = FMOD::System_Create(&system);
		FMODErrorCheck(mResult);

		// No sound cards (disable sound)
		if (mNumDrivers == 0)
		{
			//mResult = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
			//FMODErrorCheck(mResult);
		}

		// Initialise FMOD
		mResult = system->init(100, FMOD_INIT_NORMAL, 0);

		//Load sounds into memory
		for (auto itr = mSoundTable.begin(); itr != mSoundTable.end(); ++itr)
		{
			std::string soundType = itr->second.second.substr(17, 3);
			if (soundType == "BGM")
			{
				mResult = system->createStream(itr->second.second.c_str(), FMOD_DEFAULT, 0, &audio[itr->second.first]);
			}
			else if (soundType == "SFX")
			{
				mResult = system->createSound(itr->second.second.c_str(), FMOD_DEFAULT, 0, &audio[itr->second.first]);
			}
			FMODErrorCheck(mResult);
		}
	}

	FMOD_RESULT SoundManager::Play(std::string channel, std::string trackName, int fadeIn)
	{
		FMOD::Channel** soundChannel;
		if (channel == "BGM")
		{
			soundChannel = &bgmChannel;
		}
		else if (channel == "SFX")
		{
			soundChannel = &sfxChannel;
		}
		else
		{
			return FMOD_RESULT::FMOD_ERR_INVALID_PARAM;
		}

		//Look up clip by name and get its index in the sound array
		auto clipInfo = mSoundTable.find(trackName);
		if (clipInfo != mSoundTable.end())
		{
			system->playSound(audio[clipInfo->second.first], nullptr, false, soundChannel);

			mResult = system->getSoftwareFormat(&rate, 0, 0);
			FMODErrorCheck(mResult);

			mResult = (*soundChannel)->getDSPClock(0, &dspClock);
			FMODErrorCheck(mResult);

			mResult = (*soundChannel)->addFadePoint(dspClock, 0.0f);
			FMODErrorCheck(mResult);

			return (*soundChannel)->addFadePoint(dspClock + (rate * fadeIn), 1.0f);
		}
		else
		{
			return FMOD_RESULT::FMOD_ERR_INVALID_PARAM;
		}
	}

	FMOD_RESULT SoundManager::SetVolume(std::string channel, float volume)
	{
		FMOD::Channel* soundChannel;
		if (channel == "BGM")
		{
			soundChannel = bgmChannel;
		}
		else if (channel == "SFX")
		{
			soundChannel = sfxChannel;
		}
		else
		{
			return FMOD_RESULT::FMOD_ERR_INVALID_PARAM;
		}

		return soundChannel->setVolume(volume);
	}

	FMOD_RESULT SoundManager::Loop(std::string channel, int loopCount)
	{
		FMOD::Channel** soundChannel;
		if (channel == "BGM")
		{
			soundChannel = &bgmChannel;
		}
		else if (channel == "SFX")
		{
			soundChannel = &sfxChannel;
		}
		else
		{
			return FMOD_RESULT::FMOD_ERR_INVALID_PARAM;
		}

		(*soundChannel)->setMode(FMOD_LOOP_NORMAL);
		return (*soundChannel)->setLoopCount(loopCount);
	}

	FMOD_RESULT SoundManager::Stop()
	{
		sfxChannel->stop();
		return bgmChannel->stop();
	}

	FMOD_RESULT SoundManager::Update()
	{
		return system->update();
	}

	void SoundManager::FMODErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			sprintf_s(errorString, "FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			OutputDebugStringA(errorString);
			//exit(EXIT_FAILURE);
		}
	}

}