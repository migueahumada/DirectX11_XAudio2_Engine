#pragma once
#include <xaudio2.h>
#include "Audio.h"

class Channel
{
public:
	Channel();
	~Channel();

	SPtr<Audio> m_pAudioSource;

	IXAudio2SubmixVoice* m_pXAudio2SubmixVoice = nullptr;



};

