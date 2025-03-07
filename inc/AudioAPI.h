#pragma once
#include <xaudio2.h>
#include "Audio.h"
#include "Channel.h"
#include "HelperMacros.h"
#include <queue>

template <typename T>
using Queue = std::queue<T>;

class AudioAPI
{
public:
	AudioAPI(void* handleWindow);
	~AudioAPI();

	SPtr<Audio> CreateSoundEffect(const String& name, const String& filepath);

	//TODO: ERROR make this work
	SPtr<Channel> CreateChannel(SPtr<Audio> audio);

	//TODO: ERROR make this work
	void Play(SPtr<Audio> audio, SPtr<Channel> channel);
	
	void Play(SPtr<Audio> audio);

	

private:
	IXAudio2* m_pXAudio2 = nullptr;
	IXAudio2MasteringVoice* m_pMainMasteringVoice = nullptr;

	

};

